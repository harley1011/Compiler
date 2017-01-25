#include <string>
#include "State.h"

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H

template <class T>
class token {
public:
    string tokenIdentifier;
    T lexeme;
    int location;

};


#endif //COMPILER_TOKEN_H
