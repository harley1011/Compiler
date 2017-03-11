#include <vector>
#include "SymbolRecord.h"

#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H


class SymbolTable {
public:
    SymbolTable();

    bool second_pass_;
    vector<SymbolRecord> symbol_records_;

    bool create_class_entry_and_table(string kind, string type, string name);
    bool add_record_to_current_symbol_table();

    SymbolRecord* current_symbol_record_;

    bool create_program_entry_and_table();

    bool create_function_entry_and_table();

    void print();

    bool create_variable_entry(SymbolRecord* record);

    bool create_function_entry();

    bool create_function_class_entry_and_function_table(SymbolRecord *record);

    bool create_parameter_entry(SymbolRecord* record);
};


#endif //COMPILER_SYMBOLTABLE_H
