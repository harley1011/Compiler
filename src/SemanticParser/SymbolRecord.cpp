//
// Created by Harley on 3/9/2017.
//

#include "SymbolRecord.h"
#include "SymbolTable.h"


SymbolRecord::SymbolRecord(string kind, string type, string name) {
    kind_ = kind;
    type_ = type;
    name_ = name;
}

SymbolRecord::SymbolRecord() {
    kind_ ="";
    type_ = "";
    name_ ="";
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


bool SymbolRecord::generate_function_type() {
    bool first = true;
    for(SymbolRecord record: symbol_table_->symbol_records_) {
        if (first)
            first = false;
        else
            type_ += ",";
        type_ += record.type_;
    }
}