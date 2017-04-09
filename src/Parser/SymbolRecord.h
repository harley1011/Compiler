#include <string>
#include <vector>
#include "../Scanner/IntegerToken.h"

using namespace std;

#ifndef COMPILER_SYMBOLRECORD_H
#define COMPILER_SYMBOLRECORD_H

class SymbolTable;
class ExpressionTree;

class SymbolRecord {
public:
    SymbolRecord(string kind, string type, string name);
    SymbolRecord(bool second_pass);
    SymbolRecord();
    string kind_;
    string type_;
  //  vector<string> function_parameters_;
    vector<string> single_operators_before_;
    map<string, int> nested_properties_dimensions_;
    vector<string> nested_properties_;
    vector<string> accessor_code_;
    vector<ExpressionTree*> function_parameters_;
    string name_;
    string structure_;
    SymbolTable* symbol_table_;
    bool properly_declared_;
    vector<int> array_sizes;
    int current_array_position_ = 0;
    string address;
    int offset_address_;
    int parameter_offset_address_ = 0;
    int record_size_;
    int data_member_offset_address_;
    bool size_calculated_;
    int integer_value_;
    float float_value_;



    bool set_kind(string kind);
    bool set_type(string type);
    bool set_name(string name);
    bool set_structure(string structure);

    bool generate_function_type();
    string generate_nested_properties_string();
    bool add_array_size(IntegerToken integer_token);
    bool add_nested_properties_dimension_to_last_porperty();

    bool add_nested_property(string property);

    bool is_stack_variable_;

    int compute_class_byte_size();

    string type_with_array_dimensions();

    bool add_function_record(SymbolRecord *pRecord, bool second_pass);

    int compute_array_size();

    int compute_record_size();

    int compute_type_size();

    vector<SymbolRecord *> get_all_function_parameters();
};


#endif //COMPILER_SYMBOLRECORD_H

