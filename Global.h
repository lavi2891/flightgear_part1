#ifndef PROJECT_PART_1_GLOBAL_H
#define PROJECT_PART_1_GLOBAL_H

#include <mutex>
#include <string>
#include <map>
#include <list>
#include <thread>
#include <vector>
#include "Variable.h"

enum condition {BIGGER, BIG_EQUAL, SMALLER, SMALL_EQUAL, EQUAL, NOT_EQUAL};

//global variables declaration
extern mutex connection_mutex;
extern mutex send_list_mutex;
extern mutex generic_mutex;
extern bool is_finished;
extern map <string, double > generic_small_map;
extern list <string> generic_insertion_order;
extern list <string> send_list;
extern vector <thread> vecOfThreads;

#endif //PROJECT_PART_1_GLOBAL_H
