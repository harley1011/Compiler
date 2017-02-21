//
// Created by Harley Mcphee on 2017-02-06.
//

#include "ParserTests.h"
#include "gtest/gtest.h"
#include "../src/SyntaxParser/SyntaxParser.h"

SyntaxParser common_setup(string test_program, string derivation_string);

TEST(SimpleClassTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { }; program { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.errors_.size(), 0);
}

TEST(MultiRelClassErrorTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int value; int testFunc() { value = 5 < 5 < 5;  } }; program { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), false);
    EXPECT_EQ(syntaxParser.errors_.size(), 1);
}

TEST(SimpleClassTestError, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("int class nameHere { }; program { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "class id { } ; program { } ;");
    EXPECT_EQ(syntaxParser.errors_.size(), 1);

}


TEST(errorOneTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("class nameHere { int int i = 10; }; program { };", "<classDeclLst> <progBody>");
    EXPECT_TRUE(!syntaxParser.prog());
    EXPECT_EQ(syntaxParser.errors_.size(), 1);
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "class id { int id <postTypeId> ; <classBody> } ; <classDeclLst> <progBody>");
}
TEST(errorTwoTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("class nameHere { int vardecl( int var1,, int var2 ) { }; }; program { };", "<classDeclLst> <progBody>");
    EXPECT_TRUE(syntaxParser.prog());
    EXPECT_EQ(syntaxParser.errors_.size(), 1);
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "class id { int id ( int id , int id ) { } ; } ; program { } ;");
}


TEST(errorThreeTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("class nameHere { int vardecl }; program { };", "<classDeclLst> <progBody>");
    EXPECT_TRUE(!syntaxParser.prog());
    EXPECT_EQ(syntaxParser.errors_.size(), 4);
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "class id { int id ; } ; <classDeclLst> <progBody>");
}
TEST(VarDeclareClassTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int testIntVar; float testFloatVar; int testArrayIntVar[5]; nameHere idVar; }; program { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "class id { int id ; float id ; int id [ integer ] ; id id ; } ; program { } ;");
    EXPECT_EQ(syntaxParser.errors_.size(), 0);
}

TEST(VarArrayDeclareClassTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int var1[4][5][7][8][9][1][0]; }; program { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; } ; program { } ;");
    EXPECT_EQ(syntaxParser.errors_.size(), 0);
}
TEST(FuncDeclareClassTest, ParserTests)
{
    string find_min_func_string = "int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; ";
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { " + find_min_func_string + "}; program { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "class id { int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; program { } ;");
    EXPECT_EQ(syntaxParser.errors_.size(), 0);
}

TEST(FullDeclareClassTest, ParserTests)
{
    string class_body = "int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; ";
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { " + class_body + "}; program { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id > integer ; id = id - integer ) { if ( id [ id ] > id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; program { } ;");
    EXPECT_EQ(syntaxParser.errors_.size(), 0);

}


TEST(SimpleProgramTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { }; program { int i; int var1[4][5][7][8][9][1][0]; };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.errors_.size(), 0);

}

TEST(SmallProgramTest, ParserTests)
{
    string program_body = "int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); ";
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { }; program { " + program_body + " };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "class id { } ; program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ;");
    EXPECT_EQ(syntaxParser.errors_.size(), 0);

}
TEST(ProgramAndFuncTest, ParserTests) {
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Utility { }; program { int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); }; float randomize() { float value; value = 100 * (2 + 3.0 / 7.0006); value = 1.05 + ((2.04 * 2.47) - 3.0) + 7.0006 ; return (value); };", false );

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "class id { } ; program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ; float id ( ) { float id ; id = integer * ( integer + float / float ) ; id = float + ( ( float * float ) - float ) + float ; return ( id ) ; } ;");
    EXPECT_EQ(syntaxParser.errors_.size(), 0);

}

