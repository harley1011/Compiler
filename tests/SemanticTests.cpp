//
// Created by Harley on 3/9/2017.
//

#include "SemanticTests.h"
#include "gtest/gtest.h"
#include "../src/Parser/Parser.h"
SemanticParser common_setup_semantic(string test_program, string derivation_string);
bool check_if_record_exists_in_table(SymbolRecord record, SymbolTable symbol);
bool check_record_properties(string type, string name, string kind, string structure, SymbolRecord* record);
bool check_record_properties_with_array_sizes(string type, string name, string kind, string structure, vector<int> array_sizes, SymbolRecord* record);
bool check_record_properties(string type, string name, string kind, string structure, SymbolRecord* record) {
    EXPECT_EQ(record->type_, type);
    EXPECT_EQ(record->name_, name);
    EXPECT_EQ(record->kind_, kind);
    EXPECT_EQ(record->structure_, structure);

}
bool check_record_properties_with_array_sizes(string type, string name, string kind, string structure, vector<int> array_sizes, SymbolRecord* record) {
    EXPECT_EQ(record->type_, type);
    EXPECT_EQ(record->name_, name);
    EXPECT_EQ(record->kind_, kind);
    EXPECT_EQ(record->structure_, structure);

    for (int i = 0; i < record->array_sizes.size(); i++) {
        if (i < array_sizes.size()) {
            EXPECT_EQ(array_sizes[i], record->array_sizes[i]);
        } else {
            EXPECT_TRUE(false);
        }
    }

}




TEST(varDeclareInProgramTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; float y; Util util; };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = false;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 1);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 3);
    check_record_properties("int", "x", "variable", "simple", syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties("float", "y", "variable", "simple", syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[1]);
    check_record_properties("Util", "util", "variable", "class", syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]);

    syntaxParser.global_symbol_table_->print();
}

TEST(varDeclareInClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { int x; float y; Util util; }; program { };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = false;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 3);
    check_record_properties("int", "x", "variable", "simple", syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties("float", "y", "variable", "simple", syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[1]);
    check_record_properties("Util", "util", "variable", "class", syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]);

    syntaxParser.global_symbol_table_->print();
}

TEST(varDeclareInClassFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { int classFunc() { int x; float y; Util util; }; }; program { };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = false;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 1);
    syntaxParser.global_symbol_table_->print();
    check_record_properties("int", "x", "variable", "simple", syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties("float", "y", "variable", "simple", syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[1]);
    check_record_properties("Util", "util", "variable", "class", syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]);
}

TEST(varProperlyDeclaredInProgramTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int x; int y; int z; }; program { Cord cord; };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = true;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 3);
    EXPECT_TRUE(syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[0]->properly_declared_);
    syntaxParser.global_symbol_table_->print();
}

TEST(varNotProperlyDeclaredInProgramTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { Cord cord; };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = true;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 1);
    syntaxParser.global_symbol_table_->print();
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 1);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 1);
    EXPECT_FALSE(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->properly_declared_);

}

TEST(varProperlyDeclaredInClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int x; int y; int z; Cord cord; }; program { };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = true;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_TRUE(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[3]->properly_declared_);
    syntaxParser.global_symbol_table_->print();
}

TEST(varNonProperlyDeclaredInClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int x; int y; int z; Cordz cord; }; program { };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = true;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 1);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_FALSE(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[3]->properly_declared_);
    syntaxParser.global_symbol_table_->print();
}



TEST(varProperlyDeclaredInClassFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int funcClass(){ Cord cord; }; }; program { };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = true;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 1);
    EXPECT_TRUE(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->properly_declared_);

    syntaxParser.global_symbol_table_->print();
}

TEST(varNonProperlyDeclaredInClassFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int funcClass(){ Cordz cord; }; }; program { };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = true;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 1);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 1);
    EXPECT_FALSE(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->properly_declared_);

    syntaxParser.global_symbol_table_->print();
}


TEST(varDeclareFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { }; int funcTest() { int x; float y; Util util; };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = false;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_.size(), 3);
    syntaxParser.global_symbol_table_->print();
    check_record_properties("int", "x", "variable", "simple", syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[0]);
    check_record_properties("float", "y", "variable", "simple", syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[1]);
    check_record_properties("Util", "util", "variable", "class", syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[2]);
}



TEST(arrayDeclareInProgramTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x[5][10]; float y[10][12][23]; Util util[1][2]; };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = false;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 1);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 3);
    check_record_properties_with_array_sizes("int[5][10]", "x", "variable", "array", {5, 10}, syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties_with_array_sizes("float[10][12][23]", "y", "variable", "array", {10, 12, 23}, syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[1]);
    check_record_properties_with_array_sizes("Util[1][2]", "util", "variable", "class array", {1, 2}, syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]);

    syntaxParser.global_symbol_table_->print();
}

TEST(arrayDeclareInClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { int x[5][10]; float y[10][12][23]; Util util[1][2]; }; program { };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = false;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 3);
    check_record_properties_with_array_sizes("int[5][10]", "x", "variable", "array", {5, 10}, syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties_with_array_sizes("float[10][12][23]", "y", "variable", "array", {10, 12, 23}, syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[1]);
    check_record_properties_with_array_sizes("Util[1][2]", "util", "variable", "class array", {1, 2}, syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]);

    syntaxParser.global_symbol_table_->print();
}

TEST(arrayDeclareInClassFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { int classFunc() { int x[5][10]; float y[10][12][23]; Util util[1][2]; }; }; program { };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = false;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 1);
    syntaxParser.global_symbol_table_->print();
    check_record_properties_with_array_sizes("int[5][10]", "x", "variable", "array", {5, 10}, syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties_with_array_sizes("float[10][12][23]", "y", "variable", "array", {10, 12, 23}, syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[1]);
    check_record_properties_with_array_sizes("Util[1][2]", "util", "variable", "class array", {1, 2}, syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]);
}

