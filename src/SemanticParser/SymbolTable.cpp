//
// Created by Harley on 3/9/2017.
//

#include "SymbolTable.h"

bool SymbolTable::create_class_entry_and_table(string kind, string type, string name) {
    SymbolRecord symbol_record(kind, type, name);
    current_symbol_record_ = symbol_record;
    symbol_records_.push_back(symbol_record);
    return true;
}

bool SymbolTable::add_record_to_current_symbol_table() {
    SymbolRecord symbol_record;
    current_symbol_record_.symbol_table_->current_symbol_record_ = symbol_record;
    current_symbol_record_.symbol_table_->symbol_records_.push_back(symbol_record);
    return true;
}
