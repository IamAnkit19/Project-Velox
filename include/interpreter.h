#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "value.h"
#include "variable.h"
#include<map>
#include<string>
#include<vector>

class Interpreter{
public:
    bool breakFlag = false;
    bool continueFlag = false;
    bool returnFlag = false;
    Value returnValue = Value(0);
    std::vector<std::map<std::string, Variable>> scopes;
    std::map<std::string, FunctionDefNode*> functions;
    Interpreter();
    Value visit(ASTNode *node);
};

#endif