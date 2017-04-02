//
// Created by Harley on 3/30/2017.
//
#include <string>
#include <vector>
#include <stack>
#include "SymbolRecord.h"
#include "ExpressionTree.h"
#ifndef COMPILER_CODEGENERATOR_H
#define COMPILER_CODEGENERATOR_H


class CodeGenerator {


public:
    int current_stack_address_;
    bool second_pass_;
    CodeGenerator();
    vector<string> code_generation_;
    vector<string> variable_declaration_generation_;
    stack<int> current_for_loop_;
    void create_variable_code(SymbolRecord** record);

    string generate_variable_declaration();
    int loop_count_ = 0;
    int class_func_count = 0;
    int func_count = 0;

    void create_program_entry(SymbolRecord **record);
    void create_expression_code(ExpressionNode *expression);
    void create_variable_assignment_with_variable_code(SymbolRecord *variable_record, SymbolRecord *assign_record);
    void create_variable_assignment_with_value_code(SymbolRecord *variable_record, SymbolRecord *assign_record);
    void create_variable_assignment_with_register_code(SymbolRecord *variable_record, string reg);
    string generate_code();

    bool create_program_halt(bool double_pass);

    void load_record_into_register(SymbolRecord *record, string reg);

    void create_relational_expression_code(ExpressionTree *expression);

    bool create_put_code();

    bool create_for_loop();

    bool create_end_for_loop();

    bool create_for_relation_loop();

    bool create_class_func_code(SymbolRecord* record);

    bool create_func_code(SymbolRecord *record);
};


#endif //COMPILER_CODEGENERATOR_H
