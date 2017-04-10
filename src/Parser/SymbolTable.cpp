//
// Created by Harley on 3/9/2017.
//
#include <iostream>
#include <iomanip>
#include <sstream>
#include "SymbolTable.h"
#include <algorithm>

SymbolRecord* find_nested_record(SymbolRecord* record, SymbolRecord* found_record);
void determine_record_fields(SymbolRecord* record);

SymbolTable::SymbolTable() {
    second_pass_ = false;
    parent_symbol_table_ = NULL;
    code_generator_ = NULL;
    symbol_record_ = NULL;
}


bool SymbolTable::check_if_get_variable_is_correct_type_and_create_code(SymbolRecord *record) {
    if (!second_pass_)
        return true;
    if (check_if_variable_or_func_exist(record)) {
        SymbolRecord* found_record = search(record->name_);
        if (record->nested_properties_.size() > 0)
            found_record = find_nested_record(record, found_record);

        if (found_record->type_ == "int" || found_record->type_ == "float")
            get_code_generator()->create_variable_assignment_with_register(found_record, "r1");
        else
            report_error_to_highest_symbol_table("Error: can't place get value into variable " + record->name_ + " not of type int or float:");
    }
    return true;
}

bool SymbolTable::check_if_variable_or_func_exist(SymbolRecord *record) {
    if (second_pass_) {
        if (record->name_ == "")
            return true;
        SymbolRecord* found_record = search(record->name_);
        if (found_record == NULL) {
            string kind = record->kind_;
            if (record->nested_properties_.size() > 0 || kind == "") {
                kind = "variable";
            }
            report_error_to_highest_symbol_table("Error: " + record->name_ + " " + kind + " is being used without being declared:");
            return false;
        } else if (record->nested_properties_.size() > 0 ) {
            record->kind_ = found_record->kind_;
            found_record = check_nested_property_and_compute_offset(record, found_record);
            if (found_record == NULL)
                return false;
        }
        copy_stored_record(record);
        if (found_record->kind_ == "function") {
            check_func_parameters(found_record, record);
        }

    }
    return true;
}

bool SymbolTable::copy_stored_record(SymbolRecord *record) {
    SymbolRecord* found_record = search(record->name_);
    if (found_record == NULL)
        return true;
    record->address = found_record->address;
    record->is_stack_variable_ = found_record->is_stack_variable_;
    record->symbol_table_ = found_record->symbol_table_;
    record->offset_address_ = found_record->offset_address_;
    record->data_member_offset_address_ = found_record->data_member_offset_address_;
    record->structure_ = found_record->structure_;
    record->array_sizes = found_record->array_sizes;
    record->type_ = found_record->type_;
    record->record_size_ = found_record->record_size_;
    record->symbol_table_->parent_symbol_table_ = found_record->symbol_table_->parent_symbol_table_;
    return true;
}


bool SymbolTable::copy_stored_record(SymbolRecord *record, SymbolRecord* found_record) {

    record->address = found_record->address;
    record->is_stack_variable_ = found_record->is_stack_variable_;
    record->symbol_table_ = found_record->symbol_table_;
    record->offset_address_ = found_record->offset_address_;
    record->data_member_offset_address_ = found_record->data_member_offset_address_;
    record->structure_ = found_record->structure_;
    record->array_sizes = found_record->array_sizes;
    record->type_ = found_record->type_;
    record->kind_ = found_record->kind_;
    record->record_size_ = found_record->record_size_;
    record->symbol_table_->parent_symbol_table_ = found_record->symbol_table_->parent_symbol_table_;
    record->record_size_ = found_record->record_size_;
    return true;
}

SymbolRecord * SymbolTable::check_nested_property_and_compute_offset(SymbolRecord *record, SymbolRecord *found_record) {
    if (found_record->type_ == "int")
        report_error_to_highest_symbol_table("Error: " + record->name_ + " is of type int and doesn't have any properties to access:");
    else if (found_record->type_ == "float")
        report_error_to_highest_symbol_table("Error: " + record->name_ + " is of type float and doesn't have any properties to access:");
    else {
        int offset_count = 0;
        SymbolRecord* current_record = found_record;
        SymbolRecord* current_found_record;
        SymbolTable* top_symbol_table = found_record->symbol_table_->parent_symbol_table_;
        if (top_symbol_table->parent_symbol_table_ != NULL)
            top_symbol_table = top_symbol_table->parent_symbol_table_;
        for (int i = 0; i < record->nested_properties_.size(); i++) {
            string property = record->nested_properties_[i];
            current_found_record = top_symbol_table->search(current_record->type_);

            if (current_found_record != NULL) // make sure class exists
                current_record = current_found_record->symbol_table_->search(property);

            if (current_record == NULL) {
                string kind = record->kind_;
                if (record->kind_ == "")
                    kind = "variable";

                report_error_to_highest_symbol_table("Error: invalid nested " + kind + " " + property + " on variable " + record->name_ + ":");
                return current_record;
            } else {
                offset_count += current_record->offset_address_;
            }
        }
        found_record->data_member_offset_address_ = offset_count;
        return current_record;
    }
    return NULL;
}



