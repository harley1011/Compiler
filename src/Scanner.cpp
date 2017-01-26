#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "State.h"
#include "Token.h"
#include <fstream>
#include "Scanner.h"

using namespace std;

// a means a-z or A-Z, or 0-9
// d means 0-9
// t means anything that's not specified

Scanner::Scanner() {
    use_backup_ = false;
    initial_state = State();
    initial_state.next_states_['l'] = 2;

    table[0] = initial_state;

    // State 2
    table[1] = State();
    table[1].next_states_['a'] = 2;
    table[1].next_states_['_'] = 2;
    table[1].next_states_['t'] = 3;


    table[2] = State("ID", true, true);

}

vector<token> Scanner::generate_tokens(string path, bool is_file) {
    is_file_ = is_file;
    if (is_file)
        fs_.open(path.c_str());
    else {
        program_count_ = 0;
        program_string_ = path;
    }

    vector<token> tokens;

    while(true) {
        token result = next_token();
        if (result.token_identifier_ == "END")
            break;
        tokens.push_back(result);
    }
    return tokens;
}

char Scanner::next_char() {
    if (use_backup_){
        use_backup_ = false;
        return backup_buffer_;

    }

    if (is_file_) {
        char * buffer = new char [1];
        fs_.read(buffer,1);
        program_count_++;
        backup_buffer_ = buffer[0];
        return buffer[0];
    }
    else {
        backup_buffer_ = program_string_[program_count_];
        return program_string_[program_count_++];
    }

}

token Scanner::next_token() {
    State current_state = initial_state;
    string lexeme = "";
    while(current_state.token_.length() == 0) {
        char lookup = next_char();

        int next_state = current_state.get_next_state(lookup);
        current_state = table[next_state - 1];

        if (current_state.is_final_state_) {
            if (current_state.backup_) {
                use_backup_ = true;
            }
            else {
                lexeme += lookup;
            }
            return token(current_state.token_, lexeme, program_count_ - lexeme.size() - 1);
        }

        lexeme += lookup;

        if (lookup == '\000')
            return token("END", lexeme, program_count_);



    }
    return token(current_state.token_, lexeme, program_count_);

}
