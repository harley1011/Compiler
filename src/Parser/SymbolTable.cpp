//
// Created by Harley on 3/9/2017.
//
#include <iostream>
#include <iomanip>
#include <sstream>
#include "SymbolTable.h"

SymbolRecord* find_nested_record(SymbolRecord* record);
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
            if (record->type_ == "int")
                report_error_to_highest_symbol_table("Error: " + record->name_ + " is of type int and doesn't have any properties to access:");
            else if (record->type_ == "float")
                report_error_to_highest_symbol_table("Error: " + record->name_ + " is of type float and doesn't have any properties to access:");
            else {
                SymbolRecord* current_record = found_record;
                SymbolTable* top_symbol_table = found_record->symbol_table_->parent_symbol_table_;
                if (top_symbol_table->parent_symbol_table_ != NULL)
                    top_symbol_table = top_symbol_table->parent_symbol_table_;
                for (int i = 0; i < record->nested_properties_.size(); i++) {
                    string property = record->nested_properties_[i];
                    current_record = top_symbol_table->search(current_record->type_);
                    SymbolRecord* found_var_record = current_record->symbol_table_->search(property);

                    if (found_var_record == NULL) {
                        report_error_to_highest_symbol_table("Error: invalid nested property " + property + " on variable " + record->name_ + ":");
                        return true;
                    }


                }
            }
        }
    }
    return true;
}

bool SymbolTable::check_if_assign_variable_exist_and_correct_assign_type(SymbolRecord* variable_record, SymbolRecord* assign_record) {
    if (!second_pass_) {
        return true;
    }
    SymbolRecord* record = search(variable_record->name_);
    if (record == NULL) {
        report_error_to_highest_symbol_table("Error: " + variable_record->name_ + " variable is being used without being declared:");
    } else {
        if (assign_record->kind_ == "id" || assign_record->kind_ == "function") {
            SymbolRecord* found_assign_record;
            found_assign_record = search(assign_record->name_);

            if (assign_record->nested_properties_.size() > 0)
                found_assign_record = find_nested_record(found_assign_record);


            if (found_assign_record != NULL) {
                string assign_type = found_assign_record->type_;
                if (record->type_ != found_assign_record->type_ &&( record->structure_ == "class" || found_assign_record->structure_ == "class" || record->type_ != "float" && record->type_ != "int" && found_assign_record->type_ != "float" && found_assign_record->type_ != "int" ))
                    report_error_to_highest_symbol_table("Error: can't assign variable " + record->name_ + " a value of type " + found_assign_record->type_ + " it needs type " + record->type_ + ":");
            }

        }
    }
    return true;
}

SymbolRecord* find_nested_record(SymbolRecord* record) {
    SymbolRecord* current_record = record;
    SymbolTable* top_symbol_table = record->symbol_table_->parent_symbol_table_;
    if (top_symbol_table->parent_symbol_table_ != NULL)
        top_symbol_table = top_symbol_table->parent_symbol_table_;
    for (int i = 0; i < record->nested_properties_.size(); i++) {
        string property = record->nested_properties_[i];
        current_record = top_symbol_table->search(current_record->type_);
        SymbolRecord* found_var_record = current_record->symbol_table_->search(property);

        if (found_var_record == NULL) {
            return NULL;
        }

    }
}

