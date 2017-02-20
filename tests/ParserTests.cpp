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
    EXPECT_EQ(syntaxParser._errors.size(), 0);
}

TEST(MultiRelClassTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int value; int testFunc() { value = 5 < 5 < 5;  } }; program { };", false);

    SyntaxParser syntaxParser;

    EXPECT_EQ(syntaxParser.parse(tokens), false);
    EXPECT_EQ(syntaxParser._errors.size(), 1);
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


TEST(NumIntTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("200", "<num>");
    EXPECT_TRUE(syntaxParser.num());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "integer");
}

TEST(NumFloatTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("200.12", "<num>");
    EXPECT_TRUE(syntaxParser.num());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "float");
}


TEST(MultOpMultiTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("*", "<multOp>");
    EXPECT_TRUE(syntaxParser.multOp());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "*");
}

TEST(MultOpDashTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("/", "<multOp>");
    EXPECT_TRUE(syntaxParser.multOp());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "/");
}

TEST(MultOpAndTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("and", "<multOp>");
    EXPECT_TRUE(syntaxParser.multOp());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "and");
}

TEST(AddOpaddTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("+", "<addOp>");
    EXPECT_TRUE(syntaxParser.addOp());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "+");
}


TEST(AddOpSubTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("-", "<addOp>");
    EXPECT_TRUE(syntaxParser.addOp());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "-");
}


TEST(AddOpOrTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("or", "<addOp>");
    EXPECT_TRUE(syntaxParser.addOp());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "or");
}

TEST(RelOpEquivTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("==", "<relOp>");
    EXPECT_TRUE(syntaxParser.relOp());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "==");
}

TEST(RelOpNotEqTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("<>", "<relOp>");
    EXPECT_TRUE(syntaxParser.relOp());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "<>");
}

TEST(RelOpLtEquivTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("<", "<relOp>");
    EXPECT_TRUE(syntaxParser.relOp());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "<");
}

TEST(RelOpGtTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(">", "<relOp>");
    EXPECT_TRUE(syntaxParser.relOp());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, ">");
}

TEST(RelOpLtEqTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("<=", "<relOp>");
    EXPECT_TRUE(syntaxParser.relOp());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "<=");
}

TEST(RelOpGtEqTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(">=", "<relOp>");
    EXPECT_TRUE(syntaxParser.relOp());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, ">=");
}

TEST(AssignOpTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("=", "<assignOp>");
    EXPECT_TRUE(syntaxParser.assignOp());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "=");
}


TEST(aParamsTailTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(", 5 )", "<aParamsTail>");
    EXPECT_TRUE(syntaxParser.aParamsTail());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, ", integer");
}

TEST(aParamsTailNoArgTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(" )", "<aParamsTail>");
    EXPECT_TRUE(syntaxParser.aParamsTail());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "");
}

TEST(aParamsTailMultiArgTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(", 5, 10.12, varTest[10] , 10 + 10 * 10 / (10 - 100) )", "<aParamsTail>");
    EXPECT_TRUE(syntaxParser.aParamsTail());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, ", integer , float , id [ integer ] , integer + integer * integer / ( integer - integer )");
}

TEST(fParamsTailMultiArgTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(", int vardecl, float varfdecl, int vararray[10] )", "<fParamsTail>");
    EXPECT_TRUE(syntaxParser.fParamsTail());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, ", int id , float id , int id [ integer ]");
}

TEST(fParamsTailNoArgTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(")", "<fParamsTail>");
    EXPECT_TRUE(syntaxParser.fParamsTail());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "");
}



TEST(aParamsMultiArgTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("(5 + 5) , 10.12, varTest[10] , 10 + 10 * 10 / (10 - 100) )", "<aParams>");
    EXPECT_TRUE(syntaxParser.aParams());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "( integer + integer ) , float , id [ integer ] , integer + integer * integer / ( integer - integer )");
}

TEST(aParamsNoArgTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(")", "<aParams>");
    EXPECT_TRUE(syntaxParser.aParams());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "");
}

TEST(fParamsMultiArgTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("int vardecl, float varfdecl, int vararray[10] )", "<fParams>");
    EXPECT_TRUE(syntaxParser.fParams());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "int id , float id , int id [ integer ]");
}


TEST(numTypeIntTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("int", "<numType>");
    EXPECT_TRUE(syntaxParser.numType());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "int");
}

TEST(numTypeFloatTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("float", "<numType>");
    EXPECT_TRUE(syntaxParser.numType());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "float");
}

