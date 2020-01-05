#ifndef PROJECT_PART_1_VARIABLE_H
#define PROJECT_PART_1_VARIABLE_H

#include <string>
#include "Expression.h"
/*Variable class*/
class Variable: public Expression{
    //class fields
    string name;
    double value;
    bool toSim = false;
    bool fromSim = false;
public:
    //constructors declaration
    Variable(Variable *var) : name(var->name), value(var->value) {}
    Variable(string inName, double inValue) : name(inName), value(inValue) {}

    //destructors
    virtual ~Variable(){}

    //class (overloading) operators
    Variable& operator ++();
    Variable& operator --();
    Variable& operator ++(int);
    Variable& operator --(int);
    Variable& operator +=(double x);
    Variable& operator -=(double x);

    //calculate
    double calculate();

    //setters
    void set_val(double val) {this->value = val;}
    void to_sim() { toSim = true; }
    void from_sim() { fromSim = true; }
    bool is_to_sim() {return toSim;}
    bool is_from_sim() {return fromSim;}
    string get_name() { return name;}
};

class UnaryOperator: public Expression {
protected:
    Expression *e;
public:
    //constructor
    UnaryOperator(Expression *exp) : e(exp){}
    //destructor
    virtual ~UnaryOperator(){};
};

class UPlus: public UnaryOperator{
    Expression *e;
public:
    //constructor
    UPlus(Expression *exp) : UnaryOperator(exp), e(exp){}
    //destructor
    virtual ~UPlus(){
        delete e;
    }
    //calculate
    double calculate();
};

class UMinus: public UnaryOperator{
    Expression *e;
public:
    //constructor
    UMinus(Expression *exp) : UnaryOperator(exp), e(exp){}
    //destructor
    virtual ~UMinus(){
        delete e;
    }
    //calculate
    double calculate();
};

/*BinaryOperator*/
class BinaryOperator: public Expression{
protected:
    Expression *left;
    Expression *right;
public:
    //constructor
    BinaryOperator(Expression *inLeft, Expression *inRight): left(inLeft), right(inRight){}
    //destructor
    virtual ~BinaryOperator(){};
};


class Plus: public BinaryOperator{
    Expression *left;
    Expression *right;
public:
    //constructor
    Plus(Expression *inLeft, Expression *inRight):BinaryOperator(inLeft, inRight), left(inLeft), right(inRight){}
    //destructor
    virtual ~Plus(){
        delete left;
        delete right;
    };
    //calculate
    double calculate();
};

class Minus: public BinaryOperator{
    Expression *left;
    Expression *right;
public:
    //constructor
    Minus(Expression *inLeft, Expression *inRight):BinaryOperator(inLeft, inRight), left(inLeft), right(inRight){};
    //destructor
    virtual ~Minus(){
        delete left;
        delete right;
    }
    //calculate
    double calculate();
};

class Div: public BinaryOperator{
    Expression *left;
    Expression *right;
public:
    //constructor
    Div(Expression *inLeft, Expression *inRight):BinaryOperator(inLeft, inRight), left(inLeft), right(inRight){}
    //destructor
    virtual ~Div(){
        delete left;
        delete right;
    };
    //calculate
    double calculate();
};

class Mul: public BinaryOperator{
    Expression *left;
    Expression *right;
public:
    //constructor
    Mul(Expression *inLeft, Expression *inRight): BinaryOperator(inLeft, inRight), left(inLeft), right(inRight){}
    //destructor
    virtual ~Mul(){
        delete left;
        delete right;
    };
    //calculate
    double calculate();
};

//Value
class Value: public Expression{
    double val;
public:
    //constructor
    Value(double inVal) : val(inVal){}
    //destructor
    virtual ~Value(){};
    //calculate
    double calculate();
};

#endif //PROJECT_PART_1_VARIABLE_H
