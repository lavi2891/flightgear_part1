#include "Command.h"
#include "Condition.h"
/*
 * Scope functions
 */
int Scope::execute() {
    if (!upper_symbol_map->empty()) {
        symbol_map.insert(upper_symbol_map->begin(), upper_symbol_map->end());
    }
    for (auto & iter : cmd_list) {
        (*iter).execute();
    }
    return cmd_list.size();
}

//take txt file, split it and store in vector
void Scope::lexer(const string& file_name, vector<string> *v){
    char c;
    bool finish = false;
    ifstream fptr;
    fptr.open (file_name, ios::in);
    if (!fptr.is_open()) {
        cerr << "could not find the file" << endl;
    }
    else {
        while (fptr.get(c)) {
            //initiate strings
            string s = "";
            string s2 = "";

            //start at the start of the written line
            while (c == ' ' || c == '\t' || c == ')' || c == '(' || c == '\n') {
                if (!fptr.get(c)) {
                    finish = true;
                }
            }
            if (finish) { break; }

            //get the 1st word
            while (c != ' ' && c != '\t' && c != '(' && c != '\n') {
                s += c;
                if (!fptr.get(c)) { finish = true; }
            }
            if (finish) {
                if ( s.length() > 0) {v->push_back(s);}
                break;
            }

            //if all the line is without spaces - next line
            if (c == '\n') {
                if ( s.length() > 0) {v->push_back(s);}
                continue;
            }
            //if it's "" - continue
            if (s.length() < 1) { continue;}

            //save the rest of the line according to 1st word:
            //if it is "var" - save the rest of the line with "," in-between
            else if (s == "var") {
                while (c != '\n') {
                    if (c == ' ') {}
                    else if ((c == '-' && fptr.peek() == '>') || (c == '<' && fptr.peek() == '-')) {
                        s2 += ",";
                        s2 += c;
                        if (!fptr.get(c)) {
                            finish = true;
                            break;
                        }
                        s2 += c;
                        s2 += ",";
                    }
                    else if (c == '=') {
                        s2 += ",";
                        s2 += c;
                        s2 += ",";
                    }
                    else { s2 += c; }
                    if (!fptr.get(c)) {
                        finish = true;
                        break;
                    }
                }
            }

            //"Print" -  we will need the spaces
            else if (s == "Print"){
                while (c != '(') { fptr.get(c); }
                s2 += c;
                while ( fptr.get(c) && c != '\n' ) {
                    s2 += c;
                }

            }

            //"openDataServer" or "connectControlClient" - save the expression(s)
            else if (s == "openDataServer" || s == "connectControlClient" || s == "Sleep"){
                while (c != '(') { fptr.get(c); }
                while (c != '\n') {
                    if (c != ' ' && c != '\t') { s2 += c; }
                    if (!fptr.get(c)) {
                        finish = true;
                        break;
                    }
                }
                s2 = s2.substr(1, s2.length() - 2);
            }

            else if (s == "while" || s == "if") {
                //push the word "while" or "if"
                v->push_back(s);
                s = "";
                while (fptr.get(c) && c != '\n') {
                    if (c != ' ') { s += c; }
                }
                //insert the condition
                v->push_back(s);
                s = "";
                //get the scope's commands
                condition_lexer(&fptr, v);
                //set the end-scope char
                (*v).emplace_back("}");
            }

            else if (s == "{") {
                v->push_back(s);
                //reursive -> push all the command of the scope
                condition_lexer(&fptr, v);
                //set the end-scope char
                v->emplace_back("}");
            }
            //elsewhere - set all the line
            else while (fptr.get(c) && c != '\n') {
                    if (c != ' ') { s += c; }
            }

            if (s.length() > 0) { (*v).push_back(s); }
            if (s2.length() > 0) { (*v).push_back(s2); }
        }
        fptr.close();

    }
}

