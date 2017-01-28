#include <string>
#include "Token.h"
#include <list>
#include <iostream>
#include <fstream>
#include <vector>
#include "ErrorToken.h"

using namespace std;

#ifndef UNTITLED_SCANNER_H_H
#define UNTITLED_SCANNER_H_H

#endif //UNTITLED_SCANNER_H_H


class Scanner {
public:
    Scanner(string token_output_path, string error_output_path);

    ifstream program_file_;
    ofstream token_outpath_file_;
    ofstream error_output_path_;

    string program_string_;
    int program_count_;
    int current_row_count_;
    int current_column_count_;
    int previous_column_count_;

    bool is_file_;
    State initial_state;
    State table[45];
    bool use_backup_;
    char backup_buffer_;
    vector<string> reserved_words;
    static const int brackets_size = 6;
    pair<char, string> brackets[brackets_size];
    vector<ErrorToken> error_tokens_;

    Token* next_token();
    char next_char();
    vector<Token*> generate_tokens(string path, bool is_file);

    Scanner();

private:
    bool check_if_reserved_word(string word);
    void check_if_newline();
    void check_if_backup_row();
};

