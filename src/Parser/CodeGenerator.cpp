//
// Created by Harley on 3/30/2017.
//

#include "CodeGenerator.h"
#include "SymbolTable.h"

CodeGenerator::CodeGenerator() {
    func_count = 0;
}

void CodeGenerator::determine_func_stack_variable_offsets(SymbolRecord **record) {
    SymbolRecord* local_record = (*record);
    int variable_size = local_record->compute_record_size();
    int size = local_record->symbol_table_->parent_symbol_table_->symbol_records_.size();
    if (local_record->symbol_table_->parent_symbol_table_->symbol_record_ == NULL)
        return;
    local_record->symbol_table_->parent_symbol_table_->symbol_record_->offset_address_ += variable_size;

    if (size > 1) {
        SymbolRecord* previous_record = local_record->symbol_table_->parent_symbol_table_->symbol_records_[size - 2];
        local_record->offset_address_ = previous_record->offset_address_ + previous_record->compute_record_size();
    } else
        local_record->offset_address_ = 8;
    local_record->is_stack_variable_ = true;
}

void CodeGenerator::create_variable_code(SymbolRecord **record) {
    SymbolRecord* local_record = (*record);
    local_record->is_stack_variable_ = false;
    string code_variable_name = local_record->name_ + "_" + local_record->symbol_table_->parent_symbol_table_->symbol_record_->name_;
    if (local_record->array_sizes.size() > 0)
        variable_declaration_generation_.push_back(code_variable_name + " res " + to_string(local_record->compute_array_size()));
    else if (local_record->structure_ == "class" || local_record->structure_ == "class array") {
        SymbolRecord* found_record = local_record->symbol_table_->search(local_record->type_);
        if (found_record == NULL)
            return;
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

    // load the stack address in r13
    code_generation_.push_back("addi r13,r0,stack");

}
bool CodeGenerator::create_program_halt(bool double_pass) {
    if (double_pass)
        code_generation_.push_back("hlt");
    return true;

}
void CodeGenerator::create_relational_expression_code(ExpressionTree * expression) {
    ExpressionNode* left_expression = expression->root_node_->left_tree_;
    create_expression_code(left_expression);
    code_generation_.push_back("add r2,r1,r0");
    ExpressionNode* right_expression = expression->root_node_->right_tree_;
    create_expression_code(right_expression);
    string operator_type = expression->root_node_->record_->type_;
    if (operator_type == "EQUIV") {
        code_generation_.push_back("ceq r1,r2,r1");
    } else if (operator_type == "NOTEQ") {
        code_generation_.push_back("cne r1,r2,r1");
    } if (operator_type == "LT") {
        code_generation_.push_back("cle r1,r2,r1");
    } if (operator_type == "GT") {
        code_generation_.push_back("cgt r1,r2,r1");
    } if (operator_type == "LTEQ") {
        code_generation_.push_back("cle r1,r2,r1");
     }if (operator_type == "GTEQ") {
        code_generation_.push_back("cge r1,r2,r1");
    }


}

void CodeGenerator::create_expression_code(ExpressionNode *expression) {

    stack<SymbolRecord*>* tmp_post_fix_queue = new stack<SymbolRecord*>;
    expression->generate_queue(expression, tmp_post_fix_queue);
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
                code_generation_.push_back("mult r1,r2,r1");
            } else if (operator_type == "DIV") {
                code_generation_.push_back("div r1,r2,r1");
            } else if (operator_type == "AND") {
                code_generation_.push_back("and r1,r2,r1");
            } else if (operator_type == "ADD") {
                code_generation_.push_back("add r1,r2,r1");
            } else if (operator_type == "SUB") {
                code_generation_.push_back("sub r1,r2,r1");
            } else if (operator_type == "OR") {
                code_generation_.push_back("or r1,r2,r1");
            }

            code_generation_.push_back("sw 0(r13),r1");
            code_generation_.push_back("addi r13,r13,8");
            SymbolRecord* record = new SymbolRecord();
            //record->address = to_string(current_stack_address_);
            record->kind_ = "stack_variable";
            tmp_post_fix_queue->push(record);
        }
        else
            tmp_post_fix_queue->push(first_record);

    }
    load_record_into_register(tmp_post_fix_queue->top(), "r1");
    delete tmp_post_fix_queue;
    delete post_fix_queue;
}

void CodeGenerator::load_function_parameters_into_stack_memory_code(SymbolRecord* record) {
    code_generation_.push_back("addi r12,r13," + to_string(record->offset_address_));
    code_generation_.push_back("sw 0(r12),r1");

}

void CodeGenerator::load_record_into_register(SymbolRecord *record, string reg) {
    if (record->kind_ == "stack_variable") {
        code_generation_.push_back("subi r13,r13,8");
        code_generation_.push_back("lw " + reg + ",0(r13)");
    }
    else {
        load_or_call_record_into_reg(record, reg);
    }
}