//continue the lexer in case: condition
void Scope::condition_lexer(ifstream *fptr, vector<string> *v) {
    char c;
    bool finish = false;

    while ((*fptr).get(c)) {
        //start at the start of the written line
        while (c == ' ' || c == '\t' || c == ')' || c == '(' || c == '\n') {
            if (!fptr->get(c)) {
                finish = true;
                break;
            }
        }
        if (finish) {
            break;
        }

        //initiate strings
        string s = "";
        string s2 = "";

        //recursion stop condition
        if (c == '}') { return;}

        //get the 1st word
        while (c != ' ' && c != '\t' && c != '(' && c != '\n') {
            s += c;
            if (!fptr->get(c)) {
                finish = true;
                break;
            }
        }
        if (finish) {
            v->push_back(s);
            break;
        }
        //if all the line is without spaces - next line
        if (c == '\n') {
            v->push_back(s);
            continue;
        }
        //if it's "" - continue
        if (s.length() < 1) { continue;}

        //save the rest of the line according to 1st word
        if (s == "openDataServer" || s == "connectControlClient") {
            cerr << "server or client connect command not at the top of the txt" << endl;
        }
            //if it is "var" - save the rest of the line with "," in-between
        else if (s == "var") {
            while (c != '\n') {
                if (c == ' ') {}
                else if ((c == '-' && (*fptr).peek() == '>') || (c == '<' && (*fptr).peek() == '-')) {
                    s2 += ",";
                    s2 += c;
                    if (!fptr->get(c)) {
                        finish = true;
                        break;
                    }
                    s2 += c;
                    s2 += ",";
                }
                else if (c == '=') {
                    s2 += ",";
                    s2 += c;
                    s2 += ",";
                }
                else { s2 += c; }
                if (!fptr->get(c)) {
                    finish = true;
                    break;
                }
            }
        }
        //"Print" -  we will need the spaces
        else if (s == "Print"){
            while (c != '(') { (*fptr).get(c); }
            while (c != '\n') {
                s2 += c;
                if (!fptr->get(c)) {
                    finish = true;
                    break;
                }
            }
        }
        //"openDataServer" or "connectControlClient" - save the expression(s)
        else if (s == "Sleep"){
            while (c != '(') { (*fptr).get(c); }
            while (c != '\n') {
                if (c != ' ') {s2 += c;}
                if (!fptr->get(c)) {
                    finish = true;
                    break;
                }
            }
            s2 = s2.substr(1, s2.length() -2);
        }
        else if (s == "while" || s == "if") {
            //push the word "while" or "if"
            (*v).push_back(s);
            s = "";
            //push condition
            while ((*fptr).get(c) && c != '\n') {
                if (c != ' ' && c != '\t') { s += c; }
            }
            v->push_back(s);
            s = "";
            //reursive -> push all th command of the scope
            condition_lexer(fptr, v);
            //set the end-scope char
            (*v).emplace_back("}");
        }
        else if (s == "{") {
            v->push_back(s);
            //reursive -> push all the command of the scope
            condition_lexer(fptr, v);
            //set the end-scope char
            (*v).emplace_back("}");
        }

        //else - insert all the line without spaces - for setVarCommand
        else while ((*fptr).get(c) && c != '\n') {
                if (c != ' ' && c != '\t') { s += c; }
            }
        if (s.length() > 0) { (*v).push_back(s); }
        if (s2.length() > 0) { (*v).push_back(s2); }
    }
}

