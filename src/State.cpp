//
// Created by Harley on 1/24/2017.
//

#include "State.h"

int State::get_next_state(char lookup) {
    if (next_states_.count(lookup)) {
        return next_states_[lookup];
    } else {
        if (isalpha(lookup) || lookup == '_') {
            if (alphanum_state != -1)
                return alphanum_state;
            else if (letter_state != -1)
                return letter_state;
        } else if (isdigit(lookup)) {

            if (digit_state != -1)
                return digit_state;
            else if (alphanum_state != -1)
                return alphanum_state;
            else if (non_zero_state != -1 && int(lookup) > 0)
                return non_zero_state;
        }
         if (any_match_state != -1)
            return any_match_state;
    }
}