SymbolRecord* find_nested_record(SymbolRecord* record, SymbolRecord* found_record) {
    SymbolRecord* current_record = found_record;
    if (found_record == NULL)
        return NULL;
    SymbolTable* top_symbol_table = found_record->symbol_table_->parent_symbol_table_;
    if (top_symbol_table->parent_symbol_table_ != NULL)
        top_symbol_table = top_symbol_table->parent_symbol_table_;
    int offset_count = 0;
    for (int i = 0; i < record->nested_properties_.size(); i++) {
        string property = record->nested_properties_[i];
        current_record = top_symbol_table->search(current_record->type_);
        if (current_record != NULL) // make sure class exists
            current_record = current_record->symbol_table_->search(property);

        if (current_record == NULL) {
            return NULL;
        }
        offset_count += current_record->offset_address_;

    }
    found_record->data_member_offset_address_ = offset_count;
    return  current_record;
}


bool SymbolTable::check_indice_expression_is_valid(SymbolRecord* record, ExpressionTree *tree) {
    if (!second_pass_)
        return  true;
    if (check_expression_is_valid(tree)) {

        string property = record->name_;
        if (record->nested_properties_.size() > 0)
            property = record->nested_properties_[record->nested_properties_.size() - 1];

        SymbolRecord *nested_record;
        SymbolRecord *found_record = search(record->name_);
        if (found_record == NULL)
            return true;
        nested_record = find_nested_record(record, found_record);
        if (nested_record == NULL)
            nested_record = record;
        int variable_size = 4;
        get<0>(record->nested_array_parameters_[property]).push_back(tree);
        get<1>(record->nested_array_parameters_[property]) = nested_record->array_sizes;
        if (record->structure_ == "class" || record->structure_ == "class array") {
            SymbolRecord* class_record = search(record->type_);
            if (class_record == NULL)
                return true;
            variable_size = class_record->record_size_;
        }
        get<2>(record->nested_array_parameters_[property]) = variable_size;
        return true;
    }
    return true;
}

bool SymbolTable::check_expression_is_valid(ExpressionTree *tree) {
    if (!second_pass_)
        return false;

    if (tree->get_root_node()->record_->kind_ == "RELOP") {
        ExpressionNode* left_expression = tree->get_root_node()->left_tree_;
        ExpressionNode* right_expression = tree->get_root_node()->right_tree_;

        bool result = check_valid_relational_expression(left_expression);
        if (check_valid_relational_expression(right_expression) && result)
            return true;

    } else {
        if (check_valid_arithmetic_expression(tree->get_root_node()))
            return true;
    }
    return false;
}

bool SymbolTable::check_expression_is_valid_and_generate_code(ExpressionTree *tree) {
    if (!second_pass_)
        return true;
    if (check_expression_is_valid(tree)) {
        if (tree->get_root_node()->record_->kind_ == "RELOP")
            get_code_generator()->create_relational_expression_code(tree);
        else
            get_code_generator()->create_expression_code(tree->get_root_node());
    }
    return true;
}

bool SymbolTable::check_valid_relational_expression(ExpressionNode *expression) {
    if (expression->record_->kind_ == "ADDOP" || expression->record_->kind_ == "MULTOP") {
        check_valid_arithmetic_expression(expression);
        } else  {
            if (check_if_variable_or_func_exist(expression->record_)) {
                if (expression->record_->type_ != "int" || expression->record_->type_ != "float") {
                    SymbolRecord *current_found_record = search(expression->record_->name_);
                    if (expression->record_->nested_properties_.size() > 0)
                        current_found_record = find_nested_record(expression->record_, current_found_record);
                    if (current_found_record != NULL && current_found_record->type_ != "int" &&
                        current_found_record->type_ != "float") {
                        report_error_to_highest_symbol_table(
                                "Error: variable " + current_found_record->name_ + " of type " +
                                current_found_record->type_ +
                                " can't be in a relational expression it needs to be of type int or float:");
                        return false;
                    }

                }
            }
        }
    return true;
}