//parser of conditions
void Scope::parser(vector<string> v, int *iter) {

    for (; *iter < v.size(); (*iter)++) {
        //end of scope
        if (v[*iter] == "}") {return;}

        //open data server and wait until recv from simulator
        if (v[*iter] == "openDataServer") {
            Command *cmd = new OpenServerCommand(v[*iter+1]);
            cmd_list.push_back(cmd);
        }

        //open client socket
        else if (v[*iter] == "connectControlClient") {
            Command *cmd = new ConnectCommand(v[*iter+1]);
            cmd_list.push_back(cmd);
        }

        //create define var command and add it to this scope
        else if (v[*iter] == "var") {
            Command *cmd = new DefineVarCommand(v[*iter + 1], get_symbol_table());
            cmd_list.push_back(cmd);
        }

        //create sleep command and add it to this scope
        else if  (v[*iter] == "Sleep"){
            Command *cmd = new SleepCommand(v[*iter+1], get_symbol_table());
            cmd_list.push_back(cmd);
        }

        //create print command and add it to this scope
        else if  (v[*iter] == "Print"){
            Command *cmd = new PrintCommand(v[*iter+1], get_symbol_table());
            cmd_list.push_back(cmd);
        }

        //create while command and add it to this scope
        else if (v[*iter] == "while") {
            Command *cmd = new WhileCommand(v[*iter+1], get_symbol_table());
            cmd_list.push_back(cmd);
            (*iter)+=2;
            //save the command in while's scope until "}"
            dynamic_cast<WhileCommand *>(cmd)->get_scope()->parser(v, iter);
            (*iter)--;
        }

        //create if command and add it to this scope
        else if (v[*iter] == "if") {
            Command *cmd = new IfCommand(v[*iter+1], get_symbol_table());
            cmd_list.push_back(cmd);
            (*iter)+=2;
            //save the command in if's scope until "}"
            dynamic_cast<IfCommand *>(cmd)->get_scope()->parser(v, iter);
            (*iter)--;
        }

        //create scope and add it to this scope
        else if(v[*iter] == "{") {
            Command *cmd = new Scope(get_symbol_table());
            cmd_list.push_back(cmd);
            (*iter)++;
            dynamic_cast<Scope*>(cmd)->parser(v, iter);
            (*iter)--;
        }

        //create set var command and add it to this scope
        else {
            Command *cmd = new SetVarCommand(v[*iter], get_symbol_table());
            cmd_list.push_back(cmd);
            (*iter)--;
        }
        (*iter)++;
    }
}

//Scope constructor by file name
Scope::Scope(const string& file_path) {
    int iter = 0;
    upper_symbol_map = new map <string, Variable*>();
    vector<string> v;
    lexer(file_path, &v);
    parser(v, &iter);
}

/*
 * OpenServerCommand functions
 */
int OpenServerCommand::execute() {
    //lock mutex
    const lock_guard<mutex> lock(connection_mutex);
    //open server tcp client (which will unlock the mutex when it will recv data from sim.)
    thread server_thread(server, port);
    //wait until recv data from sim.
    connection_mutex.lock();
    //insert the thread to list
    vecOfThreads.push_back(move(server_thread));
    return 2;
}

void server(const string& port) {
    //calculate the port
    double dbl_port;
    auto i = new Exp_Interpreter();
    Expression *e;
    e = i->interpret(port);
    dbl_port = e->calculate();
    //deletes un-needed vars
    delete i;
    delete e;

    //open server socket (tcp)
    //create socket
    int main_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (main_socket == -1) {
        cerr << "Could not create a socket" << endl;
        return;
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons((int)dbl_port);

    if (bind(main_socket, (struct sockaddr *) &address, sizeof(address)) == -1) {
        cerr<<"Could not bind the socket to an IP"<<endl;
        return;
    }

    if (listen(main_socket, 5) == -1) {
        cerr<<"Error listening"<<endl;
        return;
    }

    int client_socket = accept(main_socket, (struct sockaddr *)&address, (socklen_t*)&address);
    if (client_socket == -1) {
        cerr<<"Error accepting client"<<endl;
        return;
    }

    //vars for receiving from the socket
    int buf_size = 5000;
    char buf[buf_size];
    int c;

    //make sure the simulator will be on when client try to connect
    do {
        c = recv(client_socket, buf, buf_size, 0);
    } while (c == 0);

    //the server is connected - unlock mutex (let client connect)
    connection_mutex.unlock();

    //while main is still running
    while (!is_finished) {
        if (c > 0) {
            //iterate over the input until '\n' and insert to generic_small_map
            char *it = &buf[0];
            for (auto & iter : generic_insertion_order) {
                string s = "";
                while (*it != '\n' && *it != ',' && it < buf + c) {
                    s += *it;
                    it++;
                }
                if (s.length() > 0) {
                    generic_mutex.lock();
                    generic_small_map.at(iter) = stod(s);
                    generic_mutex.unlock();
                }
                if (*it == '\n') { break;}
                it++;
            }
        }
        c = recv(client_socket, buf, buf_size, 0);
    }

    //close sockets [tcp: server main_socket and specific client_socket]
    close(client_socket);
    close(main_socket);
}

/*
 * ConnectCommand functions
 */

int ConnectCommand::execute() {
    int found = ip_port.find(',');
    string ip = ip_port.substr(1, found - 2);
    string port = ip_port.substr(found + 1);

    //calculate the port
    double dbl_port;
    auto i = new Exp_Interpreter();
    Expression *e;
    e = i->interpret(port);
    dbl_port = e->calculate();
    delete i;
    delete e;

    //open client_thread and add it to threads' vector
    thread client_thread(client, ip, (int)dbl_port);
    vecOfThreads.push_back(move(client_thread));
    return 2;
}

void client(string ip, int port) {
    //create socket and connect to sim
    char *ip_c = &ip[0];
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cerr << "Could not create a socket" << endl;
        return;
    }
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip_c);
    address.sin_port = htons(port);

    int is_connect = connect(sock, (struct sockaddr *)&address, sizeof(address));
    if (is_connect == -1) {
        cerr <<"Could not connect to host server"<<endl;
        return;
    } else {
        cerr <<"Client is now connected host server"<<endl;
    }

    //while main is still running and after this continue until there are no strings in send_list
    while (!is_finished || !send_list.empty()) {
        //if there is a string to send at send_list: send it!
        if (!send_list.empty()) {
            send_list_mutex.lock();
            string tmp = send_list.front();
            send_list.pop_front();
            send_list_mutex.unlock();
            char *msg = &tmp[0];
            send(sock, msg, tmp.length(), 0);
        }
    }
    close(sock);
}

