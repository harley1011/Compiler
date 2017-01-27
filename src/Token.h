#include <string>
#include "State.h"


#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

class Token {
public:
    string token_identifier_;
    string lexeme_;
    int location_;

    Token(string token_identifier, string lexeme, int location) {
        token_identifier_ = token_identifier;
        lexeme_ = lexeme;
        location_ = location;
    }

};


#endif //COMPILER_TOKEN_H
