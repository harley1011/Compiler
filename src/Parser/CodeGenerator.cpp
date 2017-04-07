//
// Created by Harley on 3/30/2017.
//

#include "CodeGenerator.h"
#include "SymbolTable.h"


CodeGenerator::CodeGenerator() {
    func_count = 0;
    enable_comments_ = true;
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
                code_variable_name + " res " + to_string(found_record->record_size_));
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

    string operator_type = expression->root_node_->record_->type_;
    code_generation_.push_back(add_comment_string("---evaluating " + operator_type + " expression below ---"));

    create_expression_code(left_expression, NULL);
    code_generation_.push_back("add r2,r1,r0");
    code_generation_.push_back("subi r8,r8,4");
    code_generation_.push_back("sw topaddr(r8),r2" + add_comment_string("store left hand side of relational expression on stack"));
    //todo store on stack
    ExpressionNode* right_expression = expression->root_node_->right_tree_;
    create_expression_code(right_expression, NULL);

    code_generation_.push_back("lw r2,topaddr(r8)");
    code_generation_.push_back("addi r8,r8,4");

    if (operator_type == "EQUIV") {
        code_generation_.push_back("ceq r1,r2,r1");
    } else if (operator_type == "NOTEQ") {
        code_generation_.push_back("cne r1,r2,r1");
    } if (operator_type == "LT") {
        code_generation_.push_back("clt r1,r2,r1");
    } if (operator_type == "GT") {
        code_generation_.push_back("cgt r1,r2,r1");
    } if (operator_type == "LTEQ") {
        code_generation_.push_back("cle r1,r2,r1");
     }if (operator_type == "GTEQ") {
        code_generation_.push_back("cge r1,r2,r1");
    }

    code_generation_.push_back(add_comment_string("---evaluating " + operator_type + " expression over ---"));
}

void CodeGenerator::create_expression_code(ExpressionNode *expression, vector<string>* code_list) {

    if (code_list == NULL)
        code_list = &code_generation_;
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
            SymbolRecord* second_record = tmp_post_fix_queue->top();
            tmp_post_fix_queue->pop();

            first_record = tmp_post_fix_queue->top();
            tmp_post_fix_queue->pop();


            code_list->push_back(add_comment_string("---evaluating " + operator_type + " expression below ---"));
            load_record_into_register(first_record, "r2", code_list);

            if (second_record->kind_ == "function") // function call might contain expression and therefore clear r1 so we need to store it on the stack so we don't lose it
            {
                code_list->push_back("subi r8,r8,4");
                code_list->push_back("sw topaddr(r8),r2");
            }

            load_record_into_register(second_record, "r1", code_list);

            if (second_record->kind_ == "function") {
                code_list->push_back("lw r2,topaddr(r8)");
                code_list->push_back("addi r8,r8,4");
            }

            if (operator_type == "MULTI") {
                code_list->push_back("mul r1,r2,r1");
            } else if (operator_type == "DIV") {
                code_list->push_back("div r1,r2,r1");
            } else if (operator_type == "AND") {
                code_list->push_back("and r1,r2,r1");
            } else if (operator_type == "ADD") {
                code_list->push_back("add r1,r2,r1");
            } else if (operator_type == "SUB") {
                code_list->push_back("sub r1,r2,r1");
            } else if (operator_type == "OR") {
                code_list->push_back("or r1,r2,r1");
            }

            code_list->push_back("subi r8,r8,4");
            code_list->push_back("sw topaddr(r8),r1");
            SymbolRecord* record = new SymbolRecord();
            //record->address = to_string(current_stack_address_);
            record->kind_ = "stack_variable";
            tmp_post_fix_queue->push(record);

            code_list->push_back(add_comment_string("---evaluting " + operator_type + " expression over ---"));

        }
        else
            tmp_post_fix_queue->push(first_record);

    }
    load_record_into_register(tmp_post_fix_queue->top(), "r1", code_list);
    delete tmp_post_fix_queue;
    delete post_fix_queue;
}

