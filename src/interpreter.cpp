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
            if(breakFlag || continueFlag){
                return result;
            }
        }
        return result;
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
    // Variable Assignment
    if(VarAssignNode *assign = dynamic_cast<VarAssignNode*>(node)){
        int value = visit(assign->value);
        // Varibale declaration
        if(assign->isDeclaration){
            if(scopes.back().find(assign->varName) != scopes.back().end()){
                std::cout<<"Variable already declared: "<<assign->varName<<std::endl;
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
        std::cout<<"Varibale not declared: "<<assign->varName<<std::endl;
        exit(1);
    }
    // Variable Access
    if(VariableNode *var = dynamic_cast<VariableNode*>(node)){
        for(int i=scopes.size()-1; i>=0; i--){
            if(scopes[i].find(var->name) != scopes[i].end()){
                return scopes[i][var->name];
            }
        }
        std::cout<<"Undefined Varibale: "<<var->name<<std::endl;
        exit(1);
    }
    // Unary Operation Node
    if(UnaryOpNode *unary = dynamic_cast<UnaryOpNode*>(node)){
        int value = visit(unary->expr);
        switch(unary->op){
            case NOT:
                return !value;
            default:
                std::cout<<"Unknown Unary Operator!"<<std::endl;
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
                    std::cout<<"Division By Zero!"<<std::endl;
                    exit(1);
                }
                return left / right;
            case MOD:
                if(right == 0){
                    std::cout<<"Modulo By Zero!"<<std::endl;
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
                std::cout<<"Unknown operator!"<<std::endl;
                exit(1);
        }
    }
    std::cout<<"Invalid AST Node!"<<std::endl;
    exit(1);
}