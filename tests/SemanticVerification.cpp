//
// Created by Harley on 3/21/2017.
//

#include "SemanticVerification.h"
#include "gtest/gtest.h"
#include "../src/Parser/Parser.h"
Parser common_setup(string test_program, string derivation_string);
// ----------------------------------------------------------------------------------------------------------------
// undefined id: variable, class, function tests
// undefined member: data member, method, including deeply nested
    // undefined variable tests
TEST(AssignNonDeclardedVarInClassFuncTest, SemanticVerification)
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


TEST(AssignNonDeclardedVarInProgTest, SemanticVerification)
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


TEST(AssignNonDeclardedArrayInProgTest, SemanticVerification)
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


TEST(UseNonDeclaredVarInForLoopConditionTest, SemanticVerification)
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


TEST(AssignNonDeclardedVarInFuncTest, SemanticVerification)
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


TEST(UseNonDeclaredVarInExpression, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens(" program { int z; z = x + y; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 2);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: x variable is being used without being declared:1:28\nError: y variable is being used without being declared:1:28\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}
TEST(AddIdInProg, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens(" program { int z; z = x * y + pFunc() * t.find(); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 4);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: x variable is being used without being declared:1:49\nError: y variable is being used without being declared:1:49\nError: pFunc function is being used without being declared:1:49\nError: t variable is being used without being declared:1:49\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(UseNonDeclaredVarInGetTest, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int idx; idx = 0; get(idc); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: idc variable is being used without being declared:1:39\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(UseNonDeclaredVarInPutTest, SemanticVerification)
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

TEST(UseNonDeclaredVarInReturnTest, SemanticVerification)
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
TEST(AssignVarNonDeclaredFuncInClassFuncTest, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int findMin() { int idx; idx = funcTest(21,idx); return (idx);}; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: funcTest function is being used without being declared:1:65\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(AssignVarNonDeclaredFuncInProgTest, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int idx; idx = funcTest(21,idx); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: funcTest function is being used without being declared:1:42\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}


TEST(AssignVarNonDeclaredFuncInFuncTest, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { }; int funcTest2() {  int idx; idx = funcTest(21,idx); return (0); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: funcTest function is being used without being declared:1:64\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


TEST(AssignVarDeclaredNestedClassFuncFuncInFuncTest, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { }; class B { A a; int funcTest() {return(1); }; }; };  class C { B b;  B funcTest() { return(b); }; }; program { A a; B b; C c; int x; x = c.b.funcTest(); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.print_semantic_errors(), "");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 4);
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
    tokens = scanner.generate_tokens("class Cord { Cordz funcClass(){ Cordz cord; return (cord); }; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 2);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: Cordz is not a valid type:1:29\nError: Cordz is not a valid type:1:45\n");

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
    EXPECT_EQ(parser.semantic_errors_.size(), 2);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: Cordz is not a valid type:1:45\nError: function funcClass has a return type of Cordz but is returning type int:1:58\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);

    parser.global_symbol_table_->print(true);
}

    // data member test
TEST(UseNonDeclaredDataMember, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { }; program { Util util; util.idx = 0; }; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: invalid nested variable idx on variable util:1:50\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


    // deeply nested

TEST(UseNonDeclaredNestedDataMember, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { }; class Util1 { Util util; }; class Util2 { Util1 util1; }; program { Util2 util2; util2.util1.idx = 0; }; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: invalid nested variable idx on variable util2:1:117\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 4);
    parser.global_symbol_table_->print(true);
}

TEST(UseNonDeclaredNestedFunc, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { int idx;}; class Util1 { Util util; }; class Util2 { Util1 util1; }; program { Util2 util2; Util2 util3; util2.util1.util.idx = util3.util1.testFunc(1); }; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: invalid nested function testFunc on variable util3:1:165\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 4);
    parser.global_symbol_table_->print(true);
}

// ----------------------------------------------------------------------------------------------------------------
// forward/circular references: implementation of two passes

TEST(CircularClassReferenceTest, SemanticVerification)
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

TEST(CircularClassNestedReferenceTest, SemanticVerification)
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

TEST(CircularClassReferenceWithoutOneTest, SemanticVerification)
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
TEST(DeclareDuplicateIntInProgram, SemanticVerification)
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



TEST(DeclareDuplicateFloatInProgram, SemanticVerification)
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


TEST(DeclareDuplicateClassInProgram, SemanticVerification)
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

TEST(DeclareDuplicateIntInClass, SemanticVerification)
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

TEST(DeclareDuplicateFloatInClass, SemanticVerification)
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
TEST(DeclareDuplicateClassInClass, SemanticVerification)
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


TEST(DeclareDuplicateIntInClassFunc, SemanticVerification)
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

TEST(DeclareDuplicateIntInClassFuncParams, SemanticVerification)
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


TEST(DeclareDuplicateClassVarInClassFuncParams, SemanticVerification)
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

TEST(DeclareDuplicateClassDefinition, SemanticVerification)
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

TEST(DeclareDuplicateClassFuncDefinition, SemanticVerification)
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

TEST(DeclareDuplicateFuncDefinition, SemanticVerification)
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

TEST(DeclareThreeDuplicateFuncDefinition, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { }; int testFunc() { return (0);}; int testFunc() { return (0);}; float testFunc() { return (0);};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 2);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: A function with the name testFunc already exists within this scope:1:57\nError: A function with the name testFunc already exists within this scope:1:90\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

// ----------------------------------------------------------------------------------------------------------------
// arrays: using right number of dimensions
TEST(UseDeclareVarAsArrayInProg, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int p[5][5]; int x; int y; x = y[5]; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: variable y is not an array but is being accessed as one:1:46\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}
TEST(AssignDeclareVarAsArrayInProg, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; int y; x[5] = y; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: variable x is not an array but is being accessed as one:1:33\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(AssignDeclareArrayWithTooFewDimensionsInProg, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int p[5][5]; int x[5][5]; int y[5]; x[4] = y[4]; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: array x is being accessed with too few dimensions:1:58\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(UseDeclareClassVarAsArrayInProg, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int x; }; program { A a; int p[5][5]; int x[5][5]; x[4][3] = a.x[4]; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: variable x is not an array but is being accessed as one:1:78\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(UseDeclareClassArrayWithTooFewDimensionsInProg, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int x[5][5]; }; program { A a; int p[5][5]; int x[5][5]; x[4][3] = a.x[4]; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: array x is being accessed with too few dimensions:1:84\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(UseDeclareNestedClassArrayWithTooFewDimensionsInProg, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class B { A a; }; class A { int x[5][5]; }; program { B b; int x[5][5]; x[4][3] = b.a.x[4]; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: array x is being accessed with too few dimensions:1:91\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}
TEST(UseDeclareNestedArrayClassWithTooFewDimensionsInProg, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class B { A a[5]; }; class A { int x[5][5]; }; program { B b; int x[5][5]; x[4][3] = b.a.x[4][4]; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: array a is being accessed with too few dimensions:1:97\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(AssignDeclareNestedArrayClassWithTooFewDimensionsInProg, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class B { A a[5]; }; class A { int x[5][5]; }; program { B b; int x[5][5]; b.a.x[4] = x[4][3]; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 2);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: array a is being accessed with too few dimensions:1:94\nError: array x is being accessed with too few dimensions:1:94\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}
// ----------------------------------------------------------------------------------------------------------------
// type checking of a complex expression

TEST(AssignIntVariableAddClassVariableWithNumber, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { }; program { A a; int x; x = a + 5; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: can't perform arithmetic operations with variable a that is not of type int or float:1:45\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(AssignIntVariableMultClassVariableWithNumber, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { }; class B { }; program { A a; B b; int x; x = a * 5; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: can't perform arithmetic operations with variable a that is not of type int or float:1:63\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(AssignIntVariableMultiClassNestedVariableWithNumber, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { }; class B { A a; }; program { B b; int x; x = b.a * 5; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: can't perform arithmetic operations with variable b.a that is not of type int or float:1:65\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(AssignIntVariableMultiVariableExpressionAndFunction, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { }; class B { A a; A funcTest() {return(a); }; }; };  class C { B b;  B funcTest() { return(b); }; }; program { A a; B b; C c; int x; x = c.b.funcTest() + a + b + c + b.a; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 5);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: can't perform arithmetic operations with function c.b.funcTest that is not of type int or float:1:180\nError: can't perform arithmetic operations with variable a that is not of type int or float:1:180\nError: can't perform arithmetic operations with variable b that is not of type int or float:1:180\nError: can't perform arithmetic operations with variable c that is not of type int or float:1:180\nError: can't perform arithmetic operations with variable b.a that is not of type int or float:1:180\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 4);
    parser.global_symbol_table_->print(true);
}
TEST(AssignIntVariableWithWrongIdentifier, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { }; class B { A testFunc() { A a; return (a); }; }; program { B b; int x; x = b.testFunc * 5; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: can't perform arithmetic operations with variable b.testFunc that is not of type int or float:1:102\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}
TEST(FunctionCallWithInvalidExpression, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { }; class B { A testFunc() { A a; return (a); }; }; program { B b; int x; x = funcTest(b.testFunc * 5); }; int funcTest(int x) { return (x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: can't perform arithmetic operations with variable b.testFunc that is not of type int or float:1:112\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 4);
    parser.global_symbol_table_->print(true);
}

TEST(FunctionClassCallWithInvalidExpression, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int testFunc(int a) { }; }; program { A a; int x; x = a.testFunc(a + 2 * 5); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: can't perform arithmetic operations with variable a that is not of type int or float:1:86\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(AssignIntVariableWithInvalidRelation, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { }; program { A a; int x; x = a == 5; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: variable a of type A can't be in a relational expression it needs to be of type int or float:1:46\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(AssignIntVariableWithValidRelation, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { }; program { int a; int x; x = a == 5; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.print_semantic_errors(), "");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(AssignIntVariableWithMultiInvalidRelation, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { }; class B { A a; A funcTest() {return(a); }; }; };  class C { B b;  B funcTest() { return(b); }; }; program { A a; B b; C c; int x; x = c.b.funcTest() + a + b < c + b.a; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 5);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: can't perform arithmetic operations with function c.b.funcTest that is not of type int or float:1:180\nError: can't perform arithmetic operations with variable a that is not of type int or float:1:180\nError: can't perform arithmetic operations with variable b that is not of type int or float:1:180\nError: can't perform arithmetic operations with variable c that is not of type int or float:1:180\nError: can't perform arithmetic operations with variable b.a that is not of type int or float:1:180\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 4);
    parser.global_symbol_table_->print(true);
}

TEST(ExpressionTreeSimpleAdd, SemanticVerification)
{
    Parser parser = common_setup("10 + 11;", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "10 11 ADD ");
}
TEST(ExpressionTreeSimpleMulti, SemanticVerification)
{
    Parser parser = common_setup("10 * 19;", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "10 19 MULTI ");
}

TEST(ExpressionTreeMultiAndAdd, SemanticVerification)
{
    Parser parser = common_setup("10 * 19 + 12;", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "10 19 MULTI 12 ADD ");
}
TEST(ExpressionTreeMultiAndAddTwo, SemanticVerification)
{
    Parser parser = common_setup("10 * 19 + 12 * 5 + 4 * 100 + 12 - 15;", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "10 19 MULTI 12 5 MULTI 4 100 MULTI 12 15 SUB ADD ADD ADD ");
}

TEST(ExpressionTreeExpression, SemanticVerification)
{
    Parser parser = common_setup("10 * ( 7 + 5 );", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "10 7 5 ADD MULTI ");
}
TEST(ExpressionTreeExpressionTwo, SemanticVerification)
{
    Parser parser = common_setup("4 + ( 2 * 7 + 5 ) + 99 * 133;", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "4 2 7 MULTI 5 ADD 99 133 MULTI ADD ADD ");
    EXPECT_EQ(tree->calculate_total(), 13190);
}
TEST(ExpressionTreeExpressionThree, SemanticVerification)
{
    Parser parser = common_setup("3 * 4 + ( 2 * 7 + 5 ) + 99 * 133;", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "3 4 MULTI 2 7 MULTI 5 ADD 99 133 MULTI ADD ADD ");
    EXPECT_EQ(tree->calculate_total(), 13198);
}
TEST(ExpressionTreeExpressionFour, SemanticVerification)
{
    Parser parser = common_setup("3 * 4 + ( 2 * 7 + 5 ) + 99 * ( 1 + 2 * 6 ) + 133;", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "3 4 MULTI 2 7 MULTI 5 ADD 99 1 2 6 MULTI ADD MULTI 133 ADD ADD ADD ");
    EXPECT_EQ(tree->calculate_total(), 1451);
}

TEST(ExpressionTreeExpressionFive, SemanticVerification)
{
    Parser parser = common_setup("3 * 4 + ( 2 * 7 + 5 * ( 2 + 5));", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "3 4 MULTI 2 7 MULTI 5 2 5 ADD MULTI ADD ADD ");
    EXPECT_EQ(tree->calculate_total(), 61);
}

TEST(ExpressionTreeExpressionSix, SemanticVerification)
{
    Parser parser = common_setup("3 * 4 + ( 2 * 7 + 6 * ( 2 + 5)) + 99 * 133;", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "3 4 MULTI 2 7 MULTI 6 2 5 ADD MULTI ADD 99 133 MULTI ADD ADD ");
    EXPECT_EQ(tree->calculate_total(), 13235);
}
TEST(ExpressionTreeExpressionSeven, SemanticVerification)
{
    Parser parser = common_setup("3 - 4;", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "3 4 SUB ");
    EXPECT_EQ(tree->calculate_total(), -1);
}
TEST(ExpressionTreeExpressionEight, SemanticVerification)
{
    Parser parser = common_setup("10 / 5 - 2 + 5;", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "10 5 DIV 2 SUB 5 ADD ");
    EXPECT_EQ(tree->calculate_total(), 5);
}

TEST(ExpressionTreeExpressionNine, SemanticVerification)
{
    Parser parser = common_setup("10 / 5 * 2;", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "10 5 DIV 2 MULTI ");
    EXPECT_EQ(tree->calculate_total(), 4);
}

TEST(ExpressionTreeExpressionTen, SemanticVerification)
{
    Parser parser = common_setup("10 / 5 * 2 - 5 * 5;", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "10 5 DIV 2 MULTI 5 5 MULTI SUB ");
    EXPECT_EQ(tree->calculate_total(), -21);
}

TEST(ExpressionTreeExpressionEleven, SemanticVerification)
{
    Parser parser = common_setup("10 * 50 / 25 * 2 / 2 - 40;", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "10 50 25 DIV 2 2 DIV MULTI MULTI 40 SUB ");
    EXPECT_EQ(tree->calculate_total(), -20);
}

TEST(ExpressionTreeExpressionTwelve, SemanticVerification)
{
    Parser parser = common_setup("10 * 50 / 25 * (2 + 6 + ( 2 + 8) * (2 + 2)) / 2 - 40;", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "10 50 25 DIV 2 6 2 8 ADD 2 2 ADD MULTI ADD ADD 2 DIV MULTI MULTI 40 SUB ");
    EXPECT_EQ(tree->calculate_total(), 440);
}
TEST(ExpressionTreeExpressionThirteen, SemanticVerification)
{
    Parser parser = common_setup("( 10 ) + 5;", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "10 5 ADD ");
    EXPECT_EQ(tree->calculate_total(), 15);
}


TEST(ExpressionTreeExpressionFourteen, SemanticVerification)
{
    Parser parser = common_setup("5 * ( 10 + 12 ) - 200 + ((112 + 21) * 32);", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "5 10 12 ADD MULTI 200 SUB 112 21 ADD 32 MULTI ADD ");
    EXPECT_EQ(tree->calculate_total(), 4166);
}
TEST(ExpressionTreeExpressionFiften, SemanticVerification)
{
    Parser parser = common_setup("(5 + 5);", "<arithExpr>");
    parser.enable_derivation_output_ = false;
    SymbolRecord *func_record = new SymbolRecord();
    ExpressionTree* tree = new ExpressionTree();
    parser.expr(func_record, tree);
    EXPECT_EQ(tree->post_order_print(), "5 5 ADD ");
    EXPECT_EQ(tree->calculate_total(), 10);
}
// ----------------------------------------------------------------------------------------------------------------
// type checking of an assignment statement

TEST(AssignDeclaredClassVarIntFunc, SemanticVerification)
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


TEST(AssignDeclaredClassVarIdClassFunc, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { }; class Func { Util funcTest(int id, int idc) { Util util; return (util); }; }; program { Util idx; Func func; idx = func.funcTest(10, 10);}; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.print_semantic_errors(), "");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}


TEST(AssignDeclaredClassVarIntClassFunc, SemanticVerification)
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

TEST(AssignDeclaredClassVarIntegerInProg, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { }; class Func { int funcTest(int id, int idc) { return (id); }; }; program { Util idx; Func func; idx = 100;}; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: can't assign variable idx a value of type int it needs type Util:1:121\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(AssignDeclaredClassVarIntegerInClassFunc, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { }; class Func { int funcTest(int id, int idc) { Util idx; Func func; idx = 100; return (id); }; }; program { }; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: can't assign variable idx a value of type int it needs type Util:1:92\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(AssignDeclaredClassDataMember, SemanticVerification)
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


TEST(AssignDeclaredClassVarNestedClassVarToAnotherDeclaredClassVar, SemanticVerification)
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


TEST(AssignDeclaredClassVarNestedClassVarWrongVarTypeTest, SemanticVerification)
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

TEST(AssignDeclaredClassVarNestedClassVarWrongFuncTypeTest, SemanticVerification)
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

//type checking of the return value of a function
TEST(FuncWithWrongReturnType, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { }; program { A a; a = funcTest(10, 10);}; A funcTest(int id, int idc) {A a; return (1); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: function funcTest has a return type of A but is returning type int:1:96\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(ClassFuncWithWrongReturnType, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { }; class B { A funcTest(int id, int idc) {A a; return (1); }; }; program { A a; B b; a = b.funcTest(10, 10);};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: function funcTest has a return type of A but is returning type int:1:67\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(ClassFuncWithWrongReturnTypeInt, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { }; class B { int funcTest(int id, int idc) {A a; return (a); }; }; program { int a; B b; a = b.funcTest(10, 10);};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: function funcTest has a return type of int but is returning type A:1:69\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

// ----------------------------------------------------------------------------------------------------------------
// function calls: right number and types of parameters upon call

// function right number parameter tests
TEST(FuncWithWrongNoOfParametersInProg, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int idx; idx = funcTest(10, idx);}; int funcTest(int id, int idc, int idk) { return (id); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: funcTest is being invoked with 2 parameters but needs 3:1:43\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(ClassFuncWithWrongNoOfParametersInProg, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int funcTest(int id, int idc, int idk) { return (id); }; }; program { A a; int idx; idx = a.funcTest(10, idx);}; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: funcTest is being invoked with 2 parameters but needs 3:1:120\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(FuncWithWrongNoOfParametersInClassFunc, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int func() { int idx; idx = funcTest(10, idx); return(0);}; };  program {}; int funcTest(int id, int idc, int idk) { return (id); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: funcTest is being invoked with 2 parameters but needs 3:1:59\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}


// function type checks
TEST(FuncWithCorrectParametersOfTypeIntAndFloatInProg, SemanticVerification)
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

TEST(FuncWithCorrectParametersOfTypeIntAndIdFloatInProg, SemanticVerification)
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

TEST(FuncWithCorrectParametersOfTypeIntAndFloatInClassFunc, SemanticVerification)
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

TEST(ClassFuncWithCorrectParametersOfTypeAndIdFloatInClassFunc, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int funcTest2(int idx, float idf) { idx = funcTest(idf, 10); return (idx); }; }; program { Cord cord; float y; int x; x = cord.funcTest2(x, y); }; int funcTest(int id, int idc) { return (id); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.print_semantic_errors(), "");
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(FuncWithCorrectParametersOfTypeCordAndIntInProg, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int idc; }; program { Cord cord; float y; int x; x = funcTest(cord, y); }; int funcTest(Cord cord, int idc) { return (cord.idc); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.print_semantic_errors(), "");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(FuncWithWrongParametersOfTypeCordAndIntInProg, SemanticVerification)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int idc; }; program { Cord cord; float y; int x; x = funcTest(x, y); }; int funcTest(Cord cord, int idc) { return (cord.idc); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    EXPECT_EQ(parser.print_semantic_errors(), "Error: parameter cord is of type Cord but type int is being passed on function call funcTest:1:81\n");

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