/*
 * DefineVarCommand functions
 */
int DefineVarCommand::execute() {
    //data form: var_name,operator(->, <- or =),simulator path or expression
    //initiate vars
    int pos, i = 0;
    string input_list[data.length()];
    string str = data;

    //divide data by ","
    pos = str.find(',');
    while (pos != -1) {
        input_list[i] = str.substr(0, pos);
        i++;
        str = str.substr(pos + 1);
        pos = str.find(',');
    }
    input_list[i] = str;

    //if operator is -> create to_sim type variable
    if (input_list [1] == "->") {
        input_list[2] = del_sim(input_list[2]);
        (*symbol_map)[input_list[0]] = new Variable(input_list[2], 0);
        symbol_map->at(input_list[0])->to_sim();
    }

    //if operator is <- create from_sim type variable
    else if (input_list [1] == "<-") {
        input_list[2] = del_sim(input_list[2]);
        (*symbol_map)[input_list[0]] = new Variable(input_list[2], 0);
        symbol_map->at(input_list[0])->from_sim();
    }

    //if operator is = create variable with no connection to sim
    else if (input_list [1] == "=") {
        auto *inter = new Exp_Interpreter();
        Expression *exp = nullptr;
        double dbl;

        //calculate the expression after the "="  operator
        //first: if there is some variable name in the data: send it with his value to interpreter
        set_vars_to_interpreter(input_list[2], inter, symbol_map);
        try {
            //calculate the expression
            exp = inter->interpret(input_list[2]);
            dbl = exp->calculate();
            (*symbol_map)[input_list[0]] = new Variable(input_list[0], dbl);
        }
        catch (const char* e) {
            cout << e << endl;
        }
        delete inter;
        delete exp;
    }
    return 2;
}

/*
 * SetVarCommand
 */
int SetVarCommand::execute() {
    //the data suppose to be: var=expression

    int pos;
    double val = 0;
    auto *inter = new Exp_Interpreter();
    Expression *exp = nullptr;
    string var_name, exp_str;

    //split var name and expression to be assign to the var
    pos = data.find('=');
    if (pos != -1) {
        var_name = data.substr(0, pos);
        exp_str = data.substr(pos + 1);
    }

    //if there is non variable with this name:
    else {
        cerr << "unknown command from fly.txt" << endl;
        return 2;
    }

    try {
        //first: if there is some variable name in the data: send it with his value to interpreter
        set_vars_to_interpreter(exp_str, inter, symbol_map);

        //calculate the expression
        exp = inter->interpret(exp_str);
        val = exp->calculate();
    }
    catch(const char* e) {
        cout << e << endl;
    }
    delete inter;
    delete exp;

    //find the variable, and update it's value
    auto current = symbol_map->find(var_name);
    if (current == symbol_map->end()) { cerr << "unknown var" << endl;}
    else {
        current->second->set_val(val);
    }

    //if it is to_sim type: add it to send list so the ConnectCommand will send it
    if (current->second->is_to_sim()) {
        string s = "set " + current->second->get_name() + " " + to_string(val) + "\r\n";
        send_list_mutex.lock();
        send_list.push_back(s);
        send_list_mutex.unlock();
    }
    return 2;
}

