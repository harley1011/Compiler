#include <iostream>
#include <string>
#include <map>

using namespace std;

#ifndef UNTITLED_STATE_H
#define UNTITLED_STATE_H


class State {
public:
    string token_;
    bool backup_;
    bool is_final_state_;
    map<char, int> next_states_;

    int get_next_state(char lookup);

    State(string token, bool backup, bool is_final_state, map<char, int> next_states) {
        token_ = token;
        backup_ = backup;
        is_final_state_ = is_final_state;
        next_states_ = next_states;
    }

    State(string token, bool backup, bool is_final_state) {
        token_ = token;
        backup_ = backup;
        is_final_state_ = is_final_state;
    }

    State() {
        backup_ = false;
        is_final_state_ = false;
    }

};


#endif //UNTITLED_STATE_H