bool SymbolTable::check_valid_arithmetic_expression(ExpressionNode *node) {
    vector<SymbolRecord*> identifiers = node->all_identifiers_in_expression();
    bool is_arithmetic = (node->record_->kind_ == "ADDOP" || node->record_->kind_ == "MULTOP");
    bool no_error = true;
    if (identifiers.size() > 0) {
            for (SymbolRecord* current_record: identifiers) {
                if (check_if_variable_or_func_exist(current_record)) {
                    SymbolRecord *current_found_record = search(current_record->name_);

                    if (identifiers.size() > 1)
                        check_correct_number_of_array_dimensions(current_found_record, current_record);

                    if (current_found_record != NULL)
                        current_found_record = find_nested_record(current_record, current_found_record);

                    if (current_found_record != NULL)
                        current_record->type_ = current_found_record->type_;

                    if (is_arithmetic && current_found_record != NULL && !check_if_record_is_num_type(current_found_record)) {
                        if (current_record->nested_properties_.size() == 0) {
                            report_error_to_highest_symbol_table(
                                    "Error: can't perform arithmetic operations with " + current_record->kind_ + " " +
                                    current_record->name_ + " that is not of type int or float:");
                            no_error = false;
                        } else {
                            report_error_to_highest_symbol_table(
                                    "Error: can't perform arithmetic operations with " + current_record->kind_ + " " +
                                    current_record->generate_nested_properties_string() +
                                    " that is not of type int or float:");
                            no_error = false;
                        }
                    }
                } else
                    no_error = false;
            }
        }
    return no_error;
}

bool SymbolTable::check_expression_tree_for_correct_type_and_create_assignment_code(SymbolRecord *variable_record,
                                                                                    ExpressionTree *tree) {
    if (!second_pass_) {
        return true;
    }
    string variable_property;
    SymbolRecord* found_variable_record = search(variable_record->name_);
    if (found_variable_record == NULL) {
        report_error_to_highest_symbol_table("Error: " + variable_record->name_ + " variable is being used without being declared:");
    } else {
        variable_property = found_variable_record->type_;
        if (variable_record->nested_properties_.size() > 0) {
            SymbolRecord* nested_variable = check_nested_property_and_compute_offset(variable_record, found_variable_record);

            if (nested_variable == NULL)
                return true;
            else
                variable_property = nested_variable->type_;
            check_correct_number_of_array_dimensions(found_variable_record, variable_record);
        } else {
            check_correct_number_of_array_dimensions(found_variable_record, variable_record);
        }
    }

    if ((tree->get_root_node()->record_->kind_ == "ADDOP" || tree->get_root_node()->record_->kind_ == "MULTOP" || tree->get_root_node()->record_->kind_ == "RELOP") && found_variable_record != NULL) {

        if (variable_property != "float" && variable_property != "int") {
            report_error_to_highest_symbol_table("Error: can't assign variable " + found_variable_record->name_ + " of type " + found_variable_record->type_  + " an arithmetic expression, it must be of type int or float:");
        }
        copy_stored_record(variable_record, found_variable_record);
        check_expression_is_valid_and_generate_code(tree);
        get_code_generator()->create_variable_assignment_with_register(variable_record, "r1");

    }
    else if (found_variable_record != NULL) {
        SymbolRecord *assign_record = tree->get_root_node()->record_;

        if (variable_property != assign_record->type_ &&
            ((assign_record->type_ == "int" && variable_property != "float") || (assign_record->type_ == "float" && variable_property != "int"))) {
            report_error_to_highest_symbol_table(
                    "Error: can't assign variable " + found_variable_record->name_ + " a value of type " +
                    assign_record->type_ + " it needs type " + found_variable_record->type_ + ":");
        } else {
            if ((assign_record->kind_ == "variable" || assign_record->kind_ == "function") &&
                check_if_variable_or_func_exist(assign_record)) {

                SymbolRecord *found_assign_record;
                found_assign_record = search(assign_record->name_);

                if (assign_record->nested_properties_.size() > 0)
                    found_assign_record = find_nested_record(assign_record, found_assign_record);

                if (found_assign_record != NULL) {
                    string assign_type = found_assign_record->type_;
                    if (check_if_matching_types(variable_property, assign_type))
                        report_error_to_highest_symbol_table(
                                "Error: can't assign variable " + found_variable_record->name_ + " a value of type " +
                                found_assign_record->type_ + " it needs type " + found_variable_record->type_ + ":");
                    else if (found_assign_record->structure_ == "class") {
                        SymbolRecord *class_record = search(found_variable_record->type_);
                        found_variable_record->record_size_ = class_record->record_size_;
                        copy_stored_record(assign_record, found_assign_record);
                        copy_stored_record(variable_record, found_variable_record);
                        get_code_generator()->create_copy_class_values(variable_record, assign_record);
                    } else {

                        copy_stored_record(assign_record, found_assign_record);
                        copy_stored_record(variable_record, found_variable_record);
                        get_code_generator()->load_or_call_record_into_reg(assign_record, "r1");
                        get_code_generator()->create_variable_assignment_with_register(variable_record, "r1");
                        check_correct_number_of_array_dimensions(search(assign_record->name_), assign_record);
                    }
                }
            } else if (assign_record->kind_ != "variable" && assign_record->kind_ != "function") {
                get_code_generator()->load_or_call_record_into_reg(assign_record, "r1");
                copy_stored_record(variable_record, found_variable_record);
                get_code_generator()->create_variable_assignment_with_register(variable_record, "r1");
            }
        }
    }


    return true;
}