bool SymbolTable::check_if_func_exists(SymbolRecord* func_record) {
    if (second_pass_) {
        SymbolRecord* local_record = search(func_record->name_);
        if (local_record == NULL) {
            report_error_to_highest_symbol_table("Error: " + func_record->name_ + " function is being used without being declared:");
        } else if (func_record->nested_properties_.size() > 0 ) {
            if (func_record->type_ == "int")
                report_error_to_highest_symbol_table("Error: " + func_record->name_ + " is of type int and doesn't have any properties to access:");
            else if (func_record->type_ == "float")
                report_error_to_highest_symbol_table("Error: " + func_record->name_ + " is of type float and doesn't have any properties to access:");
            else {
                SymbolRecord* current_record = local_record;
                SymbolTable* top_symbol_table = local_record->symbol_table_->parent_symbol_table_;
                if (top_symbol_table->parent_symbol_table_ != NULL)
                    top_symbol_table = top_symbol_table->parent_symbol_table_;
                for (int i = 0; i < func_record->nested_properties_.size(); i++) {
                    string property = func_record->nested_properties_[i];
                    current_record = top_symbol_table->search(current_record->type_);
                    SymbolRecord* found_var_record = current_record->symbol_table_->search(property);

                    if (found_var_record == NULL) {
                        report_error_to_highest_symbol_table("Error: invalid nested function " + property + " on variable " + func_record->name_ + ":");
                        return true;
                    }

                }
            }
        }
        else {
            for (int i = 0; i < local_record->function_parameters.size(); i++) {
                if (i == func_record->function_parameters_record_.size()) {
                    report_error_to_highest_symbol_table("Error: " + func_record->name_ + " is being invoked with only " + to_string(i) + " parameters but needs " + to_string(local_record->function_parameters.size()) + ":");
                    break;
                }
                string actual_type = local_record->function_parameters[i];
                string received_type = "";
                if (func_record->function_parameters_record_[i]->kind_ == "id") {
                    SymbolRecord* found_record = search(func_record->function_parameters_record_[i]->name_);
                    if (found_record != NULL) {
                        received_type = found_record->type_;
                    }
                    else {
                        received_type = actual_type;
                    }
                } else
                    received_type = func_record->function_parameters_record_[i]->type_;

                if (actual_type != received_type && actual_type != "int" && actual_type != "float" && received_type != "int" && received_type != "float") {
                    report_error_to_highest_symbol_table("Error: " + func_record->name_ + " " + to_string(i) + " parameter is of type " + actual_type + " but " + received_type + " is being passed:");
                }

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

        return true;
    }

    record = new SymbolRecord();
    record->name_ = name;
    record->kind_ = kind;
    record->type_ = type;
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
            record->symbol_table_->report_error_to_highest_symbol_table("Error:" + type + " is not a valid type:");
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
    if ((*record)->type_.substr(0,3) == "int" || (*record)->type_.substr(0, 5) == "float") {
        if (((*record)->type_.size() > 3 && ((*record) ->type_.substr(3,1) == "[" || (*record) ->type_.substr(3,1) == " "))
            || (*record)->type_.size() > 5 && ((*record)->type_.substr(5,1) == "[" || (*record)->type_.substr(5,1) == " " )) {
            (*record)->properly_declared_ = true;
            (*record)->set_structure("array");
        }
        else if ((*record)->type_.size() == 3 || (*record)->type_.size() == 5) {
            (*record)->properly_declared_ = true;
            (*record)->set_structure("simple");
        }
        else {
            (*record)->set_structure("class");

            if ((*record)->type_.find("[") != string::npos) {
                (*record)->structure_ += " array";
            }
        }
    }
    else {
        (*record)->properly_declared_ = false;
        (*record)->set_structure("class");

        if ((*record)->type_.find("[") != string::npos) {
            (*record)->structure_ += " array";
        }
    }
    insert(*record);
    return true;
}

bool SymbolTable::create_function_class_entry_and_function_table(SymbolRecord **record) {
    if (second_pass_) {
        string name = (*record)->name_;
        //delete record;
        *record = search(name);
        set_properly_declared(*record);
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
    if (record->type_.substr(0,3) == "int" || record->type_.substr(0, 5) == "float") {
        if ((record->type_.size() > 3 && (record ->type_.substr(3,1) == "[" || record ->type_.substr(3,1) == " "))
            || record->type_.size() > 5 && (record->type_.substr(5,1) == "[" || record->type_.substr(5,1) == " " )) {
            record->properly_declared_ = true;
            record->set_structure("array");
        }
        else if (record->type_.size() == 3 || record->type_.size() == 5) {
            record->properly_declared_ = true;
            record->set_structure("simple");
        }
        else {
            record->set_structure("class");

            if (record->type_.find("[") != string::npos) {
                record->structure_ += " array";
            }
        }
    }
    else {
        record->properly_declared_ = false;
        record->set_structure("class");

        if (record->type_.find("[") != string::npos) {
            record->structure_ += " array";
        }
    }
    insert(record);
    return true;
}

bool SymbolTable::insert(SymbolRecord *record) {
    if (record->symbol_table_->parent_symbol_table_ == NULL)
        record->symbol_table_->parent_symbol_table_ = this;
    if (search(record->name_) != NULL) {
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
        buffer << setw(type_width_out) << record->type_ << std::right << std::setfill(' ') << "|";
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

bool SymbolTable::check_if_assign_variable_exist() {
    return false;
}
