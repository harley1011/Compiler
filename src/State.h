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
    int non_zero_state;
    int letter_state;
    int alphanum_state;
    int digit_state;
    int any_match_state;

    int get_next_state(char lookup);

    State(string token, bool backup, bool is_final_state, map<char, int> next_states) {
        token_ = token;
        backup_ = backup;
        is_final_state_ = is_final_state;
        next_states_ = next_states;

        non_zero_state = -1;
        letter_state = -1;
        alphanum_state = -1;
        digit_state = -1;
        any_match_state -1;
    }

    State(string token, bool backup, bool is_final_state) {
        token_ = token;
        backup_ = backup;
        is_final_state_ = is_final_state;

        non_zero_state = -1;
        letter_state = -1;
        alphanum_state = -1;
        digit_state = -1;
        any_match_state -1;
    }

    State() {
        backup_ = false;
        is_final_state_ = false;

        non_zero_state = -1;
        letter_state = -1;
        alphanum_state = -1;
        digit_state = -1;
        any_match_state -1;
    }

};


#endif //UNTITLED_STATE_H
