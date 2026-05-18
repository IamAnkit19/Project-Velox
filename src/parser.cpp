#include "parser.h"
#include<iostream>

Parser::Parser(Lexer l) : lexer(l){
    currentToken = lexer.getNextToken();
    nextToken = lexer.getNextToken();
}

void Parser::eat(TokenType type){
    if(currentToken.type == type){
        currentToken = nextToken;
        nextToken = lexer.getNextToken();
    }
    else{
        std::cerr<<"Syntax Error!"<<std::endl;
        exit(1);
    }
}

ASTNode *Parser::factor(){
    Token token = currentToken;
    if(token.type == NOT){
        eat(NOT);
        ASTNode *node = factor();
        return new UnaryOpNode(NOT, node);
    }
    if(token.type == NUMBER){
        eat(NUMBER);
        return new NumberNode(token.value);
    }
    else if(token.type == STRING){
        eat(STRING);
        return new StringNode(token.value);
    }
    else if(token.type == IDENTIFIER){
        // Function call
        if(nextToken.type == LPAREN){
            std::string funcName = token.value;
            eat(IDENTIFIER);
            eat(LPAREN);
            std::vector<ASTNode*> arguments;
            if(currentToken.type != RPAREN){
                arguments.push_back(logicalOR());
                while(currentToken.type == COMMA){
                    eat(COMMA);
                    arguments.push_back(logicalOR());
                }
            }
            eat(RPAREN);
            return new FunctionCallNode(funcName, arguments);
        }
        // Variable access
        eat(IDENTIFIER);
        return new VariableNode(token.value);
    }
    else if(token.type == LPAREN){
        eat(LPAREN);
        ASTNode *node = logicalOR();
        eat(RPAREN);
        return node;
    }
    std::cerr<<"Invalid factor!"<<std::endl;
    exit(1);
}

ASTNode *Parser::term(){
    ASTNode *left = factor();
    while(currentToken.type == MULTIPLY || currentToken.type == DIVIDE || currentToken.type == MOD){
        Token op = currentToken;
        if(op.type == MULTIPLY){
            eat(MULTIPLY);
        }
        else if(op.type == DIVIDE){
            eat(DIVIDE);
        }
        else if(op.type == MOD){
            eat(MOD);
        }
        ASTNode *right = factor();
        left = new BinaryOpNode(left, op.type, right);
    }
    return left;
}

ASTNode *Parser::expr(){
    ASTNode *left = term();
    while(currentToken.type == PLUS || currentToken.type == MINUS){
        Token op = currentToken;
        if(op.type == PLUS){
            eat(PLUS);
        }
        else if(op.type == MINUS){
            eat(MINUS);
        }
        ASTNode *right = term();
        left = new BinaryOpNode(left, op.type, right);
    }
    return left;
}

ASTNode *Parser::comparison(){
    ASTNode *left = expr();
    while(
        currentToken.type == GREATER ||
        currentToken.type == LESS ||
        currentToken.type == GREATER_EQUAL ||
        currentToken.type == LESS_EQUAL ||
        currentToken.type == EQUAL_EQUAL ||
        currentToken.type == NOT_EQUAL
    ){
        Token op = currentToken;
        if(op.type == GREATER){
            eat(GREATER);
        }
        else if(op.type == GREATER_EQUAL){
            eat(GREATER_EQUAL);
        }
        else if(op.type == LESS){
            eat(LESS);
        }
        else if(op.type == LESS_EQUAL){
            eat(LESS_EQUAL);
        }
        else if(op.type == EQUAL_EQUAL){
            eat(EQUAL_EQUAL);
        }
        else if(op.type == NOT_EQUAL){
            eat(NOT_EQUAL);
        }
        ASTNode *right = expr();
        left = new BinaryOpNode(left, op.type, right);
    }
    return left;
}

