//
// Created by Harley on 3/30/2017.
//

#include "CodeGenerationTests.h"

//memory allocation: int and float variable declarations

TEST(IntDeclarationInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int i; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "i_program dw 0\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");


    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}
TEST(IntVariableAssignmentIntFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int i; i = 5; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "i_program dw 0\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\naddi r1,r0,5\nsw i_program(r0),r1\nhlt\n");


    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

//memory allocation: array variable declarations

TEST(IntArrayDeclarationInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int i[5]; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "i_program res 40\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");


    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}
//TEST(IntArrayVariableAssignmentInFunction, CodeGeneration)
//{
//    vector<Token*> tokens;
//    Scanner scanner;
//    tokens = scanner.generate_tokens("program { int i[5]; i[4] = 4;};", false);
//
//    Parser parser;
//    parser.enable_double_pass_parse_ = true;
//
//    EXPECT_EQ(parser.parse(tokens), true);
//    EXPECT_EQ(parser.semantic_errors_.size(), 0);
//    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "i_program res 40\n");
//    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");
//
//
//    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
//    parser.global_symbol_table_->print(true);
//}


//memory allocation: object variable declarations

TEST(ClassDeclarationInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int x; int y; }; program { A a; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");


    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

//loop statement: code block, jump, looping upon condition




//conditional statement: code blocks, jumping on condition ○○ 2

//Input/output: read from keyboard, write to standard output


//expressions: arithmetic, relational and logic operators

TEST(SimpleAdditionInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 20 + 85;};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");


    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(MultiAdditionInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 20 + 85 + 5;};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");


    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(SimpleAdditionWithVariableInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; int y; y = 20; x = y + 85;};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");


    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}