TEST(FullProgramTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Utility { int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; }; program { int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); }; float randomize() { float value; value = 100 * (2 + 3.0 / 7.0006); value = 1.05 + ((2.04 * 2.47) - 3.0) + 7.0006 ; return (value); };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id > integer ; id = id - integer ) { if ( id [ id ] > id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ; float id ( ) { float id ; id = integer * ( integer + float / float ) ; id = float + ( ( float * float ) - float ) + float ; return ( id ) ; } ;");
    EXPECT_EQ(syntaxParser.errors_.size(), 0);
}


TEST(LargeFullProgramTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Utility { int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; };  class UtilityTwo { int var1[4][5][7][8][9][1]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; int var1[4][5][7][8][9][1][0]; float var2; return (maxValue); }; int findMin(int array[100], float test) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; }; program { int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); }; float randomize() { float value; value = 100 * (2 + 3.0 / 7.0006); value = 1.05 + ((2.04 * 2.47) - 3.0) + 7.0006 ; return (value); };  id getEven(int var10, int array2[10000]) { float value; value = 100 * (2 + 3.0 / 7.0006); value = 1.05 + ((2.04 * 2.47) - 3.0) + 7.0006 ; return (value); };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), true);
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id > integer ; id = id - integer ) { if ( id [ id ] > id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id > integer ; id = id - integer ) { if ( id [ id ] > id ) then { id = id [ id ] ; } else { } ; } ; int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; return ( id ) ; } ; int id ( int id [ integer ] , float id ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ; float id ( ) { float id ; id = integer * ( integer + float / float ) ; id = float + ( ( float * float ) - float ) + float ; return ( id ) ; } ; id id ( int id , int id [ integer ] ) { float id ; id = integer * ( integer + float / float ) ; id = float + ( ( float * float ) - float ) + float ; return ( id ) ; } ;");
    EXPECT_EQ(syntaxParser.errors_.size(), 0);
}
TEST(NumIntTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("200", "<num>");
    EXPECT_TRUE(syntaxParser.num());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "integer");
}

TEST(NumFloatTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("200.12", "<num>");
    EXPECT_TRUE(syntaxParser.num());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "float");
}


TEST(MultOpMultiTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("*", "<multOp>");
    EXPECT_TRUE(syntaxParser.multOp());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "*");
}

TEST(MultOpDashTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("/", "<multOp>");
    EXPECT_TRUE(syntaxParser.multOp());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "/");
}

TEST(MultOpAndTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("and", "<multOp>");
    EXPECT_TRUE(syntaxParser.multOp());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "and");
}

TEST(AddOpaddTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("+", "<addOp>");
    EXPECT_TRUE(syntaxParser.addOp());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "+");
}


TEST(AddOpSubTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("-", "<addOp>");
    EXPECT_TRUE(syntaxParser.addOp());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "-");
}


TEST(AddOpOrTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("or", "<addOp>");
    EXPECT_TRUE(syntaxParser.addOp());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "or");
}

TEST(RelOpEquivTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("==", "<relOp>");
    EXPECT_TRUE(syntaxParser.relOp());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "==");
}

TEST(RelOpNotEqTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("<>", "<relOp>");
    EXPECT_TRUE(syntaxParser.relOp());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "<>");
}

TEST(RelOpLtEquivTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("<", "<relOp>");
    EXPECT_TRUE(syntaxParser.relOp());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "<");
}

TEST(RelOpGtTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(">", "<relOp>");
    EXPECT_TRUE(syntaxParser.relOp());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, ">");
}

TEST(RelOpLtEqTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("<=", "<relOp>");
    EXPECT_TRUE(syntaxParser.relOp());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "<=");
}

TEST(RelOpGtEqTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(">=", "<relOp>");
    EXPECT_TRUE(syntaxParser.relOp());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, ">=");
}

TEST(AssignOpTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("=", "<assignOp>");
    EXPECT_TRUE(syntaxParser.assignOp());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "=");
}


TEST(aParamsTailTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(", 5 )", "<aParamsTail>");
    EXPECT_TRUE(syntaxParser.aParamsTail());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, ", integer");
}

TEST(aParamsTailNoArgTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(" )", "<aParamsTail>");
    EXPECT_TRUE(syntaxParser.aParamsTail());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "");
}

TEST(aParamsTailMultiArgTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(", 5, 10.12, varTest[10] , 10 + 10 * 10 / (10 - 100) )", "<aParamsTail>");
    EXPECT_TRUE(syntaxParser.aParamsTail());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, ", integer , float , id [ integer ] , integer + integer * integer / ( integer - integer )");
}

TEST(fParamsTailMultiArgTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(", int vardecl, float varfdecl, int vararray[10] )", "<fParamsTail>");
    EXPECT_TRUE(syntaxParser.fParamsTail());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, ", int id , float id , int id [ integer ]");
}

TEST(fParamsTailNoArgTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(")", "<fParamsTail>");
    EXPECT_TRUE(syntaxParser.fParamsTail());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "");
}



TEST(aParamsMultiArgTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("(5 + 5) , 10.12, varTest[10] , 10 + 10 * 10 / (10 - 100) )", "<aParams>");
    EXPECT_TRUE(syntaxParser.aParams());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "( integer + integer ) , float , id [ integer ] , integer + integer * integer / ( integer - integer )");
}

