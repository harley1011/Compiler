//
// Created by Harley Mcphee on 2017-02-06.
//

#include "SyntaxTests.h"
#include "gtest/gtest.h"
#include "../src/Parser/Parser.h"

Parser common_setup(string test_program, string derivation_string);

SymbolRecord *record = new SymbolRecord();

bool compare_files(string first_path, string second_path);

TEST(FileTest, ParserTests) {
    vector<Token *> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("given_program.txt", true);

    Parser parser("built_given_program_parser_deriv_out.txt", "built_syntax_error_report_out.txt");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_TRUE(compare_files("given_program_parser_deriv_out.txt", "built_given_program_parser_deriv_out.txt"));
}

TEST(LargeFileTest, ParserTests) {
    vector<Token *> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("large_program_syntax.txt", true);

    Parser parser("built_large_program_parser_deriv_out.txt", "built_large_syntax_error_report_out.txt");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_TRUE(compare_files("large_program_parser_deriv_out.txt", "built_large_program_parser_deriv_out.txt"));
}

TEST(FileWithErrorsTest, ParserTests) {
    vector<Token *> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("given_program_with_errors.txt", true);

    Parser parser("built_given_program_with_errors_parser_deriv_out.txt",
                  "built_syntax_with_errors_error_report_out.txt");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_EQ(parser.parse(tokens), false);
    EXPECT_TRUE(compare_files("given_program_parser_deriv_out.txt", "built_given_program_parser_deriv_out.txt"));
    EXPECT_TRUE(
            compare_files("syntax_with_errors_error_report_out.txt", "built_syntax_with_errors_error_report_out.txt"));
}

TEST(SimpleClassTest, ParserTests) {
    vector<Token *> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { }; program { };", false);

    Parser parser;
    SymbolRecord *record = new SymbolRecord();

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.syntax_errors.size(), 0);
}

TEST(MultiRelClassErrorTest, ParserTests) {
    vector<Token *> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens(
            "class nameHere { int value; int testFunc() { value = 5 < 5 < 5;  } }; program { };", false);

    Parser parser;
    SymbolRecord *record = new SymbolRecord();

    EXPECT_EQ(parser.parse(tokens), false);
    EXPECT_EQ(parser.syntax_errors.size(), 5);
}

TEST(SimpleClassTestError, ParserTests) {
    vector<Token *> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("int class nameHere { }; program { };", false);

    Parser parser;
    SymbolRecord *record = new SymbolRecord();

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.current_rhs_derivation_, "class id { } ; program { } ;");
    EXPECT_EQ(parser.syntax_errors.size(), 1);

}


TEST(errorOneTest, ParserTests) {
    Parser parser = common_setup("class nameHere { int int i; }; program { };", "<classDeclLst> <progBody>");
    EXPECT_TRUE(parser.prog());
    EXPECT_EQ(parser.syntax_errors.size(), 1);
    EXPECT_EQ(parser.current_rhs_derivation_, "class id { int id ; } ; program { } ;");
}

TEST(errorTwoComInFuncTest, ParserTests) {
    Parser parser = common_setup("class nameHere { int vardecl( int var1,, int var2 ) { }; }; program { };",
                                 "<classDeclLst> <progBody>");
    EXPECT_TRUE(parser.prog());
    EXPECT_EQ(parser.syntax_errors.size(), 1);
    EXPECT_EQ(parser.current_rhs_derivation_, "class id { int id ( int id , int id ) { } ; } ; program { } ;");
}

TEST(errorMissingSemiTest, ParserTests) {
    Parser parser = common_setup("class nameHere { int vardecl 0 int var2 }; program { };",
                                 "<classDeclLst> <progBody>");
    EXPECT_TRUE(parser.prog());
    EXPECT_EQ(parser.syntax_errors.size(), 2);
    EXPECT_EQ(parser.current_rhs_derivation_,
              "class id { int id <errorMissingSemiColon> int id <errorMissingSemiColon> } ; program { } ;");
}

