//
// Created by Harley on 3/30/2017.
//
#include <string>
#include <vector>
#include "SymbolRecord.h"
#include "ExpressionTree.h"
#ifndef COMPILER_CODEGENERATOR_H
#define COMPILER_CODEGENERATOR_H


class CodeGenerator {


public:
    int current_stack_address_;

    CodeGenerator();
    vector<string> code_generation_;
    vector<string> variable_declaration_generation_;
    void create_variable_code(SymbolRecord** record);

    string generate_variable_declaration();

    void create_program_entry(SymbolRecord **record);
    void create_expression_code(ExpressionTree* expression);
    void create_variable_assignment_with_variable_code(SymbolRecord *variable_record, SymbolRecord *assign_record);
    void create_variable_assignment_with_value_code(SymbolRecord *variable_record, SymbolRecord *assign_record);
    string generate_code();

    bool create_program_halt(bool double_pass);

    void load_record_into_register(SymbolRecord *record, string reg);
};


#endif //COMPILER_CODEGENERATOR_H