TEST(aParamsNoArgTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(")", "<aParams>");
    EXPECT_TRUE(syntaxParser.aParams());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "");
}

TEST(fParamsMultiArgTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("int vardecl, float varfdecl, int vararray[10] )", "<fParams>");
    EXPECT_TRUE(syntaxParser.fParams());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "int id , float id , int id [ integer ]");
}


TEST(numTypeIntTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("int", "<numType>");
    EXPECT_TRUE(syntaxParser.numType());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "int");
}

TEST(numTypeFloatTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("float", "<numType>");
    EXPECT_TRUE(syntaxParser.numType());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "float");
}

TEST(typeIntTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("int", "<type>");
    EXPECT_TRUE(syntaxParser.type());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "int");
}

TEST(typeFloatTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("float", "<type>");
    EXPECT_TRUE(syntaxParser.type());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "float");
}

TEST(typeIdTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("id", "<type>");
    EXPECT_TRUE(syntaxParser.type());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id");
}


TEST(arraySizeTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("[ 5 ] [ 10 ];", "<arraySize>");
    EXPECT_TRUE(syntaxParser.arraySize());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "[ integer ] [ integer ]");
}

TEST(indiceMultiLstTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("[ 5 ] [ 10 - 10 ];", "<indiceLst>");
    EXPECT_TRUE(syntaxParser.indiceLst());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "[ integer ] [ integer - integer ]");
}

TEST(indiceLstTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("[ 5 ];", "<indiceLst>");
    EXPECT_TRUE(syntaxParser.indiceLst());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "[ integer ]");
}

TEST(indiceLstNestedFuncTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("[ id(15, 20, 30)] [10 ];", "<indiceLst>");
    EXPECT_TRUE(syntaxParser.indiceLst());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "[ id ( integer , integer , integer ) ] [ integer ]");
}

TEST(indiceFuncTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("[ id(15, 20, 30) ];", "<indice>");
    EXPECT_TRUE(syntaxParser.indice());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "[ id ( integer , integer , integer ) ]");
}

TEST(indiceMathExprTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("[ 20 / 20 + (20 * 10 ) - 10 ];", "<indice>");
    EXPECT_TRUE(syntaxParser.indice());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "[ integer / integer + ( integer * integer ) - integer ]");
}

TEST(idnestTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(". id =", "<idnest>");
    EXPECT_TRUE(syntaxParser.idnest());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, ". id");
}

TEST(idnestArrayTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(". id[5].id =", "<idnest>");
    EXPECT_TRUE(syntaxParser.idnest());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, ". id [ integer ] . id");
}

TEST(variableTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var1 . var2 =", "<variable>");
    EXPECT_TRUE(syntaxParser.variable());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id . id");
}

TEST(variableArrayAndNestedIdTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var1 [ 5 ] . var2 =", "<variable>");
    EXPECT_TRUE(syntaxParser.variable());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id [ integer ] . id");
}

TEST(varOrFuncIdNestFuncTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("( 10, 10.21, 32 + 40, var2)", "<varOrFuncIdNest>");
    EXPECT_TRUE(syntaxParser.varOrFuncIdNest());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "( integer , float , integer + integer , id )");
}

TEST(varOrFuncIdNestIdTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(". id [ 10 ] . id ( 20 , 123 , 10 + 11 , id )", "<varOrFuncIdNest>");
    EXPECT_TRUE(syntaxParser.varOrFuncIdNest());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, ". id [ integer ] . id ( integer , integer , integer + integer , id )");
}

TEST(factorIdTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var1 <", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id");
}

TEST(factorIdNestTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var1.var2 <", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id . id");
}

TEST(factorArrayIdNestTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var1[5][10] [ 20 + 20].var2 <", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id [ integer ] [ integer ] [ integer + integer ] . id");
}

TEST(factorNestedFuncTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var1[5][10][ 20 + 20].var2( 10, 10 - 20, var4.var6(10, 10) ) <", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id [ integer ] [ integer ] [ integer + integer ] . id ( integer , integer - integer , id . id ( integer , integer ) )");
}

TEST(factorNestedInvalidFuncTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var1[5](10) <", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser.errors_.size(), 2);
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id [ integer ]");
}
TEST(factorIntegerTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("200", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "integer");
}
TEST(factorFloatTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("200.12", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "float");
}

TEST(factorArithExprTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("( 20 * 20 - 10 / 10 + var2 (10, 10) )", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "( integer * integer - integer / integer + id ( integer , integer ) )");
}

TEST(factorNotTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("not var2.var1 <", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "not id . id");
}

