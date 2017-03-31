//
// Created by Harley on 3/30/2017.
//

#include "CodeGenerator.h"
#include "SymbolTable.h"

CodeGenerator::CodeGenerator() {
    current_stack_address_ = 0;
}


void CodeGenerator::create_variable_code(SymbolRecord **record) {
    SymbolRecord* local_record = (*record);
    string code_variable_name = local_record->name_ + "_" + local_record->symbol_table_->parent_symbol_table_->symbol_record_->name_;
    if (local_record->array_sizes.size() > 0)
        variable_declaration_generation_.push_back(code_variable_name + " res " + to_string(local_record->compute_array_size()));
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

            load_record_into_register(first_record, "r1");
            load_record_into_register(second_record, "r2");

            if (operator_type == "MULTI") {
                code_generation_.push_back("mult r1,r1,r2");
            } else if (operator_type == "DIV") {

            } else if (operator_type == "AND") {
            } else if (operator_type == "ADD") {
                code_generation_.push_back("add r1,r1,r2");
            } else if (operator_type == "SUB") {
                code_generation_.push_back("sub r1,r1,r2");
            } else if (operator_type == "OR") {

            }

            code_generation_.push_back("addi r2,r0," + to_string(current_stack_address_));
            code_generation_.push_back("sw topaddr(r2),r1");
            SymbolRecord* record = new SymbolRecord();
            record->address = to_string(current_stack_address_);
            record->kind_ = "stack_variable";
            tmp_post_fix_queue->push(record);
            current_stack_address_+= 32;
        }
        else
            tmp_post_fix_queue->push(first_record);

    }
    load_record_into_register(tmp_post_fix_queue->top(), "r1");
    delete tmp_post_fix_queue;
    delete post_fix_queue;
}

void CodeGenerator::load_record_into_register(SymbolRecord *record, string reg) {
    if (record->kind_ == "")
        code_generation_.push_back("addi " + reg + ",r0," + to_string(record->integer_value_));
    else if (record->kind_ == "stack_variable") {
        code_generation_.push_back("addi r3,r0," + record->address);
        code_generation_.push_back("lw " + reg + ",topaddr(r3)");
        current_stack_address_ -= 32;
    }
    else {
        code_generation_.push_back("lw " + reg + "," + record->address + "(r0)");
    }
}

void CodeGenerator::create_variable_assignment_with_variable_code(SymbolRecord *variable_record,
                                                                  SymbolRecord *assign_record) {
    string assign_record_address = assign_record->address;
    string variable_record_address = variable_record->address;

    if (assign_record->kind_ == "variable")
        code_generation_.push_back("lw r1," + assign_record->address + "(r0)");
    else if (assign_record->type_ == "int")
        code_generation_.push_back("addi r1,r0," + to_string(assign_record->integer_value_));

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
