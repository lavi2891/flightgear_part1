//
// Created by lavi on 22/12/2019.
//

#ifndef PROJECT_PART_1_COMMAND_H
#define PROJECT_PART_1_COMMAND_H

#include <string>
#include <thread>
#include <vector>
#include <mutex>
#include <list>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Global.h"
#include "Expression.h"
#include "Exp_Interpreter.h"
#include "Condition.h"
#include "main.h"


using namespace std;

void server(const string& port);
void client(string ip, int port);
void set_vars_to_interpreter(string input, Exp_Interpreter *inter, map<string, Variable*> *symbol_map);
string del_quot(const string& s);
string del_sim(const string& s);

//Command Interface
class Command {
public:
    virtual int execute() {};
    virtual ~Command() = default;
};

//scope
class Scope: public Command {
private:
    list <Command*> cmd_list;
    map <string, Variable*> symbol_map;
    map <string, Variable*> *upper_symbol_map;
public:
    int execute();
    //constructor by file path
    Scope(const string& file_path);
    //constructor by map*
    Scope(map <string, Variable*> *input_map) { upper_symbol_map = input_map; };
    //destructor - still need some work
    ~Scope(){
        for (auto & iter: cmd_list) {
            delete iter;
        }
        for (auto & iter: symbol_map){
            //if upper map is empty || no such variable in upper map || not the same variable as upper map
            if (upper_symbol_map == nullptr || upper_symbol_map->find(iter.first) == upper_symbol_map->end() ||
                    (*upper_symbol_map)[iter.first]->calculate() != symbol_map[iter.first]->calculate()){
                delete iter.second;
            }
        }
    };

    void del_upper_map() {
        delete upper_symbol_map;
        upper_symbol_map = nullptr;
    }
    //return pointers to maps
    map <string, Variable*> *get_symbol_table() {return &symbol_map;};
    //set vector by file
    static void lexer(const string& file_name, vector<string> *v);
    static void condition_lexer(ifstream *fptr, vector<string> *v);
    //set cmd_list by vector
    void parser(vector<string> v, int *iter);
};

//OpenServerCommand class
class OpenServerCommand: public Command {
private:
    string port;
public:
    OpenServerCommand(string p): port(p){};
    ~OpenServerCommand() override= default;
    int execute() override;
};

//ConnectCommand class
class ConnectCommand: public Command {
private:
    string ip_port;
public:
    ConnectCommand(string ip): ip_port(ip){};
    ~ConnectCommand() override= default;
    int execute() override;

};

//DefineVarCommand class
class DefineVarCommand: public Command {
    string data;
    map <string, Variable*> *symbol_map;
public:
    DefineVarCommand(string d, map<string, Variable*> *input_map) : data(d) {
        symbol_map = input_map;
    };
    ~DefineVarCommand() override = default;
    int execute() override;
};

//SetVarCommand class
class SetVarCommand: public Command {
    string data;
    map <string, Variable*> *symbol_map;
public:
    SetVarCommand(string d, map<string, Variable*> *input_map): data(d){
        symbol_map = input_map;
    };
    ~SetVarCommand() override = default;
    int execute() override;
};

//PrintCommand class
class PrintCommand: public Command {
    string data;
    map <string, Variable*> *symbol_map;
public:
    PrintCommand(string d, map <string, Variable*> *input_map): data(d){
        data = data.substr(1,data.length() - 2);
        symbol_map = input_map;
    };
    ~PrintCommand() override = default;
    int execute() override;
};

//SleepCommand class
class SleepCommand: public Command {
    string data;
    map <string, Variable*> *symbol_map;
public:
    SleepCommand(string t, map <string, Variable*> *input_map): data(t){
        symbol_map = input_map;
    };
    ~SleepCommand() override = default;
    int execute() override;
};

//WhileCommand class
class WhileCommand: public Command {
private:
    Condition *cond;
    Scope *cmd_scope;
public:
    WhileCommand(string con, map <string, Variable*> *input_map) {
        cond = new Condition(con, input_map);
        cmd_scope = new Scope(input_map);
    }

    int execute();
    ~WhileCommand(){
        delete cond;
        delete cmd_scope;
    };
    Scope* get_scope() {return cmd_scope;}
};

//IfCommand class
class IfCommand: public Command {
private:
    Condition *cond;
    Scope *cmd_scope;
public:
    IfCommand(string con, map <string, Variable*> *input_map) {
        cond = new Condition(con, input_map);
        cmd_scope = new Scope(input_map);
    }
    int execute();
    ~IfCommand(){
        delete cond;
        delete cmd_scope;
    };
    Scope* get_scope() {return cmd_scope;}
};
#endif //PROJECT_PART_1_COMMAND_H
