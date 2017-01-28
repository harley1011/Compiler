//
// Created by Harley on 1/27/2017.
//
#include "Token.h"
#ifndef COMPILER_FLOATTOKEN_H
#define COMPILER_FLOATTOKEN_H


class FloatToken : public Token {
public:
    float converted_lexeme_;
    FloatToken(const string &lexeme, int location, int row_location, int column_location);

    void convert_lexeme(string lexeme);


};


#endif //COMPILER_FLOATTOKEN_H
