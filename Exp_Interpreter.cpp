#include "Exp_Interpreter.h"

//return Expression from string (which can be calculate)
Expression* Exp_Interpreter::interpret(string exp) {
    //get pointer to the expression and set iterator
    int n = exp.length();
    char *iter = &exp[0];

    //initialing vars
    bool lastWasNum = false;
    enum Ops op = NOT_OPERATOR;
    queue <double> calcQueue;
    queue <bool> is_op;
    stack <char> operators;
    string var;

    //while there are tokens to be read do:
    while ((iter[0] != ' ') && (iter[0] != '\n') && (iter[0] != 0)) {
        op = isOp(iter, lastWasNum);
        //if the token is an operator, then:
        if(op != NOT_OPERATOR) {
            //set lastWasNum to false
            lastWasNum = false;
            if ((op != LEFT_BRACKET) && (op != RIGHT_BRACKET)) {
                //while (there is an operator at the top of the operator stack with greater or equal precedence)
                //and (the operator at the top of the operator stack is not a left parenthesis):
                //move the top operator to the calcQueue
                while ((!operators.empty()) && precedence(isOp(&operators.top(), true)) >= precedence(op)) {
                    //push the operator to calcQueue
                    calcQueue.push(operators.top());
                    is_op.push(true);
                    //dlt the top operator
                    operators.pop();
                }
                if (op == U_MINUS) {
                    operators.push('@');
                }
                else if (op == U_PLUS) {
                    operators.push('#');
                }
                else {
                    operators.push(iter[0]);
                }
            }

                //if the token is a left bracket then: push it to operators stack
            else if(op == LEFT_BRACKET) {
                operators.push(iter[0]);
            }
                //if the token is a right bracket, then:
            else {
                //while the operator at the top of the operator stack is not a left paren:
                // pop the operator from the operator stack onto the output queue.
                while (!(operators.empty()) && (isOp(&operators.top(),true) != LEFT_BRACKET)) {
                    //push the operator to calcQueue
                    calcQueue.push(operators.top());
                    is_op.push(true);
                    //dlt the top operator
                    operators.pop();
                }
                //if the stack runs out without finding a left paren, then there are mismatched parentheses.
                if (operators.empty()) {
                    throw "no matching parentheses";
                }
                    //if there is a left paren at the top of the operator stack, then:
                else {
                    //pop the operator from the operator stack and discard it
                    operators.pop();
                }
            }
        }

            //else if the token is a number, then:
        else if (isdigit(iter[0])){
            lastWasNum = true;
            string num(1, iter[0]);
            //string chaining to the end of the number
            while(isdigit(iter[1]) || (iter[1] == '.')) {
                num += iter[1];
                iter++;
            }
            //push it to the output queue.
            if (num.length() > 0) {
                calcQueue.push(stod(num));
                is_op.push(false);
            }
            else throw "stod get emty string";

        }

            //else if the token is variable
        else if (isalpha(iter[0])) {
            lastWasNum = true;
            var ="";
            //get the variable name
            while(iter[0] && !(isOp(iter, true))){
                var += iter[0];
                iter++;
            }
            //get the value of the variable from the vars map (if there is none var - throw)
            double val = getVal(var);
            //store the string at calcQueue
            calcQueue.push(val);
            is_op.push(false);
            //back once so the iter++ wont skip any char
            iter--;
        }

        iter++;
    }

    //if there are no more tokens to read then:
    //while there are still operator tokens on the stack:
    while (!operators.empty()) {
        //if the operator token on the top of the stack is a paren., then there are mismatched parentheses.
        if ((isOp(&operators.top(),true) == LEFT_BRACKET) || (isOp(&operators.top(),true) == RIGHT_BRACKET)) {
            throw "no matching parentheses";
        }
        //pop the operator from the operator stack onto the output queue.
        //push the operator to calcQueue
        calcQueue.push(operators.top());
        is_op.push(true);
        //dlt the top operator
        operators.pop();
    }

    /* RPN translator  */
    //initiate vars
    stack <Expression*> expStack;
    Expression *right = nullptr, *left=nullptr;

    //as long as the queue is not empty
    while(!calcQueue.empty()) {
        //if the first token is number - push to expStack
        if (!is_op.front()){
            expStack.push(new Value(calcQueue.front()));
        }
        else{
            //if it is op - '*', '/', '+' or '-' - save the top two expressions
            char x = int(calcQueue.front());
            if ((x == '*') || (x == '/') || (x == '+') || (x == '-')) {
                if (expStack.empty()) { throw "illegal math expression 1"; }
                else {
                    left = expStack.top();
                    expStack.pop();
                    if (expStack.empty()) {throw "illegal math expression 2"; }
                    else {
                        right = expStack.top();
                        expStack.pop();
                    }
                }
            }

                //if it is UnaryOperator - save the top expression
            else if ((x == '@') || (x == '#')) {
                if (expStack.empty()) { throw "illegal math expression 3"; }
                left = expStack.top();
                expStack.pop();
            }

            //create the correct expression:
            switch (x) {
                case '+':
                    expStack.push(new Plus(right, left));
                    break;
                case '-':
                    expStack.push(new Minus(right, left));
                    break;
                case '*':
                    expStack.push(new Mul(right, left));
                    break;
                case '/':
                    expStack.push(new Div(right, left));
                    break;
                case '#':
                    expStack.push(new UPlus(left));
                    break;
                case '@':
                    expStack.push(new UMinus(left));
                    break;
                case '(':
                    //there is left bracket without right one!
                    throw "no matching parentheses";
                    break;
                default:
                    break;
            }
        }
        calcQueue.pop();
        is_op.pop();
    }
    return expStack.top();
}

