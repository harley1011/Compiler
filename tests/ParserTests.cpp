//
// Created by Harley Mcphee on 2017-02-06.
//

#include "ParserTests.h"
#include "gtest/gtest.h"
#include "../src/Parser/Parser.h"

Parser common_setup(string test_program, string derivation_string);
bool compare_files(string first_path, string second_path);

TEST(FileTest, ParserTests) {
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("given_program.txt", true);

    Parser SemanticParser("built_given_program_parser_deriv_out.txt", "built_syntax_error_report_out.txt");
    EXPECT_EQ(SemanticParser.parse(tokens), true);
    EXPECT_TRUE(compare_files("given_program_parser_deriv_out.txt", "built_given_program_parser_deriv_out.txt"));
}
TEST(LargeFileTest, ParserTests) {
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("large_program_syntax.txt", true);

    Parser SemanticParser("built_large_program_parser_deriv_out.txt", "built_large_syntax_error_report_out.txt");
    EXPECT_EQ(SemanticParser.parse(tokens), true);
    EXPECT_TRUE(compare_files("large_program_parser_deriv_out.txt", "built_large_program_parser_deriv_out.txt"));
}
TEST(FileWithErrorsTest, ParserTests) {
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("given_program_with_errors.txt", true);

    Parser SemanticParser("built_given_program_with_errors_parser_deriv_out.txt", "built_syntax_with_errors_error_report_out.txt");
    EXPECT_EQ(SemanticParser.parse(tokens), false);
    EXPECT_TRUE(compare_files("given_program_parser_deriv_out.txt", "built_given_program_parser_deriv_out.txt"));
    EXPECT_TRUE(compare_files("syntax_with_errors_error_report_out.txt", "built_syntax_with_errors_error_report_out.txt"));
}
TEST(SimpleClassTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { }; program { };", false);

    Parser SemanticParser;

    EXPECT_EQ(SemanticParser.parse(tokens), true);
    EXPECT_EQ(SemanticParser.errors_.size(), 0);
}

TEST(MultiRelClassErrorTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int value; int testFunc() { value = 5 < 5 < 5;  } }; program { };", false);

    Parser SemanticParser;

    EXPECT_EQ(SemanticParser.parse(tokens), false);
    EXPECT_EQ(SemanticParser.errors_.size(), 5);
}

TEST(SimpleClassTestError, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("int class nameHere { }; program { };", false);

    Parser SemanticParser;

    EXPECT_EQ(SemanticParser.parse(tokens), true);
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "class id { } ; program { } ;");
    EXPECT_EQ(SemanticParser.errors_.size(), 1);

}


TEST(errorOneTest, ParserTests) {
    Parser SemanticParser = common_setup("class nameHere { int int i; }; program { };", "<classDeclLst> <progBody>");
    EXPECT_TRUE(SemanticParser.prog());
    EXPECT_EQ(SemanticParser.errors_.size(), 1);
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "class id { int id ; } ; program { } ;");
}
TEST(errorTwoComInFuncTest, ParserTests) {
    Parser SemanticParser = common_setup("class nameHere { int vardecl( int var1,, int var2 ) { }; }; program { };", "<classDeclLst> <progBody>");
    EXPECT_TRUE(SemanticParser.prog());
    EXPECT_EQ(SemanticParser.errors_.size(), 1);
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "class id { int id ( int id , int id ) { } ; } ; program { } ;");
}

TEST(errorMissingSemiTest, ParserTests) {
    Parser SemanticParser = common_setup("class nameHere { int vardecl 0 int var2 }; program { };", "<classDeclLst> <progBody>");
    EXPECT_TRUE(SemanticParser.prog());
    EXPECT_EQ(SemanticParser.errors_.size(), 2);
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "class id { int id <errorMissingSemiColon> int id <errorMissingSemiColon> } ; program { } ;");
}

TEST(VarDeclareClassTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int testIntVar; float testFloatVar; int testArrayIntVar[5]; nameHere idVar; }; program { };", false);

    Parser SemanticParser;

    EXPECT_EQ(SemanticParser.parse(tokens), true);
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "class id { int id ; float id ; int id [ integer ] ; id id ; } ; program { } ;");
    EXPECT_EQ(SemanticParser.errors_.size(), 0);
}

