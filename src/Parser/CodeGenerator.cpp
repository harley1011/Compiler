//
// Created by Harley on 3/30/2017.
//

#include "CodeGenerator.h"
#include "SymbolTable.h"


CodeGenerator::CodeGenerator() {
    func_count = 0;
    enable_comments_ = true;
    errors_triggered_ = false;
}



void CodeGenerator::create_program_entry(SymbolRecord **record) {
    code_generation_.push_back("program entry");
    // load the stack address in r13
    code_generation_.push_back("addi r13,r0,stack");
    code_generation_.push_back("addi r13,r13," + to_string((*record)->record_size_) + add_comment_string("allocate stack memory for program"));
    (*record)->address = "program";



}
bool CodeGenerator::create_program_halt(bool double_pass) {
    if (double_pass)
        code_generation_.push_back("hlt");
    return true;

}
void CodeGenerator::create_relational_expression_code(ExpressionTree * expression) {
    ExpressionNode* left_expression = expression->get_root_node()->left_tree_;

    string operator_type = expression->get_root_node()->record_->type_;
    code_generation_.push_back(add_comment_string("---evaluating " + operator_type + " expression below ---"));

    create_expression_code(left_expression);
    code_generation_.push_back("add r2,r1,r0");
    code_generation_.push_back("subi r8,r8,4");
    code_generation_.push_back("sw topaddr(r8),r2" + add_comment_string("store left hand side of relational expression on stack"));
    //todo store on stack
    ExpressionNode* right_expression = expression->get_root_node()->right_tree_;
    create_expression_code(right_expression);

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

void CodeGenerator::create_expression_code(ExpressionNode *expression) {
    if (errors_triggered_)
        return;

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


            code_generation_.push_back(add_comment_string("---evaluating " + operator_type + " expression below ---"));
            load_record_into_register(first_record, "r2");

            if (second_record->kind_ == "function") // function call might contain expression and therefore clear r1 so we need to store it on the stack so we don't lose it
            {
                code_generation_.push_back("subi r8,r8,4");
                code_generation_.push_back("sw topaddr(r8),r2");
            }

            load_record_into_register(second_record, "r1");

            if (second_record->kind_ == "function") {
                code_generation_.push_back("lw r2,topaddr(r8)");
                code_generation_.push_back("addi r8,r8,4");
            }

            if (operator_type == "MULTI") {
                code_generation_.push_back("mul r1,r2,r1");
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

            code_generation_.push_back("subi r8,r8,4");
            code_generation_.push_back("sw topaddr(r8),r1");
            SymbolRecord* record = new SymbolRecord();
            //record->address = to_string(current_stack_address_);
            record->kind_ = "stack_variable";
            tmp_post_fix_queue->push(record);

            code_generation_.push_back(add_comment_string("---evaluting " + operator_type + " expression over ---"));

        }
        else
            tmp_post_fix_queue->push(first_record);

    }
    load_record_into_register(tmp_post_fix_queue->top(), "r1");
    delete tmp_post_fix_queue;
    delete post_fix_queue;
}

void CodeGenerator::load_function_parameters_into_stack_memory_code(SymbolRecord* record) {
    code_generation_.push_back("addi r12,r13," + to_string(record->parameter_offset_address_) + add_comment_string("loading parameter offset"));
    code_generation_.push_back("sw 0(r12),r1" + add_comment_string("storing value in function parameter stack"));

}

void CodeGenerator::load_record_into_register(SymbolRecord *record, string reg) {
    if (record->kind_ == "stack_variable") {
        code_generation_.push_back("lw " + reg + ",topaddr(r8)");
        code_generation_.push_back("addi r8,r8,4" + add_comment_string("remove stack variable from arithmetic expression"));
    }
    else {
        load_or_call_record_into_reg(record, reg);
    }
}