bool SymbolTable::check_if_matching_types(string variable_property, string assign_type) {
    return variable_property != assign_type &&
           (variable_property != "int" || assign_type != "float") &&
           (variable_property != "float" || assign_type != "int");
}

bool SymbolTable::check_if_record_is_num_type(SymbolRecord *record) {
    if (record == NULL)
        return true;

    string assign_type = record->type_;
    return !(assign_type != "float" && assign_type != "int");
}

bool SymbolTable::check_correct_number_of_array_dimensions(SymbolRecord* found_record, SymbolRecord *record) {

    SymbolRecord* current_record = found_record;
    SymbolTable* top_symbol_table = found_record->symbol_table_->parent_symbol_table_;
    if (top_symbol_table->parent_symbol_table_ != NULL)
        top_symbol_table = top_symbol_table->parent_symbol_table_;
    int i = 0;
    do {

        int number_of_accessed_dimensions = 0;
        if (record->nested_array_parameters_.find(current_record->name_) != record->nested_array_parameters_.end())
            number_of_accessed_dimensions = (int) get<0>(record->nested_array_parameters_[current_record->name_]).size();

        if (current_record->structure_ != "array" && current_record->array_sizes.size() == 0 &&
            number_of_accessed_dimensions > 0) {
            report_error_to_highest_symbol_table(
                    "Error: variable " + current_record->name_ + " is not an array but is being accessed as one:");
        } else if (current_record->structure_ == "array" || current_record->array_sizes.size() > 0) {
            if (current_record->array_sizes.size() > number_of_accessed_dimensions) {
                report_error_to_highest_symbol_table(
                        "Error: array " + current_record->name_ + " is being accessed with too few dimensions:");
            } else if (current_record->array_sizes.size() < number_of_accessed_dimensions) {
                report_error_to_highest_symbol_table(
                        "Error: array " + current_record->name_ + " is being accessed with too many dimensions:");
            }
        }

        if (i >= record->nested_properties_.size())
            break;

        string property = record->nested_properties_[i];
        current_record = top_symbol_table->search(current_record->type_);
        if (current_record != NULL) // make sure class exists
            current_record = current_record->symbol_table_->search(property);

        if (current_record == NULL) {
            return NULL;
        }
        i++;

    } while (true);

    return true;
}

bool SymbolTable::check_if_return_type_is_correct_type_and_generate_code(SymbolRecord *func_record,
                                                                         ExpressionTree *expression) {
    if (!second_pass_)
        return true;
    check_expression_is_valid_and_generate_code(expression);

    if (expression->get_root_node()->record_->kind_ != "ADDOP" && expression->get_root_node()->record_->kind_ != "MULTOP" && expression->get_root_node()->record_->kind_ != "RELOP" ) {
        SymbolRecord* found_func_record = search(func_record->name_);
        SymbolRecord* return_record = expression->get_root_node()->record_;
        SymbolRecord* found_return_record = return_record;

        if (found_return_record->structure_ == "class" || found_return_record->structure_ == "class array") {
            found_return_record = search(return_record->name_);

            if (found_return_record == NULL)
                return true;

            if (return_record->nested_properties_.size() > 0) {
                found_return_record = find_nested_record(return_record, found_return_record);
            }
        }

        if (check_if_matching_types(found_return_record->type_, found_func_record->type_))
            report_error_to_highest_symbol_table("Error: function " + found_func_record->name_ + " has a return type of " + found_func_record->type_ + " but is returning type " + found_return_record->type_ + ":");

    }
    if (func_record->structure_ == "class")
        get_code_generator()->create_func_return_class_code(func_record);
        else
        get_code_generator()->create_func_return_code(func_record);
    return true;

}

