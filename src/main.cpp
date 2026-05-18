#include<iostream>
#include<fstream>
#include<sstream>

#include "lexer.h"
#include "parser.h"
#include "interpreter.h"

int main(int argc, char* argv[]){
    // Check file argument
    if(argc < 2){
        std::cerr<<"Usage: velox <file.vlx>"<<std::endl;
        return 1;
    }

    // Open file
    std::ifstream file(argv[1]);

    // File not found
    if(!file.is_open()){
        std::cerr<<"Cannot open file: "<<argv[1]<<std::endl;
        return 1;
    }

    // Read full file content
    std::stringstream buffer;

    buffer<<file.rdbuf();

    std::string code = buffer.str();

    file.close();
    
    Lexer lexer(code);

    Parser parser(lexer);

    ASTNode *root = parser.parse();

    Interpreter interpreter;

    interpreter.visit(root);

    return 0;
}

// int main(){
//     std::string input = "", line;
//     // std::getline(std::cin, input);
//     std::cout<<"Enter the Velox Code (Type 'end' to finish).\n";
//     while(true){
//         std::getline(std::cin, line);
//         if(line == "end"){
//             break;
//         }
//         input += line + '\n';
//     }

//     Lexer lexer(input);

//     Parser parser(lexer);

//     ASTNode *tree = parser.parse();

//     Interpreter interpreter;

//     int result = interpreter.visit(tree);

//     // std::cout<<"Result: "<<result<<std::endl;

//     // Token token = lexer.getNextToken();
//     // while(token.type != EOF_TOKEN){
//     //     std::cout<<token.toString()<<" "<<token.value<<std::endl;
//     //     token = lexer.getNextToken();
//     // }

//     return 0;
// }