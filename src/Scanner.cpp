#include <iostream>
#include <vector>
#include <map>
#include "State.h"
#include "Token.h"
#include <fstream>
#include "Scanner.h"
#include "IntegerToken.h"
#include "FloatToken.h"
#include <algorithm>

using namespace std;

Scanner::Scanner(string token_output_path, string error_output_path) {
    init();
    output_to_file_ = true;
    token_output_path_ = token_output_path;
    error_output_path_ = error_output_path;
}
Scanner::Scanner() {
    init();
}

void Scanner::init() {
    string temp[]   = {"and", "not", "or", "if", "then", "else", "for", "class", "int", "float", "get", "put", "return", "program"};
    token_output_path_ = "token_output.txt";
    error_output_path_ = "error_output.txt";
    vector<string> vec (temp, temp + sizeof(temp) / sizeof(temp[0]));
    reserved_words = vec;
    use_backup_ = false;
    current_row_count_ = 0;
    current_column_count_ = 0;
    initial_state = State();
    initial_state.next_states_[' '] = 0;
    initial_state.next_states_['\n'] = 0;
    initial_state.next_states_['\t'] = 0;
    initial_state.letter_state_ = 1;
    initial_state.non_zero_state_ = 5;
    initial_state.next_states_['0'] = 3;
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
    initial_state.next_states_['\000'] = 41;
    initial_state.error_message_ = "Invalid initial character";

    // State 1
    table[1] = State();
    table[1].alphanum_state_ = 1;
    table[1].any_match_state_ = 2;

    // State 2 ID TOKEN
    table[2] = State("ID", true, true);

    //State 3
    table[3] = State();
    table[3].next_states_['.'] = 6;
    table[3].any_match_state_ = 4;

    //State 4 INUM TOKEN
    table[4] = State("INUM", true, true);

    //State 5
    table[5] = State();
    table[5].digit_state_ = 5;
    table[5].next_states_['.'] = 6;
    table[5].any_match_state_ = 4;

    //State 6
    table[6] = State();
    table[6].non_zero_state_ = 7;
    table[6].next_states_['0'] = 8;
    table[6].error_message_ = "Invalid character proceeding numerical decimal";

    //State 7
    table[7] = State();
    table[7].non_zero_state_ = 7;
    table[7].next_states_['0'] = 9;
    table[7].any_match_state_ = 10;

    // State 8
    table[8] = State();
    table[8].non_zero_state_ = 7;
    table[8].next_states_['0'] = 9;
    table[8].any_match_state_ = 10;

    // State 9
    table[9] = State();
    table[9].next_states_['0'] = 9;
    table[9].non_zero_state_ = 7;
    table[9].error_message_ = "Invalid float, it can not end with a zero unless it's after the fraction and then only digit after the fraction";

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
    table[18].any_match_state_ = 23;

    //State 19
    table[19] = State();
    table[19].next_states_['*'] = 20;
    table[19].any_match_state_ = 19;
    table[19].next_states_['\000'] = -1;
    table[19].error_message_ = "Multi line comment doesn't have */ to close comment";

    //State 20
    table[20] = State();
    table[20].next_states_['/'] = 22;
    table[20].any_match_state_ = 19;
    table[20].next_states_['\000'] = -1;
    table[20].error_message_ = "Multi line comment doesn't have */ to close comment";

    //State 21
    table[21] = State();
    table[21].any_match_state_ = 21;
    table[21].next_states_['\n'] = 22;

    //State 22
    table[22] = State("CMT", false, true);

    //State 23 DASH
    table[23] = State("DASH", true, true);

    //State 24
    table[24] = State();
    table[24].any_match_state_ = 25;
    table[24].next_states_['='] = 26;

    //State 25
    table[25] = State("EQUAL", true, true);

    //State 26
    table[26] = State("EQUIV", false, true);


    //State 27
    table[27] = State();
    table[27].any_match_state_ = 28;
    table[27].next_states_['>'] = 29;
    table[27].next_states_['='] = 31;

    //State 28
    table[28] = State("LT", true, true);

    //State 29
    table[29] = State();
    table[29].any_match_state_ = 30;

    //State 30
    table[30] = State("NOTEQ", true, true);

    //State 31
    table[31] = State("LTEQ", false, true);

    //State 32
    table[32] = State();
    table[32].any_match_state_ = 33;
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

    //State 38
    table[38] = State("DELI", false, true);

    //State 39
    table[39] = State("COM", false, true);

    //State 40
    table[40] = State("DOT", false, true);

    table[41] = State("END", false, true);

    table[0] = initial_state;
}