bool SymbolTable::check_func_parameters(SymbolRecord *func_found_record, SymbolRecord *func_record) {
    if (!second_pass_)
        return true;
    vector<ExpressionTree *> function_expression_parameters = func_record->function_parameters_;
    if (function_expression_parameters.size() != func_found_record->get_all_function_parameters().size())
        report_error_to_highest_symbol_table("Error: " + func_found_record->name_ + " is being invoked with " + to_string(function_expression_parameters.size()) + " parameters but needs " + to_string(func_found_record->get_all_function_parameters().size()) + ":");

    for(int i = 0; i < func_found_record->get_all_function_parameters().size() && i < function_expression_parameters.size(); i++) {
        SymbolRecord* current_func_parameter = func_found_record->symbol_table_->symbol_records_[i];
        ExpressionTree* current_expression_parameter = function_expression_parameters[i];

        current_expression_parameter->get_root_node()->record_->parameter_offset_address_ = current_func_parameter->offset_address_;

        if (current_expression_parameter->get_root_node()->record_->kind_ == "ADDOP" || current_expression_parameter->get_root_node()->record_->kind_ == "MULTOP") {
            if (current_func_parameter->type_ != "float" && current_func_parameter->type_ != "int") {
                report_error_to_highest_symbol_table("Error: parameter " + current_func_parameter->name_ + " is of type " + current_func_parameter->type_  + " and an arithmetic expression is being passed that evaluates to a int or float:");
            }
            check_expression_is_valid(current_expression_parameter);
        } else {
            SymbolRecord* current_expression_found_parameter = current_expression_parameter->get_root_node()->record_;
            if (check_if_variable_or_func_exist(current_expression_found_parameter)) {

                if (current_expression_found_parameter->nested_properties_.size() > 0) {
                    current_expression_found_parameter = find_nested_record(current_expression_found_parameter,
                                                                            search(current_expression_found_parameter->name_));
                    current_expression_parameter->get_root_node()->record_->type_ = current_expression_found_parameter->type_;
                }

                if (current_expression_found_parameter->structure_ == "array" || current_func_parameter->structure_ == "array" || current_expression_found_parameter->structure_ == "class array" || current_func_parameter->structure_ == "class array") {
                    if (current_expression_found_parameter->structure_ != "array" && current_expression_found_parameter->structure_ != "class array")
                        report_error_to_highest_symbol_table("Error: parameter " + current_func_parameter->name_ + " needs an array of type " + current_func_parameter->type_ + " but type " + current_expression_found_parameter->type_ +" is being passed:");
                    else if (current_func_parameter->structure_ != "array" && current_func_parameter->structure_ != "class array")
                        report_error_to_highest_symbol_table("Error: parameter " + current_func_parameter->name_ + " is of type " + current_func_parameter->type_ + " but an array of type " + current_expression_found_parameter->type_ +" is being passed:");
                    else if (check_if_matching_types(current_expression_found_parameter->type_, current_func_parameter->type_)) {
                        report_error_to_highest_symbol_table("Error: parameter " + current_func_parameter->name_ + " needs an array of type " + current_func_parameter->type_ + " but an array of type " + current_expression_found_parameter->type_ +" is being passed:");
                    }

                }
                else if (check_if_matching_types(current_expression_found_parameter->type_, current_func_parameter->type_))
                    report_error_to_highest_symbol_table(
                            "Error: parameter " + current_func_parameter->name_ + " is of type " +
                            current_func_parameter->type_ + " but type " + current_expression_found_parameter->type_ +
                            " is being passed on function call " + func_found_record->name_ + ":");
                current_expression_found_parameter->symbol_table_->parent_symbol_table_ = this;
            }
        }
    }

    return true;
}

bool SymbolTable::create_class_entry_and_table(string kind, string type, string name) {
    SymbolRecord* record;

    if (second_pass_)
    {
        record = search(name);
        if (record->properly_declared_)
            record = new SymbolRecord();
        else
            record->properly_declared_ = true;
        current_symbol_record_ = record;
        check_for_circular_references(record);
        return true;
    }

    record = new SymbolRecord(kind, type, name);
    record->symbol_table_->symbol_record_ = record;
    record->structure_ = "class";
    current_symbol_record_ = record;
    record->properly_declared_ = false;
    insert(record);
    return true;
}

bool SymbolTable::create_program_entry_and_table() {
    SymbolRecord* record;
    if (second_pass_)
    {
        record = search("program");
        current_symbol_record_ = record;
        get_code_generator()->create_program_entry(&record);
        return true;
    }
    record = new SymbolRecord();
    record->symbol_table_->symbol_record_ = record;
    record->kind_ = "function";
    record->name_ = "program";
    current_symbol_record_ = record;
    record->properly_declared_ = true;
    insert(record);
    return true;
}

void SymbolTable::set_properly_declared(SymbolRecord* record) {

    string type = record->type_;
    int location = (int) type.find("[");
    if (location!= string::npos) {
        type = type.substr(0, location);
    }

    if (!record->properly_declared_) {
        record->properly_declared_ = search(type) != NULL;
        if (!record->properly_declared_) {
            record->symbol_table_->report_error_to_highest_symbol_table("Error: " + type + " is not a valid type:");
        }
    }
}

