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
    while(currentChar != '\0' && (isalnum(currentChar) || currentChar == '_')){
        result += currentChar;
        advance();
    }
    if(result == "int"){
        return Token(INT, result);
    }
    if(result == "string"){
        return Token(STRING_TYPE, result);
    }
    else if(result == "print"){
        return Token(PRINT, result);
    }
    else if(result == "if"){
        return Token(IF, result);
    }
    else if(result == "else"){
        return Token(ELSE, result);
    }
    else if(result == "while"){
        return Token(WHILE, result);
    }
    else if(result == "for"){
        return Token(FOR, result);
    }
    else if(result == "break"){
        return Token(BREAK, result);
    }
    else if(result == "continue"){
        return Token(CONTINUE, result);
    }
    else if(result == "return"){
        return Token(RETURN, result);
    }
    else if(result == "func"){
        return Token(FUNC, result);
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

Token Lexer::string(){
    std::string result = "";
    advance();
    while(currentChar != '"' && currentChar != '\0'){
        if(currentChar == '\\'){
            advance();
            if(currentChar == 'n'){
                result += '\n';
            }
            else if(currentChar == 't'){
                result += '\t';
            }
            else if(currentChar == '"'){
                result += '"';
            }
            else if(currentChar == '\\'){
                result += '\\';
            }
            else{
                result += currentChar;
            }
        }
        else{
            result += currentChar;
        }
        advance();
    }
    if(currentChar == '\0'){
        std::cerr<<"Unterminated string!"<<std::endl;
        exit(1);
    }
    advance();
    return Token(STRING, result);
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
        if(currentChar == '"'){
            return string();
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
        // Single Line Comment
        if(currentChar == '/' && peek() == '/'){
            while(currentChar != '\n' && currentChar != '\0'){
                advance();
            }
            continue;
        }
        // Multi Line Comment
        if(currentChar == '/' && peek() == '*'){
            advance();
            advance();
            bool closed = false;
            while(currentChar != '\0'){
                if(currentChar == '*' && peek() == '/'){
                    advance();
                    advance();
                    closed = true;
                    break;
                }
                advance();
            }
            if(!closed){
                std::cerr<<"Undetermined Multiline Comment!"<<std::endl;
                exit(1);
            }
            continue;
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
        if(currentChar == '{'){
            advance();
            return Token(LBRACE, "{");
        }
        if(currentChar == '}'){
            advance();
            return Token(RBRACE, "}");
        }
        if(currentChar == ','){
            advance();
            return Token(COMMA, ",");
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
            advance();
            return Token(NOT, "!");
        }
        if(currentChar == '&'){
            if(peek() == '&'){
                advance();
                advance();
                return Token(AND_AND, "&&");
            }
        }
        if(currentChar == '|'){
            if(peek() == '|'){
                advance();
                advance();
                return Token(OR_OR, "||");
            }
        }

        std::cerr<<"Error: Invalid Character -> "<<currentChar<<std::endl;
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