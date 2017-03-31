//
// Created by Harley on 3/30/2017.
//

#include "CodeGenerator.h"
#include "SymbolTable.h"

CodeGenerator::CodeGenerator() {

}


void CodeGenerator::create_variable_code(SymbolRecord **record) {
    SymbolRecord* local_record = (*record);
    string code_variable_name = local_record->name_ + "_" + local_record->symbol_table_->parent_symbol_table_->symbol_record_->name_;
    if (local_record->array_sizes.size() > 0)
        variable_declaration_generation_.push_back(code_variable_name + " res " + to_string(local_record->array_sizes.size()));
    else if (local_record->structure_ == "class" || local_record->structure_ == "class array") {
        SymbolRecord* found_record = local_record->symbol_table_->search(local_record->type_);
        variable_declaration_generation_.push_back(
                code_variable_name + " res " + to_string(found_record->compute_class_byte_size()));
    }
    else
        variable_declaration_generation_.push_back(code_variable_name + " dw 0");
    (*record)->address = code_variable_name;

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

void CodeGenerator::create_expression_code(ExpressionTree * expression) {

    stack<SymbolRecord*>* tmp_post_fix_queue = new stack<SymbolRecord*>;
    expression->calculate_total(expression->root_node_, tmp_post_fix_queue);
    stack<SymbolRecord*>* post_fix_queue = new stack<SymbolRecord*>;;

    while (!tmp_post_fix_queue->empty()) {
        SymbolRecord* record = tmp_post_fix_queue->top();
        post_fix_queue->push(record);
        tmp_post_fix_queue->pop();
    }

    while (post_fix_queue->size() > 0) {
        SymbolRecord* first_record = post_fix_queue->top();
        post_fix_queue->pop();
        if (first_record->kind_ == "MULTOP" || first_record->kind_ == "ADDOP") {
            string operator_type = first_record->type_;
            first_record = tmp_post_fix_queue->top();
            tmp_post_fix_queue->pop();
            SymbolRecord* second_record = tmp_post_fix_queue->top();
            tmp_post_fix_queue->pop();

            if (first_record->type_ == "int") {

            }


            if (operator_type == "MULTI") {

            } else if (operator_type == "DIV") {

            } else if (operator_type == "AND") {

            } else if (operator_type == "ADD") {

            } else if (operator_type == "SUB") {

            } else if (operator_type == "OR") {

            }
        }
        tmp_post_fix_queue->push(first_record);

    }
    SymbolRecord* return_record = tmp_post_fix_queue->top();
    delete tmp_post_fix_queue;
    delete post_fix_queue;
}

void CodeGenerator::create_variable_assignment_with_variable_code(SymbolRecord *variable_record,
                                                                  SymbolRecord *assign_record) {
    string assign_record_address = assign_record->address;
    string variable_record_address = variable_record->address;

    if (assign_record->kind_ == "variable")
        code_generation_.push_back("lw r1," + assign_record->address + "(r0)");
    else if (assign_record->type_ == "int")
        code_generation_.push_back("lw r1," + to_string(assign_record->integer_value_));

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
