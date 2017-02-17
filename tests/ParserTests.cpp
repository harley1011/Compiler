//
// Created by Harley Mcphee on 2017-02-06.
//

#include "ParserTests.h"
#include "gtest/gtest.h"
#include "../src/SyntaxParser/SyntaxParser.h"


TEST(SimpleClassTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class id { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
}

