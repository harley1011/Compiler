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

    SyntaxParser(bool enable_derivation_output);

    SyntaxParser(vector<Token *> tokens);
    SyntaxParser(string derivation_output_path, string error_output_path);

    string current_rhs_derivation_;
    string lookahead_;
    vector<string> errors_;
    vector<string> derivations_;
    Token*   current_token_;
    vector<Token*> tokens_;
    int current_token_position_;
    bool output_to_file_;
    string derivation_output_path_;
    string error_output_path_;
    ofstream error_output_file_;
    ofstream derivation_output_file_;
    bool enable_derivation_output_;
    string previous_token_;
    vector<string> token_error_buffer_;
    vector<string> skipped_token_error_buffer_;
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

    bool factorVarArray();

    bool factorVarArrayNestId();

    bool statThenBlock();

    void report_error(string expected_token, string actual_token);

    bool skip_to_next_deli(int max_search);

    bool statIfElseBlock();

    bool statElseBlock();

    bool match(string token, set<string> expected_post_tokens);
};


#endif //COMPILER_SYNTAXPARSER_H