TEST(VarArrayDeclareClassTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int var1[4][5][7][8][9][1][0]; }; program { };", false);

    Parser SemanticParser;

    EXPECT_EQ(SemanticParser.parse(tokens), true);
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; } ; program { } ;");
    EXPECT_EQ(SemanticParser.errors_.size(), 0);
}
TEST(FuncDeclareClassTest, ParserTests)
{
    string find_min_func_string = "int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; ";
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { " + find_min_func_string + "}; program { };", false);

    Parser SemanticParser;

    EXPECT_EQ(SemanticParser.parse(tokens), true);
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "class id { int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; program { } ;");
    EXPECT_EQ(SemanticParser.errors_.size(), 0);
}

TEST(FullDeclareClassTest, ParserTests)
{
    string class_body = "int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; ";
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { " + class_body + "}; program { };", false);

    Parser SemanticParser;

    EXPECT_EQ(SemanticParser.parse(tokens), true);
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id > integer ; id = id - integer ) { if ( id [ id ] > id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; program { } ;");
    EXPECT_EQ(SemanticParser.errors_.size(), 0);

}


TEST(SimpleProgramTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { }; program { int i; int var1[4][5][7][8][9][1][0]; };", false);

    Parser SemanticParser;

    EXPECT_EQ(SemanticParser.parse(tokens), true);
    EXPECT_EQ(SemanticParser.errors_.size(), 0);

}

TEST(SmallProgramTest, ParserTests)
{
    string program_body = "int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); ";
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { }; program { " + program_body + " };", false);

    Parser SemanticParser;

    EXPECT_EQ(SemanticParser.parse(tokens), true);
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "class id { } ; program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ;");
    EXPECT_EQ(SemanticParser.errors_.size(), 0);

}
TEST(ProgramAndFuncTest, ParserTests) {
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Utility { }; program { int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); }; float randomize() { float value; value = 100 * (2 + 3.0 / 7.0006); value = 1.05 + ((2.04 * 2.47) - 3.0) + 7.0006 ; return (value); };", false );

    Parser SemanticParser;

    EXPECT_EQ(SemanticParser.parse(tokens), true);
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "class id { } ; program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ; float id ( ) { float id ; id = integer * ( integer + float / float ) ; id = float + ( ( float * float ) - float ) + float ; return ( id ) ; } ;");
    EXPECT_EQ(SemanticParser.errors_.size(), 0);

}

TEST(FullProgramTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Utility { int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; }; program { int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); }; float randomize() { float value; value = 100 * (2 + 3.0 / 7.0006); value = 1.05 + ((2.04 * 2.47) - 3.0) + 7.0006 ; return (value); };", false);

    Parser SemanticParser;

    EXPECT_EQ(SemanticParser.parse(tokens), true);
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id > integer ; id = id - integer ) { if ( id [ id ] > id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ; float id ( ) { float id ; id = integer * ( integer + float / float ) ; id = float + ( ( float * float ) - float ) + float ; return ( id ) ; } ;");
    EXPECT_EQ(SemanticParser.errors_.size(), 0);
}


TEST(LargeFullProgramTest, ParserTests)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Utility { int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; };  class UtilityTwo { int var1[4][5][7][8][9][1]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; int var1[4][5][7][8][9][1][0]; float var2; return (maxValue); }; int findMin(int array[100], float test) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; }; program { int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); }; float randomize() { float value; value = 100 * (2 + 3.0 / 7.0006); value = 1.05 + ((2.04 * 2.47) - 3.0) + 7.0006 ; return (value); };  id getEven(int var10, int array2[10000]) { float value; value = 100 * (2 + 3.0 / 7.0006); value = 1.05 + ((2.04 * 2.47) - 3.0) + 7.0006 ; return (value); };", false);

    Parser SemanticParser;

    EXPECT_EQ(SemanticParser.parse(tokens), true);
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id > integer ; id = id - integer ) { if ( id [ id ] > id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id > integer ; id = id - integer ) { if ( id [ id ] > id ) then { id = id [ id ] ; } else { } ; } ; int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; return ( id ) ; } ; int id ( int id [ integer ] , float id ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ; float id ( ) { float id ; id = integer * ( integer + float / float ) ; id = float + ( ( float * float ) - float ) + float ; return ( id ) ; } ; id id ( int id , int id [ integer ] ) { float id ; id = integer * ( integer + float / float ) ; id = float + ( ( float * float ) - float ) + float ; return ( id ) ; } ;");
    EXPECT_EQ(SemanticParser.errors_.size(), 0);
}
TEST(NumIntTest, ParserTests) {
    Parser SemanticParser = common_setup("200", "<num>");
    EXPECT_TRUE(SemanticParser.num());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "integer");
}

