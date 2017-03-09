#include <string>

using namespace std;

#ifndef COMPILER_SYMBOLRECORD_H
#define COMPILER_SYMBOLRECORD_H

enum SymbolRecordType {
    parameter,
    variable,
    classUse,
    function

};
class SymbolTable;

class SymbolRecord {
public:
    SymbolRecord(string kind, string type, string name);
    SymbolRecord();
    string kind_;
    string type_;
    string name_;
    SymbolTable* symbol_table_;

    bool set_kind(string kind);
    bool set_type(string type);
    bool set_name(string name);

    bool append_to_type(string type);
};


#endif //COMPILER_SYMBOLRECORD_H

