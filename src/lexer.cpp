#include "lexer.h"
#include "token.h"
#include<string>
#include<iostream>
#include<cctype>

Lexer::Lexer(std::string input){
    text = input;
    pos = 0;
    if(!text.empty()){
        currentChar = text[pos];
    }
    else{
        currentChar = '\0';
    }
}

void Lexer::advance(){
    pos++;
    if(pos >= text.length()){
        currentChar = '\0';
    }
    else{
        currentChar = text[pos];
    }
}

void Lexer::skipWhitespace(){
    while(currentChar == ' ' || currentChar == '\t' || currentChar == '\n'){
        advance();
    }
}

Token Lexer::identifier(){
    std::string result = "";
    while(currentChar != '\0' && isalnum(currentChar)){
        result += currentChar;
        advance();
    }
    if(result == "int"){
        return Token(INT, result);
    }
    else if(result == "print"){
        return Token(PRINT, result);
    }
    return Token(IDENTIFIER, result);
}

Token Lexer::number(){
    std::string result = "";
    while(currentChar != '\0' && isdigit(currentChar)){
        result += currentChar;
        advance();
    }
    return Token(NUMBER, result);
}

Token Lexer::getNextToken(){
    while(currentChar != '\0'){
        if(currentChar == ' ' || currentChar == '\t' || currentChar == '\n'){
            skipWhitespace();
            continue;
        }
        if(isalpha(currentChar)){
            return identifier();
        }
        if(isdigit(currentChar)){
            return number();
        }
        
        if(currentChar == '+'){
            advance();
            return Token(PLUS, "+");
        }
        if(currentChar == '-'){
            advance();
            return Token(MINUS, "-");
        }
        if(currentChar == '*'){
            advance();
            return Token(MULTIPLY, "*");
        }
        if(currentChar == '/'){
            advance();
            return Token(DIVIDE, "/");
        }
        if(currentChar == '%'){
            advance();
            return Token(MOD, "%");
        }

        if(currentChar == '='){
            if(peek() == '='){
                advance();
                advance();
                return Token(EQUAL_EQUAL, "==");
            }
            advance();
            return Token(ASSIGN, "=");
        }
        if(currentChar == ';'){
            advance();
            return Token(SEMICOLON, ";");
        }
        if(currentChar == '('){
            advance();
            return Token(LPAREN, "(");
        }
        if(currentChar == ')'){
            advance();
            return Token(RPAREN, ")");
        }
        if(currentChar == '>'){
            if(peek() == '='){
                advance();
                advance();
                return Token(GREATER_EQUAL, ">=");
            }
            advance();
            return Token(GREATER, ">");
        }
        if(currentChar == '<'){
            if(peek() == '='){
                advance();
                advance();
                return Token(LESS_EQUAL, "<=");
            }
            advance();
            return Token(LESS, "<");
        }
        if(currentChar == '!'){
            if(peek() == '='){
                advance();
                advance();
                return Token(NOT_EQUAL, "!=");
            }
            // Invalid
            break;
        }

        std::cout<<"Error: Invalid Character -> "<<currentChar<<std::endl;
        exit(1);
    }
    return Token(EOF_TOKEN, "");
}

char Lexer::peek(){
    if(pos + 1 >= text.length()){
        return '\0';
    }
    return text[pos+1];
}