TEST(NumFloatTest, ParserTests) {
    Parser SemanticParser = common_setup("200.12", "<num>");
    EXPECT_TRUE(SemanticParser.num());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "float");
}


TEST(MultOpMultiTest, ParserTests) {
    Parser SemanticParser = common_setup("*", "<multOp>");
    EXPECT_TRUE(SemanticParser.multOp());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "*");
}

TEST(MultOpDashTest, ParserTests) {
    Parser SemanticParser = common_setup("/", "<multOp>");
    EXPECT_TRUE(SemanticParser.multOp());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "/");
}

TEST(MultOpAndTest, ParserTests) {
    Parser SemanticParser = common_setup("and", "<multOp>");
    EXPECT_TRUE(SemanticParser.multOp());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "and");
}

TEST(AddOpaddTest, ParserTests) {
    Parser SemanticParser = common_setup("+", "<addOp>");
    EXPECT_TRUE(SemanticParser.addOp());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "+");
}


TEST(AddOpSubTest, ParserTests) {
    Parser SemanticParser = common_setup("-", "<addOp>");
    EXPECT_TRUE(SemanticParser.addOp());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "-");
}


TEST(AddOpOrTest, ParserTests) {
    Parser SemanticParser = common_setup("or", "<addOp>");
    EXPECT_TRUE(SemanticParser.addOp());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "or");
}

TEST(RelOpEquivTest, ParserTests) {
    Parser SemanticParser = common_setup("==", "<relOp>");
    EXPECT_TRUE(SemanticParser.relOp());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "==");
}

TEST(RelOpNotEqTest, ParserTests) {
    Parser SemanticParser = common_setup("<>", "<relOp>");
    EXPECT_TRUE(SemanticParser.relOp());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "<>");
}

TEST(RelOpLtEquivTest, ParserTests) {
    Parser SemanticParser = common_setup("<", "<relOp>");
    EXPECT_TRUE(SemanticParser.relOp());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "<");
}

TEST(RelOpGtTest, ParserTests) {
    Parser SemanticParser = common_setup(">", "<relOp>");
    EXPECT_TRUE(SemanticParser.relOp());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, ">");
}

TEST(RelOpLtEqTest, ParserTests) {
    Parser SemanticParser = common_setup("<=", "<relOp>");
    EXPECT_TRUE(SemanticParser.relOp());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "<=");
}

TEST(RelOpGtEqTest, ParserTests) {
    Parser SemanticParser = common_setup(">=", "<relOp>");
    EXPECT_TRUE(SemanticParser.relOp());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, ">=");
}

TEST(AssignOpTest, ParserTests) {
    Parser SemanticParser = common_setup("=", "<assignOp>");
    EXPECT_TRUE(SemanticParser.assignOp());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "=");
}


TEST(aParamsTailTest, ParserTests) {
    Parser SemanticParser = common_setup(", 5 )", "<aParamsTail>");
    EXPECT_TRUE(SemanticParser.aParamsTail());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, ", integer");
}

TEST(aParamsTailNoArgTest, ParserTests) {
    Parser SemanticParser = common_setup(" )", "<aParamsTail>");
    EXPECT_TRUE(SemanticParser.aParamsTail());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "");
}

