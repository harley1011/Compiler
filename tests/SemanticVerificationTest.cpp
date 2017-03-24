//
// Created by Harley on 3/21/2017.
//

#include "SemanticVerificationTest.h"
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
    cout << parser.print_semantic_errors();


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
    cout << parser.print_semantic_errors();

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}


TEST(AssignNonDeclaredArrayVariableProgFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { idx[0][2] = 0; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    cout << parser.print_semantic_errors();

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}


TEST(AssignNonDeclaredVariableForLoopProgFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { for(int idx = 0; idx < idc; idx = idx + 1) { }; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    cout << parser.print_semantic_errors();

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
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
    cout << parser.print_semantic_errors();

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}



TEST(GetNonDeclaredVariableProgFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int idx; idx = 0; get(idc); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    cout << parser.print_semantic_errors();

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(PutNonDeclaredVariableProgFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int idx; idx = 0; put(idc); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    cout << parser.print_semantic_errors();

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(ReturnNonDeclaredVariableProgFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int idx; idx = 0; return(idc); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    cout << parser.print_semantic_errors();

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(AssignNonDeclaredFuncClassFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int findMin() { int idx; idx = funcTest(21,idx); }; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    cout << parser.print_semantic_errors();

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(AssignDeclaredInvalidNoParameterFuncClassFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int idx; idx = funcTest(10, idx);}; int funcTest(int id, int idc, int idk) { return (id); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    cout << parser.print_semantic_errors();

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(AssignDeclaredValidFloatNumParameterFuncClassFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int idx; idx = funcTest(10, 10.11);}; int funcTest(int id, int idc) { return (id); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    cout << parser.print_semantic_errors();

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(AssignDeclareValidFloatVarParameterFuncClassFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int idx; float idf; idx = funcTest(idf, 10);}; int funcTest(int id, int idc) { return (id); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    cout << parser.print_semantic_errors();

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(AssignDeclaredClassVarIntFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { }; program { Util idx; idx = funcTest(10, 10);}; int funcTest(int id, int idc) { return (id); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    cout << parser.print_semantic_errors();

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}


TEST(AssignDeclaredClassVarIdClassFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { }; class Func { Util funcTest(int id, int idc) { return (id); }; }; program { Util idx; Func func; idx = func.funcTest(10, 10);}; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    cout << parser.print_semantic_errors();

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(AssignDeclaredClassVarIntClassFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { }; class Func { int funcTest(int id, int idc) { return (id); }; }; program { Util idx; Func func; idx = func.funcTest(10, 10);}; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    cout << parser.print_semantic_errors();

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

