#include<iostream>
#include<string>
#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

int main(){
    std::string input = "", line;
    // std::getline(std::cin, input);
    std::cout<<"Enter the Velox Code (Type 'end' to finish).\n";
    while(true){
        std::getline(std::cin, line);
        if(line == "end"){
            break;
        }
        input += line + '\n';
    }

    Lexer lexer(input);

    Parser parser(lexer);

    ASTNode *tree = parser.parse();

    Interpreter interpreter;

    int result = interpreter.visit(tree);

    // std::cout<<"Result: "<<result<<std::endl;

    // Token token = lexer.getNextToken();
    // while(token.type != EOF_TOKEN){
    //     std::cout<<token.toString()<<" "<<token.value<<std::endl;
    //     token = lexer.getNextToken();
    // }

    return 0;
}