TEST(aParamsTailMultiArgTest, ParserTests) {
    Parser SemanticParser = common_setup(", 5, 10.12, varTest[10] , 10 + 10 * 10 / (10 - 100) )", "<aParamsTail>");
    EXPECT_TRUE(SemanticParser.aParamsTail());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, ", integer , float , id [ integer ] , integer + integer * integer / ( integer - integer )");
}

TEST(fParamsTailMultiArgTest, ParserTests) {
    SymbolRecord* record= new SymbolRecord();;
    Parser SemanticParser = common_setup(", int vardecl, float varfdecl, int vararray[10] )", "<fParamsTail>");
    EXPECT_TRUE(SemanticParser.fParamsTail(record));
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, ", int id , float id , int id [ integer ]");
}

TEST(fParamsTailNoArgTest, ParserTests) {
    SymbolRecord* record = new SymbolRecord();
    Parser SemanticParser = common_setup(")", "<fParamsTail>");
    EXPECT_TRUE(SemanticParser.fParamsTail(record));
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "");
}



TEST(aParamsMultiArgTest, ParserTests) {
    Parser SemanticParser = common_setup("(5 + 5) , 10.12, varTest[10] , 10 + 10 * 10 / (10 - 100) )", "<aParams>");
    EXPECT_TRUE(SemanticParser.aParams());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "( integer + integer ) , float , id [ integer ] , integer + integer * integer / ( integer - integer )");
}

TEST(aParamsNoArgTest, ParserTests) {
    Parser SemanticParser = common_setup(")", "<aParams>");
    EXPECT_TRUE(SemanticParser.aParams());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "");
}

TEST(fParamsMultiArgTest, ParserTests) {
    SymbolRecord* record = new SymbolRecord();
    Parser SemanticParser = common_setup("int vardecl, float varfdecl, int vararray[10] )", "<fParams>");
    EXPECT_TRUE(SemanticParser.fParams(record));
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "int id , float id , int id [ integer ]");
}


TEST(numTypeIntTest, ParserTests) {
    SymbolRecord* record = new SymbolRecord();
    Parser SemanticParser = common_setup("int", "<numType>");
    EXPECT_TRUE(SemanticParser.numType(record));
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "int");
}

TEST(numTypeFloatTest, ParserTests) {
    SymbolRecord* record = new SymbolRecord();
    Parser SemanticParser = common_setup("float", "<numType>");
    EXPECT_TRUE(SemanticParser.numType(record));
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "float");
}

TEST(typeIntTest, ParserTests) {
    SymbolRecord* record = new SymbolRecord();
    Parser SemanticParser = common_setup("int", "<type>");
    EXPECT_TRUE(SemanticParser.type(record));
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "int");
}

TEST(typeFloatTest, ParserTests) {
    SymbolRecord* record = new SymbolRecord();
    Parser SemanticParser = common_setup("float", "<type>");
    EXPECT_TRUE(SemanticParser.type(record));
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "float");
}

TEST(typeIdTest, ParserTests) {
    SymbolRecord* record = new SymbolRecord();
    Parser SemanticParser = common_setup("id", "<type>");
    EXPECT_TRUE(SemanticParser.type(record));
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id");
}


TEST(arraySizeTest, ParserTests) {
    SymbolRecord* record = new SymbolRecord();
    Parser SemanticParser = common_setup("[ 5 ] [ 10 ];", "<arraySize>");
    EXPECT_TRUE(SemanticParser.arraySize(record));
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "[ integer ] [ integer ]");
}

TEST(indiceMultiLstTest, ParserTests) {
    Parser SemanticParser = common_setup("[ 5 ] [ 10 - 10 ];", "<indiceLst>");
    EXPECT_TRUE(SemanticParser.indiceLst());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "[ integer ] [ integer - integer ]");
}

TEST(indiceLstTest, ParserTests) {
    Parser SemanticParser = common_setup("[ 5 ];", "<indiceLst>");
    EXPECT_TRUE(SemanticParser.indiceLst());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "[ integer ]");
}

TEST(indiceLstNestedFuncTest, ParserTests) {
    Parser SemanticParser = common_setup("[ id(15, 20, 30)] [10 ];", "<indiceLst>");
    EXPECT_TRUE(SemanticParser.indiceLst());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "[ id ( integer , integer , integer ) ] [ integer ]");
}

