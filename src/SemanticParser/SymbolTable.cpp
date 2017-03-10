//
// Created by Harley on 3/9/2017.
//
#include <iostream>
#include <iomanip>

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
    print_table(this, "Global");
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
    cout << "Symbol table name: " + table_name << endl;
    cout << string(64, '-') << endl;
    cout << "|" << setw(20) << "Name" << std::right << std::setfill(' ') << "|";
    cout << setw(20) << "Kind" << std::right << std::setfill(' ') << "|";
    cout << setw(20) << "Type" << std::right << std::setfill(' ') << "|" << endl;
    cout << string(64, '-') << endl;
    for(SymbolRecord record: table->symbol_records_) {
        cout << "|" << setw(20) << record.name_ << std::right << std::setfill(' ') << "|";
        cout << setw(20) << record.kind_ << std::right << std::setfill(' ') << "|";
        cout << setw(20) << record.type_ << std::right << std::setfill(' ') << "|" << endl;
        //<< record.kind_ << setw(20) << std::left <<  "|" << record.type_ << setw(20) << std::left << "|" << endl;
    }

    cout << string(64, '-') << endl << endl;

    for(SymbolRecord record: table->symbol_records_) {
        print_table(record.symbol_table_, record.name_);
    }
}