//
// Created by Harley on 3/9/2017.
//
#include <iostream>
#include <iomanip>

#include "SymbolTable.h"
void print_table(SymbolTable* table, string table_name, int indent_count);

SymbolTable::SymbolTable() {
    second_pass_ = false;
}

bool SymbolTable::create_class_entry_and_table(string kind, string type, string name) {
    if (second_pass_)
        return true;
    SymbolRecord* symbol_record = new SymbolRecord(kind, type, name);
    current_symbol_record_ = symbol_record;
    add_record(symbol_record);
    return true;
}

bool SymbolTable::create_program_entry_and_table() {
    SymbolRecord* symbol_record = new SymbolRecord("function", "", "program");
    current_symbol_record_ = symbol_record;
    add_record(symbol_record);
    return true;
}

bool SymbolTable::create_function_entry_and_table() {
    if (second_pass_)
        return true;
    SymbolRecord* record;
    current_symbol_record_ = record;
    add_record(record);
    return true;
}


void SymbolTable::print() {
    print_table(this, "Global", 0);
}

bool SymbolTable::create_variable_entry(SymbolRecord* record) {
    if (second_pass_)
        return true;
    record->kind_ = "variable";
    current_symbol_record_->symbol_table_->add_record(record);
    return true;
}

bool SymbolTable::create_function_class_entry_and_function_table(SymbolRecord *record) {
    if(second_pass_)
        return true;
    record->kind_ = "function";
    add_record(record);
    return true;
}

bool SymbolTable::create_parameter_entry(SymbolRecord* record) {
    if (second_pass_)
        return true;
    record->kind_ = "parameter";
    add_record(record);
    return true;
}

bool SymbolTable::add_record(SymbolRecord* record) {
    symbol_records_.push_back(*record);
    return true;
}

void print_table(SymbolTable* table, string table_name, int indent_count) {
    if (table->symbol_records_.size() == 0)
        return;

    int name_width_out = 20;
    int type_width_out = 20;
    int kind_width_out = 20;

    for(SymbolRecord record: table->symbol_records_) {
        if (record.kind_.size() > kind_width_out)
            kind_width_out = record.kind_.size();
        if (record.name_.size() > name_width_out)
            name_width_out = record.name_.size();
        if (record.type_.size() > type_width_out)
            type_width_out = record.type_.size();
    }

    int horizontal_width_out = name_width_out + type_width_out + kind_width_out + 4;

    cout << string(indent_count, ' ') << "Symbol table name: " + table_name << endl;
    cout << string(indent_count, ' ') << string(horizontal_width_out, '-') << endl;
    cout << string(indent_count, ' ') << "|" << setw(name_width_out) << "Name" << std::right << std::setfill(' ') << "|";
    cout << setw(kind_width_out) << "Kind" << std::right << std::setfill(' ') << "|";
    cout << setw(type_width_out) << "Type" << std::right << std::setfill(' ') << "|" << endl;
    cout <<  string(indent_count, ' ') << string(horizontal_width_out, '-') << endl;
    for(SymbolRecord record: table->symbol_records_) {
        cout  << string(indent_count, ' ') << "|" << setw(name_width_out) << record.name_ << std::right << std::setfill(' ') << "|";
        cout << setw(kind_width_out) << record.kind_ << std::right << std::setfill(' ') << "|";
        cout << setw(type_width_out) << record.type_ << std::right << std::setfill(' ') << "|" << endl;
    }

    cout << string(indent_count, ' ') << string(horizontal_width_out, '-') << endl << endl;

    for(SymbolRecord record: table->symbol_records_) {
        print_table(record.symbol_table_, record.name_, indent_count+= 10);
    }
}