void CodeGenerator:: load_function_parameters_into_stack_memory_code(SymbolRecord* record, vector<string> *code_list) {
    code_list->push_back("addi r12,r13," + to_string(record->offset_address_) + add_comment_string("loading parameter offset"));
    code_list->push_back("sw 0(r12),r1" + add_comment_string("storing value in function parameter stack"));

}

void CodeGenerator::load_record_into_register(SymbolRecord *record, string reg, vector<string>* code_list) {
    if (record->kind_ == "stack_variable") {
        code_list->push_back("lw " + reg + ",topaddr(r8)");
        code_list->push_back("addi r8,r8,4" + add_comment_string("remove stack variable from arithmetic expression"));
    }
    else {
        load_or_call_record_into_reg(record, reg, code_list);
    }
}

bool CodeGenerator::create_put_code() {
    if (!second_pass_)
        return true;

    int_and_string_converter.put_int_required = true;
    code_generation_.push_back("jl r15,putint");
    return true;
}

bool CodeGenerator::create_get_code() {
    if (!second_pass_)
        return true;

    int_and_string_converter.get_int_required = true;
    code_generation_.push_back("jl r15,getint");
    return true;
}

bool CodeGenerator::create_for_loop() {
    if (!second_pass_)
        return true;
    code_generation_.push_back("for" + to_string(loop_count_));
    current_for_loop_.push(loop_count_++);
    return true;
}
bool CodeGenerator::create_if() {
    if (!second_pass_)
        return true;
    code_generation_.push_back("bz r1,else" + to_string(if_count_));
    current_if_.push(if_count_++);
    return true;
}
bool CodeGenerator::create_if_else() {
    if (!second_pass_)
        return true;
    code_generation_.push_back("j ifend" + to_string(current_if_.top()));
    code_generation_.push_back("else" + to_string(current_if_.top()));
    return true;
}
bool CodeGenerator::create_if_end() {
    if (!second_pass_)
        return true;
    code_generation_.push_back("ifend" + to_string(current_if_.top()));
    current_if_.pop();
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
    code_generation_.push_back(record->address + add_comment_string("func " + record->name_ + " entry point"));
    // store return address in r12 in memory
    code_generation_.push_back("subi r12,r13," + to_string(record->record_size_)  + add_comment_string("store the return address in stack memory"));
    code_generation_.push_back("sw 0(r12),r11");
    return true;
}

bool CodeGenerator::create_func_return_code(SymbolRecord* record) {
    if (!second_pass_)
        return true;
    // load return address in memory
    code_generation_.push_back("subi r12,r13," + to_string(record->record_size_ ) + add_comment_string("load return address in r12"));
    code_generation_.push_back("lw r11,0(r12)");
    code_generation_.push_back("jr r11");
    return true;
}
bool CodeGenerator::assign_func_address(SymbolRecord* record) {
    record->address = "func" + to_string(func_count++);
    //reserve space for the return address in the stack
    record->record_size_ = 4;
    return true;
}

bool CodeGenerator::create_for_relation_loop() {
    if (!second_pass_)
        return true;
    code_generation_.push_back("bz r1,forend" + to_string(current_for_loop_.top()));
    return true;
}

bool CodeGenerator::create_end_for_loop() {
    if (!second_pass_)
        return true;
    code_generation_.push_back("j for" + to_string(current_for_loop_.top()));
    code_generation_.push_back("forend" + to_string(current_for_loop_.top()));
    current_for_loop_.pop();
    return true;
}

string CodeGenerator::add_comment_string(string comment) {
    if (enable_comments_)
        return " % " + comment;
    else
        return "";
}