TEST(indiceFuncTest, ParserTests) {
    Parser SemanticParser = common_setup("[ id(15, 20, 30) ];", "<indice>");
    EXPECT_TRUE(SemanticParser.indice());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "[ id ( integer , integer , integer ) ]");
}

TEST(indiceMathExprTest, ParserTests) {
    Parser SemanticParser = common_setup("[ 20 / 20 + (20 * 10 ) - 10 ];", "<indice>");
    EXPECT_TRUE(SemanticParser.indice());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "[ integer / integer + ( integer * integer ) - integer ]");
}

TEST(idnestTest, ParserTests) {
    Parser SemanticParser = common_setup(". id =", "<idnest>");
    EXPECT_TRUE(SemanticParser.idnest());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, ". id");
}

TEST(idnestArrayTest, ParserTests) {
    Parser SemanticParser = common_setup(". id[5].id =", "<idnest>");
    EXPECT_TRUE(SemanticParser.idnest());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, ". id [ integer ] . id");
}

TEST(variableTest, ParserTests) {
    Parser SemanticParser = common_setup("var1 . var2 =", "<variable>");
    EXPECT_TRUE(SemanticParser.variable());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id . id");
}

TEST(variableArrayAndNestedIdTest, ParserTests) {
    Parser SemanticParser = common_setup("var1 [ 5 ] . var2 =", "<variable>");
    EXPECT_TRUE(SemanticParser.variable());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id [ integer ] . id");
}

TEST(varOrFuncIdNestFuncTest, ParserTests) {
    Parser SemanticParser = common_setup("( 10, 10.21, 32 + 40, var2)", "<varOrFuncIdNest>");
    EXPECT_TRUE(SemanticParser.varOrFuncIdNest());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "( integer , float , integer + integer , id )");
}

TEST(varOrFuncIdNestIdTest, ParserTests) {
    Parser SemanticParser = common_setup(". id [ 10 ] . id ( 20 , 123 , 10 + 11 , id )", "<varOrFuncIdNest>");
    EXPECT_TRUE(SemanticParser.varOrFuncIdNest());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, ". id [ integer ] . id ( integer , integer , integer + integer , id )");
}

TEST(factorIdTest, ParserTests) {
    Parser SemanticParser = common_setup("var1 <", "<factor>");
    EXPECT_TRUE(SemanticParser.factor());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id");
}

TEST(factorIdNestTest, ParserTests) {
    Parser SemanticParser = common_setup("var1.var2 <", "<factor>");
    EXPECT_TRUE(SemanticParser.factor());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id . id");
}

TEST(factorArrayIdNestTest, ParserTests) {
    Parser SemanticParser = common_setup("var1[5][10] [ 20 + 20].var2 <", "<factor>");
    EXPECT_TRUE(SemanticParser.factor());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id [ integer ] [ integer ] [ integer + integer ] . id");
}

TEST(factorNestedFuncTest, ParserTests) {
    Parser SemanticParser = common_setup("var1[5][10][ 20 + 20].var2( 10, 10 - 20, var4.var6(10, 10) ) <", "<factor>");
    EXPECT_TRUE(SemanticParser.factor());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id [ integer ] [ integer ] [ integer + integer ] . id ( integer , integer - integer , id . id ( integer , integer ) )");
}

TEST(factorNestedInvalidFuncTest, ParserTests) {
    Parser SemanticParser = common_setup("var1[5](10) <", "<factor>");
    EXPECT_TRUE(SemanticParser.factor());
    EXPECT_EQ(SemanticParser.errors_.size(), 1);
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id [ integer ]");
}
TEST(factorIntegerTest, ParserTests) {
    Parser SemanticParser = common_setup("200", "<factor>");
    EXPECT_TRUE(SemanticParser.factor());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "integer");
}
TEST(factorFloatTest, ParserTests) {
    Parser SemanticParser = common_setup("200.12", "<factor>");
    EXPECT_TRUE(SemanticParser.factor());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "float");
}

