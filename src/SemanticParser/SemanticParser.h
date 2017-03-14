//
// Created by Harley on 2/16/2017.
//

#ifndef COMPILER_SYNTAXPARSER_H
#define COMPILER_SYNTAXPARSER_H

#include <iostream>
#include "../Scanner.h"
#include <set>
#include "../SemanticParser/SymbolTable.h"
#include "../SemanticParser/SymbolRecord.h"
#include "../IntegerToken.h"

class SemanticParser {

public:
    SemanticParser();

    SemanticParser(bool enable_derivation_output);

    SemanticParser(vector<Token *> tokens);
    SemanticParser(string derivation_output_path, string error_output_path);

    SymbolTable *global_symbol_table_;

    string current_rhs_derivation_;
    string lookahead_;
    vector<string> errors_;
    vector<string> semantic_errors_;
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

    Token get_last_token();
    bool prog();
    bool classDeclLst();
    bool classDecl();
    bool classBody();
    bool classInDecl();
    bool postTypeId(SymbolRecord* record);
    bool progBody();
    bool funcHead(SymbolRecord* record);
    bool funcDefLst();
    bool funcDef(SymbolRecord* record);
    bool funcBody(SymbolRecord* record);
    bool funcInBodyLst(SymbolRecord* record);
    bool funcInBody(SymbolRecord* record);
    bool varOrStat(SymbolRecord* record);
    bool statementLst();
    bool statement();
    bool statementRes(SymbolRecord* record);
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
    bool arraySize(SymbolRecord* record);
    bool type(SymbolRecord* record);
    bool numType(SymbolRecord* record);
    bool fParams(SymbolRecord* record);
    bool aParams();
    bool fParamsTail(SymbolRecord* record);
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

    bool match(string token, set<string> expected_post_tokens, string message_to_replace, string replace_message);

    IntegerToken get_last_integer_token();
};


#endif //COMPILER_SYNTAXPARSER_H
