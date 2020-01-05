#include "main.h"

//global variables
//mutex
mutex connection_mutex;
mutex send_list_mutex;
mutex generic_mutex;

//other global vars
bool is_finished = false;

//vars to update the generic_small_map
map <string, double> generic_small_map;
list <string> generic_insertion_order;
list <string> send_list;
vector <thread> vecOfThreads;

int main(int argc, char* argv[]) {
    //build vector from fly.txt
    set_map();
    //create the main scope by file_path, and execute it
    auto *main_scope = new Scope(argv[1]);
    main_scope->execute();

    //deletes the main scope (first upper_map which specific allocated, but mainly it's just pointer)
    main_scope->del_upper_map();
    delete main_scope;

    //global flag change
    is_finished = true;

    //wait until all the threads ends
    for (auto & vecOfThread : vecOfThreads) {
        vecOfThread.join();
    }

    return 0;
}


//set generic_small_map and list generic_insertion_order
void set_map() {
    generic_insertion_order.emplace_back("/instrumentation/airspeed-indicator/indicated-speed-kt");
    generic_small_map.insert(pair<string,double>("/instrumentation/airspeed-indicator/indicated-speed-kt", 0));
    generic_insertion_order.emplace_back("/sim/time/warp");
    generic_small_map.insert(pair<string,double>("/sim/time/warp", 0));
    generic_insertion_order.emplace_back("/controls/switches/magnetos");
    generic_small_map.insert(pair<string,double>("/controls/switches/magnetos", 0));
    generic_insertion_order.emplace_back("/instrumentation/heading-indicator/offset-deg");
    generic_small_map.insert(pair<string,double>("/instrumentation/heading-indicator/offset-deg", 0));
    generic_insertion_order.emplace_back("/instrumentation/altimeter/indicated-altitude-ft");
    generic_small_map.insert(pair<string,double>("/instrumentation/altimeter/indicated-altitude-ft", 0));
    generic_insertion_order.emplace_back("/instrumentation/altimeter/pressure-alt-ft");
    generic_small_map.insert(pair<string,double>("/instrumentation/altimeter/pressure-alt-ft", 0));
    generic_insertion_order.emplace_back("/instrumentation/attitude-indicator/indicated-pitch-deg");
    generic_small_map.insert(pair<string,double>("/instrumentation/attitude-indicator/indicated-pitch-deg", 0));
    generic_insertion_order.emplace_back("/instrumentation/attitude-indicator/indicated-roll-deg");
    generic_small_map.insert(pair<string,double>("/instrumentation/attitude-indicator/indicated-roll-deg", 0));
    generic_insertion_order.emplace_back("/instrumentation/attitude-indicator/internal-pitch-deg");
    generic_small_map.insert(pair<string,double>("/instrumentation/attitude-indicator/internal-pitch-deg", 0));
    generic_insertion_order.emplace_back("/instrumentation/attitude-indicator/internal-roll-deg");
    generic_small_map.insert(pair<string,double>("/instrumentation/attitude-indicator/internal-roll-deg", 0));
    generic_insertion_order.emplace_back("/instrumentation/encoder/indicated-altitude-ft");
    generic_small_map.insert(pair<string,double>("/instrumentation/encoder/indicated-altitude-ft", 0));
    generic_insertion_order.emplace_back("/instrumentation/encoder/pressure-alt-ft");
    generic_small_map.insert(pair<string,double>("/instrumentation/encoder/pressure-alt-ft", 0));
    generic_insertion_order.emplace_back("/instrumentation/gps/indicated-altitude-ft");
    generic_small_map.insert(pair<string,double>("/instrumentation/gps/indicated-altitude-ft", 0));
    generic_insertion_order.emplace_back("/instrumentation/gps/indicated-ground-speed-kt");
    generic_small_map.insert(pair<string,double>("/instrumentation/gps/indicated-ground-speed-kt", 0));
    generic_insertion_order.emplace_back("/instrumentation/gps/indicated-vertical-speed");
    generic_small_map.insert(pair<string,double>("/instrumentation/gps/indicated-vertical-speed", 0));
    generic_insertion_order.emplace_back("/instrumentation/heading-indicator/indicated-heading-deg");
    generic_small_map.insert(pair<string,double>("/instrumentation/heading-indicator/indicated-heading-deg", 0));
    generic_insertion_order.emplace_back("/instrumentation/magnetic-compass/indicated-heading-deg");
    generic_small_map.insert(pair<string,double>("/instrumentation/magnetic-compass/indicated-heading-deg", 0));
    generic_insertion_order.emplace_back("/instrumentation/slip-skid-ball/indicated-slip-skid");
    generic_small_map.insert(pair<string,double>("/instrumentation/slip-skid-ball/indicated-slip-skid", 0));
    generic_insertion_order.emplace_back("/instrumentation/turn-indicator/indicated-turn-rate");
    generic_small_map.insert(pair<string,double>("/instrumentation/turn-indicator/indicated-turn-rate", 0));
    generic_insertion_order.emplace_back("/instrumentation/vertical-speed-indicator/indicated-speed-fpm");
    generic_small_map.insert(pair<string,double>("/instrumentation/vertical-speed-indicator/indicated-speed-fpm", 0));
    generic_insertion_order.emplace_back("/controls/flight/aileron");
    generic_small_map.insert(pair<string,double>("/controls/flight/aileron", 0));
    generic_insertion_order.emplace_back("/controls/flight/elevator");
    generic_small_map.insert(pair<string,double>("/controls/flight/elevator", 0));
    generic_insertion_order.emplace_back("/controls/flight/rudder");
    generic_small_map.insert(pair<string,double>("/controls/flight/rudder", 0));
    generic_insertion_order.emplace_back("/controls/flight/flaps");
    generic_small_map.insert(pair<string,double>("/controls/flight/flaps", 0));
    generic_insertion_order.emplace_back("/controls/engines/engine/throttle");
    generic_small_map.insert(pair<string,double>("/controls/engines/engine/throttle", 0));
    generic_insertion_order.emplace_back("/controls/engines/current-engine/throttle");
    generic_small_map.insert(pair<string,double>("/controls/engines/current-engine/throttle", 0));
    generic_insertion_order.emplace_back("/controls/switches/master-avionics");
    generic_small_map.insert(pair<string,double>("/controls/switches/master-avionics", 0));
    generic_insertion_order.emplace_back("/controls/switches/starter");
    generic_small_map.insert(pair<string,double>("/controls/switches/starter", 0));
    generic_insertion_order.emplace_back("/engines/active-engine/auto-start");
    generic_small_map.insert(pair<string,double>("/engines/active-engine/auto-start", 0));
    generic_insertion_order.emplace_back("/controls/flight/speedbrake");
    generic_small_map.insert(pair<string,double>("/controls/flight/speedbrake", 0));
    generic_insertion_order.emplace_back("/sim/model/c172p/brake-parking");
    generic_small_map.insert(pair<string,double>("/sim/model/c172p/brake-parking", 0));
    generic_insertion_order.emplace_back("/controls/engines/engine/primer");
    generic_small_map.insert(pair<string,double>("/controls/engines/engine/primer", 0));
    generic_insertion_order.emplace_back("/controls/engines/current-engine/mixture");
    generic_small_map.insert(pair<string,double>("/controls/engines/current-engine/mixture", 0));
    generic_insertion_order.emplace_back("/controls/switches/master-bat");
    generic_small_map.insert(pair<string,double>("/controls/switches/master-bat", 0));
    generic_insertion_order.emplace_back("/controls/switches/master-alt");
    generic_small_map.insert(pair<string,double>("/controls/switches/master-alt", 0));
    generic_insertion_order.emplace_back("/engines/engine/rpm");
    generic_small_map.insert(pair<string,double>("/engines/engine/rpm", 0));
}

