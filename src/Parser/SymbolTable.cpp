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
}

bool SymbolTable::check_if_assign_variable_exist(SymbolRecord *record) {
    if (second_pass_) {
        SymbolRecord* found_record = search(record->name_);
        if (found_record == NULL) {
            report_error_to_highest_symbol_table("Error: " + record->name_ + " variable is being used without being declared:");
        } else if (record->nested_properties_.size() > 0 ) {
            check_nested_property(record, found_record);
        } else {
            check_correct_number_of_array_dimensions(found_record, record, record->nested_properties_dimensions_[found_record->name_]);
        }
    }
    return true;
}

SymbolRecord * SymbolTable::check_nested_property(SymbolRecord *record, SymbolRecord *found_record) {
    if (found_record->type_ == "int")
        report_error_to_highest_symbol_table("Error: " + record->name_ + " is of type int and doesn't have any properties to access:");
    else if (found_record->type_ == "float")
        report_error_to_highest_symbol_table("Error: " + record->name_ + " is of type float and doesn't have any properties to access:");
    else {
        SymbolRecord* current_record = found_record;
        SymbolRecord* current_found_record;
        SymbolTable* top_symbol_table = found_record->symbol_table_->parent_symbol_table_;
        if (top_symbol_table->parent_symbol_table_ != NULL)
            top_symbol_table = top_symbol_table->parent_symbol_table_;
        for (int i = 0; i < record->nested_properties_.size(); i++) {
            string property = record->nested_properties_[i];
            current_found_record = top_symbol_table->search(current_record->type_);
            current_record = current_found_record->symbol_table_->search(property);

            if (current_record == NULL) {
                report_error_to_highest_symbol_table("Error: invalid nested property " + property + " on variable " + record->name_ + ":");
                return current_record;
            } else {
                check_correct_number_of_array_dimensions(current_record, current_record, record->nested_properties_dimensions_[current_record->name_]);
            }


        }
        return current_record;
    }
    return NULL;
}

bool SymbolTable::check_expression_is_valid(ExpressionTree *tree) {
    if (!second_pass_)
        return true;

    if (tree->root_node_->record_->kind_ == "ADDOP" || tree->root_node_->record_->kind_ == "MULTOP") {
        vector<SymbolRecord*> identifiers = tree->all_identifiers_in_expression();
        if (identifiers.size() > 0) {
            for (SymbolRecord* current_record: identifiers) {
                SymbolRecord* current_found_record = search(current_record->name_);
                if (current_found_record != NULL)
                    current_found_record = find_nested_record(current_record, current_found_record);

                if ( current_found_record != NULL && !check_if_record_is_num_type(current_found_record) ) {
                    if (current_record->nested_properties_.size() == 0)
                        report_error_to_highest_symbol_table("Error: can't perform arithmetic operations with " + current_record->kind_ + " " + current_record->name_ + " that is not of type int or float:" );
                    else {
                        report_error_to_highest_symbol_table("Error: can't perform arithmetic operations with " + current_record->kind_ + " " + current_record->generate_nested_properties_string() + " that is not of type int or float:" );
                    }
                }
            }
        }
    }
    return true;
}