void CodeGenerator::create_function_call_code(SymbolRecord* func_record, string return_register) {
    //r13 is start of stack

    if (func_record->accessor_code_.size() > 1)
        code_generation_.insert(code_generation_.end(), func_record->accessor_code_.begin(), func_record->accessor_code_.end());

    code_generation_.push_back("addi r13,r13," + to_string(func_record->record_size_) + add_comment_string("allocate stack memory for function calls variables and parameters"));
    code_generation_.push_back("jl r11," + func_record->address);
    code_generation_.push_back("subi r13,r13," + to_string(func_record->record_size_) + add_comment_string("remove declared stack memory for function"));
    code_generation_.push_back("add " + return_register + ",r1,r0" + " % store return value in register");
}

void CodeGenerator::create_variable_assignment_with_register(SymbolRecord *variable_record, string reg) {
    create_variable_assignment_with_register(variable_record, reg, &code_generation_);
}

void CodeGenerator::create_variable_assignment_with_register(SymbolRecord *variable_record, string reg, vector<string>* code_list) {
    if (!second_pass_)
        return;

    if (variable_record->is_stack_variable_) {
        int function_size = variable_record->symbol_table_->parent_symbol_table_->symbol_record_->record_size_;
        if (variable_record->structure_ == "class") {
            code_list->push_back("subi r12,r13," + to_string(function_size - variable_record->offset_address_ - variable_record->data_member_offset_address_) + add_comment_string("load stack class data member offset"));
            code_list->push_back("add r12,r12,r9");
            code_list->push_back("add r9,r0,r0" + add_comment_string("clear array indices register"));
            code_list->push_back("sw 0(r12)," + reg + add_comment_string("store register value into stack class data member"));
        } else if (variable_record->structure_ == "array") {
            if (variable_record->accessor_code_.size() > 1) {
                code_list->push_back("add r5,r0," + reg);
                code_list->insert(code_list->end(), variable_record->accessor_code_.begin(),
                                  variable_record->accessor_code_.end());
            }
            code_list->push_back("subi r12,r13," + to_string(function_size -variable_record->offset_address_) + add_comment_string("load stack array  offset"));
            code_list->push_back("add r12,r12,r9");
            code_list->push_back("sw 0(r12),r5" + add_comment_string("store register value in stack array indiex"));
        }
        else {
            //load offset address of variable i.e function start address + variable offset within function
            code_list->push_back("subi r12,r13," + to_string(function_size - variable_record->offset_address_) + add_comment_string("load stack variable offset"));
            code_list->push_back("sw 0(r12)," + reg + add_comment_string("store register value into stack variable"));
        }


    } else if (variable_record->structure_ == "array"){
        if (variable_record->accessor_code_.size() > 1) {
            code_list->push_back("add r5,r0," + reg);
            code_list->insert(code_list->end(), variable_record->accessor_code_.begin(),
                              variable_record->accessor_code_.end());
        }

        code_list->push_back("sw " + variable_record->address + "(r9),r5" + add_comment_string("access array index"));
        //clear indice access register
        code_list->push_back("add r9,r0,r0" + add_comment_string("clear array indices register"));
    } else if (variable_record->structure_ == "class") {
        code_list->push_back("addi r7,r9," + to_string(variable_record->data_member_offset_address_) + add_comment_string("set register with class data member offset"));
        code_list->push_back("add r9,r0,r0" + add_comment_string("clear array indices register"));
        code_list->push_back("sw " +  variable_record->address + "(r7)," + reg + add_comment_string("store register value in class data member"));
    }
    else {
        code_list->push_back("sw " + variable_record->address + "(r0)," + reg);
    }
}
void CodeGenerator::load_or_call_record_into_reg(SymbolRecord *load_record, string load_reg, vector<string>* code_list) {

    if (load_record->is_stack_variable_) {
        int function_size = load_record->symbol_table_->parent_symbol_table_->symbol_record_->record_size_;
        if (load_record->structure_ == "class") {
            code_list->push_back("subi r12,r13," + to_string(function_size - load_record->offset_address_ - load_record->data_member_offset_address_) + add_comment_string("load stack class data member offset"));
            code_list->push_back("add r12,r12,r9");
            code_list->push_back("add r9,r0,r0" + add_comment_string("clear array indices register"));
            code_list->push_back("lw " + load_reg + ",0(r12)" + add_comment_string("load stack class data member into register"));
        } else if (load_record->structure_ == "array") {
            if (load_record->accessor_code_.size() > 1)
                code_list->insert(code_list->end(), load_record->accessor_code_.begin(), load_record->accessor_code_.end());
            code_list->push_back("subi r12,r13," + to_string(function_size -load_record->offset_address_) + add_comment_string("load stack array  offset"));
            code_list->push_back("add r12,r12,r9");
            code_list->push_back("lw " + load_reg + ",0(r12)" + add_comment_string("load stack array index value in function"));
        } else if (load_record->kind_ == "variable") {
            code_list->push_back("subi r12,r13," + to_string(function_size - load_record->offset_address_));
            code_list->push_back("lw " + load_reg + ",0(r12)" + add_comment_string("load stack variable value in function"));
        }
        else if (load_record->type_ == "int")
            code_list->push_back("addi " + load_reg + ",r0," + to_string(load_record->integer_value_) + add_comment_string("load integer value"));
        create_single_operator_codes(load_record, load_reg);
    } else {

        if (load_record->structure_ == "array" && load_record->kind_ == "variable") {
            if (load_record->accessor_code_.size() > 1)
                code_list->insert(code_list->end(), load_record->accessor_code_.begin(), load_record->accessor_code_.end());
            code_list->push_back("lw " + load_reg + "," + load_record->address + "(r9)" + add_comment_string("load array index value into register"));
            //clear indice access register
            code_list->push_back("add r9,r0,r0" + add_comment_string("clear array indices register"));
        } else if (load_record->kind_ == "variable" && load_record->structure_ == "class") {
            code_list->push_back("addi r7,r9," + to_string(load_record->data_member_offset_address_));
            code_list->push_back("add r9,r0,r0" + add_comment_string("clear array indices register"));
            code_list->push_back("lw " + load_reg + "," + load_record->address + "(r7)" + add_comment_string("load class data member into register"));
        } else if (load_record->kind_ == "variable")
            code_list->push_back("lw " + load_reg +"," + load_record->address + "(r0)" + add_comment_string("load variable into register"));
        else if (load_record->kind_ == "function") {
            code_list->push_back(add_comment_string("---- function call code beg----"));
            create_function_call_code(load_record, load_reg);
            code_list->push_back(add_comment_string("---- function call code end ----"));
        }
        else if (load_record->type_ == "int")
            code_list->push_back("addi " + load_reg + ",r0," + to_string(load_record->integer_value_));
        create_single_operator_codes(load_record, load_reg);
    }
}