TEST(VarDeclareClassTest, ParserTests) {
    vector<Token *> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens(
            "class nameHere { int testIntVar; float testFloatVar; int testArrayIntVar[5]; nameHere idVar; }; program { };",
            false);

    Parser parser;
    SymbolRecord *record = new SymbolRecord();

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.current_rhs_derivation_,
              "class id { int id ; float id ; int id [ integer ] ; id id ; } ; program { } ;");
    EXPECT_EQ(parser.syntax_errors.size(), 0);
}

TEST(VarArrayDeclareClassTest, ParserTests) {
    vector<Token *> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { int var1[4][5][7][8][9][1][0]; }; program { };", false);

    Parser parser;
    SymbolRecord *record = new SymbolRecord();

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.current_rhs_derivation_,
              "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; } ; program { } ;");
    EXPECT_EQ(parser.syntax_errors.size(), 0);
}

TEST(FuncDeclareClassTest, ParserTests) {
    string find_min_func_string = "int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; ";
    vector<Token *> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { " + find_min_func_string + "}; program { };", false);

    Parser parser;
    SymbolRecord *record = new SymbolRecord();

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.current_rhs_derivation_,
              "class id { int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; program { } ;");
    EXPECT_EQ(parser.syntax_errors.size(), 0);
}

TEST(FullDeclareClassTest, ParserTests) {
    string class_body = "int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; ";
    vector<Token *> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { " + class_body + "}; program { };", false);

    Parser parser;
    SymbolRecord *record = new SymbolRecord();

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.current_rhs_derivation_,
              "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id > integer ; id = id - integer ) { if ( id [ id ] > id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; program { } ;");
    EXPECT_EQ(parser.syntax_errors.size(), 0);

}


TEST(SimpleProgramTest, ParserTests) {
    vector<Token *> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { }; program { int i; int var1[4][5][7][8][9][1][0]; };", false);

    Parser parser;
    SymbolRecord *record = new SymbolRecord();

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.syntax_errors.size(), 0);

}

TEST(SmallProgramTest, ParserTests) {
    string program_body = "int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); ";
    vector<Token *> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class nameHere { }; program { " + program_body + " };", false);

    Parser parser;
    SymbolRecord *record = new SymbolRecord();

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.current_rhs_derivation_,
              "class id { } ; program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ;");
    EXPECT_EQ(parser.syntax_errors.size(), 0);

}

TEST(ProgramAndFuncTest, ParserTests) {
    vector<Token *> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens(
            "class Utility { }; program { int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); }; float randomize() { float value; value = 100 * (2 + 3.0 / 7.0006); value = 1.05 + ((2.04 * 2.47) - 3.0) + 7.0006 ; return (value); };",
            false);

    Parser parser;
    SymbolRecord *record = new SymbolRecord();

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.current_rhs_derivation_,
              "class id { } ; program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ; float id ( ) { float id ; id = integer * ( integer + float / float ) ; id = float + ( ( float * float ) - float ) + float ; return ( id ) ; } ;");
    EXPECT_EQ(parser.syntax_errors.size(), 0);

}

TEST(FullProgramTest, ParserTests) {
    vector<Token *> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens(
            "class Utility { int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; }; program { int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); }; float randomize() { float value; value = 100 * (2 + 3.0 / 7.0006); value = 1.05 + ((2.04 * 2.47) - 3.0) + 7.0006 ; return (value); };",
            false);

    Parser parser;
    SymbolRecord *record = new SymbolRecord();

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.current_rhs_derivation_,
              "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id > integer ; id = id - integer ) { if ( id [ id ] > id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ; float id ( ) { float id ; id = integer * ( integer + float / float ) ; id = float + ( ( float * float ) - float ) + float ; return ( id ) ; } ;");
    EXPECT_EQ(parser.syntax_errors.size(), 0);
}


