#include <vector>
#include "SymbolRecord.h"

#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H


class SymbolTable {
public:
    vector<SymbolRecord> symbol_records_;

    bool create_class_entry_and_table(string kind, string type, string name);
    bool add_record_to_current_symbol_table();

    SymbolRecord current_symbol_record_;
};


#endif //COMPILER_SYMBOLTABLE_H
