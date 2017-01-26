#include "gtest/gtest.h"
#include "../src/Scanner.h"
#include <vector>
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


TEST(test_case_letter_identifier, test_name) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("ide", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "ID");
    EXPECT_EQ(result[0].lexeme_, "ide");
    EXPECT_EQ(result[0].location_, 0);
}


TEST(test_case_alphanum_identifier, test_name) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("ide123", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "ID");
    EXPECT_EQ(result[0].lexeme_, "ide123");
    EXPECT_EQ(result[0].location_, 0);
}


TEST(test_case_underscore_identifier, test_name) {
    Scanner scanner;
    vector<token> result = scanner.generate_tokens("ide_123", false);
    EXPECT_EQ(result.size(), 1);
    EXPECT_EQ(result[0].token_identifier_, "ID");
    EXPECT_EQ(result[0].lexeme_, "ide_123");
    EXPECT_EQ(result[0].location_, 0);
}

