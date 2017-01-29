//
// Created by Harley on 1/27/2017.
//
#include "Token.h"
#ifndef COMPILER_INTEGERTOKEN_H
#define COMPILER_INTEGERTOKEN_H


class IntegerToken : public Token {
public:
    int converted_lexeme_;
    bool error_;

    IntegerToken(const string &lexeme, int location, int row_location, int column_location);
    void convert_lexeme(string lexeme);
};


#endif //COMPILER_INTEGERTOKEN_H
