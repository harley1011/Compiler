#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "State.h"
#include "Token.h"
#include <fstream>
#include "Scanner.h"
#include <algorithm>

using namespace std;

// a means a-z or A-Z, or 0-9
// d means 0-9
// t means anything that's not specified
// n means 1-9

const string Scanner::reserved_words[] = {"and", "not", "or", "if", "then", "else", "for", "class", "int", "float", "get", "put", "return", "program"};



Scanner::Scanner() {
    use_backup_ = false;
    initial_state = State();
    initial_state.next_states_[' '] = 0;
    initial_state.next_states_['\n'] = 0;
    initial_state.next_states_['l'] = 1;
    initial_state.next_states_['0'] = 3;
    initial_state.next_states_['n'] = 5;
    initial_state.next_states_['/'] = 18;
    initial_state.next_states_['='] = 24;
    initial_state.next_states_['<'] = 27;
    initial_state.next_states_['>'] = 32;
    initial_state.next_states_['+'] = 35;
    initial_state.next_states_['-'] = 36;
    initial_state.next_states_['*'] = 37;
    initial_state.next_states_[';'] = 38;
    initial_state.next_states_[','] = 39;
    initial_state.next_states_['.'] = 40;

    //State 0
    table[0] = initial_state;

    // State 1
    table[1] = State();
    table[1].next_states_['a'] = 1;
    table[1].next_states_['_'] = 1;
    table[1].next_states_['t'] = 2;

    // State 2 ID TOKEN
    table[2] = State("ID", true, true);

    //State 3
    table[3] = State();
    table[3].next_states_['.'] = 6;
    table[3].next_states_['t'] = 4;

    //State 4 INUM TOKEN
    table[4] = State("INUM", true, true);

    //State 5
    table[5] = State();
    table[5].next_states_['d'] = 5;
    table[5].next_states_['.'] = 6;
    table[5].next_states_['t'] = 4;

    //State 6
    table[6] = State();
    table[6].next_states_['n'] = 7;
    table[6].next_states_['0'] = 8;

    //State 7
    table[7] = State();
    table[7].next_states_['n'] = 7;
    table[7].next_states_['0'] = 9;
    table[7].next_states_['t'] = 10;

    // State 8
    table[8] = State();
    table[8].next_states_['n'] = 7;
    table[8].next_states_['0'] = 9;
    table[8].next_states_['t'] = 10;

    // State 9
    table[9] = State();
    table[9].next_states_['0'] = 9;
    table[9].next_states_['n'] = 7;

    // State 10 FNUM TOKEN
    table[10] = State("FNUM", true, true);

    // States 11-16 brackets

    brackets[0] = make_pair('(', "OPENPARA");
    brackets[1] = make_pair('[', "OPENBRA");
    brackets[2] = make_pair(')', "CLOSEPARA");
    brackets[3] = make_pair(']', "CLOSEBRA");
    brackets[4] = make_pair('{', "OPENCURL");
    brackets[5] = make_pair('}', "CLOSECURL");

    for (int i = 0; i <  6; i++) {
        pair<char, string> c = brackets[i];
        initial_state.next_states_[c.first] = i + 11;
        table[i + 11] = State(c.second, false, true);
    }

    //State 17 ERROR STATE
    table[17] = State("ERROR", false, true);

    //State 18 Comments
    table[18] = State();
    table[18].next_states_['*'] = 19;
    table[18].next_states_['/'] = 21;
    table[18].next_states_['t'] = 23;

    //State 19
    table[19] = State();
    table[19].next_states_['*'] = 20;
    table[19].next_states_['t'] = 19;

    //State 20
    table[20] = State();
    table[20].next_states_['/'] = 22;
    table[20].next_states_['t'] = 19;
    table[20].next_states_['\000'] = 17;

    //State 21
    table[21] = State();
    table[21].next_states_['t'] = 21;
    table[21].next_states_['\n'] = 22;

    //State 22
    table[22] = State("CMT", false, true);

    //State 23 DASH
    table[23] = State("DASH", true, true);

    //State 24
    table[24] = State();
    table[24].next_states_['t'] = 25;
    table[24].next_states_['='] = 26;

    //State 27
    table[27] = State();
    table[27].next_states_['t'] = 28;
    table[27].next_states_['>'] = 29;
    table[27].next_states_['='] = 31;

    //State 28
    table[28] = State("LT", true, true);

    //State 29
    table[29] = State();
    table[29].next_states_['t'] = 30;

    //State 30
    table[30] = State("NOTEQ", false, true);

    //State 31
    table[31] = State("LTEQ", false, true);

    //State 32
    table[32] = State();
    table[32].next_states_['t'] = 33;
    table[32].next_states_['='] = 34;

    //State 33
    table[33] = State("GT", true, true);

    //State 34
    table[34] = State("GTEQ", false, true);

    //State 35
    table[35] = State("ADD", false, true);

    //State 36
    table[36] = State("SUB", false, true);

    //State 37
    table[37] = State("MULT", false, true);




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
        current_state = table[next_state];

        if (current_state.is_final_state_) {
            if (current_state.backup_) {
                use_backup_ = true;
                program_count_--;
            }
            else {
                lexeme += lookup;
            }

            if(check_if_reserved_word(lexeme))
            {
                string copy = lexeme;
                transform(copy.begin(), copy.end(),copy.begin(), ::tolower);
                return token(lexeme, lexeme, program_count_ - lexeme.size());
            }

            return token(current_state.token_, lexeme, program_count_ - lexeme.size());
        }

        lexeme += lookup;

        if (lookup == '\000')
            return token("END", lexeme, program_count_);



    }
    return token(current_state.token_, lexeme, program_count_);

}


bool Scanner::check_if_reserved_word(string word) {
    for (int i = 0; i < reserved_words->size(); i++){
        string res = reserved_words[i];
        transform(res.begin(), res.end(),res.begin(), ::tolower);
        if (res == word)
            return true;
    }
    return false;
}

