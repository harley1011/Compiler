#include "gtest/gtest.h"
#include "../src/Scanner.h"
#include <vector>
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// TEST KEYWORDS

TEST(test_case_keywords, keyword_test) {
    Scanner scanner;

    for (int i = 0; i < scanner.reserved_words->size(); i++) {
        string res = scanner.reserved_words[i];
        string copy_res = res;
        transform(copy_res.begin(), copy_res.end(),copy_res.begin(), ::tolower);
        vector<token> result = scanner.generate_tokens(res, false);
        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(result[0].token_identifier_, copy_res);
        EXPECT_EQ(result[0].lexeme_, res);
        EXPECT_EQ(result[0].location_, 0);
    }

}

// IDENTIFIER TESTS

TEST(test_case_letter_identifier, identifier_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("ide", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "ID");
    EXPECT_EQ(result[0].lexeme_, "ide");
    EXPECT_EQ(result[0].location_, 0);
}


TEST(test_case_alphanum_identifier, identifier_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("ide123", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "ID");
    EXPECT_EQ(result[0].lexeme_, "ide123");
    EXPECT_EQ(result[0].location_, 0);
}


TEST(test_case_underscore_identifier, identifier_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("ide_123_aaa_1_a", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "ID");
    EXPECT_EQ(result[0].lexeme_, "ide_123_aaa_1_a");
    EXPECT_EQ(result[0].location_, 0);
}

// INTEGER TESTS

TEST(test_case_integer, integer_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("1", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "INUM");
    EXPECT_EQ(result[0].lexeme_, "1");
    EXPECT_EQ(result[0].location_, 0);
}

TEST(test_case_zero, integer_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("0", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "INUM");
    EXPECT_EQ(result[0].lexeme_, "0");
    EXPECT_EQ(result[0].location_, 0);
}

TEST(test_case_multi_integer, integer_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("1078000", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "INUM");
    EXPECT_EQ(result[0].lexeme_, "1078000");
    EXPECT_EQ(result[0].location_, 0);
}


// FLOAT TESTS

TEST(test_case_float_zero, float_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("0.0", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "FNUM");
    EXPECT_EQ(result[0].lexeme_, "0.0");
    EXPECT_EQ(result[0].location_, 0);
}



TEST(test_case_float_only_fraction, float_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("0.11101", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "FNUM");
    EXPECT_EQ(result[0].lexeme_, "0.11101");
    EXPECT_EQ(result[0].location_, 0);
}



TEST(test_case_float, float_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("100000.11101", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "FNUM");
    EXPECT_EQ(result[0].lexeme_, "100000.11101");
    EXPECT_EQ(result[0].location_, 0);
}

// (, ), [, ], {, } TESTS

TEST(test_case_brackets, brackets_test) {
    Scanner scanner;
    for (int i = 0; i < scanner.brackets_size; i++) {
        pair<char, string> c = scanner.brackets[i];
        string convert_string = string (1, c.first);
        vector<token> result = scanner.generate_tokens(string (1, c.first), false);
        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(result[0].token_identifier_, c.second);
        EXPECT_EQ(result[0].lexeme_, string (1, c.first));
        EXPECT_EQ(result[0].location_, 0);
    }
}


// COMMENTS TEST

TEST(test_case_single_line_cmt, comment_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("//Test cmt fdhhgfd rdghfd hgfd \n", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "CMT");
    EXPECT_EQ(result[0].lexeme_, "//Test cmt fdhhgfd rdghfd hgfd \n");
    EXPECT_EQ(result[0].location_, 0);
}

TEST(test_case_single_multiline_cmt, comment_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("/*Test cmt fdhhgfd rdghfd hgfd \n \n jkghkjhgkjhg \n \n 78968796 */", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "CMT");
    EXPECT_EQ(result[0].lexeme_, "/*Test cmt fdhhgfd rdghfd hgfd \n \n jkghkjhgkjhg \n \n 78968796 */");
    EXPECT_EQ(result[0].location_, 0);
}

// TEST DASH
TEST(test_case_dash, dash_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("/", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "DASH");
    EXPECT_EQ(result[0].lexeme_, "/");
    EXPECT_EQ(result[0].location_, 0);
}

// TEST OPERATORS

TEST(test_case_equal, operator_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("=", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "EQUAL");
    EXPECT_EQ(result[0].lexeme_, "=");
    EXPECT_EQ(result[0].location_, 0);
}


TEST(test_case_equiv, operator_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("=", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "EQUIV");
    EXPECT_EQ(result[0].lexeme_, "==");
    EXPECT_EQ(result[0].location_, 0);
}



TEST(test_case_lt, operator_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("<", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "LT");
    EXPECT_EQ(result[0].lexeme_, "<");
    EXPECT_EQ(result[0].location_, 0);
}


TEST(test_case_noteq, operator_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("<>", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "NOTEQ");
    EXPECT_EQ(result[0].lexeme_, "<>");
    EXPECT_EQ(result[0].location_, 0);
}


TEST(test_case_lteq, operator_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("<=", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "LTEQ");
    EXPECT_EQ(result[0].lexeme_, "<=");
    EXPECT_EQ(result[0].location_, 0);
}

TEST(test_case_gt, operator_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens(">", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "GT");
    EXPECT_EQ(result[0].lexeme_, ">");
    EXPECT_EQ(result[0].location_, 0);
}

TEST(test_case_gteq, operator_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens(">=", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "GTEQ");
    EXPECT_EQ(result[0].lexeme_, ">=");
    EXPECT_EQ(result[0].location_, 0);
}

TEST(test_case_add, operator_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("+", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "ADD");
    EXPECT_EQ(result[0].lexeme_, "+");
    EXPECT_EQ(result[0].location_, 0);
}

TEST(test_case_sub, operator_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("-", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "SUB");
    EXPECT_EQ(result[0].lexeme_, "-");
    EXPECT_EQ(result[0].location_, 0);
}

TEST(test_case_mult, operator_test) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("*", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "MULT");
    EXPECT_EQ(result[0].lexeme_, "*");
    EXPECT_EQ(result[0].location_, 0);
}




















