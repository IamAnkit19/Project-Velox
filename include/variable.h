#ifndef VARIABLE_H
#define VARIABLE_H

#include<string>
#include "value.h"

class Variable{
    public:
    std::string type;
    Value value;
    Variable(){}
    Variable(std::string t, Value v){
        type = t;
        value = v;
    }
};

#endif