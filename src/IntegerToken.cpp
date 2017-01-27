//
// Created by Harley on 1/27/2017.
//

#include "IntegerToken.h"
#include <stdlib.h>

IntegerToken::IntegerToken(const string &lexeme, int location) : Token("INUM", lexeme, location) {
    convert_lexeme(lexeme);
}

void IntegerToken::convert_lexeme(string lexeme) {
    converted_lexeme_ = atoi(lexeme.c_str());
}