ASTNode *Parser::statement(){
    if(currentToken.type == FUNC){
        eat(FUNC);
        Token funcName = currentToken;
        eat(IDENTIFIER);
        eat(LPAREN);
        std::vector<std::string> params;
        if(currentToken.type != RPAREN){
            params.push_back(currentToken.value);
            eat(IDENTIFIER);
            while(currentToken.type == COMMA){
                eat(COMMA);
                params.push_back(currentToken.value);
                eat(IDENTIFIER);
            }
        }
        eat(RPAREN);
        CompoundNode *body = block();
        return new FunctionDefNode(funcName.value, params, body);
    }
    if(currentToken.type == IF){
        eat(IF);
        eat(LPAREN);
        ASTNode *condition = logicalOR();
        eat(RPAREN);
        CompoundNode *ifBody = block();
        ASTNode *elseBody = nullptr;
        if(currentToken.type == ELSE){
            eat(ELSE);
            if(currentToken.type == IF){
                elseBody = statement();
            }
            else{
                elseBody = block();
            }
        }
        return new IfNode(condition, ifBody, elseBody);
    }
    if(currentToken.type == FOR){
        eat(FOR);
        eat(LPAREN);
        ASTNode *init = statement();
        ASTNode *condition = logicalOR();
        eat(SEMICOLON);
        ASTNode *update = statement();
        eat(RPAREN);
        CompoundNode *body = block();
        return new ForNode(init, condition, update, body);
    }
    if(currentToken.type == WHILE){
        eat(WHILE);
        eat(LPAREN);
        ASTNode *condition = logicalOR();
        eat(RPAREN);
        CompoundNode *body = block();
        return new WhileNode(condition, body);
    }
    if(currentToken.type == BREAK){
        eat(BREAK);
        eat(SEMICOLON);
        return new BreakNode();
    }
    if(currentToken.type == CONTINUE){
        eat(CONTINUE);
        eat(SEMICOLON);
        return new ContinueNode();
    }
    if(currentToken.type == RETURN){
        eat(RETURN);
        ASTNode *value = logicalOR();
        eat(SEMICOLON);
        return new ReturnNode(value);
    }
    if(currentToken.type == INT){
        eat(INT);
        Token varToken = currentToken;
        eat(IDENTIFIER);
        eat(ASSIGN);
        ASTNode *value = logicalOR();
        eat(SEMICOLON);
        return new VarAssignNode(varToken.value, value, true);
    }
    if(currentToken.type == PRINT){
        eat(PRINT);
        eat(LPAREN);
        ASTNode *expression = logicalOR();
        eat(RPAREN);
        eat(SEMICOLON);
        return new PrintNode(expression);
    }
    if(currentToken.type == IDENTIFIER && nextToken.type == ASSIGN){
        Token varToken = currentToken;
        eat(IDENTIFIER);
        eat(ASSIGN);
        ASTNode *value = logicalOR();
        eat(SEMICOLON);
        return new VarAssignNode(varToken.value, value, false);
    }
    ASTNode *node = logicalOR();
    eat(SEMICOLON);
    return node;
}

ASTNode *Parser::logicalAND(){
    ASTNode *left = comparison();
    while(currentToken.type == AND_AND){
        Token op = currentToken;
        eat(AND_AND);
        ASTNode *right = comparison();
        left = new BinaryOpNode(left, op.type, right);
    }
    return left;
}

ASTNode *Parser::logicalOR(){
    ASTNode *left = logicalAND();
    while(currentToken.type == OR_OR){
        Token op = currentToken;
        eat(OR_OR);
        ASTNode *right = logicalAND();
        left = new BinaryOpNode(left, op.type, right);
    }
    return left;
}

ASTNode *Parser::parse(){
    CompoundNode *compound = new CompoundNode();
    while(currentToken.type != EOF_TOKEN){
        compound->statements.push_back(statement());
    }
    return compound;
}

CompoundNode *Parser::block(){
    CompoundNode *compound = new CompoundNode();
    eat(LBRACE);
    while(currentToken.type != RBRACE){
        compound->statements.push_back(statement());
    }
    eat(RBRACE);
    return compound;
}