bool CodeGenerator::create_put_code() {
    if (!second_pass_)
        return true;

    code_generation_.push_back("putc r1");
    return true;
}
bool CodeGenerator::create_for_loop() {
    if (!second_pass_)
        return true;
    code_generation_.push_back("for" + to_string(loop_count_));
    current_for_loop_.push(loop_count_);
    return true;
}

bool CodeGenerator::create_class_func_code(SymbolRecord* record) {
    if (!second_pass_)
        return true;
    code_generation_.push_back("class_func" + to_string(class_func_count));
    record->address = "class_func" + to_string(class_func_count++);
    return true;
}

bool CodeGenerator::create_func_code(SymbolRecord* record) {
    code_generation_.push_back(record->address);
    // store return address in r12 in memory
    code_generation_.push_back("subi r12,r13," + to_string(record->offset_address_ ));
    code_generation_.push_back("sw 0(r12),r11");
    return true;
}

bool CodeGenerator::create_func_return_code(SymbolRecord* record) {
    if (!second_pass_)
        return true;
    // load return address in memory
    code_generation_.push_back("subi r12,r13," + to_string(record->offset_address_ ));
    code_generation_.push_back("lw r11,0(r12)");
    code_generation_.push_back("jr r11");
    return true;
}
bool CodeGenerator::assign_func_address(SymbolRecord* record) {
    record->address = "func" + to_string(func_count++);
    //reserve space for the return address in the stack
    record->offset_address_ = 8;
    return true;
}

bool CodeGenerator::create_for_relation_loop() {
    if (!second_pass_)
        return true;
    code_generation_.push_back("add r14,r1,r0");
    return true;
}

bool CodeGenerator::create_end_for_loop() {
    if (!second_pass_)
        return true;
    code_generation_.push_back("bnz r14,for" + to_string(loop_count_++));
    current_for_loop_.pop();
    return true;
}

void CodeGenerator::create_function_call_code(SymbolRecord* func_record, string return_register) {
    //r13 is start of stack
    code_generation_.push_back("addi r13,r13," + to_string(func_record->offset_address_));
    code_generation_.push_back("jl r11," + func_record->address);
    code_generation_.push_back("subi r13,r13," + to_string(func_record->offset_address_));
    code_generation_.push_back("add " + return_register + ",r1,r0");
}


void CodeGenerator::create_variable_assignment_with_register(SymbolRecord *variable_record, string reg) {
    if (variable_record->is_stack_variable_) {
        //load offset address of variable i.e function start address + variable offset within function
        code_generation_.push_back("subi r12,r13," + to_string(variable_record->symbol_table_->parent_symbol_table_->symbol_record_->offset_address_ - variable_record->offset_address_));
        code_generation_.push_back("sw 0(r12)," + reg);
    } else if (variable_record->structure_ == "array"){
        code_generation_.push_back("sw " + variable_record->address + "(r9)," + reg);
        //clear indice access register
        code_generation_.push_back("add r8,r0,r0");
    }
    else {
        code_generation_.push_back("sw " + variable_record->address + "(r0)," + reg);
    }
}


void CodeGenerator::load_or_call_record_into_reg(SymbolRecord *load_record, string load_reg) {
    if (load_record->is_stack_variable_) {

        if (load_record->kind_ == "variable") {
            code_generation_.push_back("subi r12,r13," + to_string(load_record->symbol_table_->parent_symbol_table_->symbol_record_->offset_address_ - load_record->offset_address_));
            code_generation_.push_back("lw " + load_reg + ",0(r12)");
        }
        else if (load_record->type_ == "int")
            code_generation_.push_back("addi " + load_reg + ",r0," + to_string(load_record->integer_value_));
    } else {

        if (load_record->structure_ == "array" && load_record->kind_ == "variable") {
            code_generation_.push_back("lw " + load_reg + "," + load_record->address + "(r9)");
            //clear indice access register
            code_generation_.push_back("add r8,r0,r0");
        } else if (load_record->kind_ == "variable")
            code_generation_.push_back("lw " + load_reg +"," + load_record->address + "(r0)");
        else if (load_record->kind_ == "function") {
            create_function_call_code(load_record, load_reg);
        }
        else if (load_record->type_ == "int")
            code_generation_.push_back("addi " + load_reg + ",r0," + to_string(load_record->integer_value_));

    }

}
void CodeGenerator::create_array_indice_storage_code(SymbolRecord* record) {
    if (!second_pass_ || record->array_sizes.size() == 0)
        return;
    int size = record->compute_type_size();
    int array_dimension_size = record->array_sizes[0];
    record->array_sizes.erase(record->array_sizes.begin());
    //increase dimension access
    code_generation_.push_back("mul r9,r9,r8");

    //times the last calculated expression by type size
    code_generation_.push_back("muli r1,r1," + to_string(size));

    // add new dimension access to offset register
    code_generation_.push_back("add r9,r9,r1");
    // store
    code_generation_.push_back("addi r8,r0," + to_string(array_dimension_size * size));

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