TEST(LargeFullProgramTest, ParserTests) {
    vector<Token *> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens(
            "class Utility { int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; };  class UtilityTwo { int var1[4][5][7][8][9][1]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; int var1[4][5][7][8][9][1][0]; float var2; return (maxValue); }; int findMin(int array[100], float test) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; }; program { int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); }; float randomize() { float value; value = 100 * (2 + 3.0 / 7.0006); value = 1.05 + ((2.04 * 2.47) - 3.0) + 7.0006 ; return (value); };  id getEven(int var10, int array2[10000]) { float value; value = 100 * (2 + 3.0 / 7.0006); value = 1.05 + ((2.04 * 2.47) - 3.0) + 7.0006 ; return (value); };",
            false);

    Parser parser;
    SymbolRecord *record = new SymbolRecord();

    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.current_rhs_derivation_,
              "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id > integer ; id = id - integer ) { if ( id [ id ] > id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id > integer ; id = id - integer ) { if ( id [ id ] > id ) then { id = id [ id ] ; } else { } ; } ; int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; return ( id ) ; } ; int id ( int id [ integer ] , float id ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ; program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ; float id ( ) { float id ; id = integer * ( integer + float / float ) ; id = float + ( ( float * float ) - float ) + float ; return ( id ) ; } ; id id ( int id , int id [ integer ] ) { float id ; id = integer * ( integer + float / float ) ; id = float + ( ( float * float ) - float ) + float ; return ( id ) ; } ;");
    EXPECT_EQ(parser.syntax_errors.size(), 0);
}

