//
// Created by Harley on 2/16/2017.
//

#ifndef COMPILER_SYNTAXPARSER_H
#define COMPILER_SYNTAXPARSER_H

#include <iostream>
#include "../Scanner.h"

class SyntaxParser {

    string _lookahead;
    vector<Token*> _tokens;
    int _current_token_position;

    string next_token();
    bool parse(vector<Token*> tokens);

    bool prog();
    bool classDeclLst();

};


#endif //COMPILER_SYNTAXPARSER_H