TEST(typeIntTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("int", "<type>");
    EXPECT_TRUE(syntaxParser.type());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "int");
}

TEST(typeFloatTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("float", "<type>");
    EXPECT_TRUE(syntaxParser.type());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "float");
}

TEST(typeIdTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("id", "<type>");
    EXPECT_TRUE(syntaxParser.type());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id");
}


TEST(arraySizeTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("[ 5 ] [ 10 ];", "<arraySize>");
    EXPECT_TRUE(syntaxParser.arraySize());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "[ integer ] [ integer ]");
}

TEST(indiceMultiLstTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("[ 5 ] [ 10 - 10 ];", "<indiceLst>");
    EXPECT_TRUE(syntaxParser.indiceLst());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "[ integer ] [ integer - integer ]");
}

TEST(indiceLstTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("[ 5 ];", "<indiceLst>");
    EXPECT_TRUE(syntaxParser.indiceLst());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "[ integer ]");
}

TEST(indiceLstNestedFuncTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("[ id(15, 20, 30)] [10 ];", "<indiceLst>");
    EXPECT_TRUE(syntaxParser.indiceLst());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "[ id ( integer , integer , integer ) ] [ integer ]");
}

TEST(indiceFuncTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("[ id(15, 20, 30) ];", "<indice>");
    EXPECT_TRUE(syntaxParser.indice());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "[ id ( integer , integer , integer ) ]");
}

TEST(indiceMathExprTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("[ 20 / 20 + (20 * 10 ) - 10 ];", "<indice>");
    EXPECT_TRUE(syntaxParser.indice());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "[ integer / integer + ( integer * integer ) - integer ]");
}

TEST(idnestTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(". id =", "<idnest>");
    EXPECT_TRUE(syntaxParser.idnest());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, ". id");
}

TEST(idnestArrayTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(". id[5].id =", "<idnest>");
    EXPECT_TRUE(syntaxParser.idnest());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, ". id [ integer ] . id");
}

TEST(variableTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var1 . var2 =", "<variable>");
    EXPECT_TRUE(syntaxParser.variable());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id . id");
}

TEST(variableArrayAndNestedIdTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var1 [ 5 ] . var2 =", "<variable>");
    EXPECT_TRUE(syntaxParser.variable());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id [ integer ] . id");
}

TEST(varOrFuncIdNestFuncTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("( 10, 10.21, 32 + 40, var2)", "<varOrFuncIdNest>");
    EXPECT_TRUE(syntaxParser.varOrFuncIdNest());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "( integer , float , integer + integer , id )");
}

TEST(varOrFuncIdNestIdTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(". id [ 10 ] . id ( 20 , 123 , 10 + 11 , id )", "<varOrFuncIdNest>");
    EXPECT_TRUE(syntaxParser.varOrFuncIdNest());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, ". id [ integer ] . id ( integer , integer , integer + integer , id )");
}

TEST(factorIdTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var1 <", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id");
}

TEST(factorIdNestTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var1.var2 <", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id . id");
}

TEST(factorArrayIdNestTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var1[5][10] [ 20 + 20].var2 <", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id [ integer ] [ integer ] [ integer + integer ] . id");
}

TEST(factorNestedFuncTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var1[5][10][ 20 + 20].var2( 10, 10 - 20, var4.var6(10, 10) ) <", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id [ integer ] [ integer ] [ integer + integer ] . id ( integer , integer - integer , id . id ( integer , integer ) )");
}

TEST(factorNestedInvalidFuncTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var1[5](10) <", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser._errors.size(), 2);
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id [ integer ]");
}
TEST(factorIntegerTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("200", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "integer");
}
TEST(factorFloatTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("200.12", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "float");
}

TEST(factorArithExprTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("( 20 * 20 - 10 / 10 + var2 (10, 10) )", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "( integer * integer - integer / integer + id ( integer , integer ) )");
}

TEST(factorNotTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("not var2.var1 <", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "not id . id");
}

TEST(factorSignPlusTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("+var2.var1 <", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "+ id . id");
}
TEST(factorSignNegTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("-var2.var1 <", "<factor>");
    EXPECT_TRUE(syntaxParser.factor());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "- id . id");
}

TEST(termMultTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 * 10.21;", "<term>");
    EXPECT_TRUE(syntaxParser.term());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id * float");
}