TEST(factorSignPlusTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("+var2.var1 <", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "+ id . id");
}
TEST(factorSignNegTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("-var2.var1 <", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "- id . id");
}

TEST(termMultTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 * 10.21;", "<term>");
    EXPECT_TRUE(syntaxParser.term());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id * float");
}

TEST(termDashTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 / 10.21;", "<term>");
    EXPECT_TRUE(syntaxParser.term());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id / float");
}

TEST(termAndTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 and 1;", "<term>");
    EXPECT_TRUE(syntaxParser.term());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id and integer");
}

TEST(termMultipleTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 * 10.21 * ( 10 + 10 ) / var5(1, 3) * id.id[10];", "<term>");
    EXPECT_TRUE(syntaxParser.term());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id * float * ( integer + integer ) / id ( integer , integer ) * id . id [ integer ]");
}


TEST(arithExprAddTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 + 10.21;", "<arithExpr>");
    EXPECT_TRUE(syntaxParser.arithExpr());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id + float");
}


TEST(arithExprSubTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 - 10.21;", "<arithExpr>");
    EXPECT_TRUE(syntaxParser.arithExpr());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id - float");
}

TEST(arithExprOrTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 or 10.21;", "<arithExpr>");
    EXPECT_TRUE(syntaxParser.arithExpr());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id or float");
}

TEST(arithExprAddOrSubTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 * 10.21 * ( 10 + 10 ) / var5(1, 3) * id.id[10];", "<arithExpr>");
    EXPECT_TRUE(syntaxParser.arithExpr());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id * float * ( integer + integer ) / id ( integer , integer ) * id . id [ integer ]");
}


TEST(arithExprMultiTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 + 10.21 * ( 10 + 10 ) / var5(1, 3) - id.id[10];", "<arithExpr>");
    EXPECT_TRUE(syntaxParser.arithExpr());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id + float * ( integer + integer ) / id ( integer , integer ) - id . id [ integer ]");
}



TEST(exprNotEqTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("2 <> 5;", "<expr>");
    EXPECT_TRUE(syntaxParser.expr());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "integer <> integer");
}

TEST(exprEqTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 == var3.var1;", "<expr>");
    EXPECT_TRUE(syntaxParser.expr());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id == id . id");
}

TEST(exprLtTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2[5].var5(10, 10) < var3.var1;", "<expr>");
    EXPECT_TRUE(syntaxParser.expr());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id [ integer ] . id ( integer , integer ) < id . id");
}


TEST(exprGtTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(" 10 * var2 > 11 * 2;", "<expr>");
    EXPECT_TRUE(syntaxParser.expr());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "integer * id > integer * integer");
}


TEST(exprLtEqTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(" 10 * var2[5] <= 11 * 2;", "<expr>");
    EXPECT_TRUE(syntaxParser.expr());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "integer * id [ integer ] <= integer * integer");
}


TEST(exprGtEqTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(" 10 * (10 + var2(10, 10)) - 10 >= (var3[10].var4 - 10) * 2;", "<expr>");
    EXPECT_TRUE(syntaxParser.expr());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "integer * ( integer + id ( integer , integer ) ) - integer >= ( id [ integer ] . id - integer ) * integer");
}

TEST(statBlockSingleLineIfTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("{ if ( 10 < 20 ) then var7 = 10; else var9 = var6(10, 10); };", "<statBlock>");
        EXPECT_TRUE(syntaxParser.statBlock());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "{ if ( integer < integer ) then id = integer ; else id = id ( integer , integer ) ; } ;");
}

TEST(statBlockMultiLineIfTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("{ if ( 10 <  var10(10, 10) ) then {var7 = 10; var10 = 10 * 20 / 20; }else { var9 = var6(10, 10); }; };", "<statBlock>");
    EXPECT_TRUE(syntaxParser.statBlock());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "{ if ( integer < id ( integer , integer ) ) then { id = integer ; id = integer * integer / integer ; } else { id = id ( integer , integer ) ; } ; } ;");
}
TEST(statBlockMultiLineEmptyElseIfTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("{ if ( 10 <  var10(10, 10) ) then {var7 = 10; var10 = 10 * 20 / 20; }else { }; };", "<statBlock>");
    EXPECT_TRUE(syntaxParser.statBlock());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "{ if ( integer < id ( integer , integer ) ) then { id = integer ; id = integer * integer / integer ; } else { } ; } ;");
}


TEST(statBlockMultiLineAndSingleLineIfTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("{ if ( 10 <  var10(10, 10) ) then var2 = var3; else { var9 = var6(10, 10); }; };", "<statBlock>");
    EXPECT_TRUE(syntaxParser.statBlock());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "{ if ( integer < id ( integer , integer ) ) then id = id ; else { id = id ( integer , integer ) ; } ; } ;");
}