void CodeGenerator::load_or_call_record_into_reg(SymbolRecord *load_record, string load_reg) {
    load_or_call_record_into_reg(load_record, load_reg, &code_generation_);

}
void CodeGenerator::create_single_operator_codes(SymbolRecord *record, string reg) {
    create_single_operator_codes(record, reg, &code_generation_);
}

void CodeGenerator::create_single_operator_codes(SymbolRecord *record, string reg, vector<string>* code_list) {
    for(string operator_type: record->single_operators_before_) {
        if (operator_type == "NOT") {
            code_list->push_back("not " + reg + "," + reg);
        }else if (operator_type == "SUB") {
            code_list->push_back("sub " + reg + ",r0," + reg);
        }
    }
}

void CodeGenerator::create_array_indice_storage_code(SymbolRecord* record,  vector<string> *code_list) {
    if (!second_pass_ || record->array_sizes.size() == 0)
        return;
    int array_dimension_size = record->array_sizes[record->current_array_position_++];
    //increase dimension access

    code_list->push_back("muli r9,r9," + to_string(array_dimension_size));
    code_list->push_back("muli r1,r1," + to_string(4) + add_comment_string("multiply last evaluated expression stored in r1 by the arrays type size"));
    code_list->push_back("add r9,r9,r1" + add_comment_string("add new dimension access to current offset counter"));


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

    result += int_and_string_converter.generate_required_functions();
    result += "stack";
    return result;
}
