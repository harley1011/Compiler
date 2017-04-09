//
// Created by Harley on 3/30/2017.
//
#include <string>
#include <vector>
#include <stack>
#include "SymbolRecord.h"
#include "ExpressionTree.h"
#include "IntAndStringConversion.h"
#ifndef COMPILER_CODEGENERATOR_H
#define COMPILER_CODEGENERATOR_H


class CodeGenerator {


public:
    bool second_pass_;
    bool enable_comments_;
    bool errors_triggered_;
    CodeGenerator();
    vector<string> code_generation_;
    vector<string> variable_declaration_generation_;
    stack<int> current_for_loop_;
    stack<int> current_if_;
    void create_variable_code(SymbolRecord** record);

    string generate_variable_declaration();
    int loop_count_ = 0;
    int copy_object_count = 0;
    int if_count_ = 0;
    int class_func_count = 0;
    int func_count = 0;

    IntAndStringConversion int_and_string_converter;

    void create_program_entry(SymbolRecord **record);
    void create_expression_code(ExpressionNode *expression, vector<string>* code_list);
    void load_or_call_record_into_reg(SymbolRecord *load_record, string load_reg);
    void create_variable_assignment_with_register(SymbolRecord *variable_record, string reg);
    string generate_code();

    bool create_program_halt(bool double_pass);

    void load_record_into_register(SymbolRecord *record, string reg, vector<string>* code_list);

    void create_relational_expression_code(ExpressionTree *expression);

    bool create_put_code();

    bool create_for_loop();

    bool create_end_for_loop();

    bool create_for_relation_loop();

    bool create_class_func_code(SymbolRecord* record);

    bool create_func_code(SymbolRecord *record);

    void determine_func_stack_variable_offsets(SymbolRecord **record);

    bool assign_func_address(SymbolRecord *record);

    void create_function_call_code(SymbolRecord *func_record, string return_register);

    bool create_func_return_code(SymbolRecord* record);

    void load_function_parameters_into_stack_memory_code(SymbolRecord *record, vector<string> *code_list);

    void create_array_indice_storage_code(SymbolRecord* record,  vector<string> *code_list);

    string add_comment_string(string comment);

    void convert_int_to_string(int number);

    bool create_if();
    bool create_if_else();
    bool create_if_end();

    bool create_get_code();

    void create_single_operator_codes(SymbolRecord *record, string reg);

    void load_or_call_record_into_reg(SymbolRecord *load_record, string load_reg, vector<string> *code_list);

    void create_single_operator_codes(SymbolRecord *record, string reg, vector<string> *code_list);

    void create_variable_assignment_with_register(SymbolRecord *variable_record, string reg, vector<string> *code_list);

    void create_function_parameter_code(SymbolRecord *load_record,  string load_reg, vector<string>* code_list);

    void create_copy_class_values(SymbolRecord *variable_record, SymbolRecord *assign_record);
};


#endif //COMPILER_CODEGENERATOR_H
