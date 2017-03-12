#include <string>
#include <vector>
#include "../IntegerToken.h"

using namespace std;

#ifndef COMPILER_SYMBOLRECORD_H
#define COMPILER_SYMBOLRECORD_H

class SymbolTable;

class SymbolRecord {
public:
    SymbolRecord(string kind, string type, string name);
    SymbolRecord();
    string kind_;
    string type_;
    string name_;
    string structure_;
    SymbolTable* symbol_table_;
    bool properly_declared_;
    vector<int> array_sizes;
    string address;

    bool set_kind(string kind);
    bool set_type(string type);
    bool set_name(string name);
    bool set_structure(string structure);
    bool append_to_type(string type);

    bool generate_function_type();

    bool add_array_size(IntegerToken integer_token);
};


#endif //COMPILER_SYMBOLRECORD_H

