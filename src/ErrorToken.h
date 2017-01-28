//
// Created by Harley on 1/27/2017.
//
#include "Token.h"
#ifndef COMPILER_ERRORTOKEN_H
#define COMPILER_ERRORTOKEN_H


class ErrorToken: public Token {
public:
    string error_message_;

    string generate_error_message();
    ErrorToken(const string &token_identifier, const string &lexeme, int location, int row_location,
               int column_location, string error_message);

};


#endif //COMPILER_ERRORTOKEN_H
