#include <string>
#include "State.h"

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

class token {
public:
    string tokenIdentifier;
    string lexeme;
    int location;

};


#endif //COMPILER_TOKEN_H
