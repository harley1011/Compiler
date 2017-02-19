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
    tokens = scanner.generate_tokens("class nameHere { }; program { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser._errors.size(), 0);
}

TEST(MultiRelClassTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int value; int testFunc() { value = 5 < 5 < 5;  } }; program { };", false);

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
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "class id { } ; program { } ;");
    EXPECT_EQ(syntaxParser._errors.size(), 1);
}

TEST(VarDeclareClassTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int testIntVar; float testFloatVar; int testArrayIntVar[5]; nameHere idVar; }; program { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "class id { int id ; float id ; int id [ integer ] ; id id ; } ; program { } ;");
    EXPECT_EQ(syntaxParser._errors.size(), 0);
}

TEST(VarArrayDeclareClassTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int var1[4][5][7][8][9][1][0]; }; program { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; } ; program { } ;");
    EXPECT_EQ(syntaxParser._errors.size(), 0);
}
TEST(FuncDeclareClassTest, ParserTests)
{
    string find_min_func_string = "int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; ";
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { " + find_min_func_string + "}; program { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "class id { int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; program { } ;");
    EXPECT_EQ(syntaxParser._errors.size(), 0);
}

TEST(FullDeclareClassTest, ParserTests)
{
    string class_body = "int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; ";
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { " + class_body + "}; program { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id > integer ; id = id - integer ) { if ( id [ id ] > id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; program { } ;");
    EXPECT_EQ(syntaxParser._errors.size(), 0);

}


TEST(SimpleProgramTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { }; program { int i; int var1[4][5][7][8][9][1][0]; };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser._errors.size(), 0);

}

TEST(SmallProgramTest, ParserTests)
{
    string program_body = "int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); ";
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { }; program { " + program_body + " };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "class id { } ; program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ;");
    EXPECT_EQ(syntaxParser._errors.size(), 0);

}
TEST(ProgramAndFuncTest, ParserTests) {
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Utility { }; program { int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); }; float randomize() { float value; value = 100 * (2 + 3.0 / 7.0006); value = 1.05 + ((2.04 * 2.47) - 3.0) + 7.0006 ; return (value); };", false );

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "class id { } ; program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ; float id ( ) { float id ; id = integer * ( integer + float / float ) ; id = float + ( ( float * float ) - float ) + float ; return ( id ) ; } ;");
    EXPECT_EQ(syntaxParser._errors.size(), 0);

}

TEST(FullProgramTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Utility { int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; }; program { int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); }; float randomize() { float value; value = 100 * (2 + 3.0 / 7.0006); value = 1.05 + ((2.04 * 2.47) - 3.0) + 7.0006 ; return (value); };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id > integer ; id = id - integer ) { if ( id [ id ] > id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ; float id ( ) { float id ; id = integer * ( integer + float / float ) ; id = float + ( ( float * float ) - float ) + float ; return ( id ) ; } ;");
    EXPECT_EQ(syntaxParser._errors.size(), 0);
}
