#include "interpreter.h"
#include<iostream>

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
        }
        return result;
    }
    if(IfNode *ifNode = dynamic_cast<IfNode*>(node)){
        int condition = visit(ifNode->condition);
        if(condition){
            return visit(ifNode->body);
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
        variables[assign->varName] = value;
        return value;
    }
    // Variable Access
    if(VariableNode *var = dynamic_cast<VariableNode*>(node)){
        if(variables.find(var->name) != variables.end()){
            return variables[var->name];
        }
        std::cout<<"Undefined Varibale: "<<var->name<<std::endl;
        exit(1);
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
            default:
                std::cout<<"Unknown operator!"<<std::endl;
                exit(1);
        }
    }
    std::cout<<"Invalid AST Node!"<<std::endl;
    exit(1);
}