bool SymbolTable::create_function_entry_and_table(SymbolRecord** record) {
    if (second_pass_) {
        string name = (*record)->name_;
      //  delete record;
        (*record) = search(name);
        current_symbol_record_ = (*record);
        set_properly_declared(*record);
        code_generator_->create_func_code(*record);
        return true;
    }
    (*record)->kind_ = "function";
    current_symbol_record_ = (*record);
    (*record)->symbol_table_->symbol_record_ = (*record);
    if ((*record)->type_.substr(0,3) == "int" || (*record)->type_.substr(0, 5) == "float") {
        if (((*record)->type_.size() > 3 && ((*record) ->type_.substr(3,1) == "[" || (*record) ->type_.substr(3,1) == " "))
            || (*record)->type_.size() > 5 && ((*record)->type_.substr(5,1) == "[" || (*record)->type_.substr(5,1) == " " ) ) {
            (*record)->properly_declared_ = true;
        }
        else if ((*record)->type_.size() == 3 || (*record)->type_.size() == 5) {
            (*record)->properly_declared_ = true;
        }
        else {
            (*record)->set_structure("class");
        }
    }
    else {
        (*record)->properly_declared_ = false;
    }
    code_generator_->assign_func_address(*record);
    insert((*record));
    return true;
}

string SymbolTable::print(bool output_to_console) {
    string result = generate_symbol_table_string(this, "Global", 0);
    if (output_to_console)
        cout << result;
    return result;

}

void SymbolTable::check_for_circular_references(SymbolRecord *record, SymbolRecord *member_record, vector<string> already_checked_types) {

    if (find(already_checked_types.begin(), already_checked_types.end(), member_record->type_) != already_checked_types.end())
        return;
    SymbolRecord* member_type_record = search_top_table(member_record->type_);
    if (member_type_record == NULL)
        return;

    for (int i = 0; i < member_type_record->symbol_table_->symbol_records_.size(); i ++) {
        SymbolRecord* current_record = member_type_record->symbol_table_->symbol_records_[i];
        if (current_record->kind_ == "variable" && current_record->structure_ == "class" && current_record->type_ == record->name_) {
            already_checked_types.push_back(member_type_record->name_);
            report_error_to_highest_symbol_table("Error: circular reference in class " + record->name_ + " variable " + member_record->name_  + " is of type " + member_type_record->name_ + " which also has one or more variables or nested variables of type " + record->name_);
        } else {
            already_checked_types.push_back(member_type_record->name_);
            check_for_circular_references(record, current_record, already_checked_types);
        }
    }
}

bool SymbolTable::check_for_circular_references(SymbolRecord *record) {
    vector<string> already_checked_types;
    for (int i = 0; i < record->symbol_table_->symbol_records_.size(); i ++) {
        SymbolRecord* member_record = record->symbol_table_->symbol_records_[i];
        if (member_record->kind_ == "variable" && member_record->structure_ == "class") {
            if (member_record != NULL) {
                check_for_circular_references(record, member_record, vector<string>());
            }
        }
    }
}

bool SymbolTable::create_variable_entry(SymbolRecord** record) {
    if (second_pass_) {
        string name = (*record)->name_;
        SymbolRecord* found_record = search((*record)->name_);
        delete (*record);
        (*record) = found_record;
        set_properly_declared((*record));
        return true;
    }

    (*record)->kind_ = "variable";
    determine_record_fields(*record);
    insert(*record);

    if (symbol_record_ != NULL && symbol_record_->kind_ != "class") {
        SymbolRecord* local_record = (*record);
        determine_func_stack_variable_offsets(local_record);
    }

    return true;
}

bool SymbolTable::create_parameter_entry(SymbolRecord* record) {
    if (second_pass_) {
        string name = record->name_;
        SymbolRecord* found_record = search(record->name_);
        delete record;
        record = found_record;
        set_properly_declared(record);
        return true;
    }
    record->kind_ = "parameter";
    determine_record_fields(record);
    record->symbol_table_->parent_symbol_table_ = this;
    insert(record);
    if (get_code_generator() != NULL)
        determine_func_stack_variable_offsets(record);
    return true;
}


void SymbolTable::determine_func_stack_variable_offsets(SymbolRecord *local_record) {
    int variable_size;
    if (local_record->structure_ == "class" || local_record->structure_ == "class array") {
        if ( local_record->kind_ == "parameter")
            variable_size = 4;
        else {
            SymbolRecord *found_class_record = search(local_record->type_);
            if (found_class_record == NULL)
                return;
            variable_size = found_class_record->record_size_;

            if (local_record->structure_ == "class array")
                variable_size = local_record->compute_array_size() / 4 * variable_size;

        }
    } else {
        if (local_record->kind_  == "parameter" && (local_record->structure_ == "array" || local_record->structure_ == "class array" || local_record->structure_ == "class"))
            variable_size = 4;
        else
            variable_size = local_record->compute_record_size();
    }
    int size = local_record->symbol_table_->parent_symbol_table_->symbol_records_.size();
    SymbolRecord* parent_record = local_record->symbol_table_->parent_symbol_table_->symbol_record_;
    parent_record->record_size_ += variable_size;

    if (size > 1) {
        SymbolRecord* previous_record = local_record->symbol_table_->parent_symbol_table_->symbol_records_[size - 2];
        if (previous_record->structure_ == "class") {
            SymbolRecord * found_class_record = search(previous_record->type_);
            if (found_class_record == NULL)
                return;
            local_record->offset_address_ = previous_record->offset_address_ + found_class_record->record_size_;
        } else if (previous_record->kind_ == "parameter" && (previous_record->structure_ == "array" || previous_record->structure_ == "class array"))
            local_record->offset_address_ = previous_record->offset_address_ + 4;
         else
            local_record->offset_address_ = previous_record->offset_address_ + previous_record->compute_record_size();
    } else if (parent_record->name_ != "program")
        local_record->offset_address_ = 4;
    local_record->is_stack_variable_ = true;
}