TEST(NumIntTest, ParserTests) {
    Parser parser = common_setup("200", "<num>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.num(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "integer");
}

TEST(NumFloatTest, ParserTests) {
    Parser parser = common_setup("200.12", "<num>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.num(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "float");
}


TEST(MultOpMultiTest, ParserTests) {
    Parser parser = common_setup("*", "<multOp>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.multOp(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "*");
}

TEST(MultOpDashTest, ParserTests) {
    Parser parser = common_setup("/", "<multOp>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.multOp(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "/");
}

TEST(MultOpAndTest, ParserTests) {
    Parser parser = common_setup("and", "<multOp>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.multOp(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "and");
}

TEST(AddOpaddTest, ParserTests) {
    Parser parser = common_setup("+", "<addOp>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.addOp(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "+");
}


TEST(AddOpSubTest, ParserTests) {
    Parser parser = common_setup("-", "<addOp>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.addOp(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "-");
}


TEST(AddOpOrTest, ParserTests) {
    Parser parser = common_setup("or", "<addOp>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.addOp(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "or");
}

TEST(RelOpEquivTest, ParserTests) {
    Parser parser = common_setup("==", "<relOp>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.relOp(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "==");
}

TEST(RelOpNotEqTest, ParserTests) {
    Parser parser = common_setup("<>", "<relOp>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.relOp(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "<>");
}

TEST(RelOpLtEquivTest, ParserTests) {
    Parser parser = common_setup("<", "<relOp>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.relOp(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "<");
}

TEST(RelOpGtTest, ParserTests) {
    Parser parser = common_setup(">", "<relOp>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.relOp(record));
    EXPECT_EQ(parser.current_rhs_derivation_, ">");
}

TEST(RelOpLtEqTest, ParserTests) {
    Parser parser = common_setup("<=", "<relOp>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.relOp(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "<=");
}

TEST(RelOpGtEqTest, ParserTests) {
    Parser parser = common_setup(">=", "<relOp>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.relOp(record));
    EXPECT_EQ(parser.current_rhs_derivation_, ">=");
}

TEST(AssignOpTest, ParserTests) {
    Parser parser = common_setup("=", "<assignOp>");
    SymbolRecord *func_record = new SymbolRecord();
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.assignOp());
    EXPECT_EQ(parser.current_rhs_derivation_, "=");
}


TEST(aParamsTailTest, ParserTests) {
    Parser parser = common_setup(", 5 )", "<aParamsTail>");
    SymbolRecord *func_record = new SymbolRecord();
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.aParamsTail(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, ", integer");
}

TEST(aParamsTailNoArgTest, ParserTests) {
    Parser parser = common_setup(" )", "<aParamsTail>");
    SymbolRecord *func_record = new SymbolRecord();
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.aParamsTail(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "");
}

TEST(aParamsTailMultiArgTest, ParserTests) {
    Parser parser = common_setup(", 5, 10.12, varTest[10] , 10 + 10 * 10 / (10 - 100) )", "<aParamsTail>");
    SymbolRecord *func_record = new SymbolRecord();
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.aParamsTail(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_,
              ", integer , float , id [ integer ] , integer + integer * integer / ( integer - integer )");
}

TEST(fParamsTailMultiArgTest, ParserTests) {
    SymbolRecord *record = new SymbolRecord();;
    Parser parser = common_setup(", int vardecl, float varfdecl, int vararray[10] )", "<fParamsTail>");

    EXPECT_TRUE(parser.fParamsTail(record));
    EXPECT_EQ(parser.current_rhs_derivation_, ", int id , float id , int id [ integer ]");
}

TEST(fParamsTailNoArgTest, ParserTests) {
    SymbolRecord *record = new SymbolRecord();
    Parser parser = common_setup(")", "<fParamsTail>");;
    EXPECT_TRUE(parser.fParamsTail(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "");
}


TEST(aParamsMultiArgTest, ParserTests) {
    Parser parser = common_setup("(5 + 5) , 10.12, varTest[10] , 10 + 10 * 10 / (10 - 100) )", "<aParams>");
    SymbolRecord *func_record = new SymbolRecord();
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.aParams(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_,
              "( integer + integer ) , float , id [ integer ] , integer + integer * integer / ( integer - integer )");
}

TEST(aParamsNoArgTest, ParserTests) {
    Parser parser = common_setup(")", "<aParams>");
    SymbolRecord *func_record = new SymbolRecord();
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.aParams(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "");
}

TEST(fParamsMultiArgTest, ParserTests) {
    SymbolRecord *record = new SymbolRecord();
    Parser parser = common_setup("int vardecl, float varfdecl, int vararray[10] )", "<fParams>");
    EXPECT_TRUE(parser.fParams(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "int id , float id , int id [ integer ]");
}


TEST(numTypeIntTest, ParserTests) {
    SymbolRecord *record = new SymbolRecord();
    Parser parser = common_setup("int", "<numType>");
    EXPECT_TRUE(parser.numType(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "int");
}

TEST(numTypeFloatTest, ParserTests) {
    SymbolRecord *record = new SymbolRecord();
    Parser parser = common_setup("float", "<numType>");
    EXPECT_TRUE(parser.numType(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "float");
}

TEST(typeIntTest, ParserTests) {
    SymbolRecord *record = new SymbolRecord();
    Parser parser = common_setup("int", "<type>");
    EXPECT_TRUE(parser.type(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "int");
}

TEST(typeFloatTest, ParserTests) {
    SymbolRecord *record = new SymbolRecord();
    Parser parser = common_setup("float", "<type>");
    EXPECT_TRUE(parser.type(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "float");
}

TEST(typeIdTest, ParserTests) {
    SymbolRecord *record = new SymbolRecord();
    Parser parser = common_setup("id", "<type>");
    EXPECT_TRUE(parser.type(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "id");
}


TEST(arraySizeTest, ParserTests) {
    SymbolRecord *record = new SymbolRecord();
    Parser parser = common_setup("[ 5 ] [ 10 ];", "<arraySize>");
    EXPECT_TRUE(parser.arraySize(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "[ integer ] [ integer ]");
}

TEST(indiceMultiLstTest, ParserTests) {
    Parser parser = common_setup("[ 5 ] [ 10 - 10 ];", "<indiceLst>");
    SymbolRecord *func_record = new SymbolRecord();
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.indiceLst(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "[ integer ] [ integer - integer ]");
}

TEST(indiceLstTest, ParserTests) {
    Parser parser = common_setup("[ 5 ];", "<indiceLst>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.indiceLst(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "[ integer ]");
}

TEST(indiceLstNestedFuncTest, ParserTests) {
    Parser parser = common_setup("[ id(15, 20, 30)] [10 ];", "<indiceLst>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.indiceLst(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "[ id ( integer , integer , integer ) ] [ integer ]");
}

TEST(indiceFuncTest, ParserTests) {
    Parser parser = common_setup("[ id(15, 20, 30) ];", "<indice>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.indice(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "[ id ( integer , integer , integer ) ]");
}

TEST(indiceMathExprTest, ParserTests) {
    Parser parser = common_setup("[ 20 / 20 + (20 * 10 ) - 10 ];", "<indice>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.indice(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "[ integer / integer + ( integer * integer ) - integer ]");
}

TEST(idnestTest, ParserTests) {
    Parser parser = common_setup(". id =", "<idnest>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.idnest(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, ". id");
}

TEST(idnestArrayTest, ParserTests) {
    Parser parser = common_setup(". id[5].id =", "<idnest>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.idnest(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, ". id [ integer ] . id");
}

TEST(variableTest, ParserTests) {
    Parser parser = common_setup("var1 . var2 =", "<variable>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.variable(func_record,record));
    EXPECT_EQ(parser.current_rhs_derivation_, "id . id");
}

TEST(variableArrayAndNestedIdTest, ParserTests) {
    Parser parser = common_setup("var1 [ 5 ] . var2 =", "<variable>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.variable(func_record,record));
    EXPECT_EQ(parser.current_rhs_derivation_, "id [ integer ] . id");
}

TEST(varOrFuncIdNestFuncTest, ParserTests) {
    Parser parser = common_setup("( 10, 10.21, 32 + 40, var2)", "<varOrFuncIdNest>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.varOrFuncIdNest(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "( integer , float , integer + integer , id )");
}

TEST(varOrFuncIdNestIdTest, ParserTests) {
    Parser parser = common_setup(". id [ 10 ] . id ( 20 , 123 , 10 + 11 , id )", "<varOrFuncIdNest>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.varOrFuncIdNest(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, ". id [ integer ] . id ( integer , integer , integer + integer , id )");
}

TEST(factorIdTest, ParserTests) {
    Parser parser = common_setup("var1 <", "<factor>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.factor(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "id");
}

TEST(factorIdNestTest, ParserTests) {
    Parser parser = common_setup("var1.var2 <", "<factor>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.factor(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "id . id");
}

TEST(factorArrayIdNestTest, ParserTests) {
    Parser parser = common_setup("var1[5][10] [ 20 + 20].var2 <", "<factor>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.factor(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "id [ integer ] [ integer ] [ integer + integer ] . id");
}

TEST(factorNestedFuncTest, ParserTests) {
    Parser parser = common_setup("var1[5][10][ 20 + 20].var2( 10, 10 - 20, var4.var6(10, 10) ) <", "<factor>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.factor(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_,
              "id [ integer ] [ integer ] [ integer + integer ] . id ( integer , integer - integer , id . id ( integer , integer ) )");
}

TEST(factorNestedInvalidFuncTest, ParserTests) {
    Parser parser = common_setup("var1[5](10) <", "<factor>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.factor(func_record, record));
    EXPECT_EQ(parser.syntax_errors.size(), 1);
    EXPECT_EQ(parser.current_rhs_derivation_, "id [ integer ]");
}

TEST(factorIntegerTest, ParserTests) {
    Parser parser = common_setup("200", "<factor>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.factor(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "integer");
}

TEST(factorFloatTest, ParserTests) {
    Parser parser = common_setup("200.12", "<factor>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.factor(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "float");
}

TEST(factorArithExprTest, ParserTests) {
    Parser parser = common_setup("( 20 * 20 - 10 / 10 + var2 (10, 10) )", "<factor>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.factor(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "( integer * integer - integer / integer + id ( integer , integer ) )");
}

TEST(factorNotTest, ParserTests) {
    Parser parser = common_setup("not var2.var1 <", "<factor>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.factor(func_record,record));
    EXPECT_EQ(parser.current_rhs_derivation_, "not id . id");
}

TEST(factorSignPlusTest, ParserTests) {
    Parser parser = common_setup("+var2.var1 <", "<factor>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.factor(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "+ id . id");
}

TEST(factorSignNegTest, ParserTests) {
    Parser parser = common_setup("-var2.var1 <", "<factor>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.factor(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "- id . id");
}

TEST(termMultTest, ParserTests) {
    Parser parser = common_setup("var2 * 10.21;", "<term>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.term(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "id * float");
}

TEST(termDashTest, ParserTests) {
    Parser parser = common_setup("var2 / 10.21;", "<term>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.term(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "id / float");
}

TEST(termAndTest, ParserTests) {
    Parser parser = common_setup("var2 and 1;", "<term>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.term(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "id and integer");
}

TEST(termMultipleTest, ParserTests) {
    Parser parser = common_setup("var2 * 10.21 * ( 10 + 10 ) / var5(1, 3) * id.id[10];", "<term>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.term(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_,
              "id * float * ( integer + integer ) / id ( integer , integer ) * id . id [ integer ]");
}


TEST(arithExprAddTest, ParserTests) {
    Parser parser = common_setup("var2 + 10.21;", "<arithExpr>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.arithExpr(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "id + float");
}


TEST(arithExprSubTest, ParserTests) {
    Parser parser = common_setup("var2 - 10.21;", "<arithExpr>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.arithExpr(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "id - float");
}

TEST(arithExprOrTest, ParserTests) {
    Parser parser = common_setup("var2 or 10.21;", "<arithExpr>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.arithExpr(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "id or float");
}

TEST(arithExprAddOrSubTest, ParserTests) {
    Parser parser = common_setup("var2 * 10.21 * ( 10 + 10 ) / var5(1, 3) * id.id[10];", "<arithExpr>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.arithExpr(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_,
              "id * float * ( integer + integer ) / id ( integer , integer ) * id . id [ integer ]");
}


TEST(arithExprMultiTest, ParserTests) {
    Parser parser = common_setup("var2 + 10.21 * ( 10 + 10 ) / var5(1, 3) - id.id[10];", "<arithExpr>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.arithExpr(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_,
              "id + float * ( integer + integer ) / id ( integer , integer ) - id . id [ integer ]");
}


TEST(exprNotEqTest, ParserTests) {
    Parser parser = common_setup("2 <> 5;", "<expr>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.expr(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "integer <> integer");
}

TEST(exprEqTest, ParserTests) {
    Parser parser = common_setup("var2 == var3.var1;", "<expr>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.expr(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "id == id . id");
}

TEST(exprLtTest, ParserTests) {
    Parser parser = common_setup("var2[5].var5(10, 10) < var3.var1;", "<expr>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.expr(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "id [ integer ] . id ( integer , integer ) < id . id");
}


TEST(exprGtTest, ParserTests) {
    Parser parser = common_setup(" 10 * var2 > 11 * 2;", "<expr>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.expr(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "integer * id > integer * integer");
}


TEST(exprLtEqTest, ParserTests) {
    Parser parser = common_setup(" 10 * var2[5] <= 11 * 2;", "<expr>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.expr(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_, "integer * id [ integer ] <= integer * integer");
}


TEST(exprGtEqTest, ParserTests) {
    Parser parser = common_setup(" 10 * (10 + var2(10, 10)) - 10 >= (var3[10].var4 - 10) * 2;", "<expr>");
    SymbolRecord *record = new SymbolRecord();
    SymbolRecord *func_record = new SymbolRecord();
    EXPECT_TRUE(parser.expr(func_record, record));
    EXPECT_EQ(parser.current_rhs_derivation_,
              "integer * ( integer + id ( integer , integer ) ) - integer >= ( id [ integer ] . id - integer ) * integer");
}

TEST(statBlockSingleLineIfTest, ParserTests) {
    Parser parser = common_setup("{ if ( 10 < 20 ) then var7 = 10; else var9 = var6(10, 10); };", "<statBlock>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.statBlock(record));
    EXPECT_EQ(parser.current_rhs_derivation_,
              "{ if ( integer < integer ) then id = integer ; else id = id ( integer , integer ) ; } ;");
}

TEST(statBlockMultiLineIfTest, ParserTests) {
    Parser parser = common_setup(
            "{ if ( 10 <  var10(10, 10) ) then {var7 = 10; var10 = 10 * 20 / 20; }else { var9 = var6(10, 10); }; };",
            "<statBlock>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.statBlock(record));
    EXPECT_EQ(parser.current_rhs_derivation_,
              "{ if ( integer < id ( integer , integer ) ) then { id = integer ; id = integer * integer / integer ; } else { id = id ( integer , integer ) ; } ; } ;");
}

TEST(statBlockMultiLineEmptyElseIfTest, ParserTests) {
    Parser parser = common_setup("{ if ( 10 <  var10(10, 10) ) then {var7 = 10; var10 = 10 * 20 / 20; }else { }; };",
                                 "<statBlock>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.statBlock(record));
    EXPECT_EQ(parser.current_rhs_derivation_,
              "{ if ( integer < id ( integer , integer ) ) then { id = integer ; id = integer * integer / integer ; } else { } ; } ;");
}


TEST(statBlockMultiLineAndSingleLineIfTest, ParserTests) {
    Parser parser = common_setup("{ if ( 10 <  var10(10, 10) ) then var2 = var3; else { var9 = var6(10, 10); }; };",
                                 "<statBlock>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.statBlock(record));
    EXPECT_EQ(parser.current_rhs_derivation_,
              "{ if ( integer < id ( integer , integer ) ) then id = id ; else { id = id ( integer , integer ) ; } ; } ;");
}


TEST(statBlockSingleLineForTest, ParserTests) {
    Parser parser = common_setup("{ for(int i = 0; i < 12; i = i + 1 ) count = 18 * 19; };", "<statBlock>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.statBlock(record));
    EXPECT_EQ(parser.current_rhs_derivation_,
              "{ for ( int id = integer ; id < integer ; id = id + integer ) id = integer * integer ; } ;");
}

TEST(statBlockMultiLineForTest, ParserTests) {
    Parser parser = common_setup(
            "{ for(int i = 0; i < 12; i = i + 1 ) { count = 18 * 19; count1 = func(10, 20, 10); }; };", "<statBlock>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.statBlock(record));
    EXPECT_EQ(parser.current_rhs_derivation_,
              "{ for ( int id = integer ; id < integer ; id = id + integer ) { id = integer * integer ; id = id ( integer , integer , integer ) ; } ; } ;");

}


TEST(statBlockGetTest, ParserTests) {
    Parser parser = common_setup("{ get ( var2); };", "<statBlock>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.statBlock(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "{ get ( id ) ; } ;");
}

TEST(statBlockPutTest, ParserTests) {
    Parser parser = common_setup("{ put(var2  + 10); };", "<statBlock>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.statBlock(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "{ put ( id + integer ) ; } ;");
}


TEST(statBlockReturnTest, ParserTests) {
    Parser parser = common_setup("{ return(var2  + 10); };", "<statBlock>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.statBlock(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "{ return ( id + integer ) ; } ;");
}

TEST(funcBodyTest, ParserTests) {
    Parser parser = common_setup(
            "{ var1 = 10.21 * 10 / 10 - 12; for (int i = 0; i < 10; i = i + 1) { if ( var2 > 10 ) then var1 = var1 + 10; else var3 = var3 + 10; }; return(var2  + 10); };",
            "<funcBody>");
    SymbolRecord *record = new SymbolRecord();
    EXPECT_TRUE(parser.funcBody(record));
    EXPECT_EQ(parser.current_rhs_derivation_,
              "{ id = float * integer / integer - integer ; for ( int id = integer ; id < integer ; id = id + integer ) { if ( id > integer ) then id = id + integer ; else id = id + integer ; } ; return ( id + integer ) ; }");
}

TEST(funcDefTest, ParserTests) {
    Parser parser = common_setup(
            "int varFunc(int a, int b[5], float c){ var1 = 10.21 * 10 / 10 - 12; for (int i = 0; i < 10; i = i + 1) { if ( var2 > 10 ) then var1 = var1 + 10; else var3 = var3 + 10; }; return(var2  + 10); };",
            "<funcDef>");
    EXPECT_TRUE(parser.funcDef());
    EXPECT_EQ(parser.current_rhs_derivation_,
              "int id ( int id , int id [ integer ] , float id ) { id = float * integer / integer - integer ; for ( int id = integer ; id < integer ; id = id + integer ) { if ( id > integer ) then id = id + integer ; else id = id + integer ; } ; return ( id + integer ) ; } ;");
}

TEST(funcHeadTest, ParserTests) {
    Parser parser = common_setup("int varFunc(int a, int b[5], float c)", "<funcHead>");
    SymbolRecord **record;
    SymbolRecord *r = new SymbolRecord();
    record = &r;
    EXPECT_TRUE(parser.funcHead(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "int id ( int id , int id [ integer ] , float id )");
}

TEST(funcHeadEmptyTest, ParserTests) {
    Parser parser = common_setup("var varFunc()", "<funcHead>");
    SymbolRecord **record;
    SymbolRecord *r = new SymbolRecord();
    record = &r;
    EXPECT_TRUE(parser.funcHead(record));
    EXPECT_EQ(parser.current_rhs_derivation_, "id id ( )");
}

TEST(classBodyTest, ParserTests) {
    Parser parser = common_setup(
            "var varFunc() { int var2; var2 = 2; }; int var4; int var7; int var5Func(int a) { }; }", "<classBody>");
    EXPECT_TRUE(parser.classBody());
    EXPECT_EQ(parser.current_rhs_derivation_,
              "id id ( ) { int id ; id = integer ; } ; int id ; int id ; int id ( int id ) { } ;");
}

TEST(classBodyEmptyTest, ParserTests) {
    Parser parser = common_setup("}", "<classBody>");
    EXPECT_TRUE(parser.classBody());
    EXPECT_EQ(parser.current_rhs_derivation_, "");
}


TEST(classDeclTest, ParserTests) {
    Parser parser = common_setup(
            "class className { var varFunc() { int var2; var2 = 2; }; int var4; int var7; int var5Func(int a) { }; };",
            "<classDecl>");
    EXPECT_TRUE(parser.classDecl());
    EXPECT_EQ(parser.current_rhs_derivation_,
              "class id { id id ( ) { int id ; id = integer ; } ; int id ; int id ; int id ( int id ) { } ; } ;");
}

TEST(classDeclLargeTest, ParserTests) {
    Parser parser = common_setup(
            "class className { int var1[4][5][7][8][9][1][0]; float var2; int findMax(int array[100]) { int maxValue; int idx; maxValue = array[100]; for( int idx = 99; idx > 0; idx = idx - 1 ) { if(array[idx] > maxValue) then { maxValue = array[idx]; }else{}; }; return (maxValue); }; int findMin(int array[100]) { int minValue; int idx; minValue = array[100]; for( int idx = 1; idx <= 99; idx = ( idx ) + 1) { if(array[idx] < maxValue) then { maxValue = array[idx]; }else{}; }; return (minValue); }; };",
            "<classDecl>");
    EXPECT_TRUE(parser.classDecl());
    EXPECT_EQ(parser.current_rhs_derivation_,
              "class id { int id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] ; float id ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id > integer ; id = id - integer ) { if ( id [ id ] > id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; int id ( int id [ integer ] ) { int id ; int id ; id = id [ integer ] ; for ( int id = integer ; id <= integer ; id = ( id ) + integer ) { if ( id [ id ] < id ) then { id = id [ id ] ; } else { } ; } ; return ( id ) ; } ; } ;");
}


TEST(progTest, ParserTests) {
    Parser parser = common_setup(
            "program {int sample[100]; int idx; int maxValue; int minValue; Utility utility; Utility arrayUtility[2][3][6][7]; for(int t = 0; t<=100 ; t = t + 1) { get(sample[t]); sample[t] = (sample[t] * randomize()); }; maxValue = utility.findMax(sample); minValue = utility.findMin(sample); utility. var1[4][1][0][0][0][0][0] = 10; arrayUtility[1][1][1][1].var1[4][1][0][0][0][0][0] = 2; put(maxValue); put(minValue); };",
            "<progBody>");
    EXPECT_TRUE(parser.progBody());
    EXPECT_EQ(parser.current_rhs_derivation_,
              "program { int id [ integer ] ; int id ; int id ; int id ; id id ; id id [ integer ] [ integer ] [ integer ] [ integer ] ; for ( int id = integer ; id <= integer ; id = id + integer ) { get ( id [ id ] ) ; id [ id ] = ( id [ id ] * id ( ) ) ; } ; id = id . id ( id ) ; id = id . id ( id ) ; id . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; id [ integer ] [ integer ] [ integer ] [ integer ] . id [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] [ integer ] = integer ; put ( id ) ; put ( id ) ; } ;");
}


Parser common_setup(string test_program, string derivation_string) {
    SymbolRecord *record = new SymbolRecord();
    vector<Token *> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens(test_program, false);
    Parser parser(tokens);
    parser.global_symbol_table_->current_symbol_record_ = new SymbolRecord();
    parser.current_rhs_derivation_ = derivation_string;
    return parser;
}