bool SymbolTable::check_expression_tree_for_correct_type(SymbolRecord *variable_record, ExpressionTree *tree) {
    if (!second_pass_) {
        return true;
    }
    string variable_property;
    SymbolRecord* record = search(variable_record->name_);
    if (record == NULL) {
        report_error_to_highest_symbol_table("Error: " + variable_record->name_ + " variable is being used without being declared:");
    } else {
        variable_property = record->type_;
        if (variable_record->nested_properties_.size() > 0) {
            SymbolRecord* nested_variable = check_nested_property(variable_record, record);
            if (nested_variable == NULL)
                return true;
            else
                variable_property = nested_variable->type_;
        } else {
            check_correct_number_of_array_dimensions(record, variable_record, variable_record->nested_properties_dimensions_[variable_record->name_]);
        }
    }

    if (tree->root_node_->record_->kind_ == "ADDOP" || tree->root_node_->record_->kind_ == "MULTOP") {

        if (record->type_ != "float" && record->type_ != "int") {
            report_error_to_highest_symbol_table("Error: can't assign variable " + record->name_ + " of type " + record->type_  + " an arithmetic expression, it must be of type int or float:");
        }
        check_expression_is_valid(tree);
    }
    else if (record != NULL) {
        SymbolRecord* assign_record = tree->root_node_->record_;

        if (variable_property != assign_record->type_ && (assign_record->type_ == "int" || assign_record->type_ == "float")) {
            report_error_to_highest_symbol_table("Error: can't assign variable " + record->name_ + " a value of type " + assign_record->type_ + " it needs type " + record->type_ + ":");
        } else {
            SymbolRecord *found_assign_record;
            found_assign_record = search(assign_record->name_);

            if (assign_record->nested_properties_.size() > 0)
                found_assign_record = find_nested_record(assign_record, found_assign_record);

            if (found_assign_record != NULL) {
                string assign_type = found_assign_record->type_;
                if (variable_property != found_assign_record->type_ &&
                    (variable_property != "int" || assign_type != "float") &&
                    (variable_property != "float" || assign_type != "int"))
                    report_error_to_highest_symbol_table(
                            "Error: can't assign variable " + record->name_ + " a value of type " +
                            found_assign_record->type_ + " it needs type " + record->type_ + ":");
            }
        }
    }

    return true;
}

bool SymbolTable::check_if_record_is_num_type(SymbolRecord *record) {
    if (record == NULL)
        return true;

    string assign_type = record->type_;
    if ( assign_type != "float" && assign_type != "int")
        return false;
    return true;
}


bool SymbolTable::check_correct_number_of_array_dimensions(SymbolRecord* found_record, SymbolRecord *record, int number_of_accessed_dimensions) {
    if (found_record->structure_ != "array" && found_record->array_sizes.size() == 0  && number_of_accessed_dimensions > 0) {
        report_error_to_highest_symbol_table("Error: variable " + record->name_ + " is not an array but is being accessed as one:");
    } else if (found_record->structure_ == "array" || found_record->array_sizes.size() > 0) {
        if (found_record->array_sizes.size() > number_of_accessed_dimensions) {
            report_error_to_highest_symbol_table("Error: array " + record->name_ + " is being accessed with too few dimensions:");
        } else if (found_record->array_sizes.size() < number_of_accessed_dimensions) {
            report_error_to_highest_symbol_table("Error: array " + record->name_ + " is being accessed with too many dimensions:");
        }

    }
    return true;
}

SymbolRecord* find_nested_record(SymbolRecord* record, SymbolRecord* found_record) {
    SymbolRecord* current_record = found_record;
    SymbolTable* top_symbol_table = found_record->symbol_table_->parent_symbol_table_;
    if (top_symbol_table->parent_symbol_table_ != NULL)
        top_symbol_table = top_symbol_table->parent_symbol_table_;
    for (int i = 0; i < record->nested_properties_.size(); i++) {
        string property = record->nested_properties_[i];
        current_record = top_symbol_table->search(current_record->type_);
        current_record = current_record->symbol_table_->search(property);

        if (current_record == NULL) {
            return NULL;
        }

    }
    return  current_record;
}

bool SymbolTable::check_if_func_exists(SymbolRecord *func_record) {
    if (!second_pass_)
        return true;
    SymbolRecord* local_record = search(func_record->name_);

    if (func_record->nested_properties_.size() > 0 ) {
        if (local_record == NULL) {
            report_error_to_highest_symbol_table("Error: " + func_record->name_ + " variable is being used without being declared:");
            return false;
        }
        if (func_record->type_ == "int" || func_record->type_ == "float") {
            report_error_to_highest_symbol_table(
                    "Error: " + func_record->name_ + " is of type " + func_record->type_ + " and doesn't have any properties to access:");
            return false;
        }
        else {
            SymbolRecord* current_record = local_record;
            SymbolTable* top_symbol_table = local_record->symbol_table_->parent_symbol_table_;
            if (top_symbol_table->parent_symbol_table_ != NULL)
                top_symbol_table = top_symbol_table->parent_symbol_table_;
            for (int i = 0; i < func_record->nested_properties_.size(); i++) {
                string property = func_record->nested_properties_[i];
                current_record = top_symbol_table->search(current_record->type_);
                current_record = current_record->symbol_table_->search(property);

                if (current_record == NULL) {
                    report_error_to_highest_symbol_table("Error: invalid nested function " + property + " on variable " + func_record->name_ + ":");
                    return false;
                }

            }
        }
    }
    else if (local_record == NULL) {
        report_error_to_highest_symbol_table(
                "Error: " + func_record->name_ + " function is being used without being declared:");
        return false;
    }
    return true;
}

