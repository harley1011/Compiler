//
// Created by Harley on 1/27/2017.
//

#include "FloatToken.h"
#include <stdlib.h>

using namespace std;

FloatToken::FloatToken(const string &lexeme, int location, int row_location, int column_location) : Token("FNUM", lexeme, location, row_location, column_location) {
    convert_lexeme(lexeme);

}

void FloatToken::convert_lexeme(string lexeme) {
    converted_lexeme_ = strtof (lexeme.c_str(), NULL);
}