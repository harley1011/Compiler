//
// Created by Harley on 2/16/2017.
//

#ifndef COMPILER_SYNTAXPARSER_H
#define COMPILER_SYNTAXPARSER_H

#include <iostream>
#include "../Scanner.h"

class SyntaxParser {

public:
    SyntaxParser();

    string _lookahead;
    vector<Token*> _tokens;
    int _current_token_position;

    map<string, vector<string>> _first_sets;

    string next_token();
    bool parse(vector<Token*> tokens);
    bool match(string token);

    bool prog();
    bool classDeclLst();
    bool classDecl();
    bool classBody();
    bool classInDecl();
    bool postTypeId();
    bool progBody();
    bool funcHead();
    bool funcDefLst();
    bool funcDef();
    bool funcBody();
    bool funcInBodyLst();
    bool funcInBody();
    bool varOrStat();
    bool statementLst();
    bool statement();
    bool statementRes();
    bool assignStat();
    bool statBlock();
    bool expr();
    bool relOrAri();
    bool arithExpr();
    bool arithExprD();
    bool sign();
    bool term();
    bool termD();
    bool factor();
    bool factorVarOrFunc();
    bool varOrFuncIdNest();
    bool variable();
    bool variableIndice();
    bool idnest();
    bool indice();
    bool indiceLst();
    bool arraySize();
    bool type();
    bool numType();
    bool fParams();
    bool aParams();
    bool fParamsTail();
    bool aParamsTail();
    bool assignOp();
    bool relOp();
    bool addOp();
    bool multOp();
    bool num();
};


#endif //COMPILER_SYNTAXPARSER_H
