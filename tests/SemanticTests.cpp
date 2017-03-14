//
// Created by Harley on 3/9/2017.
//

#include "SemanticTests.h"
#include "gtest/gtest.h"
#include "../src/SemanticParser/SemanticParser.h"
SemanticParser common_setup_semantic(string test_program, string derivation_string);
bool check_if_record_exists_in_table(SymbolRecord record, SymbolTable symbol);

TEST(arraySizeTest, SemanticTests) {
    SymbolRecord* record = new SymbolRecord();
    SemanticParser syntaxParser = common_setup_semantic("[ 5 ] [ 10 ];", "<arraySize>");
    EXPECT_TRUE(syntaxParser.arraySize(record));
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "[ integer ] [ integer ]");
    EXPECT_EQ(record->type_, "[5][10]");
    EXPECT_EQ(record->array_sizes[0], 5);
    EXPECT_EQ(record->array_sizes[1], 10);
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
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 4);
    syntaxParser.global_symbol_table_->print();
}

TEST(FuncDeclareReturnClassTest, SemanticTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int x; int y; int z; }; program { }; Cord create_cord(int x, int y, int z) { Cord cord; cord.x = x; cord.y = y; cord.z = z; return(cord); };", false);

    SemanticParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.semantic_errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 4);
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

TEST(FuncDeclareClassTest, SemanticTests)
{
    string find_min_func_string = "int findMin(int array[100]) { int minValue; int idxc; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; ";
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { " + find_min_func_string + "}; program { };", false);

    SemanticParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "class id { int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; program { } ;");
    EXPECT_EQ(syntaxParser.errors_.size(), 0);

    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_.size(), 2);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 1);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 4);
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


TEST(progTest, SemanticTests) {
    SemanticParser syntaxParser = common_setup_semantic("program {int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); };", "<progBody>");
    EXPECT_TRUE(syntaxParser.progBody());
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 7);
    syntaxParser.global_symbol_table_->print();
}

TEST(progWithClassTest, SemanticTests) {
    SemanticParser syntaxParser = common_setup_semantic("program { Utility utility; };", "<progBody>");
    EXPECT_TRUE(syntaxParser.progBody());
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_.size(), 1);
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[0]->symbol_table_->symbol_records_[0]->name_, "utility");
    syntaxParser.global_symbol_table_->print();
}

TEST(funcTest, SemanticTests) {
    SemanticParser syntaxParser = common_setup_semantic("program { }; int findMin(int x) { int r; int y; float p;  Utility utility;};", "<progBody>");
    EXPECT_TRUE(syntaxParser.progBody());
    EXPECT_EQ(syntaxParser.global_symbol_table_->symbol_records_[1]->symbol_table_->symbol_records_.size(), 5);
    syntaxParser.global_symbol_table_->print();
}

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