bool CodeGenerator::create_put_code() {
    if (!second_pass_)
        return true;

    int_and_string_converter.put_int_required = true;
    code_generation_.push_back("jl r15,putint");
    code_generation_.push_back("addi r1,r0,10");
    code_generation_.push_back("putc r1");
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
bool CodeGenerator::create_func_return_class_code(SymbolRecord* record) {
    if (!second_pass_)
        return true;
    // load return address in memory
    code_generation_.push_back("subi r12,r13," + to_string(record->record_size_ ) + add_comment_string("load return address in r12"));
    code_generation_.push_back("lw r11,0(r12)");
    code_generation_.push_back("jr r11");
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
    if (!second_pass_)
        return;
    create_array_index_calculation_code(func_record);

    code_generation_.push_back("addi r13,r13," + to_string(func_record->record_size_) + add_comment_string("allocate stack memory for function calls variables and parameters"));
    code_generation_.push_back("jl r11," + func_record->address);
    code_generation_.push_back("subi r13,r13," + to_string(func_record->record_size_) + add_comment_string("remove declared stack memory for function"));
    code_generation_.push_back("add " + return_register + ",r1,r0" + " % store return value in register");
}


void CodeGenerator::create_copy_class_values(SymbolRecord* variable_record, SymbolRecord* assign_record) {

    int function_size = variable_record->symbol_table_->parent_symbol_table_->symbol_record_->record_size_;

    code_generation_.push_back(add_comment_string("--- copying class variable ---"));

    if (assign_record->kind_ == "function") {
        create_function_parameter_code(assign_record, "r1");
        code_generation_.push_back(add_comment_string("---- function call code beg----"));
        create_function_call_code(assign_record, "r1");
        code_generation_.push_back(add_comment_string("---- function call code end ----"));
        code_generation_.push_back("add r11,r0,r1" + add_comment_string("load class address from return of functionc all"));
    }
    else
        code_generation_.push_back("subi r11,r13," + to_string(function_size - assign_record->offset_address_) + add_comment_string("load assign variable record offset"));

    code_generation_.push_back("add r1,r0,r0");
    code_generation_.push_back("subi r12,r13," + to_string(function_size - variable_record->offset_address_) + add_comment_string("load variable record offset"));
    code_generation_.push_back("copyclass" + to_string(copy_object_count));
    code_generation_.push_back("ceqi r2,r1," + to_string(variable_record->record_size_) + add_comment_string("check if more to copy"));
    code_generation_.push_back("bnz r2,copyclassend" + to_string(copy_object_count));
    code_generation_.push_back("lw r2,0(r11)");
    code_generation_.push_back("sw 0(r12),r2" + add_comment_string("copy class variable value"));
    code_generation_.push_back("addi r1,r1,4" + add_comment_string("move on to next property to copy"));
    code_generation_.push_back("addi r12,r12,4");
    code_generation_.push_back("addi r11,r11,4");
    code_generation_.push_back("j copyclass" + to_string(copy_object_count));
    code_generation_.push_back("copyclassend" + to_string(copy_object_count++));
    code_generation_.push_back(add_comment_string("--- copying class variable over ---"));
}

void CodeGenerator::create_variable_assignment_with_register(SymbolRecord *variable_record, string reg) {
    if (!second_pass_ || errors_triggered_)
        return;
    int function_size = variable_record->symbol_table_->parent_symbol_table_->symbol_record_->record_size_;
    if (variable_record->structure_ == "class") {

        code_generation_.push_back("add r5,r0," + reg);
        create_array_index_calculation_code(variable_record);

        if (variable_record->kind_ == "parameter") {
            code_generation_.push_back("subi r12,r13," + to_string(function_size - variable_record->offset_address_) +
                                 add_comment_string("load paramater class address"));
            code_generation_.push_back("lw r12,0(r12)" + add_comment_string("load stack class address"));
            code_generation_.push_back("addi r12,r12," + to_string(variable_record->data_member_offset_address_));
        } else {

            code_generation_.push_back("subi r12,r13," + to_string(
                    function_size - variable_record->offset_address_ - variable_record->data_member_offset_address_) +
                                 add_comment_string("load stack class data member offset"));
        }
        code_generation_.push_back("add r12,r12,r9");
        code_generation_.push_back("add r9,r0,r0" + add_comment_string("clear array indices register"));
        code_generation_.push_back("sw 0(r12),r5" + add_comment_string("store register value into stack class data member"));
    } else if (variable_record->structure_ == "array") {
        code_generation_.push_back("add r5,r0," + reg);

        create_array_index_calculation_code(variable_record);

        code_generation_.push_back("subi r12,r13," + to_string(function_size -variable_record->offset_address_) + add_comment_string("load stack array  offset"));

        if (variable_record->kind_ == "parameter") {
            code_generation_.push_back("lw r12,0(r12)" + add_comment_string("load array address"));
        }

        code_generation_.push_back("add r12,r12,r9");
        code_generation_.push_back("sw 0(r12),r5" + add_comment_string("store register value in stack array index"));
        code_generation_.push_back("add r9,r0,r0" + add_comment_string("clear array indices register"));
    }
    else {
        //load offset address of variable i.e function start address + variable offset within function
        code_generation_.push_back("subi r12,r13," + to_string(function_size - variable_record->offset_address_) + add_comment_string("load stack variable offset"));
        code_generation_.push_back("sw 0(r12)," + reg + add_comment_string("store register value into stack variable"));
    }



}
void CodeGenerator::load_or_call_record_into_reg(SymbolRecord *load_record, string load_reg) {

    if (load_record->type_ == "int" && load_record->structure_ == "")
        code_generation_.push_back("addi " + load_reg + ",r0," + to_string(load_record->integer_value_) + add_comment_string("load integer value"));
    else  if (load_record->kind_ == "function") {
        create_function_parameter_code(load_record, load_reg);
        code_generation_.push_back(add_comment_string("---- function call code beg----"));
        create_function_call_code(load_record, load_reg);
        code_generation_.push_back(add_comment_string("---- function call code end ----"));
    }
    else {
        int function_size = load_record->symbol_table_->parent_symbol_table_->symbol_record_->record_size_;
        if (load_record->structure_ == "class") {
            if (load_record->type_ == "int" || load_record->type_ == "float") {
                create_array_index_calculation_code(load_record);

                if (load_record->kind_ == "parameter") {
                    code_generation_.push_back("subi r12,r13," + to_string(function_size - load_record->offset_address_) + add_comment_string("load paramater class address"));
                    code_generation_.push_back("lw r12,0(r12)" + add_comment_string("load stack class address"));
                    code_generation_.push_back("addi r12,r12," + to_string(load_record->data_member_offset_address_));
                } else {

                    code_generation_.push_back("subi r12,r13," + to_string(
                            function_size - load_record->offset_address_ - load_record->data_member_offset_address_) +
                                         add_comment_string("load stack class data member offset"));
                }
                code_generation_.push_back("add r12,r12,r9");
                code_generation_.push_back("add r9,r0,r0" + add_comment_string("clear array indices register"));
                code_generation_.push_back("lw " + load_reg + ",0(r12)" +
                                     add_comment_string("load stack class data member into register"));
            } else {
                code_generation_.push_back("subi r1,r13," + to_string(function_size - load_record->offset_address_) + add_comment_string("load stack class address"));
            }

        } else if (load_record->structure_ == "array") {
            if (load_record->nested_array_parameters_.size() > 0) {

                create_array_index_calculation_code(load_record);

                code_generation_.push_back("subi r12,r13," + to_string(function_size - load_record->offset_address_) + add_comment_string("load stack array  offset"));
                code_generation_.push_back("add r12,r12,r9");
                code_generation_.push_back("lw " + load_reg + ",0(r12)" + add_comment_string("load stack array index value in function"));
                code_generation_.push_back("add r9,r0,r0" + add_comment_string("clear array indices register"));
            } else {
                code_generation_.push_back("subi r1,r13," + to_string(function_size - load_record->offset_address_) + add_comment_string("load stack array address"));
                //code_generation_.push_back("add " + load_reg + ",r12,r9" + add_comment_string("load array address"));
            }
        } else if (load_record->kind_ == "variable" || load_record->kind_ == "parameter") {
            code_generation_.push_back("subi r12,r13," + to_string(function_size - load_record->offset_address_));
            code_generation_.push_back(
                    "lw " + load_reg + ",0(r12)" + add_comment_string("load stack variable value in function"));
        }
    }

    create_single_operator_codes(load_record, load_reg);

}

void CodeGenerator::create_function_parameter_code(SymbolRecord *load_record,  string load_reg) {
    vector<ExpressionTree *> function_expression_parameters = load_record->function_parameters_;

    for (ExpressionTree* tree: function_expression_parameters) {
        SymbolRecord* current_record = tree->get_root_node()->record_;
        if (current_record->kind_ == "ADDOP" || current_record->kind_ == "MULTOP")
            create_expression_code(tree->get_root_node());
        else
            load_or_call_record_into_reg(current_record, load_reg);
        load_function_parameters_into_stack_memory_code(current_record);
    }


}

void CodeGenerator::create_single_operator_codes(SymbolRecord *record, string reg) {
    for(string operator_type: record->single_operators_before_) {
        if (operator_type == "NOT") {
            code_generation_.push_back("not " + reg + "," + reg);
        }else if (operator_type == "SUB") {
            code_generation_.push_back("sub " + reg + ",r0," + reg);
        }
    }
}

void CodeGenerator::create_array_index_calculation_code(SymbolRecord *record) {
    if (!second_pass_ || record->nested_array_parameters_.size() == 0)
        return;

    typedef map<string,pair< vector<ExpressionTree*>,vector<int>>>::iterator it_type;
    for(it_type iterator = record->nested_array_parameters_.begin(); iterator != record->nested_array_parameters_.end(); iterator++) {

        auto const current_pair = iterator->second;

        int current_array_dimension = 0;
        for (ExpressionTree *tree: current_pair.first ) {
            int array_dimension_size = current_pair.second[current_array_dimension++];

            code_generation_.push_back(add_comment_string("--- compute index value ---"));

            if (tree->get_root_node()->record_->kind_ == "RELOP") {
                create_relational_expression_code(tree);
            } else
                create_expression_code(tree->get_root_node());

            code_generation_.push_back(add_comment_string("--- compute index over ---"));

            code_generation_.push_back("muli r9,r9," + to_string(array_dimension_size));
            code_generation_.push_back("muli r1,r1," + to_string(4) + add_comment_string(
                    "multiply last evaluated expression stored in r1 by the arrays type size"));
            code_generation_.push_back(
                    "add r9,r9,r1" + add_comment_string("add new dimension access to current offset counter"));

        }
    }

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

