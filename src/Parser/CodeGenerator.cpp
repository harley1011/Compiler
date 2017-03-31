//
// Created by Harley on 3/30/2017.
//

#include "CodeGenerator.h"
#include "SymbolTable.h"

CodeGenerator::CodeGenerator() {

}


void CodeGenerator::create_variable_code(SymbolRecord **record) {
    SymbolRecord* local_record = (*record);
    if (local_record->type_ == "int" || local_record->type_ == "float") {
        string code_variable_name = local_record->name_ + "_" + local_record->symbol_table_->parent_symbol_table_->symbol_record_->name_;
        variable_declaration_generation_.push_back(code_variable_name + " dw 0");
        (*record)->address = code_variable_name;
    }
}

void CodeGenerator::create_program_entry(SymbolRecord **record) {
    code_generation_.push_back("program entry");
    (*record)->address = "program";

}
bool CodeGenerator::create_program_halt(bool double_pass) {
    if (double_pass)
        code_generation_.push_back("hlt");
    return true;

}

void CodeGenerator::create_variable_assignment_code(SymbolRecord *variable_record, SymbolRecord *assign_record) {
    string assign_record_address = assign_record->address;
    string variable_record_address = variable_record->address;

    if (assign_record->kind_ == "variable")
        code_generation_.push_back("lw r1," + assign_record->address + "(r0)");
    else if (assign_record->type_ == "int")
        code_generation_.push_back("lw r1," + assign_record->integer_value_);

    code_generation_.push_back("sw " + variable_record_address + "(r0),r1");

}

string CodeGenerator::generate_variable_declaration() {
    string result = "";
    for (string declaration: variable_declaration_generation_)
        result += declaration + "\n";
    return result;
}

string CodeGenerator::generate_code() {
    string result = "";
    for (string code: code_generation_)
        result += code + "\n";
    return result;
}
