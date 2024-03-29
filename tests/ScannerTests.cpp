
#include "ScannerTests.h"
#include "gtest/gtest.h"

#include "../src/Scanner/Scanner.h"
#include "../src/Scanner/IntegerToken.h"
#include "../src/Scanner/FloatToken.h"
#include <vector>

bool check_location(string program, string search_string, int row, int column);
bool compare_files(string first_path, string second_path);
// TEST KEYWORDS

TEST(test_case_keywords, keyword_test) {
    Scanner scanner;
    EXPECT_EQ(scanner.reserved_words.size(), 14);
    for (int i = 0; i < scanner.reserved_words.size(); i++) {
        string res = scanner.reserved_words[i];
        string copy_res = res;
        transform(copy_res.begin(), copy_res.end(),copy_res.begin(), ::toupper);
        vector<Token*> result = scanner.generate_tokens(res, false);
        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(result[0]->token_identifier_, copy_res);
        EXPECT_EQ(result[0]->lexeme_, res);
        EXPECT_EQ(result[0]->location_, 0);
    }

}

// IDENTIFIER TESTS

TEST(test_case_letter_identifier, identifier_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("ide", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "ID");
    EXPECT_EQ(result[0]->lexeme_, "ide");
    EXPECT_EQ(result[0]->location_, 0);
}


TEST(test_case_alphanum_identifier, identifier_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("ide123", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "ID");
    EXPECT_EQ(result[0]->lexeme_, "ide123");
    EXPECT_EQ(result[0]->location_, 0);
}


TEST(test_case_underscore_identifier, identifier_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("ide_123_aaa_1_a", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "ID");
    EXPECT_EQ(result[0]->lexeme_, "ide_123_aaa_1_a");
    EXPECT_EQ(result[0]->location_, 0);
}

// INTEGER TESTS

TEST(test_case_integer, integer_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("1", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "INUM");
    EXPECT_EQ(result[0]->lexeme_, "1");
    EXPECT_EQ(result[0]->location_, 0);
    IntegerToken int_token = static_cast<IntegerToken&>(*result[0]);
    EXPECT_EQ(int_token.converted_lexeme_, 1);
}

TEST(test_case_zero, integer_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("0", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "INUM");
    EXPECT_EQ(result[0]->lexeme_, "0");
    EXPECT_EQ(result[0]->location_, 0);
    IntegerToken int_token = static_cast<IntegerToken&>(*result[0]);
    EXPECT_EQ(int_token.converted_lexeme_, 0);
}

TEST(test_case_multi_integer, integer_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("1078000", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "INUM");
    EXPECT_EQ(result[0]->lexeme_, "1078000");
    EXPECT_EQ(result[0]->location_, 0);
    IntegerToken int_token = static_cast<IntegerToken&>(*result[0]);
    EXPECT_EQ(int_token.converted_lexeme_, 1078000);
}



// FLOAT TESTS

TEST(test_case_float_zero, float_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("0.0", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "FNUM");
    EXPECT_EQ(result[0]->lexeme_, "0.0");
    EXPECT_EQ(result[0]->location_, 0);
    FloatToken int_token = static_cast<FloatToken&>(*result[0]);
    EXPECT_EQ(int_token.converted_lexeme_, (float)0.0);
}



TEST(test_case_float_only_fraction, float_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("0.11101", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "FNUM");
    EXPECT_EQ(result[0]->lexeme_, "0.11101");
    EXPECT_EQ(result[0]->location_, 0);
    FloatToken int_token = static_cast<FloatToken&>(*result[0]);
    EXPECT_EQ(int_token.converted_lexeme_, (float)0.11101);
}



TEST(test_case_float, float_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("100000.11101", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "FNUM");
    EXPECT_EQ(result[0]->lexeme_, "100000.11101");
    EXPECT_EQ(result[0]->location_, 0);
    FloatToken int_token = static_cast<FloatToken&>(*result[0]);
    EXPECT_EQ(int_token.converted_lexeme_, (float)100000.11101);
}

// (, ), [, ], {, } TESTS

TEST(test_case_brackets, brackets_test) {
    Scanner scanner;
    for (int i = 0; i < scanner.brackets_size; i++) {
        pair<char, string> c = scanner.brackets[i];
        string convert_string = string (1, c.first);
        vector<Token*> result = scanner.generate_tokens(string (1, c.first), false);
        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(result[0]->token_identifier_, c.second);
        EXPECT_EQ(result[0]->lexeme_, string (1, c.first));
        EXPECT_EQ(result[0]->location_, 0);
    }
}