bool SymbolTable::check_if_func_exists_and_parameters_are_valid(SymbolRecord *func_record, vector<ExpressionTree *> *function_expression_parameters) {
    if (!second_pass_)
        return true;
    if (check_if_func_exists(func_record)) {
        SymbolRecord* local_record = search(func_record->name_);
        if (local_record == NULL)
            local_record = find_nested_record(func_record, local_record);
        if (function_expression_parameters->size() != local_record->function_parameters_.size())
        report_error_to_highest_symbol_table("Error: " + func_record->name_ + " is being invoked with " + to_string(function_expression_parameters->size()) + " parameters but needs " + to_string(local_record->function_parameters_.size()) + ":");
    }




//            for (int i = 0; i < local_record->function_parameters_.size(); i++) {
//                if (i == function_expression_parameters->size()) {
//                    report_error_to_highest_symbol_table("Error: " + func_record->name_ + " is being invoked with only " + to_string(i) + " parameters but needs " + to_string(local_record->function_parameters_.size()) + ":");
//                    break;
//                }
//                string actual_type = local_record->function_parameters_[i];
//                string received_type = "";
//                if (func_record->function_parameters_record_[i]->kind_ == "variable") {
//                    SymbolRecord* found_record = search(func_record->function_parameters_record_[i]->name_);
//                    if (found_record != NULL) {
//                        received_type = found_record->type_;
//                    }
//                    else {
//                        received_type = actual_type;
//                    }
//                } else
//                    received_type = func_record->function_parameters_record_[i]->type_;
//
//                if (actual_type != received_type && (actual_type != "int" || received_type != "float") && (actual_type != "float" || received_type != "int")) {
//                    report_error_to_highest_symbol_table("Error: " + func_record->name_ + " " + to_string(i + 1) + " parameter is of type " + actual_type + " but " + received_type + " is being passed:");
//                }
//
//            }

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
        return true;
    }
    record = new SymbolRecord();
    record->kind_ = "function";
    record->name_ = "program";
    current_symbol_record_ = record;
    record->properly_declared_ = true;
    insert(record);
    return true;
}

void SymbolTable::set_properly_declared(SymbolRecord* record) {

    string type = record->type_;
    int location = type.find("[");
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
        return true;
    }
    (*record)->kind_ = "function";
    current_symbol_record_ = (*record);
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
    return true;
}

bool SymbolTable::create_function_class_entry_and_function_table(SymbolRecord **record) {
    if (second_pass_) {
        string name = (*record)->name_;
        //delete record;
        *record = search(name);
        set_properly_declared(*record);
        check_for_circular_references(*record);
        return true;
    }
    (*record)->kind_ = "function";
    (*record)->symbol_table_->parent_symbol_table_ = this;
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

    insert(record);
    return true;
}

void determine_record_fields(SymbolRecord* record) {

    if (record->type_ == "int" || record->type_ == "float") {
        if (record->array_sizes.size() > 0) {
            record->properly_declared_ = true;
            record->set_structure("array");
        }
        else {
            record->properly_declared_ = true;
            record->set_structure("simple");
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

    if (parent_symbol_table_ == NULL)
        errors_.push_back(error_message + to_string(current_token->row_location_ + 1) + ":" + to_string(current_token->column_location_ + 1));
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

        if (record->type_.size() > type_width_out)
            type_width_out = record->type_.size();
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
