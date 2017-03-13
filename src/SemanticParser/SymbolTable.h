#include <vector>
#include "SymbolRecord.h"

#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H


class SymbolTable {
public:
    SymbolTable();

    bool second_pass_;

    Token* current_token;

    vector<string> errors_;

    vector<SymbolRecord*> symbol_records_;

    SymbolTable* parent_symbol_table_;

    bool create_class_entry_and_table(string kind, string type, string name);

    SymbolRecord* current_symbol_record_;

    bool create_program_entry_and_table();

    bool create_function_entry_and_table();

    void print();

    bool create_variable_entry(SymbolRecord* record);


    bool create_function_class_entry_and_function_table(SymbolRecord *record);

    bool create_parameter_entry(SymbolRecord* record);

    bool insert(SymbolRecord *record);

    void report_error_to_highest_symbol_table(string error_message);



    SymbolRecord* search(string name);
};


#endif //COMPILER_SYMBOLTABLE_H
