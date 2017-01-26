//
// Created by Harley on 1/24/2017.
//

#include "State.h"

int State::get_next_state(char lookup) {
    if (next_states_.count(lookup)) {
        return next_states_[lookup];
    } else {
        if (isalpha(lookup)) {
            if (next_states_.count('a')) // Check if it has state for alphanumerical char
                return next_states_['a'];
            else if (next_states_.count('l')) // Check if it has state for letter char
                return next_states_['l'];
        } else if (isdigit(lookup)) {
            if (next_states_.count('d')) // Check if it has state for digit char
                return next_states_['d'];
            else if (next_states_.count('a')) // Check if it has state for alphanumerical char
                return next_states_['a'];
            else if (next_states_.count('n') && int(lookup) > 0) // Not zero
                return next_states_['n'];
        } else if (next_states_.count('t')) {
            return next_states_['t'];
        }
    }
}