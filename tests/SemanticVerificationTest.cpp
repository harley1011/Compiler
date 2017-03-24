//
// Created by Harley on 3/21/2017.
//

#include "SemanticVerificationTest.h"
#include "gtest/gtest.h"
#include "../src/Parser/Parser.h"

// ----------------------------------------------------------------------------------------------------------------
// undefined id: variable, class, function tests
// undefined member: data member, method, including deeply nested
    // undefined variable tests
TEST(AssignNonDeclardedVarInClassFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int findMin() { idx = 0; return (0);}; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;


    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: idx variable is being used without being declared:1:41\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


TEST(AssignNonDeclardedVarInProgTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { idx = 0; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: idx variable is being used without being declared:1:18\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}


TEST(AssignNonDeclardedArrayInProgTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { idx[0][2] = 0; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: idx variable is being used without being declared:1:24\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}


TEST(UseNonDeclaredVarInForLoopConditionTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { for(int idx = 0; idx < idc; idx = idx + 1) { }; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: idc variable is being used without being declared:1:37\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}


TEST(AssignNonDeclardedVarInFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { }; int funcTest() { idx = 0; return (0); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: idx variable is being used without being declared:1:38\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}



TEST(UseNonDeclaredVarInGetTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int idx; idx = 0; get(idc); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: idc variable is being used without being declared:1:36\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(UseNonDeclaredVarInPutTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int idx; idx = 0; put(idc); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: idc variable is being used without being declared:1:36\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(UseNonDeclaredVarInReturnTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int idx; idx = 0; return(idc); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: idc variable is being used without being declared:1:39\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

    // undefined function test
TEST(AssignVarNonDeclaredFuncInClassFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int findMin() { int idx; idx = funcTest(21,idx); return (idx);}; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: funcTest function is being used without being declared:1:64\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(AssignVarNonDeclaredFuncInProgTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int idx; idx = funcTest(21,idx); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: funcTest function is being used without being declared:1:41\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}


TEST(AssignVarNonDeclaredFuncInFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { }; int funcTest2() {  int idx; idx = funcTest(21,idx); return (0); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: funcTest function is being used without being declared:1:63\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

    // undefined class test

TEST(DeclaredUnDefinedClassInClassFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int funcClass(){ Cordz cord; return(0); }; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: Cordz is not a valid type:1:43\n");

    parser.global_symbol_table_->print(true);
}

TEST(DeclaredUnDefinedClassInProgTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int funcClass(){ return (0); }; }; program { Cordz cord; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: Cordz is not a valid type:1:71\n");


    parser.global_symbol_table_->print(true);
}

TEST(DeclaredUnDefinedClassInFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int funcClass(){ return (0); }; }; program { }; int funcClass(){ Cordz cord; return (0); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: Cordz is not a valid type:1:91\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);

    parser.global_symbol_table_->print(true);
}

TEST(ReturnUnDefinedClassInClassFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { Cordz funcClass(){ return (0); }; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: Cordz is not a valid type:1:29\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);

    parser.global_symbol_table_->print(true);
}

TEST(ReturnUnDefinedClassInFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { }; program { }; Cordz funcClass(){ return (0); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: Cordz is not a valid type:1:45\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);

    parser.global_symbol_table_->print(true);
}

    // data member test
TEST(UseNonDeclaredDataMember, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { }; program { Util util; util.idx = 0; }; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: invalid nested property idx on variable util:1:50\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


    // deeply nested

TEST(UseNonDeclaredNestedDataMember, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { }; class Util1 { Util util; }; class Util2 { Util1 util1; }; program { Util2 util2; util2.util1.idx = 0; }; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: invalid nested property idx on variable util2:1:117\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 4);
    parser.global_symbol_table_->print(true);
}

TEST(UseNonDeclaredNestedFunc, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { int idx;}; class Util1 { Util util; }; class Util2 { Util1 util1; }; program { Util2 util2; Util2 util3; util2.util1.util.idx = util3.util1.testFunc(1); }; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: invalid nested function testFunc on variable util3:1:164\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 4);
    parser.global_symbol_table_->print(true);
}

// ----------------------------------------------------------------------------------------------------------------
// forward/circular references: implementation of two passes

TEST(CircularClassReferenceTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { B b;}; class B { A a;}; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 2);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: circular reference in class A variable b is of type B which also has one or more variables or nested variables of type A1:9\nError: circular reference in class B variable a is of type A which also has one or more variables or nested variables of type B1:26\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(CircularClassNestedReferenceTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { B b;}; class B { C c;}; class C {  A a;}; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 3);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: circular reference in class A variable c is of type C which also has one or more variables or nested variables of type A1:9\nError: circular reference in class B variable a is of type A which also has one or more variables or nested variables of type B1:26\nError: circular reference in class C variable b is of type B which also has one or more variables or nested variables of type C1:43\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 4);
    parser.global_symbol_table_->print(true);
}

TEST(CircularClassReferenceWithoutOneTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { B b;}; class B { C c;}; class C {  B b;}; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 2);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: circular reference in class B variable c is of type C which also has one or more variables or nested variables of type B1:26\nError: circular reference in class C variable b is of type B which also has one or more variables or nested variables of type C1:43\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 4);
    parser.global_symbol_table_->print(true);
}

// ----------------------------------------------------------------------------------------------------------------
// multiply defined id: variable, class, function, class member
    // duplicates variables
TEST(DeclareDuplicateIntInProgram, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int idx; int idx; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: A variable with the name idx already exists within this scope:1:27\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}



TEST(DeclareDuplicateFloatInProgram, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { float idx; int idx; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: A variable with the name idx already exists within this scope:1:29\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}


TEST(DeclareDuplicateClassInProgram, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { }; program { A a; A a; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: A variable with the name a already exists within this scope:1:34\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

    // duplicate class member variables

TEST(DeclareDuplicateIntInClass, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int idx; int idx;}; program { A a; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: A variable with the name idx already exists within this scope:1:28\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(DeclareDuplicateFloatInClass, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { float idx; float idx;}; program { A a; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: A variable with the name idx already exists within this scope:1:32\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(DeclareDuplicateClassInClass, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class C { }; class A { C c; C c;}; program { A a; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: A variable with the name c already exists within this scope:1:33\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}


TEST(DeclareDuplicateIntInClassFunc, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int func(int idx) { int idx; return (idx); }; }; program { A a; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: A variable with the name idx already exists within this scope:1:38\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(DeclareDuplicateIntInClassFuncParams, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int func(int idx, int idx) { return (0); }; }; program { A a; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: A variable with the name idx already exists within this scope:1:36\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


TEST(DeclareDuplicateClassVarInClassFuncParams, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class B { int idx; }; class A { int func(B b, B b) { return (b.idx); }; }; program { A a; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: A variable with the name b already exists within this scope:1:50\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}


    // duplicate class definition

TEST(DeclareDuplicateClassDefinition, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { }; class A { }; class B { }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: A class definition with the name A already exists:1:22\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

    // duplicate func definition

TEST(DeclareDuplicateClassFuncDefinition, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int testFunc() { return (0);}; int testFunc() { return (0);}; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: A function with the name testFunc already exists within this scope:1:54\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(DeclareDuplicateFuncDefinition, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { }; int testFunc() { return (0);}; int testFunc() { return (0);};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: A function with the name testFunc already exists within this scope:1:57\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(DeclareThreeDuplicateFuncDefinition, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { }; int testFunc() { return (0);}; int testFunc() { return (0);}; float testFunc() { return (0);};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 2);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: A function with the name testFunc already exists within this scope:1:57\\nError: A function with the name testFunc already exists within this scope:1:90\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

// ----------------------------------------------------------------------------------------------------------------
// arrays: using right number of dimensions
TEST(UseDeclareVarAsArrayInProg, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int p[5][5]; int x; int y; x = y[5]; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: can't assign variable idx a value of type int it needs type Util:1:59\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}
TEST(AssignDeclareVarAsArrayInProg, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; int y; x[5] = y; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: can't assign variable idx a value of type int it needs type Util:1:59\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

// ----------------------------------------------------------------------------------------------------------------
// type checking of an assignment statement

TEST(AssignDeclaredClassVarIntFuncTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { }; program { Util idx; idx = funcTest(10, 10);}; int funcTest(int id, int idc) { return (id); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: can't assign variable idx a value of type int it needs type Util:1:59\n");

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
    EXPECT_EQ(parser.print_semantic_errors(), "Error: can't assign variable idx a value of type int it needs type Util:1:139\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(AssignDeclaredClassDataMember, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { int idx; }; program { Util util; util.idx = 0; }; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);


    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


TEST(AssignDeclaredClassVarNestedClassVarToAnotherDeclaredClassVar, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { }; class Util1 { Util util; }; class Util2 { Util1 util1; }; program { Util util; Util util1; Util2 util2; util2.util1.util = util; }; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);


    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 4);
    parser.global_symbol_table_->print(true);
}


TEST(AssignDeclaredClassVarNestedClassVarWrongVarTypeTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { }; class Util1 { Util util; }; class Util2 { Util1 util1; }; program { Util util; Util util1; Util2 util2; util2.util1.util = util2; }; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: can't assign variable util2 a value of type Util2 it needs type Util2:1:145\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 4);
    parser.global_symbol_table_->print(true);
}

TEST(AssignDeclaredClassVarNestedClassVarWrongFuncTypeTest, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { }; class Util1 { Util util; }; class Util2 { Util1 util1; }; program { Util util; Util util1; Util2 util2; util2.util1.util = util2; }; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: can't assign variable util2 a value of type Util2 it needs type Util2:1:145\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 4);
    parser.global_symbol_table_->print(true);
}






// ----------------------------------------------------------------------------------------------------------------
// function calls: right number and types of parameters upon call

// function right number parameter tests
TEST(FuncWithWrongNoOfParametersInProg, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int idx; idx = funcTest(10, idx);}; int funcTest(int id, int idc, int idk) { return (id); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: funcTest is being invoked with only 2 parameters but needs 3:1:42\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(FuncWithWrongNoOfParametersInClassFunc, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int func() { int idx; idx = funcTest(10, idx); return(0);}; };  program {}; int funcTest(int id, int idc, int idk) { return (id); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: funcTest is being invoked with only 2 parameters but needs 3:1:58\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}


// function type checks
TEST(FuncWithCorrectParametersOfTypeIntAndFloatInProg, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int idx; idx = funcTest(10, 10.11);}; int funcTest(int id, int idc) { return (id); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(FuncWithCorrectParametersOfTypeIntAndIdFloatInProg, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int idx; float idf; idx = funcTest(idf, 10);}; int funcTest(int id, int idc) { return (id); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(FuncWithCorrectParametersOfTypeIntAndFloatInClassFunc, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int funcTest2() {int idx; idx = funcTest(10, 10.11); return (idx); }; }; program { }; int funcTest(int id, int idc) { return (id); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(ClassFuncWithCorrectParametersOfTypeAndIdFloatInClassFunc, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int funcTest2(int idx, float idf) { idx = funcTest(idf, 10); return (idx); }; }; program { Cord cord; float y; int x; x = cord.funcTest2(x, y); }; int funcTest(int id, int idc) { return (id); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(FuncWithCorrectParametersOfTypeCordAndIntInProg, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int idc; }; program { Cord cord; float y; int x; x = funcTest(cord, y); }; int funcTest(Cord cord, int idc) { return (cord.idc); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(FuncWithWrongParametersOfTypeCordAndIntInProg, SemanticVerificationTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int idc; }; program { Cord cord; float y; int x; x = funcTest(x, y); }; int funcTest(Cord cord, int idc) { return (cord.idc); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: funcTest 1 parameter is of type Cord but int is being passed:1:80\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}
