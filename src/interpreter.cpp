#include "interpreter.h"
#include<iostream>

Interpreter::Interpreter(){
    scopes.push_back({});
}

Value Interpreter::visit(ASTNode *node){
    // Number node
    if(NumberNode *num = dynamic_cast<NumberNode*>(node)){
        return Value(std::stoi(num->value));
    }
    // String Node
    if(StringNode *str = dynamic_cast<StringNode*>(node)){
        return Value(str->value);
    }
    // Compound Node
    if(CompoundNode *compound = dynamic_cast<CompoundNode*>(node)){
        Value result = Value(0);
        for(ASTNode *stmt : compound->statements){
            result = visit(stmt);
            if(breakFlag || continueFlag || returnFlag){
                return result;
            }
        }
        return result;
    }
    // Function Definition
    if(FunctionDefNode *func = dynamic_cast<FunctionDefNode*>(node)){
        functions[func->name] = func;
        return Value(0);
    }
    // If Node
    if(IfNode *ifNode = dynamic_cast<IfNode*>(node)){
        Value condition = visit(ifNode->condition);
        if(condition.type != Value::INT){
            std::cerr<<"Condition must be integer!"<<std::endl;
            exit(1);
        }
        // If branch
        if(condition.intValue){
            scopes.push_back({});
            Value result = visit(ifNode->ifBody);
            scopes.pop_back();
            return result;
        }
        // Else branch
        if(ifNode->elseBody != nullptr){
            scopes.push_back({});
            Value result = visit(ifNode->elseBody);
            scopes.pop_back();
            return result;
        }
        return Value(0);
    }
    // For Node
    if(ForNode *forNode = dynamic_cast<ForNode*>(node)){
        scopes.push_back({});
        visit(forNode->init);
        while(true){
            Value condition = visit(forNode->condition);
            if(condition.type != Value::INT){
                std::cerr<<"Condition must be integer!"<<std::endl;
                exit(1);
            }
            if(!condition.intValue){
                break;
            }
            scopes.push_back({});
            visit(forNode->body);
            scopes.pop_back();
            if(returnFlag){
                scopes.pop_back();
                return returnValue;
            }
            if(breakFlag){
                breakFlag = false;
                break;
            }
            if(continueFlag){
                continueFlag = false;
                visit(forNode->update);
                continue;
            }
            visit(forNode->update);
        }
        scopes.pop_back();
        return Value(0);
    }
    // While Node
    if(WhileNode *whileNode = dynamic_cast<WhileNode*>(node)){
        while(true){
            Value condition = visit(whileNode->condition);
            if(condition.type != Value::INT){
                std::cerr<<"Condition must be integer!"<<std::endl;
                exit(1);
            }
            if(!condition.intValue){
                break;
            }
            scopes.push_back({});
            visit(whileNode->body);
            scopes.pop_back();
            if(returnFlag){
                return returnValue;
            }
            if(breakFlag){
                breakFlag = false;
                break;
            }
            if(continueFlag){
                continueFlag = false;
                continue;
            }
        }
        return Value(0);
    }
    // Print Node
    if(PrintNode *printNode = dynamic_cast<PrintNode*>(node)){
        Value value = visit(printNode->expr);
        if(value.type == Value::INT){
            std::cout<<value.intValue<<'\n';
        }
        else if(value.type == Value::STRING){
            std::cout<<value.stringValue<<'\n';
        }
        return value;
    }
    // Function Call
    if(FunctionCallNode *call = dynamic_cast<FunctionCallNode*>(node)){
        if(call->name == "input"){
            std::string input;
            std::getline(std::cin, input);
            // Check for number
            bool isNumber = true;
            for(char c : input){
                if(!isdigit(c)){
                    isNumber = false;
                    break;
                }
            }
            if(isNumber){
                return Value(std::stoi(input));
            }
            return Value(input);
        }
        if(functions.find(call->name) == functions.end()){
            std::cerr<<"Undefinded Function: "<<call->name<<std::endl;
            exit(1);
        }
        FunctionDefNode *func = functions[call->name];
        // Argument count check
        if(call->arguments.size() != func->params.size()){
            std::cerr<<"Arguments count mismatch!"<<std::endl;
            exit(1);
        }
        scopes.push_back({});
        for(int i=0; i<func->params.size(); i++){
            Value value = visit(call->arguments[i]);
            scopes.back()[func->params[i]] = value;
        }
        returnFlag = false;
        returnValue = Value(0);
        visit(func->body);
        Value result = returnValue;
        returnFlag = false;
        returnValue = Value(0);
        scopes.pop_back();
        return result;
    }
    // Variable Assignment
    if(VarAssignNode *assign = dynamic_cast<VarAssignNode*>(node)){
        Value value = visit(assign->value);
        // Varibale declaration
        if(assign->isDeclaration){
            if(scopes.back().find(assign->varName) != scopes.back().end()){
                std::cerr<<"Variable already declared: "<<assign->varName<<std::endl;
                exit(1);
            }
            scopes.back()[assign->varName] = value;
            return value;
        }
        // Variable Reassignment
        for(int i=scopes.size()-1; i>=0; i--){
            if(scopes[i].find(assign->varName) != scopes[i].end()){
                scopes[i][assign->varName] = value;
                return value;
            }
        }
        std::cerr<<"Varibale not declared: "<<assign->varName<<std::endl;
        exit(1);
    }
    // Variable Access
    if(VariableNode *var = dynamic_cast<VariableNode*>(node)){
        for(int i=scopes.size()-1; i>=0; i--){
            if(scopes[i].find(var->name) != scopes[i].end()){
                return scopes[i][var->name];
            }
        }
        std::cerr<<"Undefined Varibale: "<<var->name<<std::endl;
        exit(1);
    }
    // Unary Operation Node
    if(UnaryOpNode *unary = dynamic_cast<UnaryOpNode*>(node)){
        Value value = visit(unary->expr);
        switch(unary->op){
            case NOT:
                if(value.type != Value::INT){
                    std::cerr<<"Invalid operand for !"<<std::endl;
                    exit(1);
                }
                return Value(!value.intValue);
            default:
                std::cerr<<"Unknown Unary Operator!"<<std::endl;
                exit(1);
        }
    }
    // Break node
    if(dynamic_cast<BreakNode*>(node)){
        breakFlag = true;
        return Value(0);
    }
    // Continue Node
    if(dynamic_cast<ContinueNode*>(node)){
        continueFlag = true;
        return Value(0);
    }
    // Return node
    if(ReturnNode *returnNode = dynamic_cast<ReturnNode*>(node)){
        returnValue = visit(returnNode->value);
        returnFlag = true;
        return returnValue;
    }
    // Binary Operation node
    if(BinaryOpNode *binOp = dynamic_cast<BinaryOpNode*>(node)){
        Value left = visit(binOp->left);
        Value right = visit(binOp->right);
        switch(binOp->op){
            case PLUS:
                if(left.type == Value::INT && right.type == Value::INT){
                    return Value(left.intValue + right.intValue);
                }
                if(left.type == Value::STRING && right.type == Value::STRING){
                    return Value(left.stringValue + right.stringValue);
                }
                std::cerr<<"Invalid operands for +"<<std::endl;
                exit(1);
            case MINUS:
                if(left.type == Value::INT && right.type == Value::INT){
                    return Value(left.intValue - right.intValue);
                }
                std::cerr<<"Invalid operands for -"<<std::endl;
                exit(1);
            case MULTIPLY:
                if(left.type == Value::INT && right.type == Value::INT){
                    return Value(left.intValue * right.intValue);
                }
                std::cerr<<"Invalid operands for *"<<std::endl;
                exit(1);
            case DIVIDE:
                if(left.type == Value::INT && right.type == Value::INT){
                    if(right.intValue == 0){
                        std::cerr<<"Division By Zero!"<<std::endl;
                        exit(1);
                    }
                    return Value(left.intValue / right.intValue);
                }
                std::cerr<<"Invalid operands for /"<<std::endl;
                exit(1);
            case MOD:
                if(left.type == Value::INT && right.type == Value::INT){
                    if(right.intValue == 0){
                        std::cerr<<"Modulo By Zero!"<<std::endl;
                        exit(1);
                    }
                    return Value(left.intValue % right.intValue);
                }
                std::cerr<<"Invalid operands for %"<<std::endl;
                exit(1);
            case GREATER:
                if(left.type == Value::INT && right.type == Value::INT){
                    return Value(left.intValue > right.intValue);
                }
                std::cerr<<"Invalid operands for >"<<std::endl;
                exit(1);
            case LESS:
                if(left.type == Value::INT && right.type == Value::INT){
                    return Value(left.intValue < right.intValue);
                }
                std::cerr<<"Invalid operands for <"<<std::endl;
                exit(1);
            case GREATER_EQUAL:
                if(left.type == Value::INT && right.type == Value::INT){
                    return Value(left.intValue >= right.intValue);
                }
                std::cerr<<"Invalid operands for >="<<std::endl;
                exit(1);
            case LESS_EQUAL:
                if(left.type == Value::INT && right.type == Value::INT){
                    return Value(left.intValue <= right.intValue);
                }
                std::cerr<<"Invalid operands for <="<<std::endl;
                exit(1);
            case EQUAL_EQUAL:
                if(left.type == Value::INT && right.type == Value::INT){
                    return Value(left.intValue == right.intValue);
                }
                if(left.type == Value::STRING && right.type == Value::STRING){
                    return Value(left.stringValue == right.stringValue);
                }
                std::cerr<<"Invalid operands for =="<<std::endl;
                exit(1);
            case NOT_EQUAL:
                if(left.type == Value::INT && right.type == Value::INT){
                    return Value(left.intValue != right.intValue);
                }
                if(left.type == Value::STRING && right.type == Value::STRING){
                    return Value(left.stringValue != right.stringValue);
                }
                std::cerr<<"Invalid operands for !="<<std::endl;
                exit(1);
            case AND_AND:
                if(left.type == Value::INT && right.type == Value::INT){
                    return Value(left.intValue && right.intValue);
                }
                std::cerr<<"Invalid operands for &&"<<std::endl;
                exit(1);
            case OR_OR:
                if(left.type == Value::INT && right.type == Value::INT){
                    return Value(left.intValue || right.intValue);
                }
                std::cerr<<"Invalid operands for ||"<<std::endl;
                exit(1);
            default:
                std::cerr<<"Unknown operator!"<<std::endl;
                exit(1);
        }
    }
    std::cerr<<"Invalid AST Node!"<<std::endl;
    exit(1);
}