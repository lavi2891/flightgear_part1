#include "Condition.h"

void set_vars(string input, Exp_Interpreter *inter, map<string, Variable*> *symbol_map);
/*
 * Condition functions
 */
bool Condition::is_true()  {

    //set expressions and interpreter
    auto *i = new Exp_Interpreter();
    Expression *left, *right;

    //set the variables in interpreter
    set_vars(right_exp, i, symbol_map);
    set_vars(left_exp, i, symbol_map);

    //get the expressions
    right = i->interpret(right_exp);
    left = i->interpret(left_exp);

    //caculate the right and left expression
    double left_dbl = left->calculate();
    double right_dbl = right->calculate();

    //deletes the expressions and interpreter
    delete i;
    delete right;
    delete left;

    //return true or false according to left ? right
    switch (cond) {
        case BIGGER:
            return left_dbl > right_dbl;
        case BIG_EQUAL:
            return left_dbl >= right_dbl;
        case SMALLER:
            return left_dbl < right_dbl;
        case SMALL_EQUAL:
            return left_dbl <= right_dbl;
        case EQUAL:
            return left_dbl == right_dbl;
        case NOT_EQUAL:
            return left_dbl != right_dbl;
        default:
            cerr << "unknown condition" << endl;
            return false;
    }
}

//constructor by string and
Condition::Condition(string input_str, map <string, Variable*> *input_map) {
    //initiate vars
    int i = 0;
    char c = input_str[i];
    string s = "";

    //give condition the matching symbol_map
    symbol_map = input_map;

    //get left_exp: until condition's operator
    while (c != '!' && c != '<' && c != '>' && c != '=' ) {
        //ignore spaces
        if (c != ' ') {s += c;}
        //go to next char
        i++;
        c = input_str[i];
    }
    left_exp = s;

    //get condition
    switch (c) {
        case '!':
            if (input_str[i+1] == '=') {
                i++;
                cond = NOT_EQUAL;
            }
            else {cerr << "invalid condition sign, no '!' yet" << endl;}
            break;
        case '=':
            if (input_str[i+1] == '=') {
                i++;
                cond = EQUAL;
            }
            else {cerr << "invalid condition sign '=' is not condition!" << endl;}
            break;
        case '<':
            if (input_str[i+1] == '=') {
                i++;
                cond = SMALL_EQUAL;
            }
            else {
                cond = SMALLER;
            }
            break;
        case '>':
            if (input_str[i+1] == '=') {
                i++;
                cond = BIG_EQUAL;
            }
            else {
                cond = BIGGER;
            }
            break;
        default:
            break;
    }

    //get right_exp
    s = "";
    i++;
    c = input_str[i];
    while (c != '{') {
        if (c != ' ') {s += c;}
        i++;
        c = input_str[i];
    }
    right_exp = s;
}

//get string, interpreter, and map of values and translate every variable name in string to Variable in interpreter
//the values in the map
void set_vars(string input, Exp_Interpreter *inter, map<string, Variable*> *symbol_map) {
    //initiate vars
    int j = 0;
    bool flag;
    string var_name, generic_key, val;

    //iterate over the string
    while (j < input.length()) {
        var_name = "";
        flag = false;
        //if the first char is letter - it is variable - read it as long it is letter, digit or '_'
        if (isalpha(input[j])) { flag = true; }
        while (flag && j < input.length() && (isalpha(input[j]) || isdigit(input[j]) || input[j] == '_')){
            var_name += input[j];
            j++;
        }
        //if there is variable name
        if (var_name.length() > 0) {
            //find the variable at symbol_map
            auto current = symbol_map->find(var_name);
            //if it is indeed in the map
            if (current != symbol_map->end()) {
                generic_key = current->second->get_name();
                //if the variable is update by the sim type: set the value by generic_small_map
                if (current->second->is_from_sim()) {

                    generic_mutex.lock();
                    //find the value in the generic_small_map
                    auto generic = generic_small_map.find(generic_key);
                    if (generic != generic_small_map.end()) {
                        //update symbol_map by this value
                        current->second->set_val(generic->second);
                        generic_mutex.unlock();

                        //val get the value of the symbol map variable (which is now up to date)
                        val = to_string(symbol_map->at(var_name)->calculate());
                        try {
                            //set the variable at the interpreter
                            var_name += "=" + val;
                            inter->setVariables(var_name);
                        }
                        catch (const char* e) {
                            cout << e << endl;
                        }

                    }
                    //if didnt found such var at generic_small_map:
                    else { cerr << "Didnt found var_name at generic_small_map :(" << endl; }
                }
                //if the variable is not from_sim type
                else {

                    //set the variable at the interpreter by symbol_map value
                    val = to_string(current->second->calculate());
                    try {
                        var_name += "=" + val;
                        inter->setVariables(var_name);
                    }
                    catch (const char* e) {
                        cout << e << endl;
                    }
                }
            }
            //if there is no such variable in symbol map:
            else {
                cerr << "Didnt found var_name at symbol_map :(" << endl;
            }
        }
        //go to next char
        j++;
    }
}