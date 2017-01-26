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