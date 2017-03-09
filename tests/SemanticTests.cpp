//
// Created by Harley on 3/9/2017.
//

#include "SemanticTests.h"
#include "gtest/gtest.h"
#include "../src/SemanticParser/SemanticParser.h"
SemanticParser common_setup_semantic(string test_program, string derivation_string);

TEST(arraySizeTest, SemanticTests) {
    SymbolRecord record;
    SemanticParser syntaxParser = common_setup_semantic("[ 5 ] [ 10 ];", "<arraySize>");
    EXPECT_TRUE(syntaxParser.arraySize(record));
  //  EXPECT_EQ(syntaxParser.current_rhs_derivation_, "[ integer ] [ integer ]");
}

SemanticParser common_setup_semantic(string test_program, string derivation_string) {
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens(test_program, false);

    SemanticParser semanticParser(tokens);
    semanticParser.current_rhs_derivation_ = derivation_string;
    return semanticParser;
}
