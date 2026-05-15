#ifndef AST_H
#define AST_H

#include<string>
#include<vector>
#include "token.h"

class ASTNode{
    public:
    virtual ~ASTNode() {}
};

class NumberNode : public ASTNode{
    public:
    std::string value;
    NumberNode(std::string v){
        value = v;
    }
};

class VariableNode : public ASTNode{
    public:
    std::string name;
    VariableNode(std::string n){
        name = n;
    }
};

class BinaryOpNode : public ASTNode{
    public:
    ASTNode *left, *right;
    TokenType op;
    BinaryOpNode(ASTNode *l, TokenType o, ASTNode *r){
        left = l;
        op = o;
        right = r;
    }
};

class VarAssignNode : public ASTNode{
    public:
    std::string varName;
    ASTNode *value;
    bool isDeclaration;
    VarAssignNode(std::string n, ASTNode *v, bool decl){
        varName = n;
        value = v;
        isDeclaration = decl;
    }
};

class CompoundNode : public ASTNode{
    public:
    std::vector<ASTNode*> statements;
    CompoundNode(){
        
    }
};

class PrintNode : public ASTNode{
    public:
    ASTNode *expr;
    PrintNode(ASTNode *e){
        expr = e;
    }
};

class IfNode : public ASTNode{
    public:
    ASTNode *condition;
    CompoundNode *ifBody;
    ASTNode *elseBody;
    IfNode(ASTNode *c, CompoundNode *b, ASTNode *e){
        condition = c;
        ifBody = b;
        elseBody = e;
    }
};

class WhileNode : public ASTNode{
    public:
    ASTNode *condition;
    CompoundNode *body;
    WhileNode(ASTNode *c, CompoundNode *b){
        condition = c;
        body = b;
    }
};

#endif