TEST(termDashTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 / 10.21;", "<term>");
    EXPECT_TRUE(syntaxParser.term());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id / float");
}

TEST(termAndTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 and 1;", "<term>");
    EXPECT_TRUE(syntaxParser.term());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id and integer");
}

TEST(termMultipleTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 * 10.21 * ( 10 + 10 ) / var5(1, 3) * id.id[10];", "<term>");
    EXPECT_TRUE(syntaxParser.term());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id * float * ( integer + integer ) / id ( integer , integer ) * id . id [ integer ]");
}


TEST(arithExprAddTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 + 10.21;", "<arithExpr>");
    EXPECT_TRUE(syntaxParser.arithExpr());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id + float");
}


TEST(arithExprSubTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 - 10.21;", "<arithExpr>");
    EXPECT_TRUE(syntaxParser.arithExpr());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id - float");
}

TEST(arithExprOrTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 or 10.21;", "<arithExpr>");
    EXPECT_TRUE(syntaxParser.arithExpr());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id or float");
}

TEST(arithExprAddOrSubTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 * 10.21 * ( 10 + 10 ) / var5(1, 3) * id.id[10];", "<arithExpr>");
    EXPECT_TRUE(syntaxParser.arithExpr());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id * float * ( integer + integer ) / id ( integer , integer ) * id . id [ integer ]");
}


TEST(arithExprMultiTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 + 10.21 * ( 10 + 10 ) / var5(1, 3) - id.id[10];", "<arithExpr>");
    EXPECT_TRUE(syntaxParser.arithExpr());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id + float * ( integer + integer ) / id ( integer , integer ) - id . id [ integer ]");
}



TEST(exprNotEqTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("2 <> 5;", "<expr>");
    EXPECT_TRUE(syntaxParser.expr());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "integer <> integer");
}

TEST(exprEqTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2 == var3.var1;", "<expr>");
    EXPECT_TRUE(syntaxParser.expr());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id == id . id");
}

TEST(exprLtTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("var2[5].var5(10, 10) < var3.var1;", "<expr>");
    EXPECT_TRUE(syntaxParser.expr());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "id [ integer ] . id ( integer , integer ) < id . id");
}


TEST(exprGtTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(" 10 * var2 > 11 * 2;", "<expr>");
    EXPECT_TRUE(syntaxParser.expr());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "integer * id > integer * integer");
}


TEST(exprLtEqTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(" 10 * var2[5] <= 11 * 2;", "<expr>");
    EXPECT_TRUE(syntaxParser.expr());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "integer * id [ integer ] <= integer * integer");
}


TEST(exprGtEqTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup(" 10 * (10 + var2(10, 10)) - 10 >= (var3[10].var4 - 10) * 2;", "<expr>");
    EXPECT_TRUE(syntaxParser.expr());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "integer * ( integer + id ( integer , integer ) ) - integer >= ( id [ integer ] . id - integer ) * integer");
}

TEST(statBlockSingleLineIfTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("{ if ( 10 < 20 ) then var7 = 10; else var9 = var6(10, 10); };", "<statBlock>");
        EXPECT_TRUE(syntaxParser.statBlock());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "{ if ( integer < integer ) then id = integer ; else id = id ( integer , integer ) ; } ;");
}

TEST(statBlockMultiLineIfTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("{ if ( 10 <  var10(10, 10) ) then {var7 = 10; var10 = 10 * 20 / 20; }else { var9 = var6(10, 10); }; };", "<statBlock>");
    EXPECT_TRUE(syntaxParser.statBlock());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "{ if ( integer < id ( integer , integer ) ) then { id = integer ; id = integer * integer / integer ; } else { id = id ( integer , integer ) ; } ; } ;");
}


TEST(statBlockMultiLineAndSingleLineIfTest, ParserTests) {
    SyntaxParser syntaxParser = common_setup("{ if ( 10 <  var10(10, 10) ) then var2 = var3; else { var9 = var6(10, 10); }; };", "<statBlock>");
    EXPECT_TRUE(syntaxParser.statBlock());
    EXPECT_EQ(syntaxParser._current_rhs_derivation, "{ if ( integer < id ( integer , integer ) ) then id = id ; else { id = id ( integer , integer ) ; } ; } ;");
}

SyntaxParser common_setup(string test_program, string derivation_string) {
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens(test_program, false);

    SyntaxParser syntaxParser(tokens);
    syntaxParser._current_rhs_derivation = derivation_string;
    return syntaxParser;
}