bool SymbolTable::calculate_class_offsets() {
    if (second_pass_)
        return true;
    int offset_calculation_remain = false;
    int previous_loop_count = (int) symbol_records_.size();
    int loop_count = 0;
    while(true) {
        previous_loop_count = loop_count;
        for (SymbolRecord *record: symbol_records_) {
            if (record->kind_ == "class" && !record->size_calculated_) {
                int offset_count = 0;
                int previous_off_set_count = 0;
                int set_class_address = true;
                for (SymbolRecord* class_record: record->symbol_table_->symbol_records_) {

                    if (class_record->kind_ == "variable") {

                        if (class_record->structure_ == "class" || class_record->structure_ == "class array") {
                            SymbolRecord* found_class_record = search(class_record->type_);
                            if (found_class_record == NULL) {
                                offset_calculation_remain = false;
                                break;
                            }
                            if (!found_class_record->size_calculated_) {
                                offset_calculation_remain = true;
                                set_class_address = false;
                                break;
                            }

                            if (class_record->structure_ == "class array")
                                offset_count += found_class_record->record_size_ * class_record->compute_array_size() / 4;
                            else
                                offset_count += found_class_record->record_size_;

                        } else
                            offset_count += class_record->compute_record_size();

                        class_record->offset_address_ = previous_off_set_count;
                        previous_off_set_count = offset_count;
                    }
                }
                if (set_class_address) {
                    loop_count++;
                    record->record_size_ = offset_count;
                    record->size_calculated_ = true;
                }
            }
        }
        if (!offset_calculation_remain || previous_loop_count == loop_count)
            break;
        offset_calculation_remain = false;
    }
    return true;

}


CodeGenerator* SymbolTable::get_code_generator() {
    if (code_generator_ == NULL)
        if (parent_symbol_table_ == NULL)
            return NULL;
        else
            return parent_symbol_table_->get_code_generator();
    return code_generator_;
}

bool SymbolTable::create_function_class_entry_and_function_table(SymbolRecord **record) {
    if (second_pass_) {
        string name = (*record)->name_;
        //delete record;
        *record = search(name);
        set_properly_declared(*record);
        check_for_circular_references(*record);
        get_code_generator()->create_class_func_code((*record));
        return true;
    }
    (*record)->kind_ = "function";
    (*record)->symbol_table_->parent_symbol_table_ = this;
    (*record)->symbol_table_->symbol_record_ = (*record);
    if ((*record)->type_.substr(0,3) == "int" || (*record)->type_.substr(0, 5) == "float") {
        if (((*record)->type_.size() > 3 && (*record) ->type_.substr(3,1) == "[") || (*record)->type_.size() > 5 && (*record)->type_.substr(5,1) == "[" ) {
            (*record)->properly_declared_ = true;
        }
        else if ((*record)->type_.size() == 3 || (*record)->type_.size() == 5) {
            (*record)->properly_declared_ = true;
        }
        else {
            (*record)->set_structure("class");
        }
    }
    else {
        (*record)->properly_declared_ = false;
    }

    insert((*record));
    return true;
}


void determine_record_fields(SymbolRecord* record) {

    if (record->type_ == "int" || record->type_ == "float") {
        if (record->array_sizes.size() > 0) {
            record->properly_declared_ = true;
            record->structure_ = "array";
        }
        else {
            record->properly_declared_ = true;
            record->structure_ = "simple";
        }
    }
    else {
        record->properly_declared_ = false;
        record->set_structure("class");
        if (record->array_sizes.size() > 0) {
            record->structure_ += " array";
        }
    }
}
bool SymbolTable::insert(SymbolRecord *record) {
    if (record->symbol_table_->parent_symbol_table_ == NULL)
        record->symbol_table_->parent_symbol_table_ = this;
    if (search(record->name_) != NULL) {
        if (record->kind_ == "class")
            report_error_to_highest_symbol_table("Error: A class definition with the name " + record->name_ + " already exists:");
        else if (record->kind_ == "function")
            report_error_to_highest_symbol_table("Error: A function with the name " + record->name_ + " already exists within this scope:");
        else
            report_error_to_highest_symbol_table("Error: A variable with the name " + record->name_ + " already exists within this scope:");
        return true;
    }

    symbol_records_.push_back(record);
    return true;
}

