#include <string>
#include "Token.h"
#include <list>
#include <iostream>
#include <fstream>
using namespace std;

#ifndef UNTITLED_SCANNER_H_H
#define UNTITLED_SCANNER_H_H

#endif //UNTITLED_SCANNER_H_H


class Scanner {
public:
    ifstream fs_;
    string program_string_;
    int program_count_;
    bool is_file_;
    State initial_state;
    State table[45];
    bool use_backup_;
    char backup_buffer_;


    char scan();
    token next_token();
    char next_char();
    vector<token> generate_tokens(string path, bool is_file);
    Scanner();

};