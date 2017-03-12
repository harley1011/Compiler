//
// Created by Harley on 3/9/2017.
//

#include "SymbolRecord.h"
#include "SymbolTable.h"
#include "../IntegerToken.h"


SymbolRecord::SymbolRecord(string kind, string type, string name) {
    kind_ = kind;
    type_ = type;
    name_ = name;
    symbol_table_ = new SymbolTable();
}

SymbolRecord::SymbolRecord() {
    kind_ ="";
    type_ = "";
    name_ ="";
    symbol_table_ = new SymbolTable();
}

bool SymbolRecord::set_kind(string kind) {
    kind_ = kind;
    return true;
}

bool SymbolRecord::set_type(string type) {
    type_ = type;
    return true;
}

bool SymbolRecord::set_name(string name) {
    name_ = name;
    return true;
}

bool SymbolRecord::append_to_type(string type) {
    type_ += type;
    return true;
}
bool SymbolRecord::set_structure(string structure) {
    structure_ = structure;
    return true;
}

bool SymbolRecord::add_array_size(IntegerToken integer_token) {
    append_to_type("[" + integer_token.lexeme_);
    array_sizes.push_back(integer_token.converted_lexeme_);
    return true;
}


bool SymbolRecord::generate_function_type() {
    bool first = true;
    for(SymbolRecord record: symbol_table_->symbol_records_) {
        if (first)
            first = false;
        else
            type_ += ",";
        type_ += record.type_;
    }
    return true;
}