SymbolRecord* SymbolTable::search(string name) {

    for (int i = 0; i < symbol_records_.size(); i++) {
        if (symbol_records_[i]->name_ == name)
            return symbol_records_[i];
    }

    if (parent_symbol_table_ != NULL) {
        return parent_symbol_table_->search(name);
    }
    return NULL;
}

SymbolRecord* SymbolTable::search_top_table(string name) {
    if (parent_symbol_table_ != NULL) {
        return parent_symbol_table_->search(name);
    } else {
        for (int i = 0; i < symbol_records_.size(); i++) {
            if (symbol_records_[i]->name_ == name)
                return symbol_records_[i];
        }
    }
    return NULL;
}

SymbolRecord* SymbolTable::search_type(string type) {

    for (int i = 0; i < symbol_records_.size(); i++) {
        if (symbol_records_[i]->type_ == type)
            return symbol_records_[i];
    }

    if (parent_symbol_table_ != NULL) {
        return parent_symbol_table_->search_type(type);
    }
    return NULL;
}

void SymbolTable::report_error_to_highest_symbol_table(string error_message) {

    if (parent_symbol_table_ == NULL) {
        errors_.push_back(error_message + to_string(current_token->row_location_ + 1) + ":" +
                          to_string(current_token->column_location_ + 1));
        get_code_generator()->errors_triggered_ = true;
    }
    else
        parent_symbol_table_->report_error_to_highest_symbol_table(error_message);
}

void SymbolTable::set_second_pass(bool second_pass) {
    second_pass_ = second_pass;
    for (SymbolRecord* record: symbol_records_) {
        record->symbol_table_->set_second_pass(second_pass);
    }
}

string SymbolTable::generate_symbol_table_string(SymbolTable *table, string table_name, int indent_count) {
    if (table->symbol_records_.size() == 0)
        return "";

    int name_width_out = 20;
    int type_width_out = 20;
    int kind_width_out = 20;
    int structure_width_out = 20;
    int declared_width_out = 10;

    for(SymbolRecord *record: table->symbol_records_) {
        if (record->kind_.size() > kind_width_out)
            kind_width_out = record->kind_.size();
        if (record->name_.size() > name_width_out)
            name_width_out = record->name_.size();

        if (record->structure_.size() > structure_width_out)
            structure_width_out = record->structure_.size();

        if (record->type_with_array_dimensions().size() > type_width_out)
            type_width_out = record->type_with_array_dimensions().size();
    }

    int horizontal_width_out = name_width_out + type_width_out + kind_width_out + structure_width_out + declared_width_out + 6;
    std::stringstream buffer;

    buffer << string(indent_count, ' ') << "Symbol table name: " + table_name << endl;
    buffer << string(indent_count, ' ') << string(horizontal_width_out, '-') << endl;
    buffer << string(indent_count, ' ') << "|" << setw(name_width_out) << "Name" << std::right << std::setfill(' ') << "|";
    buffer << setw(kind_width_out) << "Kind" << std::right << std::setfill(' ') << "|";
    buffer << setw(structure_width_out) << "Structure" << std::right << std::setfill(' ') << "|";
    buffer << setw(type_width_out) << "Type" << std::right << std::setfill(' ') << "|";
    buffer << setw(declared_width_out) << "Declared" << std::right << std::setfill(' ') << "|" << endl;
    buffer <<  string(indent_count, ' ') << string(horizontal_width_out, '-') << endl;

    for(SymbolRecord* record: table->symbol_records_) {

        buffer  << string(indent_count, ' ') << "|" << setw(name_width_out) << record->name_ << std::right << std::setfill(' ') << "|";
        buffer << setw(kind_width_out) << record->kind_ << std::right << std::setfill(' ') << "|";
        buffer << setw(structure_width_out) << record->structure_ << std::right << std::setfill(' ') << "|";
        buffer << setw(type_width_out) << record->type_with_array_dimensions() << std::right << std::setfill(' ') << "|";
        buffer << setw(declared_width_out) << record->properly_declared_ << std::right << std::setfill(' ') << "|" << endl;
    }

    buffer << string(indent_count, ' ') << string(horizontal_width_out, '-') << endl << endl;
    indent_count += 10;
    string return_string = buffer.str();
    for(SymbolRecord* record: table->symbol_records_) {
        return_string += generate_symbol_table_string(record->symbol_table_, record->name_, indent_count);
    }
    return return_string;
}


