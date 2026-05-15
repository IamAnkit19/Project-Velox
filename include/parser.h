#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"

class Parser{
private:
    Lexer lexer;
    Token currentToken;
    Token nextToken;
    void eat(TokenType type);
    ASTNode *factor();
    ASTNode *term(); // Handles multiplication/division
    ASTNode *expr(); // Handles addition/subtraction
    ASTNode *statement();
    ASTNode *comparison();
    CompoundNode *block();
    ASTNode *logicalAND();
    ASTNode *logicalOR();
public:
    Parser(Lexer l);
    ASTNode *parse();
};

#endif