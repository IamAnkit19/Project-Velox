#ifndef TOKEN_H
#define TOKEN_H

#include<string>

enum TokenType{
    // Keywords
    INT,
    PRINT,
    IF,

    // Identifiers and Literals
    IDENTIFIER,
    NUMBER,

    // Operators
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MOD,

    // Symbols
    ASSIGN,
    SEMICOLON,
    LPAREN,
    RPAREN,
    LBRACE,
    RBRACE,

    GREATER,
    LESS,
    GREATER_EQUAL,
    LESS_EQUAL,
    EQUAL_EQUAL,
    NOT_EQUAL,


    EOF_TOKEN
};

struct Token{
    TokenType type;
    std::string value;

    Token(){
        type = EOF_TOKEN;
        value = "";
    }

    Token(TokenType t, std::string v){
        type = t;
        value = v;
    }

    std::string toString(){
        switch(type){
            case INT: return "INT";
            case PRINT: return "PRINT";
            case IF: return "IF";
            case IDENTIFIER: return "IDENTIFIER";
            case NUMBER: return "NUMBER";
            case PLUS: return "PLUS";
            case MINUS: return "MINUS";
            case MULTIPLY: return "MULTIPLY";
            case DIVIDE: return "DIVIDE";
            case MOD: return "MOD";
            case ASSIGN: return "ASSIGN";
            case SEMICOLON: return "SEMICOLON";
            case LPAREN: return "LPAREN";
            case RPAREN: return "RPAREN";
            case LBRACE: return "LBRACE";
            case RBRACE: return "RBRACE";
            case EOF_TOKEN: return "EOF_TOKEN";
            case GREATER: return "GREATER";
            case GREATER_EQUAL: return "GREATER_EQUAL";
            case LESS: return "LESS";
            case LESS_EQUAL: return "LESS_EQUAL";
            case EQUAL_EQUAL: return "EQUAL_EQUAL";
            case NOT_EQUAL: return "NOT_EQUAL";
        }
    }
};

#endif