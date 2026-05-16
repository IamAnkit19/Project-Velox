#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include<map>
#include<string>
#include<vector>

class Interpreter{
public:
    bool breakFlag = false;
    bool continueFlag = false;
    bool returnFlag = false;
    int returnValue = 0;
    std::vector<std::map<std::string, int>> scopes;
    Interpreter();
    int visit(ASTNode *node);
};

#endif