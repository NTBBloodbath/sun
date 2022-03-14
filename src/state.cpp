#include <iostream>

using std::string;

struct State {
    int current_ln = 0;
    int go_back = 0;
    string sun_source_file;
    string sun_out_file;
};