/*
 * PrintCommand function
 */
int PrintCommand::execute() {
    //if it is a string - print it
    if (data[0] == '\"') {
        cout << del_quot(data) << endl;
    }

    else {
        //remove spaces from the string
        string exp = "";
        for (char i : data) {
            if (i != ' ' && i != '\t') { exp+=i; }
        }

        //calculate the expression value
        auto *i = new Exp_Interpreter();
        Expression *e;
        double d;
        set_vars_to_interpreter(exp, i, symbol_map);
        e = i->interpret(exp);
        d = e->calculate();
        delete i;
        delete e;

        //print the value
        cout << d << endl;
    }
    return 2;
}

/*
 * SleepCommand function
 */
int SleepCommand::execute() {
    //calculate the time for sleep command (in milliseconds)
    auto *i = new Exp_Interpreter();
    Expression *e;
    double dbl_time;
    set_vars_to_interpreter(data, i, symbol_map);
    e = i->interpret(data);
    dbl_time = e->calculate();
    delete i;
    delete e;

    //sleep
    this_thread::sleep_for(chrono::milliseconds((int)dbl_time));
    return 2;
}

/*
 * WhileCommand functions
 */
int WhileCommand::execute() {
    while (cond->is_true()) {
        cmd_scope->execute();
    }
    return 2;
}

/*
 * IfCommand functions
 */
int IfCommand::execute() {
    if (cond->is_true()) {
        cmd_scope->execute();
    }
    return 2;
}

//functions
//delete " " from string
string del_quot(const string& s) {
    string res = s.substr(1, s.length() - 2);
    return res;
}

//delete "sim("/...") and return the new string (...)
string del_sim(const string& s) {
    int pos = s.find('\"');
    string res = del_quot(s.substr(pos, s.length() - pos - 1));
    return res;
}

//get string and for each variable in the string create variable to interpreter
void set_vars_to_interpreter(string input, Exp_Interpreter *inter, map<string, Variable*> *symbol_map) {
    //initiate vars
    int j = 0;
    bool flag;
    string var_name, generic_key, val;

    //iterate over the input string
    while (j < input.length()) {
        var_name = "";
        flag = false;

        //variable name has to start with letter and contain only letters, digits and "_"
        //get var_name
        if (isalpha(input[j])) { flag = true; }
        while (flag && j < input.length() && (isalpha(input[j]) || isdigit(input[j]) || input[j] == '_')){
            var_name += input[j];
            j++;
        }

        //if there is a variable name
        if (var_name.length() > 0) {
            //find it in symbol table and get it's value:
            auto current = symbol_map->find(var_name);
            if (current != symbol_map->end()) {
                generic_key = current->second->get_name();

                //if it's from sim type variable
                if (current->second->is_from_sim()) {
                    generic_mutex.lock();
                    auto generic = generic_small_map.find(generic_key);
                    if (generic != generic_small_map.end()) {
                        //set the val by the generic_small_map
                        current->second->set_val(generic->second);
                        generic_mutex.unlock();
                        val = to_string(symbol_map->at(var_name)->calculate());
                        try {
                            //send to interpreter "var_name=value" so later on it will be able to use this data
                            var_name += "=" + val;
                            inter->setVariables(var_name);
                        }
                        catch (const char* e) {
                            cout << e << endl;
                        }
                    }
                    else { cerr << "Didnt found var_name at generic_small_map :(" << endl; }
                }

                //if it's not from sim type - get the value from symbol_map
                else {
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
            else {
                cerr << "Didnt found var_name at symbol_map :(" << endl;
            }
        }
        j++;
    }
}