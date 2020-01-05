#ifndef PROJECT_PART_1_CONDITION_H
#define PROJECT_PART_1_CONDITION_H

#include <string>
#include "Global.h"
#include "Expression.h"
#include "Exp_Interpreter.h"

//Condition class
class Condition {
private:
    map <string, Variable*> *symbol_map;
    string left_exp;
    string right_exp;
    int cond;
public:
    Condition(string s, map<string, Variable*> *input_map);
    ~Condition() = default;
    bool is_true();
};


#endif //PROJECT_PART_1_CONDITION_H
