#include "parser.h"
#include<iostream>

Parser::Parser(Lexer l) : lexer(l){
    currentToken = lexer.getNextToken();
}

void Parser::eat(TokenType type){
    if(currentToken.type == type){
        currentToken = lexer.getNextToken();
    }
    else{
        std::cout<<"Syntax Error!"<<std::endl;
        exit(1);
    }
}

ASTNode *Parser::factor(){
    Token token = currentToken;
    if(token.type == NUMBER){
        eat(NUMBER);
        return new NumberNode(token.value);
    }
    else if(token.type == IDENTIFIER){
        eat(IDENTIFIER);
        return new VariableNode(token.value);
    }
    else if(token.type == LPAREN){
        eat(LPAREN);
        ASTNode *node = comparison();
        eat(RPAREN);
        return node;
    }
    std::cout<<"Invalid factor!"<<std::endl;
    exit(1);
}

ASTNode *Parser::term(){
    ASTNode *left = factor();
    while(currentToken.type == MULTIPLY || currentToken.type == DIVIDE || currentToken.type == MOD){
        Token op = currentToken;
        if(op.type == MULTIPLY){
            eat(MULTIPLY);
        }
        else if(op.type == DIVIDE){
            eat(DIVIDE);
        }
        else if(op.type == MOD){
            eat(MOD);
        }
        ASTNode *right = factor();
        left = new BinaryOpNode(left, op.type, right);
    }
    return left;
}

ASTNode *Parser::expr(){
    ASTNode *left = term();
    while(currentToken.type == PLUS || currentToken.type == MINUS){
        Token op = currentToken;
        if(op.type == PLUS){
            eat(PLUS);
        }
        else if(op.type == MINUS){
            eat(MINUS);
        }
        ASTNode *right = term();
        left = new BinaryOpNode(left, op.type, right);
    }
    return left;
}

ASTNode *Parser::comparison(){
    ASTNode *left = expr();
    while(
        currentToken.type == GREATER ||
        currentToken.type == LESS ||
        currentToken.type == GREATER_EQUAL ||
        currentToken.type == LESS_EQUAL ||
        currentToken.type == EQUAL_EQUAL ||
        currentToken.type == NOT_EQUAL
    ){
        Token op = currentToken;
        if(op.type == GREATER){
            eat(GREATER);
        }
        else if(op.type == GREATER_EQUAL){
            eat(GREATER_EQUAL);
        }
        else if(op.type == LESS){
            eat(LESS);
        }
        else if(op.type == LESS_EQUAL){
            eat(LESS_EQUAL);
        }
        else if(op.type == EQUAL_EQUAL){
            eat(EQUAL_EQUAL);
        }
        else if(op.type == NOT_EQUAL){
            eat(NOT_EQUAL);
        }
        ASTNode *right = expr();
        left = new BinaryOpNode(left, op.type, right);
    }
    return left;
}

ASTNode *Parser::statement(){
    if(currentToken.type == IF){
        eat(IF);
        eat(LPAREN);
        ASTNode *condition = comparison();
        eat(RPAREN);
        CompoundNode *body = block();
        return new IfNode(condition, body);
    }
    if(currentToken.type == INT){
        eat(INT);
        Token varToken = currentToken;
        eat(IDENTIFIER);
        eat(ASSIGN);
        ASTNode *value = comparison();
        eat(SEMICOLON);
        return new VarAssignNode(varToken.value, value);
    }
    if(currentToken.type == PRINT){
        eat(PRINT);
        eat(LPAREN);
        ASTNode *expression = comparison();
        eat(RPAREN);
        eat(SEMICOLON);
        return new PrintNode(expression);
    }
    ASTNode *node = comparison();
    eat(SEMICOLON);
    return node;
}

ASTNode *Parser::parse(){
    CompoundNode *compound = new CompoundNode();
    while(currentToken.type != EOF_TOKEN){
        compound->statements.push_back(statement());
    }
    return compound;
}

CompoundNode *Parser::block(){
    CompoundNode *compound = new CompoundNode();
    eat(LBRACE);
    while(currentToken.type != RBRACE){
        compound->statements.push_back(statement());
    }
    eat(RBRACE);
    return compound;
}