#include "interpreter.h"
#include<iostream>

Interpreter::Interpreter(){
    scopes.push_back({});
}

int Interpreter::visit(ASTNode *node){
    // Number node
    if(NumberNode *num = dynamic_cast<NumberNode*>(node)){
        return std::stoi(num->value);
    }
    // Compound Node
    if(CompoundNode *compound = dynamic_cast<CompoundNode*>(node)){
        int result = 0;
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
        return 0;
    }
    // If Node
    if(IfNode *ifNode = dynamic_cast<IfNode*>(node)){
        int condition = visit(ifNode->condition);
        // If branch
        if(condition){
            scopes.push_back({});
            int result = visit(ifNode->ifBody);
            scopes.pop_back();
            return result;
        }
        // Else branch
        if(ifNode->elseBody != nullptr){
            scopes.push_back({});
            int result = visit(ifNode->elseBody);
            scopes.pop_back();
            return result;
        }
        return 0;
    }
    // For Node
    if(ForNode *forNode = dynamic_cast<ForNode*>(node)){
        scopes.push_back({});
        visit(forNode->init);
        while(visit(forNode->condition)){
            visit(forNode->body);
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
        return 0;
    }
    // While Node
    if(WhileNode *whileNode = dynamic_cast<WhileNode*>(node)){
        while(visit(whileNode->condition)){
            scopes.push_back({});
            visit(whileNode->body);
            if(returnFlag){
                scopes.pop_back();
                return returnValue;
            }
            scopes.pop_back();
            if(breakFlag){
                breakFlag = false;
                break;
            }
            if(continueFlag){
                continueFlag = false;
                continue;
            }
        }
        return 0;
    }
    // Print Node
    if(PrintNode *printNode = dynamic_cast<PrintNode*>(node)){
        int value = visit(printNode->expr);
        std::cout<<value<<'\n';
        return value;
    }
    // Function Call
    if(FunctionCallNode *call = dynamic_cast<FunctionCallNode*>(node)){
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
            int value = visit(call->arguments[i]);
            scopes.back()[func->params[i]] = value;
        }
        returnFlag = false;
        returnValue = 0;
        visit(func->body);
        int result = returnValue;
        returnFlag = false;
        returnValue = 0;
        scopes.pop_back();
        return result;
    }
    // Variable Assignment
    if(VarAssignNode *assign = dynamic_cast<VarAssignNode*>(node)){
        int value = visit(assign->value);
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
        int value = visit(unary->expr);
        switch(unary->op){
            case NOT:
                return !value;
            default:
                std::cerr<<"Unknown Unary Operator!"<<std::endl;
                exit(1);
        }
    }
    // Break node
    if(dynamic_cast<BreakNode*>(node)){
        breakFlag = true;
        return 0;
    }
    // Continue Node
    if(dynamic_cast<ContinueNode*>(node)){
        continueFlag = true;
        return 0;
    }
    // Return node
    if(ReturnNode *returnNode = dynamic_cast<ReturnNode*>(node)){
        returnValue = visit(returnNode->value);
        returnFlag = true;
        return returnValue;
    }
    // Binary Operation node
    if(BinaryOpNode *binOp = dynamic_cast<BinaryOpNode*>(node)){
        int left = visit(binOp->left);
        int right = visit(binOp->right);
        switch(binOp->op){
            case PLUS:
                return left + right;
            case MINUS:
                return left - right;
            case MULTIPLY:
                return left * right;
            case DIVIDE:
                if(right == 0){
                    std::cerr<<"Division By Zero!"<<std::endl;
                    exit(1);
                }
                return left / right;
            case MOD:
                if(right == 0){
                    std::cerr<<"Modulo By Zero!"<<std::endl;
                    exit(1);
                }
                return left % right;
            case GREATER:
                return left > right;
            case LESS:
                return left < right;
            case GREATER_EQUAL:
                return left >= right;
            case LESS_EQUAL:
                return left <= right;
            case EQUAL_EQUAL:
                return left == right;
            case NOT_EQUAL:
                return left != right;
            case AND_AND:
                return left && right;
            case OR_OR:
                return left || right;
            default:
                std::cerr<<"Unknown operator!"<<std::endl;
                exit(1);
        }
    }
    std::cerr<<"Invalid AST Node!"<<std::endl;
    exit(1);
}