TEST(statBlockSingleLineForTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("{ for(int i = 0; i < 12; i = i + 1 ) count = 18 * 19; };", "<statBlock>");
    EXPECT_TRUE(syntaxParser.statBlock());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "{ for ( int id = integer ; id < integer ; id = id + integer ) id = integer * integer ; } ;");
}

TEST(statBlockMultiLineForTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("{ for(int i = 0; i < 12; i = i + 1 ) { count = 18 * 19; count1 = func(10, 20, 10); }; };", "<statBlock>");
    EXPECT_TRUE(syntaxParser.statBlock());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "{ for ( int id = integer ; id < integer ; id = id + integer ) { id = integer * integer ; id = id ( integer , integer , integer ) ; } ; } ;");

}


TEST(statBlockGetTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("{ get ( var2); };", "<statBlock>");
    EXPECT_TRUE(syntaxParser.statBlock());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "{ get ( id ) ; } ;");
}

TEST(statBlockPutTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("{ put(var2  + 10); };", "<statBlock>");
    EXPECT_TRUE(syntaxParser.statBlock());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "{ put ( id + integer ) ; } ;");
}


TEST(statBlockReturnTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("{ return(var2  + 10); };", "<statBlock>");
    EXPECT_TRUE(syntaxParser.statBlock());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "{ return ( id + integer ) ; } ;");
}

TEST(funcBodyTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(
            "{ var1 = 10.21 * 10 / 10 - 12; for (int i = 0; i < 10; i = i + 1) { if ( var2 > 10 ) then var1 = var1 + 10; else var3 = var3 = + 10; }; return(var2  + 10); };",
            "<funcBody>");
    EXPECT_TRUE(syntaxParser.funcBody());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_,
              "{ id = float * integer / integer - integer ; for ( int id = integer ; id < integer ; id = id + integer ) { if ( id > integer ) then id = id + integer ; else id = id + integer ; } ; return ( id + integer ) ; }");
}

TEST(funcDefTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("int varFunc(int a, int b[5], float c){{ var1 = 10.21 * 10 / 10 - 12; for (int i = 0; i < 10; i = i + 1) { if ( var2 > 10 ) then var1 = var1 + 10; else var3 = var3 = + 10; }; return(var2  + 10); }; };", "<funcDef>");
    EXPECT_TRUE(syntaxParser.funcDef());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "int id ( int id , int id [ integer ] , float id ) { id = float * integer / integer - integer ; for ( int id = integer ; id < integer ; id = id + integer ) { if ( id > integer ) then id = id + integer ; else id = id + integer ; } ; return ( id + integer ) ; } ;");
}
TEST(funcHeadTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("int varFunc(int a, int b[5], float c)", "<funcHead>");
    EXPECT_TRUE(syntaxParser.funcHead());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "int id ( int id , int id [ integer ] , float id )");
}

TEST(funcHeadEmptyTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var varFunc()", "<funcHead>");
    EXPECT_TRUE(syntaxParser.funcHead());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id id ( )");
}

TEST(classBodyTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var varFunc() { int var2; var2 = 2; }; int var4; int var7; int var5Func(int a) { }; }", "<classBody>");
    EXPECT_TRUE(syntaxParser.classBody());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "id id ( ) { int id ; id = integer ; } ; int id ; int id ; int id ( int id ) { } ;");
}

TEST(classBodyEmptyTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("}", "<classBody>");
    EXPECT_TRUE(syntaxParser.classBody());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "");
}


TEST(classDeclTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("class className { var varFunc() { int var2; var2 = 2; }; int var4; int var7; int var5Func(int a) { }; };", "<classDecl>");
    EXPECT_TRUE(syntaxParser.classDecl());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "class id { id id ( ) { int id ; id = integer ; } ; int id ; int id ; int id ( int id ) { } ; } ;");
}

TEST(classDeclLargeTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("class className { int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; };", "<classDecl>");
    EXPECT_TRUE(syntaxParser.classDecl());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id > integer ; id = id - integer ) { if ( id [ id ] > id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ;");
}


TEST(progTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("program {int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); };", "<progBody>");
    EXPECT_TRUE(syntaxParser.progBody());
    EXPECT_EQ(syntaxParser.current_rhs_derivation_, "program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ;");
}


SyntaxParser common_setup(string test_program, string derivation_string) {
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens(test_program, false);

    SyntaxParser syntaxParser(tokens);
    syntaxParser.current_rhs_derivation_ = derivation_string;
    return syntaxParser;
}