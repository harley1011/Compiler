//
// Created by Harley on 4/6/2017.
//

#include "IntAndStringConversion.h"

string IntAndStringConversion::generate_required_functions() {
    string result = "";
    if (get_int_required)
        result += get_int;
    if (put_int_required)
        result += put_int;
    return result;

}
