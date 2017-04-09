#include <vector>
#include "SymbolRecord.h"
#include "ExpressionTree.h"
#include "CodeGenerator.h"

#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H


class SymbolTable {
public:


    bool second_pass_;
    Token* current_token;
    vector<string> errors_;
    vector<SymbolRecord*> symbol_records_;
    SymbolTable* parent_symbol_table_;
    SymbolRecord* symbol_record_;
    SymbolRecord* current_symbol_record_;
    CodeGenerator* code_generator_;
    SymbolTable();
    bool create_class_entry_and_table(string kind, string type, string name);
    bool create_program_entry_and_table();
    bool create_function_entry_and_table(SymbolRecord** record);
    string print(bool output_to_console);
    bool create_variable_entry(SymbolRecord** record);
    bool create_function_class_entry_and_function_table(SymbolRecord **record);
    bool create_parameter_entry(SymbolRecord* record);
    bool insert(SymbolRecord *record);
    void report_error_to_highest_symbol_table(string error_message);
    SymbolRecord* search(string name);
    void set_second_pass(bool second_pass);
    void set_properly_declared(SymbolRecord *record);
    bool check_if_return_type_is_correct_type(SymbolRecord *func_record, ExpressionTree* expression);
    string generate_symbol_table_string(SymbolTable *table, string table_name, int indent_count);
    bool check_if_variable_or_func_exist(SymbolRecord *record);
    bool check_func_parameters(SymbolRecord *found_record,  SymbolRecord *func_record);
    bool check_expression_tree_for_correct_type_and_create_assignment_code(SymbolRecord *variable_record,
                                                                           ExpressionTree *tree);
    SymbolRecord *search_type(string type);
    SymbolRecord * check_nested_property_and_compute_offset(SymbolRecord *record, SymbolRecord *found_record);
    bool check_for_circular_references(SymbolRecord *record);
    void check_for_circular_references(SymbolRecord *record, SymbolRecord *member_record,
                                           vector<string> already_checked_types);
    SymbolRecord *search_top_table(string name);
    bool check_correct_number_of_array_dimensions(SymbolRecord* found_record, SymbolRecord *record);
    bool check_if_record_is_num_type(SymbolRecord *record);
    bool check_expression_is_valid_and_generate_code(ExpressionTree *tree);
    bool check_valid_arithmetic_expression(ExpressionNode *node);
    bool check_valid_relational_expression(ExpressionNode *expression);
    bool check_if_matching_types(string variable_property, string assign_type);
    CodeGenerator *get_code_generator();
    bool calculate_class_offsets();
    void determine_func_stack_variable_offsets(SymbolRecord *record);
    bool copy_stored_record(SymbolRecord *record);
    bool check_indice_expression_is_valid(SymbolRecord* record, ExpressionTree *tree);
    bool check_if_get_variable_is_int_or_float_and_exists(SymbolRecord *record);
    bool copy_stored_record(SymbolRecord *record, SymbolRecord *found_record);
    bool check_expression_is_valid(ExpressionTree *tree);
};


#endif //COMPILER_SYMBOLTABLE_H
