//
// Created by Harley on 3/9/2017.
//

#include "SemanticTests.h"
#include "gtest/gtest.h"
#include "../src/Parser/Parser.h"

bool check_record_properties(string type, string name, string kind, string structure, SymbolRecord* record);
bool check_record_properties_and_declared(string type, string name, string kind, string structure, SymbolRecord* record);
bool check_record_properties_with_array_sizes(string type, string name, string kind, string structure, vector<int> array_sizes, SymbolRecord* record);
bool compare_files(string first_path, string second_path);
bool check_record_properties_and_declared(string type, string name, string kind, string structure, SymbolRecord* record) {
    check_record_properties(type, name, kind, structure, record);
    EXPECT_TRUE(record->properly_declared_);
}

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

    Parser parser;
    parser.enable_double_pass_parse_ = false;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 3);
    check_record_properties("int", "x", "variable", "simple", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties("float", "y", "variable", "simple", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[1]);
    check_record_properties("Util", "util", "variable", "class", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]);

    parser.global_symbol_table_->print(true);
}

TEST(varDeclareInClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { int x; float y; Util util; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = false;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 3);
    check_record_properties("int", "x", "variable", "simple", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties("float", "y", "variable", "simple", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[1]);
    check_record_properties("Util", "util", "variable", "class", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]);

    parser.global_symbol_table_->print(true);
}

TEST(varDeclareInClassFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { int classFunc() { int x; float y; Util util; }; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = false;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
    check_record_properties("int", "x", "variable", "simple", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties("float", "y", "variable", "simple", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[1]);
    check_record_properties("Util", "util", "variable", "class", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]);
}

TEST(varProperlyDeclaredInProgramTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int x; int y; int z; }; program { Cord cord; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 3);
    EXPECT_TRUE(parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[0]->properly_declared_);
    parser.global_symbol_table_->print(true);
}

TEST(varArrayProperlyDeclaredInProgramTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int x; int y; int z; }; program { Cord cord[5]; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 3);
    EXPECT_TRUE(parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[0]->properly_declared_);
    parser.global_symbol_table_->print(true);
}

TEST(varNotProperlyDeclaredInProgramTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { Cord cord; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);
    parser.global_symbol_table_->print(true);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 1);
    EXPECT_FALSE(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->properly_declared_);

}

TEST(varProperlyDeclaredInClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int x; int y; int z; Cord cord; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_TRUE(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[3]->properly_declared_);
    parser.global_symbol_table_->print(true);
}

TEST(varNonProperlyDeclaredInClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int x; int y; int z; Cordz cord; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_FALSE(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[3]->properly_declared_);
    parser.global_symbol_table_->print(true);
}

TEST(varProperlyDeclaredInClassFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int funcClass(){ Cord cord; }; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 1);
    EXPECT_TRUE(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->properly_declared_);

    parser.global_symbol_table_->print(true);
}

TEST(varNonProperlyDeclaredInClassFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int funcClass(){ Cordz cord; }; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 1);
    EXPECT_FALSE(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->properly_declared_);

    parser.global_symbol_table_->print(true);
}


TEST(varDeclareFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { }; int funcTest() { int x; float y; Util util; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = false;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
    check_record_properties("int", "x", "variable", "simple", parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[0]);
    check_record_properties("float", "y", "variable", "simple", parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[1]);
    check_record_properties("Util", "util", "variable", "class", parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[2]);
}

TEST(varDeclareFunc2Test, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = funcTest();}; int funcTest() { int x; float y; Util util; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = false;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
    check_record_properties("int", "x", "variable", "simple", parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[0]);
    check_record_properties("float", "y", "variable", "simple", parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[1]);
    check_record_properties("Util", "util", "variable", "class", parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[2]);
}



TEST(arrayDeclareInProgramTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x[5][10]; float y[10][12][23]; Util util[1][2]; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = false;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 3);
    check_record_properties_with_array_sizes("int[5][10]", "x", "variable", "array", {5, 10}, parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties_with_array_sizes("float[10][12][23]", "y", "variable", "array", {10, 12, 23}, parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[1]);
    check_record_properties_with_array_sizes("Util[1][2]", "util", "variable", "class array", {1, 2}, parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]);

    parser.global_symbol_table_->print(true);
}

