//
// Created by Harley on 2/16/2017.
//

#ifndef COMPILER_SYNTAXPARSER_H
#define COMPILER_SYNTAXPARSER_H

#include <iostream>
#include "../Scanner.h"
#include <set>

class SyntaxParser {

public:
    SyntaxParser();

    string _current_rhs_derivation;
    string _lookahead;
    vector<string> _errors;
    vector<string> _derivations;
    Token*   _current_token;
    vector<Token*> _tokens;
    int _current_token_position;

    map<string, vector<string>> _first_sets;

    string next_token();
    bool form_derivation_string(string non_terminal, string rhs);
    bool skip_errors(set<string> first_set, set<string> follow_set, bool epsilon);
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

    bool is_lookahead_a_type();

    bool is_lookahead_a_statement();

    bool relExpr();

    bool is_lookahead_a_value();

    bool check_if_lookahead_is_in_set(set<string> values);
};


#endif //COMPILER_SYNTAXPARSER_H
