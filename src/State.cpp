//
// Created by Harley on 1/24/2017.
//

#include "State.h"

int State::get_next_state(char lookup) {
    if (next_states_.count(lookup)) {
        return next_states_[lookup];
    } else {
        if (isalpha(lookup)) {
            if (next_states_.count('a'))
                return next_states_['a'];
            else if (next_states_.count('l'));
                return next_states_['l'];
        } else if (isdigit(lookup)) {
            if (next_states_.count('d'))
                return next_states_['d'];
            else if (next_states_.count('a'));
                return next_states_['a'];
        } else if (next_states_.count('t')) {
            return next_states_['t'];
        }
    }
}