TEST(arrayDeclareInClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { int x[5][10]; float y[10][12][23]; Util util[1][2]; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = false;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 3);
    check_record_properties_with_array_sizes("int[5][10]", "x", "variable", "array", {5, 10}, parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties_with_array_sizes("float[10][12][23]", "y", "variable", "array", {10, 12, 23}, parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[1]);
    check_record_properties_with_array_sizes("Util[1][2]", "util", "variable", "class array", {1, 2}, parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]);

    parser.global_symbol_table_->print(true);
}

TEST(arrayDeclareInClassFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Util { int classFunc() { int x[5][10]; float y[10][12][23]; Util util[1][2]; }; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = false;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
    check_record_properties_with_array_sizes("int[5][10]", "x", "variable", "array", {5, 10}, parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties_with_array_sizes("float[10][12][23]", "y", "variable", "array", {10, 12, 23}, parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[1]);
    check_record_properties_with_array_sizes("Util[1][2]", "util", "variable", "class array", {1, 2}, parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]);
}

TEST(arrayDeclareFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { }; int funcTest() { int x[5][10]; float y[10][12][23]; Util util[1][2]; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = false;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
    check_record_properties_with_array_sizes("int[5][10]", "x", "variable", "array", {5, 10}, parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[0]);
    check_record_properties_with_array_sizes("float[10][12][23]", "y", "variable", "array", {10, 12, 23}, parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[1]);
    check_record_properties_with_array_sizes("Util[1][2]", "util", "variable", "class array", {1, 2}, parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[2]);
}


TEST(FuncDeclareTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    string find_min_func_string = "int findMin(int array[100]) { int minValue; int maxValue; int idxc; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; ";
    tokens = scanner.generate_tokens("program { }; " + find_min_func_string, false);

    Parser parser;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_.size(), 5);
    check_record_properties("int", "findMin", "function", "simple", parser.global_symbol_table_->symbol_records_[1]);
    check_record_properties("int[100]", "array", "parameter", "array", parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[0]);
    check_record_properties("int", "minValue", "variable", "simple", parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[1]);
    check_record_properties("int", "maxValue", "variable", "simple", parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[2]);
    check_record_properties("int", "idxc", "variable", "simple", parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[3]);
    check_record_properties("int", "idx", "variable", "simple", parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[4]);
    parser.global_symbol_table_->print(true);
}

TEST(FuncDeclareClassTest, SemanticTests)
{
    string find_min_func_string = "int findMin(int array[100]) { int minValue; int maxValue; int idxc; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; ";
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { " + find_min_func_string + "}; program { };", false);

    Parser parser;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 5);
    check_record_properties("int", "findMin", "function", "simple", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties("int[100]", "array", "parameter", "array", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties("int", "minValue", "variable", "simple", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[1]);
    check_record_properties("int", "maxValue", "variable", "simple", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]);
    check_record_properties("int", "idxc", "variable", "simple", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[3]);
    check_record_properties("int", "idx", "variable", "simple", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[4]);
    parser.global_symbol_table_->print(true);
}


TEST(FuncDeclareClassWithDuplicateVariablesTest, SemanticTests)
{
    string find_min_func_string = "class Utility { int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; };";
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens( find_min_func_string + " program { };", false);

    Parser parser;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.syntax_errors.size(), 0);
    EXPECT_EQ(parser.semantic_errors_.size(), 3);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_.size(), 3);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[3]->symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}


TEST(FuncDeclareReturnClassWithDoublePassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { }; program { }; Cord create_cord() { Cord cord; return(cord); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_.size(), 1);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[2]->properly_declared_, true);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_[0]->properly_declared_, true);
    parser.global_symbol_table_->print(true);
}

TEST(FuncDeclareReturnClassWithoutDoublePassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { }; program { }; Cord create_cord() { Cord cord; return(cord); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = false;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_.size(), 1);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[2]->properly_declared_, false);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_[0]->properly_declared_, false);
    parser.global_symbol_table_->print(true);
}

TEST(FuncDeclareReturnNonDeclareClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { }; Cord create_cord() { Cord cord; return(cord); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 2);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_.size(), 1);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[0]->properly_declared_, false);
    parser.global_symbol_table_->print(true);
}

TEST(DeclareDuplicateIntClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int idx; int idx; }; program { };", false);

    Parser parser;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(DeclareDuplicateFloatClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { float idx; float idx; }; program { };", false);

    Parser parser;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(DeclareDuplicateIdClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { nameHere idx; nameHere idx; }; program { };", false);

    Parser parser;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(DeclareDuplicateIdProgramTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { }; program { nameHere idx; nameHere idx;  };", false);

    Parser parser;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(DeclareDuplicateIntProgramTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { }; program { int idx; int idx;  };", false);

    Parser parser;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(DeclareDuplicateFloatProgramTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { }; program { int idx; int idx;  };", false);

    Parser parser;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(DeclareDuplicateIdInFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere {  }; program { }; int funcTest() { nameHere idx; nameHere idx;};", false);

    Parser parser;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(DeclareDuplicateIntInFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere {  }; program { }; int funcTest() { int idx; int idx;};", false);

    Parser parser;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(DeclareDuplicateFloatInFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere {  }; program { }; int funcTest() { float idx; float idx;};", false);

    Parser parser;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

TEST(DeclareDuplicateClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int idx; }; class A { int idc; float k; };program { A a;};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    check_record_properties("", "A", "class", "class", parser.global_symbol_table_->symbol_records_[0]);
    parser.global_symbol_table_->print(true);
}

TEST(DeclareDuplicateFuncTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { }; int testFunc(int r, float p) { int x; int y;}; int testFunc() { }; int testFunc2() { int x; int y;};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_.size(), 2);
    check_record_properties("int", "testFunc", "function", "simple", parser.global_symbol_table_->symbol_records_[1]);
    parser.global_symbol_table_->print(true);
}

TEST(FuncDeclareMultiParamTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord {}; program { }; Cord create_cord(Cord cord, int x, float y, int z[10]) { return(cord); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_[0]->properly_declared_, true);
    check_record_properties("Cord", "cord", "parameter", "class", parser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_[0]);
    check_record_properties("int", "x", "parameter", "simple", parser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_[1]);
    check_record_properties("float", "y", "parameter", "simple", parser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_[2]);
    check_record_properties_with_array_sizes("int[10]", "z", "parameter", "array", {10} ,parser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_[3]);
    parser.global_symbol_table_->print(true);
}

TEST(FuncDeclareMultiParamWithUndeclaredClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { }; Cord create_cord(Cord cord, int x, float y, int z[10]) { return(cord); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 2);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[0]->properly_declared_, false);
    check_record_properties("Cord", "cord", "parameter", "class", parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[0]);
    check_record_properties("int", "x", "parameter", "simple", parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[1]);
    check_record_properties("float", "y", "parameter", "simple", parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[2]);
    check_record_properties_with_array_sizes("int[10]", "z", "parameter", "array", {10} ,parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[3]);
    parser.global_symbol_table_->print(true);
}


TEST(ClassFuncDeclareMultiParamTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { Cord create_cord(Cord cord, int x, float y, int z[10]) { return(cord); }; }; program { Cord cord; }; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 1);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->properly_declared_, true);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->properly_declared_, true);
    check_record_properties("Cord", "cord", "parameter", "class", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties("int", "x", "parameter", "simple", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[1]);
    check_record_properties("float", "y", "parameter", "simple", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]);
    check_record_properties_with_array_sizes("int[10]", "z", "parameter", "array", {10} ,parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[3]);
    parser.global_symbol_table_->print(true);
}

TEST(ClassFuncDeclareMultiParamUndeclaredClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { Cordz create_cord(Cordz cord, int x, float y, int z[10]) { return(cord); }; }; program { Cord cord; }; ", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 2);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 1);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->properly_declared_, false);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->properly_declared_, false);
    check_record_properties("Cordz", "cord", "parameter", "class", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties("int", "x", "parameter", "simple", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[1]);
    check_record_properties("float", "y", "parameter", "simple", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]);
    check_record_properties_with_array_sizes("int[10]", "z", "parameter", "array", {10} ,parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[3]);
    parser.global_symbol_table_->print(true);
}

TEST(ClassWithClassVarTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { float idx; float idx; }; program { };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 1);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


TEST(MultiClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int idx; }; class B { int idx; A a; }; class C { int idx; A a; B b; }; program { C c;  c.a.idx = 10;};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 4);
    check_record_properties("", "A", "class", "class", parser.global_symbol_table_->symbol_records_[0]);
    check_record_properties("", "B", "class", "class", parser.global_symbol_table_->symbol_records_[1]);
    check_record_properties("", "C", "class", "class", parser.global_symbol_table_->symbol_records_[2]);
    check_record_properties("int", "idx", "variable", "simple", parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]);
    check_record_properties("int", "idx", "variable", "simple", parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[0]);
    check_record_properties_and_declared("A", "a", "variable", "class", parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_[1]);
    check_record_properties("int", "idx", "variable", "simple", parser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_[0]);
    check_record_properties_and_declared("A", "a", "variable", "class", parser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_[1]);
    check_record_properties_and_declared("B", "b", "variable", "class", parser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_[2]);
    check_record_properties_and_declared("C", "c", "variable", "class", parser.global_symbol_table_->symbol_records_[3]->symbol_table_->symbol_records_[0]);
    parser.global_symbol_table_->print(true);
}