TEST(factorArithExprTest, ParserTests) {
    Parser SemanticParser = common_setup("( 20 * 20 - 10 / 10 + var2 (10, 10) )", "<factor>");
    EXPECT_TRUE(SemanticParser.factor());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "( integer * integer - integer / integer + id ( integer , integer ) )");
}

TEST(factorNotTest, ParserTests) {
    Parser SemanticParser = common_setup("not var2.var1 <", "<factor>");
    EXPECT_TRUE(SemanticParser.factor());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "not id . id");
}

TEST(factorSignPlusTest, ParserTests) {
    Parser SemanticParser = common_setup("+var2.var1 <", "<factor>");
    EXPECT_TRUE(SemanticParser.factor());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "+ id . id");
}
TEST(factorSignNegTest, ParserTests) {
    Parser SemanticParser = common_setup("-var2.var1 <", "<factor>");
    EXPECT_TRUE(SemanticParser.factor());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "- id . id");
}

TEST(termMultTest, ParserTests) {
    Parser SemanticParser = common_setup("var2 * 10.21;", "<term>");
    EXPECT_TRUE(SemanticParser.term());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id * float");
}

TEST(termDashTest, ParserTests) {
    Parser SemanticParser = common_setup("var2 / 10.21;", "<term>");
    EXPECT_TRUE(SemanticParser.term());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id / float");
}

TEST(termAndTest, ParserTests) {
    Parser SemanticParser = common_setup("var2 and 1;", "<term>");
    EXPECT_TRUE(SemanticParser.term());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id and integer");
}

TEST(termMultipleTest, ParserTests) {
    Parser SemanticParser = common_setup("var2 * 10.21 * ( 10 + 10 ) / var5(1, 3) * id.id[10];", "<term>");
    EXPECT_TRUE(SemanticParser.term());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id * float * ( integer + integer ) / id ( integer , integer ) * id . id [ integer ]");
}


TEST(arithExprAddTest, ParserTests) {
    Parser SemanticParser = common_setup("var2 + 10.21;", "<arithExpr>");
    EXPECT_TRUE(SemanticParser.arithExpr());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id + float");
}


TEST(arithExprSubTest, ParserTests) {
    Parser SemanticParser = common_setup("var2 - 10.21;", "<arithExpr>");
    EXPECT_TRUE(SemanticParser.arithExpr());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id - float");
}

TEST(arithExprOrTest, ParserTests) {
    Parser SemanticParser = common_setup("var2 or 10.21;", "<arithExpr>");
    EXPECT_TRUE(SemanticParser.arithExpr());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id or float");
}

TEST(arithExprAddOrSubTest, ParserTests) {
    Parser SemanticParser = common_setup("var2 * 10.21 * ( 10 + 10 ) / var5(1, 3) * id.id[10];", "<arithExpr>");
    EXPECT_TRUE(SemanticParser.arithExpr());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id * float * ( integer + integer ) / id ( integer , integer ) * id . id [ integer ]");
}


TEST(arithExprMultiTest, ParserTests) {
    Parser SemanticParser = common_setup("var2 + 10.21 * ( 10 + 10 ) / var5(1, 3) - id.id[10];", "<arithExpr>");
    EXPECT_TRUE(SemanticParser.arithExpr());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id + float * ( integer + integer ) / id ( integer , integer ) - id . id [ integer ]");
}



TEST(exprNotEqTest, ParserTests) {
    Parser SemanticParser = common_setup("2 <> 5;", "<expr>");
    EXPECT_TRUE(SemanticParser.expr());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "integer <> integer");
}

TEST(exprEqTest, ParserTests) {
    Parser SemanticParser = common_setup("var2 == var3.var1;", "<expr>");
    EXPECT_TRUE(SemanticParser.expr());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id == id . id");
}

TEST(exprLtTest, ParserTests) {
    Parser SemanticParser = common_setup("var2[5].var5(10, 10) < var3.var1;", "<expr>");
    EXPECT_TRUE(SemanticParser.expr());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id [ integer ] . id ( integer , integer ) < id . id");
}


