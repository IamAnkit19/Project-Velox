#ifndef VALUE_H
#define VALUE_H

#include<string>

class Value{
    public:
    enum Type{
        INT,
        STRING
    };
    Type type;
    int intValue;
    std::string stringValue;
    // Int Constructor
    Value(int v){
        type = INT;
        intValue = v;
    }
    // String Constructor
    Value(std::string v){
        type = STRING;
        stringValue = v;
    }
    Value(){}
};

#endif