// COMMENTS TEST

TEST(test_case_single_line_cmt, comment_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("//Test cmt fdhhgfd rdghfd hgfd \n", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "CMT");
    EXPECT_EQ(result[0]->lexeme_, "//Test cmt fdhhgfd rdghfd hgfd \n");
    EXPECT_EQ(result[0]->location_, 0);
}

TEST(test_case_single_multiline_cmt, comment_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("/*Test cmt fdhhgfd rdghfd hgfd \n \n jkghkjhgkjhg \n \n 78968796 */", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "CMT");
    EXPECT_EQ(result[0]->lexeme_, "/*Test cmt fdhhgfd rdghfd hgfd \n \n jkghkjhgkjhg \n \n 78968796 */");
    EXPECT_EQ(result[0]->location_, 0);
}

// TEST DASH
TEST(test_case_dash, dash_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("/", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "DASH");
    EXPECT_EQ(result[0]->lexeme_, "/");
    EXPECT_EQ(result[0]->location_, 0);
}

// TEST OPERATORS

TEST(test_case_equal, operator_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("=", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "EQUAL");
    EXPECT_EQ(result[0]->lexeme_, "=");
    EXPECT_EQ(result[0]->location_, 0);
}


TEST(test_case_equiv, operator_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("==", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "EQUIV");
    EXPECT_EQ(result[0]->lexeme_, "==");
    EXPECT_EQ(result[0]->location_, 0);
}



TEST(test_case_lt, operator_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("<", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "LT");
    EXPECT_EQ(result[0]->lexeme_, "<");
    EXPECT_EQ(result[0]->location_, 0);
}


TEST(test_case_noteq, operator_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("<>", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "NOTEQ");
    EXPECT_EQ(result[0]->lexeme_, "<>");
    EXPECT_EQ(result[0]->location_, 0);
}


TEST(test_case_lteq, operator_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("<=", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "LTEQ");
    EXPECT_EQ(result[0]->lexeme_, "<=");
    EXPECT_EQ(result[0]->location_, 0);
}

TEST(test_case_gt, operator_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens(">", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "GT");
    EXPECT_EQ(result[0]->lexeme_, ">");
    EXPECT_EQ(result[0]->location_, 0);
}

TEST(test_case_gteq, operator_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens(">=", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "GTEQ");
    EXPECT_EQ(result[0]->lexeme_, ">=");
    EXPECT_EQ(result[0]->location_, 0);
}

TEST(test_case_add, operator_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("+", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "ADD");
    EXPECT_EQ(result[0]->lexeme_, "+");
    EXPECT_EQ(result[0]->location_, 0);
}

TEST(test_case_sub, operator_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("-", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "SUB");
    EXPECT_EQ(result[0]->lexeme_, "-");
    EXPECT_EQ(result[0]->location_, 0);
}

TEST(test_case_mult, operator_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("*", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "MULT");
    EXPECT_EQ(result[0]->lexeme_, "*");
    EXPECT_EQ(result[0]->location_, 0);
}


//TEST PUNCTUATION

TEST(test_case_semicolon, punctuation_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens(";", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "DELI");
    EXPECT_EQ(result[0]->lexeme_, ";");
    EXPECT_EQ(result[0]->location_, 0);
}


TEST(test_case_comma, punctuation_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens(",", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "COM");
    EXPECT_EQ(result[0]->lexeme_, ",");
    EXPECT_EQ(result[0]->location_, 0);
}

TEST(test_case_dot, punctuation_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens(".", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0]->token_identifier_, "DOT");
    EXPECT_EQ(result[0]->lexeme_, ".");
    EXPECT_EQ(result[0]->location_, 0);
}


// TEST INT AND FLOAT CONVERTERS
TEST(test_case_string_int_convert, conversion_test) {
    IntegerToken token("23929083", 0, 0, 0);
    EXPECT_EQ(token.converted_lexeme_, 23929083);

}

TEST(test_case_string_neg_int_convert, conversion_test) {
    IntegerToken token("-23929083", 0, 0, 0);
    EXPECT_EQ(token.converted_lexeme_, -23929083);

}

