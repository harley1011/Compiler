//
// Created by Harley on 3/9/2017.
//
#include <iostream>

#include "SymbolTable.h"
void print_table(SymbolTable* table, string table_name);

SymbolTable::SymbolTable() {
    second_pass_ = false;
}

bool SymbolTable::create_class_entry_and_table(string kind, string type, string name) {
    if (second_pass_)
        return true;
    SymbolRecord* symbol_record = new SymbolRecord(kind, type, name);
    current_symbol_record_ = symbol_record;
    symbol_records_.push_back(*symbol_record);
    return true;
}

bool SymbolTable::add_record_to_current_symbol_table() {
    SymbolRecord* symbol_record;
    current_symbol_record_->symbol_table_->current_symbol_record_ = symbol_record;
    current_symbol_record_->symbol_table_->symbol_records_.push_back(*symbol_record);
    return true;
}


bool SymbolTable::create_program_entry_and_table() {
    SymbolRecord* symbol_record = new SymbolRecord("function", "", "program");
    current_symbol_record_ = symbol_record;
    symbol_records_.push_back(*symbol_record);
    return true;
}

bool SymbolTable::create_function_entry_and_table() {
    if (second_pass_)
        return true;
    SymbolRecord* record;
    current_symbol_record_ = record;
    symbol_records_.push_back(*record);
    return true;
}


void SymbolTable::print() {
    print_table(this, "Symbol table: Global");
}

bool SymbolTable::create_variable_entry(SymbolRecord* record) {
    if (second_pass_)
        return true;
    record->kind_ = "variable";
    current_symbol_record_->symbol_table_->symbol_records_.push_back(*record);
    return true;
}

bool SymbolTable::create_function_class_entry(SymbolRecord* record) {
    if(second_pass_)
        return true;
    record->kind_ = "function";
    current_symbol_record_->symbol_table_->symbol_records_.push_back(*record);
    return true;
}

bool SymbolTable::create_parameter_entry(SymbolRecord* record) {
    if (second_pass_)
        return true;
    record->kind_ = "parameter";
    symbol_records_.push_back(*record);
    return true;
}

void print_table(SymbolTable* table, string table_name) {
    if (table->symbol_records_.size() == 0)
        return;
    cout << table_name;
    printf("|%10s|%10s|%10s|%10s|\r\n", "Name", "Kind", "Type");

    for(SymbolRecord record: table->symbol_records_) {
        printf("|%10s|%10s|%10s|%10s|\r\n", record.name_, record.kind_, record.type_);
    }

    for(SymbolRecord record: table->symbol_records_) {
        print_table(record.symbol_table_, record.name_);
    }
}