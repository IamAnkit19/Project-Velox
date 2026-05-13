#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include<map>
#include<string>

class Interpreter{
public:
    std::map<std::string, int> variables;
    int visit(ASTNode *node);
};

#endif