TEST(exprGtTest, ParserTests) {
    Parser SemanticParser = common_setup(" 10 * var2 > 11 * 2;", "<expr>");
    EXPECT_TRUE(SemanticParser.expr());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "integer * id > integer * integer");
}


TEST(exprLtEqTest, ParserTests) {
    Parser SemanticParser = common_setup(" 10 * var2[5] <= 11 * 2;", "<expr>");
    EXPECT_TRUE(SemanticParser.expr());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "integer * id [ integer ] <= integer * integer");
}


TEST(exprGtEqTest, ParserTests) {
    Parser SemanticParser = common_setup(" 10 * (10 + var2(10, 10)) - 10 >= (var3[10].var4 - 10) * 2;", "<expr>");
    EXPECT_TRUE(SemanticParser.expr());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "integer * ( integer + id ( integer , integer ) ) - integer >= ( id [ integer ] . id - integer ) * integer");
}

TEST(statBlockSingleLineIfTest, ParserTests) {
    Parser SemanticParser = common_setup("{ if ( 10 < 20 ) then var7 = 10; else var9 = var6(10, 10); };", "<statBlock>");
        EXPECT_TRUE(SemanticParser.statBlock());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "{ if ( integer < integer ) then id = integer ; else id = id ( integer , integer ) ; } ;");
}

TEST(statBlockMultiLineIfTest, ParserTests) {
    Parser SemanticParser = common_setup("{ if ( 10 <  var10(10, 10) ) then {var7 = 10; var10 = 10 * 20 / 20; }else { var9 = var6(10, 10); }; };", "<statBlock>");
    EXPECT_TRUE(SemanticParser.statBlock());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "{ if ( integer < id ( integer , integer ) ) then { id = integer ; id = integer * integer / integer ; } else { id = id ( integer , integer ) ; } ; } ;");
}
TEST(statBlockMultiLineEmptyElseIfTest, ParserTests) {
    Parser SemanticParser = common_setup("{ if ( 10 <  var10(10, 10) ) then {var7 = 10; var10 = 10 * 20 / 20; }else { }; };", "<statBlock>");
    EXPECT_TRUE(SemanticParser.statBlock());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "{ if ( integer < id ( integer , integer ) ) then { id = integer ; id = integer * integer / integer ; } else { } ; } ;");
}


TEST(statBlockMultiLineAndSingleLineIfTest, ParserTests) {
    Parser SemanticParser = common_setup("{ if ( 10 <  var10(10, 10) ) then var2 = var3; else { var9 = var6(10, 10); }; };", "<statBlock>");
    EXPECT_TRUE(SemanticParser.statBlock());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "{ if ( integer < id ( integer , integer ) ) then id = id ; else { id = id ( integer , integer ) ; } ; } ;");
}


TEST(statBlockSingleLineForTest, ParserTests) {
    Parser SemanticParser = common_setup("{ for(int i = 0; i < 12; i = i + 1 ) count = 18 * 19; };", "<statBlock>");
    EXPECT_TRUE(SemanticParser.statBlock());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "{ for ( int id = integer ; id < integer ; id = id + integer ) id = integer * integer ; } ;");
}

TEST(statBlockMultiLineForTest, ParserTests) {
    Parser SemanticParser = common_setup("{ for(int i = 0; i < 12; i = i + 1 ) { count = 18 * 19; count1 = func(10, 20, 10); }; };", "<statBlock>");
    EXPECT_TRUE(SemanticParser.statBlock());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "{ for ( int id = integer ; id < integer ; id = id + integer ) { id = integer * integer ; id = id ( integer , integer , integer ) ; } ; } ;");

}


TEST(statBlockGetTest, ParserTests) {
    Parser SemanticParser = common_setup("{ get ( var2); };", "<statBlock>");
    EXPECT_TRUE(SemanticParser.statBlock());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "{ get ( id ) ; } ;");
}

TEST(statBlockPutTest, ParserTests) {
    Parser SemanticParser = common_setup("{ put(var2  + 10); };", "<statBlock>");
    EXPECT_TRUE(SemanticParser.statBlock());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "{ put ( id + integer ) ; } ;");
}


