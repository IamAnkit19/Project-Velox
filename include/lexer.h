#ifndef LEXER_H
#define LEXER_H

#include<string>
#include "token.h"

class Lexer{
    std::string text; // Input code
    int pos; // Current position
    char currentChar; // Current character

public:
    Lexer(std::string input);

    // Move to next character
    void advance();

    // Skip spaces
    void skipWhitespace();

    // Read Identifiers or Keywords
    Token identifier();

    // Read Number
    Token number();

    // Main function to get next token
    Token getNextToken();

    // See Next Char
    char peek();
};

#endif