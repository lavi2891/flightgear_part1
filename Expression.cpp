#include "Variable.h"

/*Variable class*/

//overloading operators
//++
Variable& Variable:: operator ++(){
    value = value + 1;
    return *this;
}
//--
Variable& Variable:: operator --(){
    value = value - 1;
    return *this;
}
//++
Variable& Variable:: operator ++(int){
    value = value + 1;
    return *this;
}
//--
Variable& Variable:: operator --(int){
    Variable *cpyVar = new Variable(this);
    value = value - 1;
    return *cpyVar;
}
//+=
Variable& Variable:: operator +=(double x){
    value = value + x;
    return *this;
}
//-=
Variable& Variable:: operator -=(double x){
    value = value - x;
    return *this;
}

//calc.
double Variable::calculate() {
    return value;
}

/*UnaryOperator*/
double UPlus::calculate() {
    return e->calculate();
}

double UMinus::calculate() {
    return e->calculate()*-1;
}

/*BinaryOperator*/
double Mul::calculate() {
    return left->calculate() * right->calculate();
}

double Div::calculate() {
    if (right->calculate() == 0) {throw "division by zero";}
    return left->calculate() / right->calculate();
}

double Minus::calculate() {
    return left->calculate() - right->calculate();
}

double Plus::calculate() {
    return left->calculate() + right->calculate();
}

//Value
double Value::calculate() {
    return val;
}