TEST(test_case_string_float_convert, conversion_test) {
    FloatToken token("239.2035", 0, 0, 0);
    EXPECT_EQ(token.converted_lexeme_, (float)239.2035);

}

TEST(test_case_string_neg_float_convert, conversion_test) {
    FloatToken token("-239.2035", 0, 0, 0);
    EXPECT_EQ(token.converted_lexeme_, (float)-239.2035);

}


// COMBOS


TEST(test_case_all_combo, combo_test) {
    Scanner scanner;
    string program = "identifier_Test 102020 10.3 ()[]{}/* test comment */ // single line comment\n / = == < <> <= > >= + - * ; , .";
    vector<Token*> result = scanner.generate_tokens(program, false);
    string expected_tokens[] = {"ID", "INUM", "FNUM", "OPENPARA", "CLOSEPARA", "OPENBRA", "CLOSEBRA", "OPENCURL", "CLOSECURL", "CMT", "CMT", "DASH", "EQUAL", "EQUIV", "LT", "NOTEQ", "LTEQ", "GT", "GTEQ", "ADD", "SUB", "MULT", "DELI", "COM", "DOT"};
    int size = sizeof(expected_tokens)/ sizeof(expected_tokens[0]);
    EXPECT_EQ(result.size(), size);
    EXPECT_EQ(result[0]->token_identifier_, expected_tokens[0]);
    for(int i = 0; i < size; i++) {
        EXPECT_EQ(result[i]->token_identifier_, expected_tokens[i]);
        EXPECT_EQ(result[i]->lexeme_, program.substr(result[i]->location_, result[i]->lexeme_.size()));
    }
}


TEST(test_case_all_multi_line_combo, combo_test) {
    Scanner scanner;
    string program = "identifier_Test 102020 10.3 ()[]{}/* test comment */ // single line comment\n / = == < <> <= > >= + - * ; , .\n";
    program += program;
    vector<Token*> result = scanner.generate_tokens(program, false);
    string expected_tokens[] = {"ID", "INUM", "FNUM", "OPENPARA", "CLOSEPARA", "OPENBRA", "CLOSEBRA", "OPENCURL", "CLOSECURL", "CMT", "CMT", "DASH", "EQUAL", "EQUIV", "LT", "NOTEQ", "LTEQ", "GT", "GTEQ", "ADD", "SUB", "MULT", "DELI", "COM", "DOT"};
    int size = sizeof(expected_tokens)/ sizeof(expected_tokens[0]);
    EXPECT_EQ(result.size(), size * 2);
    EXPECT_EQ(result[0]->token_identifier_, expected_tokens[0]);

    for(int i = 0; i < size * 2; i++) {
        EXPECT_TRUE(check_location(program, result[i]->lexeme_, result[i]->row_location_, result[i]->column_location_));
        EXPECT_EQ(result[i]->token_identifier_, expected_tokens[i % size]);
        EXPECT_EQ(result[i]->lexeme_, program.substr(result[i]->location_, result[i]->lexeme_.size()));
    }
}



// TEST ERRORS

TEST(test_case_start_unknown, error_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("? identi_fier", false);
    EXPECT_EQ(result[0]->token_identifier_, "ERROR");
    EXPECT_EQ(result[1]->token_identifier_, "ID");
    EXPECT_EQ(result[1]->lexeme_, "identi_fier");
    EXPECT_EQ(result[1]->location_, 2);
    EXPECT_EQ(scanner.error_tokens_[0].error_message_, scanner.table[0].error_message_);
}

TEST(test_case_start_dot, error_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens(".10 identi_fier", false);
    EXPECT_EQ(result[0]->token_identifier_, "DOT");
    EXPECT_EQ(result[1]->token_identifier_, "INUM");
    EXPECT_EQ(result[1]->lexeme_, "10");
    EXPECT_EQ(result[1]->location_, 1);
    EXPECT_EQ(result[2]->token_identifier_, "ID");
    EXPECT_EQ(result[2]->lexeme_, "identi_fier");
}

