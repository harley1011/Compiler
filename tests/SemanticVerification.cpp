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
    tokens = scanner.generate_tokens("class nameHere { int findMin() { idx = 0; }; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


TEST(AssignNonDeclaredVariableProgFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { idx = 0; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}


TEST(AssignNonDeclaredVariableForLoopProgFuncTest, SemanticVerificationTests)
{
//    vector<Token*> tokens;
//    Scanner scanner;
//    tokens = scanner.generate_tokens("program { for(int idx = 0; idx < idc; idx = idx + 1) { }; };", false);
//
//    Parser parser;
//    parser.enable_double_pass_parse_ = true;
//
//    EXPECT_EQ(parser.parse(tokens), true);
//    EXPECT_EQ(parser.semantic_errors_.size(), 1);
//
//    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
//    parser.global_symbol_table_->print(true);

    int i = 1.21;
    i++;
}


TEST(AssignNonDeclaredVariableFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { }; int funcTest() { idx = 0; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}