//
// Created by Harley on 2/16/2017.
//

#ifndef COMPILER_SYNTAXPARSER_H
#define COMPILER_SYNTAXPARSER_H

#include <iostream>
#include "../Scanner.h"

class SyntaxParser {

    SyntaxParser();

    string _lookahead;
    vector<Token*> _tokens;
    int _current_token_position;

    map<string, vector<string>> _first_sets;

    string next_token();
    bool parse(vector<Token*> tokens);

    bool prog();
    bool classDeclLst();
    bool classDec();

    bool match();

    bool match(string token);

    bool classDecl();

    bool classBody();

    bool classInDecl();

    bool postTypeId();

    bool progBody();

    bool funcHead();

    bool funcDefLst();

    bool funcDef();

    bool funcBody();

    static bool arithExpr;
public:
    bool multOp();

    bool multOp();

    bool addOp();

    bool relOp();

    bool assignOp();

    bool num();

    bool aParamsTail();

    bool fParamsTail();

    bool aParams();

    bool fParams();

    bool numType();

    bool arraySize();

    bool indiceLst();
};


#endif //COMPILER_SYNTAXPARSER_H