vector<Token*> Scanner::generate_tokens(string path, bool is_file) {

    is_file_ = is_file;
    if (is_file)
        program_file_.open(path.c_str());
    else {
        program_count_ = 0;
        program_string_ = path;
    }

    vector<Token*> tokens;
    ofstream error_output_file;
    ofstream token_output_file;
    if (output_to_file_) {
        error_output_file.open(error_output_path_);
        token_output_file.open(token_output_path_);
    }
    while(true) {
        try {
            Token *result = next_token();

            if (result->token_identifier_ == "ERROR") {
                ErrorToken error_token = static_cast<ErrorToken &>(*result);
                error_tokens_.push_back(error_token);

                if (output_to_file_) {
                    error_output_file << error_token.generate_error_message() + "\n";
                }
            }

            if (result->token_identifier_ == "END")
                break;
            if (output_to_file_) {
                token_output_file << result->token_identifier_ + " ";
            }
            tokens.push_back(result);
        }
        catch(int e) {
            cout << "An exception occurred. Exception Nr. " << e << '\n';

        }
    }
    error_output_file.close();
    token_output_file.close();
    return tokens;
}

char Scanner::next_char() {
    if (use_backup_){
        use_backup_ = false;
        check_if_newline();
        program_count_++;
        return backup_buffer_;
    }

    if (is_file_) {
        char c;
        if (program_file_.eof()) {
            backup_buffer_ = '\000';
            return backup_buffer_;
        }
        program_file_.get(c);
        program_count_++;
        check_if_newline();
        backup_buffer_ = c;

        return c;
    }
    else {
        check_if_newline();
        backup_buffer_ = program_string_[program_count_];

        return program_string_[program_count_++];
    }

}

Token* Scanner::next_token() {
    State current_state = initial_state;
    string lexeme = "";
    while(true) {
        char lookup = next_char();

        int next_state = current_state.get_next_state(lookup);

        if (next_state == -1) {
            // If we reach an invalid character than everything before the current character and previous Token will be considered apart of the error
            if (lexeme.size() > 1) {
                use_backup_ = true;
                program_count_--;
                current_column_count_--;
            }
            cout << (int)lookup;
            return new ErrorToken("ERROR", lexeme, program_count_ - lexeme.size(), current_row_count_, current_column_count_, current_state.error_message_);

        }
        current_state = table[next_state];

        if (current_state.is_final_state_) {
            if (current_state.backup_) {
                use_backup_ = true;
                program_count_--;
                check_if_backup_row();
            }
            else {
                lexeme += lookup;
            }
            int location = program_count_ - lexeme.size();
            int column_location = current_column_count_ - lexeme.size();

            if(check_if_reserved_word(lexeme))
            {
                string copy = lexeme;
                transform(copy.begin(), copy.end(),copy.begin(), ::toupper);
                return new Token(copy, lexeme, location, current_row_count_, column_location);
            }
            else if (current_state.token_ == "INUM")
                return new IntegerToken(lexeme, location, current_row_count_, column_location);
            else if (current_state.token_ == "FNUM")
                return new FloatToken(lexeme, location, current_row_count_, column_location);

            return new Token(current_state.token_, lexeme, location, current_row_count_, column_location);
        }
        if (next_state != 0)
            lexeme += lookup;
    }
}


bool Scanner::check_if_reserved_word(string word) {
    for (int i = 0; i < reserved_words.size(); i++){
        string res = reserved_words[i];
        transform(res.begin(), res.end(),res.begin(), ::tolower);
        if (res == word)
            return true;
    }
    return false;
}

void Scanner::check_if_backup_row() {
    if (backup_buffer_ == '\n') {
        current_row_count_--;
        current_column_count_ = previous_column_count_;
    }
    else  {
        current_column_count_--;
    }
}

void Scanner::check_if_newline() {
    if (backup_buffer_ == '\n'){
        current_row_count_++;
        previous_column_count_ = current_column_count_;
        current_column_count_ = 1;
    } else {
        current_column_count_++;
    }
}

