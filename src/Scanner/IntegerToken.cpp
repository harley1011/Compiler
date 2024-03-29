//
// Created by Harley on 1/27/2017.
//

#include "IntegerToken.h"
#include <stdlib.h>
#include <limits>
#include <iostream>
#include <string>

IntegerToken::IntegerToken(const string &lexeme, int location, int row_location, int column_location) : Token("INUM", lexeme, location, row_location, column_location) {
    convert_lexeme(lexeme);
}

void IntegerToken::convert_lexeme(string lexeme) {
    //string str1 = "45";
//    int myint1 = stoi(str1);
    converted_lexeme_ = atoi(lexeme.c_str());
}