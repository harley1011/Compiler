#include <iostream>
#include <string>
#include <map>

using namespace std;

#ifndef UNTITLED_STATE_H
#define UNTITLED_STATE_H


class State {
public:
    string token_;l
    bool backup_;
    bool is_final_state_;
    map<char, int> next_states_;
    int non_zero_state_;
    int letter_state_;
    int alphanum_state_;
    int digit_state_;
    int any_match_state_;
    string error_message_;

    int get_next_state(char lookup);

    State(string token, bool backup, bool is_final_state, map<char, int> next_states) {
        token_ = token;
        backup_ = backup;
        is_final_state_ = is_final_state;
        next_states_ = next_states;

        non_zero_state_ = -1;
        letter_state_ = -1;
        alphanum_state_ = -1;
        digit_state_ = -1;
        any_match_state_ = -1;
    }

    State(string token, bool backup, bool is_final_state) {
        token_ = token;
        backup_ = backup;
        is_final_state_ = is_final_state;

        non_zero_state_ = -1;
        letter_state_ = -1;
        alphanum_state_ = -1;
        digit_state_ = -1;
        any_match_state_ = -1;
    }

    State() {
        backup_ = false;
        is_final_state_ = false;

        non_zero_state_ = -1;
        letter_state_ = -1;
        alphanum_state_ = -1;
        digit_state_ = -1;
        any_match_state_ = -1;
    }

};


#endif //UNTITLED_STATE_H
