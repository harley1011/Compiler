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

TEST(SimpleForLoopInFunc, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; for (int i = 0; i < 10; i = i + 1) { x = x + 10; }; put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


//conditional statement: code blocks, jumping on condition ○○ 2


TEST(SimpleIfInFunc, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 90; if ( 10 == 10 ) then { x = x + 10; }; put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(SimpleIfElseInFunc, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 90; if ( 10 == 9 ) then { x = x + 40; x = x - 20;} else { x = x + 20; }; put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(SimpleIfElseOneLineInFunc, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 90; if ( 10 == 9 ) then x = x + 10; else x = x + 20; put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
//Input/output: read from keyboard, write to standard output

TEST(SimpleWriteIntInFunc, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { put(100); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(SimpleWriteIntVarInFunc, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program {int x; x = 100; put(x};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(SimpleReadIntVarInFunc, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program {int x; get(x};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

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

TEST(SimpleAdditionWithAllVariableInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; int y; int z; int k; k = 10; z = 85; y = 20; x = y + z + k;};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(SimpleEquivInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 20 == 85;};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(SimpleLessInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; int y; y = 5; x = 2 < y;};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

// expressions: composite expressions and intermediate result

TEST(MultipleAdditionInFunction, CodeGeneration)
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

TEST(MultipleAdditionAndMultiInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 20 + 85 * 5; put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

// function declaration code block (alias to jump to, jump back)

TEST(FunctionDeclaration, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; }; int funcTest() { int i; i = 5; return(i); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(FunctionWithClassVariableDeclaration, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class B { int k; A a; }; class A {int i; int k; int p; }; program { int x; }; int funcTest() { int i; i = 5; A a; return(i); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

// function call mechanism: jump on call, return value

TEST(FunctionCall, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = funcTest(); put(x); }; int funcTest() { int i; i = 100; return(i); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


TEST(FunctionCallInFunctionCall, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = funcTest(); put(x); }; int funcTest() { int i; i = funcTest2(); return(i); }; int funcTest2() { int k; k = 120; return(k); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(FunctionCallInExpression, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 95 + funcTest(); }; int funcTest() { int i; i = 5; return(i); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(FunctioCallnWithClassVariableDeclaration, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class B { int x; int y; }; class A {int i; int k; B b; int p; }; program { int x; x = funcTest(); put(x); }; int funcTest() { int i; A a; i = 100; a.k = 120; return(a.k); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(FunctioCallnWithArrayVariableDeclaration, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = funcTest(); put(x); }; int funcTest() { int i[5]; i[2] = 100; return(i[2]); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


TEST(FunctionCallWithIf, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = funcTest(10, 10); put(x); }; int funcTest(int x, int y) { if ( x < y ) then return(100); else return(110); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(FunctionCallWithForLoop, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = funcTest(10, 0); put(x); }; int funcTest(int x, int y) { for(int i = 0; i < 10; i = i + 1) y = x + y; return(y); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

// parameter passing mechanism

TEST(FunctionParameter, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; int y; y = 40; x = funcTest(60, y); put(x);}; int funcTest(int k, int x) { int i; i = k + x; return(i); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

// offset calculation mechanism: arrays processing (uni- and multi-dimensional), using data members

TEST(ArrayAccess, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x[5]; x[4] = 100; put(x[4]); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


TEST(DoubleArrayAccess, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x[5][4]; x[4][3] = 120; put(x[4][3]); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


TEST(ClassDataMemberAccess, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int x; int y; }; program { int k; A a; a.y = 110; put(a.y); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


// passing array/object as parameter



// function call stack implementation: recursive/circular function calls

TEST(RecursiveFunctionCall, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = countSum(10); put(x); }; int countSum(int x) { return(i); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_variable_declaration(), "a_program res 16\n");
    EXPECT_EQ(parser.code_generator_->generate_code(), "program entry\nhlt\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}