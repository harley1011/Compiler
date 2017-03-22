//
// Created by Harley on 2/16/2017.
//

#ifndef COMPILER_SYNTAXPARSER_H
#define COMPILER_SYNTAXPARSER_H

#include <iostream>
#include "../Scanner.h"
#include <set>
#include "../Parser/SymbolTable.h"
#include "../Parser/SymbolRecord.h"
#include "../IntegerToken.h"
#include "../Token.h"

class Parser {

public:
    Parser();
    Parser(bool enable_derivation_output);
    Parser(vector<Token*> tokens);
    Parser(string derivation_output_path, string symbol_table_output_path,  string syntax_error_output_path, string semantic_error_output_path);
    Parser(string derivation_output_path, string syntax_error_output_path);

    SymbolTable *global_symbol_table_;

    bool enable_double_pass_parse_;
    string current_rhs_derivation_;
    string lookahead_;
    vector<string> syntax_errors;
    vector<string> semantic_errors_;
    vector<string> derivations_;
    Token*   current_token_;
    vector<Token*> tokens_;
    int current_token_position_;
    bool output_to_file_;
    string derivation_output_path_;
    string symbol_table_output_path_;
    string syntax_error_output_path_;
    string semantic_error_output_path_;
    ofstream syntax_error_output_file_;
    ofstream semantic_error_output_file_;
    ofstream derivation_output_file_;
    ofstream symbol_table_output_file_;
    bool enable_derivation_output_;
    string previous_token_;

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
    bool postTypeId(SymbolRecord** record);
    bool progBody();
    bool funcHead(SymbolRecord** record);
    bool funcDefLst();
    bool funcDef();
    bool funcBody(SymbolRecord* func_record);
    bool funcInBodyLst(SymbolRecord* func_record);
    bool funcInBody(SymbolRecord* func_record);
    bool varOrStat(SymbolRecord* func_record, SymbolRecord** record);
    bool statementLst(SymbolRecord* func_record);
    bool statement(SymbolRecord* func_record);
    bool statementRes(SymbolRecord* func_record);
    bool assignStat(SymbolRecord* func_record, SymbolRecord* record);
    bool statBlock(SymbolRecord* func_record);
    bool expr(SymbolRecord* func_record, SymbolRecord* record);
    bool relOrAri(SymbolRecord* func_record, SymbolRecord* record);
    bool arithExpr(SymbolRecord* func_record, SymbolRecord* record);
    bool arithExprD(SymbolRecord* func_record, SymbolRecord* record);
    bool sign(SymbolRecord* record);
    bool term(SymbolRecord* func_record, SymbolRecord* record);
    bool termD(SymbolRecord* func_record, SymbolRecord* record);
    bool factor(SymbolRecord* func_record, SymbolRecord* record);
    bool factorVarOrFunc(SymbolRecord* func_record, SymbolRecord* record);
    bool varOrFuncIdNest(SymbolRecord* func_record, SymbolRecord* record);
    bool variable(SymbolRecord* func_record, SymbolRecord* record);
    bool variableIndice(SymbolRecord* func_record, SymbolRecord* record);
    bool idnest(SymbolRecord* func_record, SymbolRecord* record);
    bool indice(SymbolRecord* func_record, SymbolRecord* record);
    bool indiceLst(SymbolRecord* func_record, SymbolRecord* record);
    bool arraySize(SymbolRecord* record);
    bool type(SymbolRecord* record);
    bool numType(SymbolRecord* record);
    bool fParams(SymbolRecord* record);
    bool aParams(SymbolRecord* func_record, SymbolRecord* record);
    bool fParamsTail(SymbolRecord* record);
    bool aParamsTail(SymbolRecord* func_record, SymbolRecord* record);
    bool assignOp();
    bool relOp(SymbolRecord* record);
    bool addOp(SymbolRecord* record);
    bool multOp(SymbolRecord* record);
    bool num(SymbolRecord* record);

    bool is_lookahead_a_type();

    bool is_lookahead_a_statement();

    bool relExpr(SymbolRecord* func_record, SymbolRecord* record);

    bool is_lookahead_a_value();
    bool check_if_lookahead_is_in_set(set<string> values);

    bool factorVarArray(SymbolRecord* func_record, SymbolRecord* record);

    bool factorVarArrayNestId(SymbolRecord* func_record, SymbolRecord* record);

    bool statThenBlock(SymbolRecord* func_record);

    void report_error(string expected_token, string actual_token);

    bool skip_to_next_deli(int max_search);

    bool statIfElseBlock(SymbolRecord* func_record);

    bool statElseBlock(SymbolRecord* func_record);

    bool match(string token, set<string> expected_post_tokens);

    bool match(string token, set<string> expected_post_tokens, string message_to_replace, string replace_message);

    IntegerToken get_last_integer_token();

};


#endif //COMPILER_SYNTAXPARSER_H
