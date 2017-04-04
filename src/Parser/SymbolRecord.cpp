//
// Created by Harley on 3/9/2017.
//


#include "SymbolTable.h"
#include "../Scanner/IntegerToken.h"


SymbolRecord::SymbolRecord(string kind, string type, string name) {
    kind_ = kind;
    type_ = type;
    name_ = name;
    symbol_table_ = new SymbolTable();
    is_stack_variable_ = false;
    size_calculated_ = false;
    offset_address_ = 0;
    size_calculated_ = 0;
}

SymbolRecord::SymbolRecord() {
    kind_ ="";
    type_ = "";
    name_ ="";
    symbol_table_ = new SymbolTable();
    is_stack_variable_ = false;
    size_calculated_ = false;
    offset_address_ = 0;
    size_calculated_ = 0;
}

SymbolRecord::SymbolRecord(bool second_pass) {
    kind_ ="";
    type_ = "";
    name_ ="";
    symbol_table_ = new SymbolTable();
    is_stack_variable_ = false;
    symbol_table_->second_pass_ = second_pass;
    offset_address_ = 0;
    size_calculated_ = false;
    size_calculated_ = 0;
}
int SymbolRecord::compute_type_size() {
    if (type_ == "int" || type_ == "float")
        return 8;
    return 8;
}
int SymbolRecord::compute_record_size() {
    if (array_sizes.size() > 0)
         return compute_array_size();
    else if (structure_ == "class" || structure_ == "class array")
        return compute_class_byte_size();
    else
        return 8;
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

bool SymbolRecord::set_structure(string structure) {
    structure_ = structure;
    return true;
}

bool SymbolRecord::add_nested_property(string property) {
    nested_properties_.push_back(property);
    nested_properties_dimensions_[property] = 0;
    return true;
}


bool SymbolRecord::add_array_size(IntegerToken integer_token) {
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
    for(SymbolRecord* record: symbol_table_->symbol_records_) {
        function_parameters_.push_back(record->type_);
    }
    return true;
}

bool SymbolRecord::add_nested_properties_dimension_to_last_porperty() {
    string property = name_;
    if (nested_properties_.size() > 0)
        property = nested_properties_[nested_properties_.size() - 1];
    nested_properties_dimensions_[property] += 1;
    return true;
}

string SymbolRecord::generate_nested_properties_string() {
    string nested_property_string = name_;
    for (string property: nested_properties_)
        nested_property_string += "." + property;
    return nested_property_string;
}

bool SymbolRecord::add_function_record(SymbolRecord *pRecord, bool second_pass) {
    if (!second_pass)
        function_parameters_.push_back(pRecord->name_);
    return true;
}

int SymbolRecord::compute_class_byte_size() {
    int size = 0;
    for(SymbolRecord* record: symbol_table_->symbol_records_) {
        if (record->type_ == "int" || record->type_ == "float") {
            if (record->array_sizes.size() > 0) {
                for(int i : record->array_sizes)
                    size += i;
            } else
                size++;
        }
        else
            size += record->compute_class_byte_size();
    }
    return size * 8;
}
int SymbolRecord::compute_array_size() {
    int size = 0;
    for (int i : array_sizes)
        size += i;
    return size * 8;
}