TEST(arrayDeclareFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { }; int funcTest() { int x[5][10]; float y[10][12][23]; Util util[1][2]; };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = false;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_.size(), 3);
    syntaxParser.global_symbol_table_->print();
    check_record_properties_with_array_sizes("int[5][10]", "x", "variable", "array", {5, 10}, syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[0]);
    check_record_properties_with_array_sizes("float[10][12][23]", "y", "variable", "array", {10, 12, 23}, syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[1]);
    check_record_properties_with_array_sizes("Util[1][2]", "util", "variable", "class array", {1, 2}, syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[2]);
}


TEST(FuncDeclareTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    string find_min_func_string = "int findMin(int array[100]) { int minValue; int idxc; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; ";
    tokens = scanner.generate_tokens("program { }; " + find_min_func_string, false);

    SemanticParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_.size(), 4);
    check_record_properties("int : int[100]", "findMin", "function", "simple", syntaxParser.global_symbol_table_->symbol_records_[1]);
    check_record_properties("int[100]", "array", "parameter", "simple", syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[0]);
    check_record_properties("int", "minValue", "variable", "simple", syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[1]);
    check_record_properties("int", "idxc", "variable", "simple", syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[2]);
    check_record_properties("int", "idx", "variable", "simple", syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[3]);
    syntaxParser.global_symbol_table_->print();
}

TEST(FuncDeclareClassTest, SemanticTests)
{
    string find_min_func_string = "int findMin(int array[100]) { int minValue; int idxc; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; ";
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { " + find_min_func_string + "}; program { };", false);

    SemanticParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 4);
    check_record_properties("int : int[100]", "findMin", "function", "simple", syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties("int[100]", "array", "parameter", "simple", syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties("int", "minValue", "variable", "simple", syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[1]);
    check_record_properties("int", "idxc", "variable", "simple", syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]);
    check_record_properties("int", "idx", "variable", "simple", syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[3]);
    syntaxParser.global_symbol_table_->print();
}


TEST(FuncDeclareClassWithDuplicateVariablesTest, SemanticTests)
{
    string find_min_func_string = "class Utility { int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; };";
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens( find_min_func_string + " program { };", false);

    SemanticParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.errors_.size(), 0);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 2);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_.size(), 3);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[3]->symbol_table_->symbol_records_.size(), 3);
    syntaxParser.global_symbol_table_->print();
}


TEST(FuncDeclareReturnClassWithDoublePassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { }; program { }; Cord create_cord() { Cord cord; return(cord); };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = true;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 3);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_.size(), 1);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[2]->properly_declared_, true);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_[0]->properly_declared_, true);
    syntaxParser.global_symbol_table_->print();
}

TEST(FuncDeclareReturnClassWithoutDoublePassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { }; program { }; Cord create_cord() { Cord cord; return(cord); };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = false;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 3);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_.size(), 1);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[2]->properly_declared_, false);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_[0]->properly_declared_, false);
    syntaxParser.global_symbol_table_->print();
}

TEST(FuncDeclareReturnNonDeclareClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { }; Cord create_cord() { Cord cord; return(cord); };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = true;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 2);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_.size(), 1);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[0]->properly_declared_, false);
    syntaxParser.global_symbol_table_->print();
}

TEST(FuncDeclareDuplicateVarClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int idx; int idx; }; program { };", false);

    SemanticParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 1);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    syntaxParser.global_symbol_table_->print();
}

TEST(ClassWithClassVarTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { int idx; }; program { Util util; };", false);

    SemanticParser syntaxParser;
    syntaxParser.enable_double_pass_parse_ = true;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    syntaxParser.global_symbol_table_->print();
}

TEST(FullProgramTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Utility { int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idp; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idp; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; }; program { int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); }; float randomize() { float value; value = 100 * (2 + 3.0 / 7.0006); value = 1.05 + ((2.04 * 2.47) - 3.0) + 7.0006 ; return (value); };", false);

    SemanticParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 3);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[3]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_.size(), 7);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_.size(), 1);
    syntaxParser.global_symbol_table_->print();
}

//
//TEST(progTest, SemanticTests) {
//    Parser syntaxParser = common_setup_semantic("program {int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); };", "<progBody>");
//    EXPECT_TRUE(syntaxParser.progBody());
//    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 7);
//    syntaxParser.global_symbol_table_->print();
//}

//TEST(progWithClassTest, SemanticTests) {
//    Parser syntaxParser = common_setup_semantic("program { Utility utility; };", "<progBody>");
//    EXPECT_TRUE(syntaxParser.progBody());
//    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 1);
//    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->name_, "utility");
//    syntaxParser.global_symbol_table_->print();
//}
//
//TEST(funcTest, SemanticTests) {
//    Parser syntaxParser = common_setup_semantic("program { }; int findMin(int x) { int r; int y; float p;  Utility utility;};", "<progBody>");
//    EXPECT_TRUE(syntaxParser.progBody());
//    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_.size(), 5);
//    syntaxParser.global_symbol_table_->print();
//}

bool check_if_record_exists_in_table(SymbolRecord record, SymbolTable symbol) {
    SymbolRecord* found_record = symbol.search(record.name_);

    if (found_record == NULL)
        return false;

    return found_record->type_ == record.type_ && found_record->kind_ == record.kind_ && found_record->structure_ == record.structure_;
}

SemanticParser common_setup_semantic(string test_program, string derivation_string) {
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens(test_program, false);

    SemanticParser semanticParser(tokens);
    semanticParser.current_rhs_derivation_ = derivation_string;
    semanticParser.enable_derivation_output_ = false;
    return semanticParser;
}
