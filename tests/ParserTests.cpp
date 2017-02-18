//
// Created by Harley Mcphee on 2017-02-06.
//

#include "ParserTests.h"
#include "gtest/gtest.h"
#include "../src/SyntaxParser/SyntaxParser.h"

string find_min_func_string = "int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; ";
TEST(SimpleClassTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { }; program { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser._errors.size(), 0);
}
TEST(SimpleClassTestError, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("int class nameHere { }; program { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser._errors.size(), 1);
}

TEST(VarDeclareClassTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int testIntVar; float testFloatVar; int testArrayIntVar[5]; nameHere idVar; }; program { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser._errors.size(), 0);
}
TEST(FuncDeclareClassTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { " + find_min_func_string + "}; program { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser._errors.size(), 0);
}