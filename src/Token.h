#include <string>
#include "State.h"


#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

class Token {
public:
    string token_identifier_;
    string lexeme_;
    int location_;
    int row_location_;
    int column_location_;

    Token(string token_identifier, string lexeme, int location, int row_location, int column_location) {
        token_identifier_ = token_identifier;
        lexeme_ = lexeme;
        location_ = location;
        row_location_ = row_location;
        column_location_ = column_location;
    }

};


#endif //COMPILER_TOKEN_H