TEST(test_case_not_float, error_test) {
    Scanner scanner;
    vector<Token*> result = scanner.generate_tokens("float k = .0;", false);
    EXPECT_EQ(result[0]->token_identifier_, "FLOAT");
    EXPECT_EQ(result[1]->token_identifier_, "ID");
    EXPECT_EQ(result[1]->lexeme_, "k");
    EXPECT_EQ(result[2]->token_identifier_, "EQUAL");
    EXPECT_EQ(result[2]->lexeme_, "=");
    EXPECT_EQ(result[3]->token_identifier_, "DOT");
    EXPECT_EQ(result[3]->lexeme_, ".");
    EXPECT_EQ(result[4]->token_identifier_, "INUM");
    EXPECT_EQ(result[4]->lexeme_, "0");
    EXPECT_EQ(result[5]->token_identifier_, "DELI");
    EXPECT_EQ(result[5]->lexeme_, ";");
}

TEST(test_case_float_end_non_digit, error_test) {
    Scanner scanner("test.txt", "error.txt");
    vector<Token*> result = scanner.generate_tokens("101. identi_fier 101.i identi_fier", false);
    EXPECT_EQ(result[0]->token_identifier_, "ERROR");
    EXPECT_EQ(result[1]->token_identifier_, "ID");
    EXPECT_EQ(result[1]->lexeme_, "identi_fier");
    EXPECT_EQ(result[1]->location_, 5);
    EXPECT_EQ(scanner.error_tokens_[0].error_message_, scanner.table[6].error_message_);
    EXPECT_EQ(result[2]->token_identifier_, "ERROR");
    EXPECT_EQ(result[3]->token_identifier_, "ID");
    EXPECT_EQ(result[3]->lexeme_, "i");
    EXPECT_EQ(result[4]->token_identifier_, "ID");
    EXPECT_EQ(result[4]->lexeme_, "identi_fier");
}


TEST(test_case_read_input_file, full_program_test) {
    Scanner scanner("built_simple_program_out.txt", "error_out.txt");
    vector<Token*> result = scanner.generate_tokens("simple_program.txt", true);
    EXPECT_TRUE(compare_files("simple_program_out.txt", "built_simple_program_out.txt"));
}

TEST(test_case_large_file, full_program_test) {
    Scanner scanner("built_large_program_out.txt", "large_error_out.txt");
    vector<Token*> result = scanner.generate_tokens("large_program.txt", true);
    EXPECT_TRUE(compare_files("large_program_out.txt", "built_large_program_out.txt"));
}

TEST(test_case_read_input_file_with_errors, full_program_test) {
    Scanner scanner("built_simple_program_with_errors_token_out.txt", "built_simple_program_with_errors_out.txt");
    vector<Token*> result = scanner.generate_tokens("simple_program_errors.txt", true);
    EXPECT_TRUE(compare_files("simple_program_with_errors_token_out.txt", "built_simple_program_with_errors_token_out.txt"));
    EXPECT_TRUE(compare_files("simple_program_with_errors_out.txt", "built_simple_program_with_errors_out.txt"));
}

TEST(test_case_large_file_2, full_program_test) {
    Scanner scanner("built_large_program_out_2.txt", "built_large_error_out_2.txt");
    vector<Token*> result = scanner.generate_tokens("large_program_2.txt", true);
    EXPECT_TRUE(compare_files("large_program_out_2.txt", "built_large_program_out_2.txt"));
    EXPECT_TRUE(compare_files("large_error_out_2.txt", "built_large_error_out_2.txt"));
}

bool check_location(string program, string search_string, int row, int column) {
    for(int i = 0; program.size(); i++) {
        if (row == 0) {
            string result = program.substr(i + column, search_string.size());
            return result == search_string;
        }
        if (program[i] == '\n')
            row--;
    }
    return false;
}


bool compare_files(string first_path, string second_path) {
    ifstream f_file;
    ifstream s_file;
    f_file.open(first_path);
    s_file.open(second_path);
    int row_count = 0;
    int column_count = 0;
    while(true) {
        char f;
        char s;

        if (f_file.eof() && s_file.eof()) {
            f_file.close();
            s_file.close();
            return true;
        }
        else if (f_file.eof() || s_file.eof()) {
            f_file.close();
            s_file.close();
            return false;
        }
        f = f_file.get();
        s = s_file.get();

        if (f == '\r')
            f = f_file.get();
        if (s == '\r')
            s = f_file.get();

        if (f != s) {
            f_file.close();
            s_file.get();
            return false;
        }

    }
}