TEST(FullProgramTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Utility { int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idp; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idp; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; }; program { int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); }; float randomize() { float value; value = 100 * (2 + 3.0 / 7.0006); value = 1.05 + ((2.04 * 2.47) - 3.0) + 7.0006 ; return (value); };", false);

    Parser parser("der_out.txt", "built_full_program_symbol_table_out.txt", "syntax_error_output.txt", "built_full_program_semantic_error_output.txt");
    
    parser.enable_double_pass_parse_ = true;
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[3]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_.size(), 7);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_.size(), 1);

    parser.global_symbol_table_->print(true);
}

TEST(FullProgramFromFileTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("semantic_program.txt", true);

    Parser parser("der_out.txt", "built_semantic_program_symbol_table_out.txt", "syntax_error_output.txt", "built_semantic_program_error_output.txt");

    parser.enable_double_pass_parse_ = true;
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[3]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_.size(), 7);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_.size(), 1);

    EXPECT_TRUE(compare_files("built_semantic_program_symbol_table_out.txt", "semantic_program_symbol_table_out.txt"));
    parser.global_symbol_table_->print(true);
}


TEST(FullComplexProgramFromFileTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("semantic_program_complex.txt", true);

    Parser parser("der_out.txt", "built_semantic_program_complex_symbol_table_out.txt", "syntax_error_output.txt", "built_semantic_program_complex_error_output.txt");

    parser.enable_double_pass_parse_ = true;
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);

    EXPECT_TRUE(compare_files("built_semantic_program_complex_symbol_table_out.txt", "semantic_program_complex_symbol_table_out.txt"));
    parser.global_symbol_table_->print(true);
}

TEST(FullProgramFromFileWithErrorsTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("semantic_program_with_errors.txt", true);

    Parser parser("der_out.txt", "built_semantic_program_with_errors_symbol_table_out.txt", "syntax_error_output.txt", "built_semantic_program_with_errors_error_out.txt");

    parser.enable_double_pass_parse_ = true;
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 9);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 4);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_.size(), 3);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_.size(), 3);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_.size(), 8);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[2]->symbol_table_->symbol_records_.size(), 1);
    EXPECT_EQ(parser.global_symbol_table_->symbol_records_[3]->symbol_table_->symbol_records_.size(), 1);

    EXPECT_TRUE(compare_files("built_semantic_program_with_errors_symbol_table_out.txt", "semantic_program_with_errors_symbol_table_out.txt"));
    EXPECT_TRUE(compare_files("built_semantic_program_with_errors_error_out.txt", "semantic_program_with_errors_error_out.txt"));
    parser.global_symbol_table_->print(true);
}
