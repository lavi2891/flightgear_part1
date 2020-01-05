#ifndef UNTITLED_INTERPRETER_H
#define UNTITLED_INTERPRETER_H

#include <iostream>
#include<string>
#include<cstring>
#include<stack>
#include <queue>
#include <map>
#include <sstream>
#include "Expression.h"
#include "Variable.h"

using namespace std;

enum Ops {NOT_OPERATOR, PLUS, MINUS, MUL, DIV, U_MINUS, U_PLUS, LEFT_BRACKET, RIGHT_BRACKET};

class Exp_Interpreter{
    map <string, double> vars;
public:
    Expression* interpret(string exp);
    void setVariables(const string& s);
    double getVal(const string& key);
};

enum Ops isOp(const char *c, bool lastWasNum);
int precedence(enum Ops op);

#endif
