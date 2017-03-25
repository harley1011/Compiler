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

SymbolRecord::SymbolRecord(bool second_pass) {
    kind_ ="";
    type_ = "";
    name_ ="";
    symbol_table_ = new SymbolTable();
    symbol_table_->second_pass_ = second_pass;
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

bool SymbolRecord::add_function_parameter(string type) {
    function_parameters_.push_back(type);
    return true;
}
bool SymbolRecord::set_structure(string structure) {
    structure_ = structure;
    return true;
}

bool SymbolRecord::add_function_record(SymbolRecord* record) {
    function_parameters_record_.push_back(record);
    return true;
}

bool SymbolRecord::add_nested_property(string property) {
    nested_properties_.push_back(property);
    return true;
}


bool SymbolRecord::add_array_size(IntegerToken integer_token) {
    //append_to_type("[" + integer_token.lexeme_);
    array_sizes.push_back(integer_token.converted_lexeme_);
    return true;
}

string SymbolRecord::type_with_array_dimensions() {
    string result = type_;
    for(int i : array_sizes) {
        result += "[" + to_string(i) + "]";
    }
    return result;
}

bool SymbolRecord::generate_function_type() {
    bool first = true;
    for(SymbolRecord* record: symbol_table_->symbol_records_) {
        function_parameters_.push_back(record->type_);
//        if (first){
//            first = false;
//            type_ += " : ";
//        }
//        else
//            type_ += ", ";
//        type_ += record->type_;
    }
    return true;
}

bool SymbolRecord::add_nested_properties_dimension_to_last_porperty() {
    string property = name_;
    if (nested_properties_.size() > 1)
        property = nested_properties_[nested_properties_.size() - 1];
    nested_properties_dimensions_[property] += 1;
    return true;
}
