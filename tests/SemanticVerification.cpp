//
// Created by Harley on 3/21/2017.
//

#include "SemanticVerification.h"
#include "gtest/gtest.h"
#include "../src/Parser/Parser.h"

TEST(AssignNonDeclaredVariableClassFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { float idx; float idx; int findMin() { }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}