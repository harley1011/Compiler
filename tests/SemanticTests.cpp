//
// Created by Harley on 3/9/2017.
//

#include "SemanticTests.h"
#include "gtest/gtest.h"
#include "../src/SemanticParser/SemanticParser.h"
SemanticParser common_setup_semantic(string test_program, string derivation_string);

TEST(arraySizeTest, SemanticTests) {
    SymbolRecord* record = new SymbolRecord();
    SemanticParser syntaxParser = common_setup_semantic("[ 5 ] [ 10 ];", "<arraySize>");
    EXPECT_TRUE(syntaxParser.arraySize(record));
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "[ integer ] [ integer ]");
    EXPECT_EQ(record->type_, "[5][10]");
}

TEST(FuncDeclareClassTest, SemanticTests)
{
    string find_min_func_string = "int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; ";
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { " + find_min_func_string + "}; program { };", false);

    SemanticParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "class id { int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; program { } ;");
    EXPECT_EQ(syntaxParser.errors_.size(), 0);

    syntaxParser.global_symbol_table_.print();
}



SemanticParser common_setup_semantic(string test_program, string derivation_string) {
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens(test_program, false);

    SemanticParser semanticParser(tokens);
    semanticParser.current_rhs_derivation_ = derivation_string;
    return semanticParser;
}