//return enum to operator (if not operator return NOT_OPERATOR)
enum Ops isOp (const char *c, bool lastWasNum){
    if (c[0] == '+') { return PLUS;}
    if (c[0] == '-') {
        if (!lastWasNum) { return U_MINUS;}
        return MINUS;
    }
    if (c[0] == '*') { return MUL;}
    if (c[0] == '/') { return DIV;}
    if (c[0] == '(') { return LEFT_BRACKET;}
    if (c[0] == ')') { return RIGHT_BRACKET;}
    if (c[0] == '@') { return U_MINUS;}
    if (c[0] == '#') { return U_PLUS;}
    return NOT_OPERATOR;
}

//return the precedence of operator
int precedence(enum Ops op) {
    switch (op) {

        case MUL:
        case DIV:
        case U_PLUS:
        case U_MINUS:
            return 2;
        case PLUS:
        case MINUS:
            return 1;
        case LEFT_BRACKET:
            return 0;
        default:
            return NOT_OPERATOR;
    }
}

//get string and (if valid => "x=...;y=...;..." insert vars to Exp_Interpreter map
void Exp_Interpreter::setVariables(const string& s) {
    //get pointer to the expression
    int n = s.length();
    char *tmp = new char[n + 1];
    char *iter;
    strcpy(tmp, s.c_str());
    iter = tmp;
    int sign = 1;

    bool first = true;
    do {
        if (!first) { iter++;}
        string var_name;
        string val;
        var_name = "";
        val = "";

        if (isalpha(iter[0]) || iter[0] == '_'){
            while (isalpha(iter[0]) || iter[0] == '_' || isdigit(iter[0])){
                var_name += iter[0];
                iter++;
            }
            if (iter[0] == '=') {
                iter++;
                if (iter[0] == '-') {
                    sign = -1;
                    iter++;
                }
                if (!isdigit(iter[0])) {throw "illegal variable assignment!";}
                while (isdigit(iter[0]) || iter[0] == '.'){
                    val += iter[0];
                    iter++;
                }
            }
        }
        auto it = vars.find(var_name);
        if (it != vars.end() && val.length() > 0){
            it->second = stod(val) * sign;
        }
        else if (val.length() > 0) {vars.insert({var_name, stod(val) * sign});}
        else throw "stod get emty string";
        first = false;
    } while (iter[0] == ';');
    if (iter[0] != 0) {throw "Invalid input";}
    delete[] tmp;
}

//get the value of var
double Exp_Interpreter::getVal(const string& key) {
    //check for local (in the Exp_Interpreter) vars
    auto it = vars.find(key);
    if (it != vars.end()){
        return it->second;
    }
    else throw "using nonexistent variable";
}