TEST(statBlockReturnTest, ParserTests) {
    Parser SemanticParser = common_setup("{ return(var2  + 10); };", "<statBlock>");
    EXPECT_TRUE(SemanticParser.statBlock());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "{ return ( id + integer ) ; } ;");
}

TEST(funcBodyTest, ParserTests) {
    Parser SemanticParser = common_setup(
            "{ var1 = 10.21 * 10 / 10 - 12; for (int i = 0; i < 10; i = i + 1) { if ( var2 > 10 ) then var1 = var1 + 10; else var3 = var3 + 10; }; return(var2  + 10); };",
            "<funcBody>");
    SymbolRecord* record = new SymbolRecord();
    EXPECT_TRUE(SemanticParser.funcBody(record));
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "{ id = float * integer / integer - integer ; for ( int id = integer ; id < integer ; id = id + integer ) { if ( id > integer ) then id = id + integer ; else id = id + integer ; } ; return ( id + integer ) ; }");
}

TEST(funcDefTest, ParserTests) {
    Parser SemanticParser = common_setup("int varFunc(int a, int b[5], float c){ var1 = 10.21 * 10 / 10 - 12; for (int i = 0; i < 10; i = i + 1) { if ( var2 > 10 ) then var1 = var1 + 10; else var3 = var3 + 10; }; return(var2  + 10); };", "<funcDef>");
    EXPECT_TRUE(SemanticParser.funcDef());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "int id ( int id , int id [ integer ] , float id ) { id = float * integer / integer - integer ; for ( int id = integer ; id < integer ; id = id + integer ) { if ( id > integer ) then id = id + integer ; else id = id + integer ; } ; return ( id + integer ) ; } ;");
}
TEST(funcHeadTest, ParserTests) {
    Parser SemanticParser = common_setup("int varFunc(int a, int b[5], float c)", "<funcHead>");
    SymbolRecord* record = new SymbolRecord();
    EXPECT_TRUE(SemanticParser.funcHead(record));
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "int id ( int id , int id [ integer ] , float id )");
}

TEST(funcHeadEmptyTest, ParserTests) {
    Parser SemanticParser = common_setup("var varFunc()", "<funcHead>");
    SymbolRecord* record = new SymbolRecord();
    EXPECT_TRUE(SemanticParser.funcHead(record));
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id id ( )");
}

TEST(classBodyTest, ParserTests) {
    Parser SemanticParser = common_setup("var varFunc() { int var2; var2 = 2; }; int var4; int var7; int var5Func(int a) { }; }", "<classBody>");
    EXPECT_TRUE(SemanticParser.classBody());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "id id ( ) { int id ; id = integer ; } ; int id ; int id ; int id ( int id ) { } ;");
}

TEST(classBodyEmptyTest, ParserTests) {
    Parser SemanticParser = common_setup("}", "<classBody>");
    EXPECT_TRUE(SemanticParser.classBody());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "");
}


TEST(classDeclTest, ParserTests) {
    Parser SemanticParser = common_setup("class className { var varFunc() { int var2; var2 = 2; }; int var4; int var7; int var5Func(int a) { }; };", "<classDecl>");
    EXPECT_TRUE(SemanticParser.classDecl());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "class id { id id ( ) { int id ; id = integer ; } ; int id ; int id ; int id ( int id ) { } ; } ;");
}

TEST(classDeclLargeTest, ParserTests) {
    Parser SemanticParser = common_setup("class className { int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; };", "<classDecl>");
    EXPECT_TRUE(SemanticParser.classDecl());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id > integer ; id = id - integer ) { if ( id [ id ] > id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ;");
}


TEST(progTest, ParserTests) {
    Parser SemanticParser = common_setup("program {int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); };", "<progBody>");
    EXPECT_TRUE(SemanticParser.progBody());
    EXPECT_EQ(SemanticParser.current_rhs_derivation_, "program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ;");
}


Parser common_setup(string test_program, string derivation_string) {
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens(test_program, false);

    Parser SemanticParser(tokens);
    SemanticParser.global_symbol_table_->current_symbol_record_ = new SymbolRecord();
    SemanticParser.current_rhs_derivation_ = derivation_string;
    return SemanticParser;
}
