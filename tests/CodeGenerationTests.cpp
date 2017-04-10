//
// Created by Harley on 3/30/2017.
//

#include "CodeGenerationTests.h"
//memory allocation: int and float variable declarations

TEST(IntDeclarationInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int i; float k; int p; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;

    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,12 % allocate stack memory for program\n"
            "hlt\n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);


    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}
TEST(IntVariableAssignmentIntFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int i; int k; int p; i = 5; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,12\n"
            "addi r1,r0,5\n"
            "subi r12,r13,12\n"
            "sw 0(r12),r1\n"
            "hlt\n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

//memory allocation: array variable declarations

TEST(IntArrayDeclarationInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int i[5]; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,20\n"
            "hlt\n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);


    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}
TEST(IntDoubleArrayDeclarationInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int i[5][6]; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,120\n"
            "hlt\n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);


    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}


//memory allocation: object variable declarations

TEST(ClassDeclarationInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int x; int y; }; program { int k; A a2; int p; A a1; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,24\n"
            "hlt\n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);


    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(ClassDeclarationWithClassVariableInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class B { A a; int y; int k; };class A { int x; int y; }; program { B b; };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,16\n"
            "hlt\n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);


    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}


TEST(ClassAssignmentInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int x; int y; }; program { A a; a.x = 100; a.y = 10202; A a1; a1 = a; put(a1.y); a.x = 120; put(a.x); put(a1.x);  };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,16\n"
            "addi r1,r0,100\n"
            "add r5,r0,r1\n"
            "subi r12,r13,16\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "addi r1,r0,10202\n"
            "add r5,r0,r1\n"
            "subi r12,r13,12\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "\n"
            "subi r11,r13,16\n"
            "add r1,r0,r0\n"
            "subi r12,r13,8\n"
            "copyclass0\n"
            "ceqi r2,r1,8\n"
            "bnz r2,copyclassend0\n"
            "lw r2,0(r11)\n"
            "sw 0(r12),r2\n"
            "addi r1,r1,4\n"
            "addi r12,r12,4\n"
            "addi r11,r11,4\n"
            "j copyclass0\n"
            "copyclassend0\n"
            "\n"
            "subi r12,r13,4\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "addi r1,r0,120\n"
            "add r5,r0,r1\n"
            "subi r12,r13,16\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "subi r12,r13,16\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "subi r12,r13,8\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);


    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

//loop statement: code block, jump, looping upon condition

TEST(SimpleForLoopInFunc, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; for (int i = 0; i < 10; i = i + 1) { x = x + 10; }; put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,8\n"
            "addi r1,r0,0\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "for0\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "add r2,r1,r0\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "addi r1,r0,10\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "clt r1,r2,r1\n"
            "\n"
            "bz r1,forend0\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,1\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "subi r12,r13,8\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,10\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "j for0\n"
            "forend0\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}


//conditional statement: code blocks, jumping on condition ○○ 2


TEST(SimpleIfInFunc, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 90; if ( 10 == 10 ) then { x = x + 10; }; put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "addi r1,r0,90\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r1,r0,10\n"
            "add r2,r1,r0\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "addi r1,r0,10\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "ceq r1,r2,r1\n"
            "\n"
            "bz r1,else0\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,10\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "j ifend0\n"
            "else0\n"
            "ifend0\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}
TEST(SimpleIfElseInFunc, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 90; if ( 10 == 9 ) then { x = x + 40; x = x - 20;} else { x = x + 20; }; put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "addi r1,r0,90\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r1,r0,10\n"
            "add r2,r1,r0\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "addi r1,r0,9\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "ceq r1,r2,r1\n"
            "\n"
            "bz r1,else0\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,40\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,20\n"
            "sub r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "j ifend0\n"
            "else0\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,20\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "ifend0\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(SimpleIfElseOneLineInFunc, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 90; if ( 10 == 9 ) then x = x + 10; else x = x + 20; put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "addi r1,r0,90\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r1,r0,10\n"
            "add r2,r1,r0\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "addi r1,r0,9\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "ceq r1,r2,r1\n"
            "\n"
            "bz r1,else0\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,10\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "j ifend0\n"
            "else0\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,20\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "ifend0\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}
//Input/output: read from keyboard, write to standard output

TEST(SimpleWriteIntInFunc, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { put(100); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,0\n"
            "addi r1,r0,100\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}
TEST(WriteExpressionInFunc, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { put(100 + 200 * 20 / 20 + 5); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,0\n"
            "\n"
            "addi r2,r0,20\n"
            "addi r1,r0,20\n"
            "div r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "\n"
            "addi r2,r0,200\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "mul r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "addi r1,r0,5\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "\n"
            "addi r2,r0,100\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}
TEST(SimpleWriteIntVarInFunc, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program {int x; x = 12000; put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "addi r1,r0,12000\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}
TEST(SimpleReadIntVarInFunc, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program {int x; get(x); put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "jl r15,getint\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "getint   add    r1,r0,r0         % n := 0 (result)\n"
            "         add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "getint1  getc   r2               % read c\n"
            "         ceqi   r4,r2,32\n"
            "         bnz    r4,getint1       % skip blanks\n"
            "         ceqi   r4,r2,43\n"
            "         bnz    r4,getint2       % branch if c is '+'\n"
            "         ceqi   r4,r2,45\n"
            "         bz     r4,getint3       % branch if c is not '-'\n"
            "         addi   r3,r0,1          % s := 1 (number is negative)\n"
            "getint2  getc   r2               % read c\n"
            "getint3  ceqi   r4,r2,10\n"
            "         bnz    r4,getint5       % branch if c is \\n\n"
            "         cgei   r4,r2,48\n"
            "         bz     r4,getint4       % c < 0\n"
            "         clei   r4,r2,57\n"
            "         bz     r4,getint4       % c > 9\n"
            "         muli   r1,r1,10         % n := 10 * n\n"
            "         add    r1,r1,r2         % n := n + c\n"
            "         subi   r1,r1,48         % n := n - '0'\n"
            "         j      getint2\n"
            "getint4  addi   r2,r0,63         % c := '?'\n"
            "         putc   r2               % write c\n"
            "         j      getint           % Try again\n"
            "getint5  bz     r3,getint6       % branch if s = 0 (number is positive)\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "getint6  jr     r15              % return\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

//expressions: arithmetic, relational and logic operators
TEST(NotInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = not 0; put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "addi r1,r0,0\n"
            "not r1,r1\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(SimpleAdditionInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 20 + 85; put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "\n"
            "addi r2,r0,20\n"
            "addi r1,r0,85\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(SimpleSubInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 20 - 85; put(x);};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "\n"
            "addi r2,r0,20\n"
            "addi r1,r0,85\n"
            "sub r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(SimpleDivInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = -40 / 5; put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "\n"
            "addi r2,r0,40\n"
            "sub r2,r0,r2\n"
            "addi r1,r0,5\n"
            "div r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(SimpleMultInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = - 40 * 5; put(x);};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "\n"
            "addi r2,r0,40\n"
            "sub r2,r0,r2\n"
            "addi r1,r0,5\n"
            "mul r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(SimpleAdditionWithVariableInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; int y; y = 20; x = y + 85; put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,8\n"
            "addi r1,r0,20\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,85\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(SimpleAdditionWithAllVariableInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; int y; int z; int k; k = 10; z = 85; y = 20; x = y + z + k; put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,16\n"
            "addi r1,r0,10\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "addi r1,r0,85\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "addi r1,r0,20\n"
            "subi r12,r13,12\n"
            "sw 0(r12),r1\n"
            "\n"
            "subi r12,r13,8\n"
            "lw r2,0(r12)\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "\n"
            "subi r12,r13,12\n"
            "lw r2,0(r12)\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,16\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,16\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(SimpleEquivInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 20 == 85; put(x);};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "\n"
            "addi r1,r0,20\n"
            "add r2,r1,r0\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "addi r1,r0,85\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "ceq r1,r2,r1\n"
            "\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}
TEST(SimpleLessInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; int y; y = 5; x = 2 < y; put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,8\n"
            "addi r1,r0,5\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r1,r0,2\n"
            "add r2,r1,r0\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "clt r1,r2,r1\n"
            "\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

// expressions: composite expressions and intermediate result

TEST(MultipleAdditionInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 20 + 85 + 5; put(x);};", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "\n"
            "addi r2,r0,85\n"
            "addi r1,r0,5\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "\n"
            "addi r2,r0,20\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(MultipleAdditionAndMultiInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 20 + 85 * 5; put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "\n"
            "addi r2,r0,85\n"
            "addi r1,r0,5\n"
            "mul r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "\n"
            "addi r2,r0,20\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(ComplexExpressionInFunction, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 5 * ( 10 + 12 ) - 200 + ((112 + 21) * 32); put(x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "\n"
            "addi r2,r0,10\n"
            "addi r1,r0,12\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "\n"
            "addi r2,r0,5\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "mul r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "addi r1,r0,200\n"
            "sub r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "\n"
            "addi r2,r0,112\n"
            "addi r1,r0,21\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "addi r1,r0,32\n"
            "mul r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

// function declaration code block (alias to jump to, jump back)

TEST(FunctionDeclaration, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; }; int funcTest() { int i; i = 5; return(i); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r11\n"
            "addi r1,r0,5\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "subi r12,r13,8\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(FunctionWithClassVariableDeclaration, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class B { int k; A a; }; class A {int i; int k; int p; }; program { int x; }; int funcTest() { int i; i = 5; A a; return(i); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,20\n"
            "sw 0(r12),r11\n"
            "addi r1,r0,5\n"
            "subi r12,r13,16\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,16\n"
            "lw r1,0(r12)\n"
            "subi r12,r13,20\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 4);
    parser.global_symbol_table_->print(true);
}

// function call mechanism: jump on call, return value

TEST(FunctionCall, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = funcTest(); put(x); }; int funcTest() { int i; i = 100; return(i); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "\n"
            "addi r13,r13,8\n"
            "jl r11,func0\n"
            "subi r13,r13,8\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r11\n"
            "addi r1,r0,100\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "subi r12,r13,8\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


TEST(FunctionCallInFunctionCall, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = funcTest(); put(x); }; int funcTest() { int i; i = funcTest2(); return(i); }; int funcTest2() { int k; k = 120; return(k); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "\n"
            "addi r13,r13,8\n"
            "jl r11,func0\n"
            "subi r13,r13,8\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r11\n"
            "\n"
            "addi r13,r13,8\n"
            "jl r11,func1\n"
            "subi r13,r13,8\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "subi r12,r13,8\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "func1\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r11\n"
            "addi r1,r0,120\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "subi r12,r13,8\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}
TEST(FunctionCallInExpression, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = 95 + funcTest(); put(x); }; int funcTest() { int i; i = 5 + 100; return(i); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "\n"
            "addi r2,r0,95\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "\n"
            "addi r13,r13,8\n"
            "jl r11,func0\n"
            "subi r13,r13,8\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r11\n"
            "\n"
            "addi r2,r0,5\n"
            "addi r1,r0,100\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "subi r12,r13,8\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(FunctioCallnWithClassVariableDeclaration, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class B { int x; int y; }; class A {int i; int k; B b; int p; }; program { int x; x = funcTest(); put(x); }; int funcTest() { int i; A a; i = 100; a.k = 120; return(a.k); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "\n"
            "addi r13,r13,28\n"
            "jl r11,func0\n"
            "subi r13,r13,28\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,28\n"
            "sw 0(r12),r11\n"
            "addi r1,r0,100\n"
            "subi r12,r13,24\n"
            "sw 0(r12),r1\n"
            "addi r1,r0,120\n"
            "add r5,r0,r1\n"
            "subi r12,r13,16\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "subi r12,r13,16\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "subi r12,r13,28\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 4);
    parser.global_symbol_table_->print(true);
}

TEST(FunctioCallnWithArrayVariableDeclaration, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = funcTest(); put(x); }; int funcTest() { int i[5]; i[2] = 100; return(i[2]); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "\n"
            "addi r13,r13,24\n"
            "jl r11,func0\n"
            "subi r13,r13,24\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,24\n"
            "sw 0(r12),r11\n"
            "addi r1,r0,100\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,20\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,20\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "subi r12,r13,24\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


TEST(FunctionCallWithIf, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = funcTest(10, 10); put(x); }; int funcTest(int x, int y) { if ( x < y ) then return(100); else return(110); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "addi r1,r0,10\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "addi r1,r0,10\n"
            "addi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,12\n"
            "jl r11,func0\n"
            "subi r13,r13,12\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,12\n"
            "sw 0(r12),r11\n"
            "\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "add r2,r1,r0\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "clt r1,r2,r1\n"
            "\n"
            "bz r1,else0\n"
            "addi r1,r0,100\n"
            "subi r12,r13,12\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "j ifend0\n"
            "else0\n"
            "addi r1,r0,110\n"
            "subi r12,r13,12\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "ifend0\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(FunctionCallWithForLoop, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = funcTest(10, 0); put(x); }; int funcTest(int x, int y) { for(int i = 0; i < 10; i = i + 1) y = x + y; return(y); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "addi r1,r0,10\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "addi r1,r0,0\n"
            "addi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,16\n"
            "jl r11,func0\n"
            "subi r13,r13,16\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,16\n"
            "sw 0(r12),r11\n"
            "addi r1,r0,0\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "for0\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "add r2,r1,r0\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "addi r1,r0,10\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "clt r1,r2,r1\n"
            "\n"
            "bz r1,forend0\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,1\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "subi r12,r13,12\n"
            "lw r2,0(r12)\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "j for0\n"
            "forend0\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "subi r12,r13,16\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(FunctionCallReturnClass, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A {int x; int y; int z; }; program { A a; a = funcTest(10, 777); put(a.x); put(a.y); put(a.z); }; A funcTest(int x, int y) { A a; a.x = x; a.y = y; a.z = a.x + a.y; put(a.x); put(a.y); put(a.z); return(a); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,12\n"
            "\n"
            "addi r1,r0,10\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "addi r1,r0,777\n"
            "addi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,24\n"
            "jl r11,func0\n"
            "subi r13,r13,24\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "add r11,r0,r1\n"
            "add r1,r0,r0\n"
            "subi r12,r13,12\n"
            "copyclass0\n"
            "ceqi r2,r1,12\n"
            "bnz r2,copyclassend0\n"
            "lw r2,0(r11)\n"
            "sw 0(r12),r2\n"
            "addi r1,r1,4\n"
            "addi r12,r12,4\n"
            "addi r11,r11,4\n"
            "j copyclass0\n"
            "copyclassend0\n"
            "\n"
            "subi r12,r13,12\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "subi r12,r13,8\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "subi r12,r13,4\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,24\n"
            "sw 0(r12),r11\n"
            "subi r12,r13,20\n"
            "lw r1,0(r12)\n"
            "add r5,r0,r1\n"
            "subi r12,r13,12\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "subi r12,r13,16\n"
            "lw r1,0(r12)\n"
            "add r5,r0,r1\n"
            "subi r12,r13,8\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "\n"
            "subi r12,r13,12\n"
            "add r12,r12,r9\n"
            "lw r2,0(r12)\n"
            "subi r12,r13,8\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "add r5,r0,r1\n"
            "subi r12,r13,4\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "subi r12,r13,12\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "subi r12,r13,8\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "subi r12,r13,4\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "subi r1,r13,12\n"
            "subi r12,r13,24\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

// parameter passing mechanism

TEST(FunctionParameter, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; int y; y = 40; x = funcTest(60, y); put(x);}; int funcTest(int k, int x) { int i; i = k + x; return(i); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,8\n"
            "addi r1,r0,40\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "addi r1,r0,60\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "addi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,16\n"
            "jl r11,func0\n"
            "subi r13,r13,16\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,16\n"
            "sw 0(r12),r11\n"
            "\n"
            "subi r12,r13,12\n"
            "lw r2,0(r12)\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "subi r12,r13,16\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


TEST(FunctionClassNestedParameter, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int x; int y; }; program {A a; a.x = 100; int x; x = funcTest(60, a.x); put(x);}; int funcTest(int k, int x) { int i; i = k + x; return(i); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,12\n"
            "addi r1,r0,100\n"
            "add r5,r0,r1\n"
            "subi r12,r13,12\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "addi r1,r0,60\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,12\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "addi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,16\n"
            "jl r11,func0\n"
            "subi r13,r13,16\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,16\n"
            "sw 0(r12),r11\n"
            "\n"
            "subi r12,r13,12\n"
            "lw r2,0(r12)\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "subi r12,r13,16\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}
TEST(FunctionFunctionCallParameter, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int x; int y; }; program {A a; a.x = 100; int x; x = funcTest(funcTest(100, 50), a.x); put(x);}; int funcTest(int k, int x) { int i; i = k + x; return(i); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,12\n"
            "addi r1,r0,100\n"
            "add r5,r0,r1\n"
            "subi r12,r13,12\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "addi r1,r0,100\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "addi r1,r0,50\n"
            "addi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,16\n"
            "jl r11,func0\n"
            "subi r13,r13,16\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,12\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "addi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,16\n"
            "jl r11,func0\n"
            "subi r13,r13,16\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,16\n"
            "sw 0(r12),r11\n"
            "\n"
            "subi r12,r13,12\n"
            "lw r2,0(r12)\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "subi r12,r13,16\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}


TEST(MultiFunctionCallWithParameter, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; int y; y = 40; x = funcTest(60 - 20) + funcTest(60 - 40); put(x);}; int funcTest(int k) { return(k); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,8\n"
            "addi r1,r0,40\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "\n"
            "addi r2,r0,60\n"
            "addi r1,r0,20\n"
            "sub r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,8\n"
            "jl r11,func0\n"
            "subi r13,r13,8\n"
            "add r2,r1,r0 % store return value in register\n"
            "\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "\n"
            "addi r2,r0,60\n"
            "addi r1,r0,40\n"
            "sub r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,8\n"
            "jl r11,func0\n"
            "subi r13,r13,8\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r11\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "subi r12,r13,8\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


TEST(FunctionParameterExpression, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; int y; y = 40; x = funcTest(60 + 100 * 20, y + 10); put(x);}; int funcTest(int k, int x) { int i; i = k + x; return(i); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,8\n"
            "addi r1,r0,40\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r2,r0,100\n"
            "addi r1,r0,20\n"
            "mul r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "\n"
            "addi r2,r0,60\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,10\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "addi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,16\n"
            "jl r11,func0\n"
            "subi r13,r13,16\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,16\n"
            "sw 0(r12),r11\n"
            "\n"
            "subi r12,r13,12\n"
            "lw r2,0(r12)\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "subi r12,r13,16\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(FunctionToOtherParameter, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; int y; y = 40; x = funcTest(60, y); put(x);}; int funcTest(int k, int x) { int i; i = k + x; return(funcTest2(i)); }; int funcTest2(int k)  { return (k / 10); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,8\n"
            "addi r1,r0,40\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "addi r1,r0,60\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "addi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,16\n"
            "jl r11,func0\n"
            "subi r13,r13,16\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,16\n"
            "sw 0(r12),r11\n"
            "\n"
            "subi r12,r13,12\n"
            "lw r2,0(r12)\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,8\n"
            "jl r11,func1\n"
            "subi r13,r13,8\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,16\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "func1\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r11\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,10\n"
            "div r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,8\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

// offset calculation mechanism: arrays processing (uni- and multi-dimensional), using data members

TEST(ArrayAccess, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x[5]; x[4] = 100; put(x[4]); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,20\n"
            "addi r1,r0,100\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,4\n"
            "muli r7,r7,5\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,20\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,4\n"
            "muli r7,r7,5\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,20\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(ArrayAddAccess, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x[5]; x[3] = 200; x[2] = 100; x[4] = x[3] + x[2]; put(x[4]); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,20\n"
            "addi r1,r0,200\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,3\n"
            "muli r7,r7,5\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,20\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "addi r1,r0,100\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,20\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,3\n"
            "muli r7,r7,5\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,20\n"
            "add r12,r12,r9\n"
            "lw r2,0(r12)\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,20\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,4\n"
            "muli r7,r7,5\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,20\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,4\n"
            "muli r7,r7,5\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,20\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(ArrayAccessWithVar, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x[5]; int k; k = 4; x[k] = 100; put(x[4]); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,24\n"
            "addi r1,r0,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "addi r1,r0,100\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "muli r7,r7,5\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,24\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,4\n"
            "muli r7,r7,5\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,24\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(ArrayForLoopAccess, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x[20]; for(int i = 0; i < 20; i = i + 1) x[i] = i * i; for(int p = 0; p < 20; p = p + 1) put(x[p]); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,88\n"
            "addi r1,r0,0\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "for0\n"
            "\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "add r2,r1,r0\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "addi r1,r0,20\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "clt r1,r2,r1\n"
            "\n"
            "bz r1,forend0\n"
            "\n"
            "subi r12,r13,8\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,1\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "\n"
            "subi r12,r13,8\n"
            "lw r2,0(r12)\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "mul r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "muli r7,r7,20\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,88\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "j for0\n"
            "forend0\n"
            "addi r1,r0,0\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "for1\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "add r2,r1,r0\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "addi r1,r0,20\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "clt r1,r2,r1\n"
            "\n"
            "bz r1,forend1\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,1\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "muli r7,r7,20\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,88\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "j for1\n"
            "forend1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}

TEST(ArrayForLoopAccessInExternalFunc, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = funcTest(); }; int funcTest() { int x[20]; for(int i = 0; i < 20; i = i + 1) x[i] = i * i; for(int p = 0; p < 20; p = p + 1) put(x[p]); return(0); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "\n"
            "addi r13,r13,92\n"
            "jl r11,func0\n"
            "subi r13,r13,92\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,92\n"
            "sw 0(r12),r11\n"
            "addi r1,r0,0\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "for0\n"
            "\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "add r2,r1,r0\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "addi r1,r0,20\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "clt r1,r2,r1\n"
            "\n"
            "bz r1,forend0\n"
            "\n"
            "subi r12,r13,8\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,1\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r1\n"
            "\n"
            "subi r12,r13,8\n"
            "lw r2,0(r12)\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "mul r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "subi r12,r13,8\n"
            "lw r1,0(r12)\n"
            "muli r7,r7,20\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,88\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "j for0\n"
            "forend0\n"
            "addi r1,r0,0\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "for1\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "add r2,r1,r0\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "addi r1,r0,20\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "clt r1,r2,r1\n"
            "\n"
            "bz r1,forend1\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,1\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "muli r7,r7,20\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,88\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "j for1\n"
            "forend1\n"
            "addi r1,r0,0\n"
            "subi r12,r13,92\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


TEST(DoubleArrayAccess, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x[4][3]; x[3][2] = 120; put(x[3][2]); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,48\n"
            "addi r1,r0,120\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,3\n"
            "muli r7,r7,4\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "addi r1,r0,2\n"
            "muli r7,r7,3\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,48\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,3\n"
            "muli r7,r7,4\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "addi r1,r0,2\n"
            "muli r7,r7,3\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,48\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 1);
    parser.global_symbol_table_->print(true);
}


TEST(ClassDataMemberAccess, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int x; int y; }; program { int k; A a; a.y = 110; put(a.y); put(a.x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,12\n"
            "addi r1,r0,110\n"
            "add r5,r0,r1\n"
            "subi r12,r13,4\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "subi r12,r13,4\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "subi r12,r13,8\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}


TEST(ClassInClassDataMemberAccess, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int x; int y; }; class B { int k; int p; A a; }; program { int k; B b; b.a.x = 100; put(b.a.x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,20\n"
            "addi r1,r0,100\n"
            "add r5,r0,r1\n"
            "subi r12,r13,8\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "subi r12,r13,8\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}
TEST(ClassInClassArrayDataMemberAccess, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int x[10]; int y; }; class B { int k[5]; int p; A a; }; program { int k; B b; b.a.x[5] = 100; put(b.a.x[5]); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,72\n"
            "addi r1,r0,100\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,5\n"
            "muli r7,r7,10\n"
            "muli r1,r1,68\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,44\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,5\n"
            "muli r7,r7,10\n"
            "muli r1,r1,68\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,44\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}


TEST(ClassArrayDataMemberAccess, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int x; int y[5]; }; program { A a; a.y[3] = 1200; put(a.y[3]); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,24\n"
            "addi r1,r0,1200\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,3\n"
            "muli r7,r7,5\n"
            "muli r1,r1,24\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,20\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,3\n"
            "muli r7,r7,5\n"
            "muli r1,r1,24\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,20\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(ClassDoubleArrayDataMemberAccess, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int x; int y[5][5]; }; program { A a; a.y[3][2] = 1200; a.y[2][2] = 2100; put(a.y[3][2]); put(a.y[2][2]); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,104\n"
            "addi r1,r0,1200\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,3\n"
            "muli r7,r7,5\n"
            "muli r1,r1,104\n"
            "add r7,r7,r1\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,104\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,100\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "addi r1,r0,2100\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,104\n"
            "add r7,r7,r1\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,104\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,100\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,3\n"
            "muli r7,r7,5\n"
            "muli r1,r1,104\n"
            "add r7,r7,r1\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,104\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,100\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,104\n"
            "add r7,r7,r1\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,104\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,100\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(ClassArrayDataMemberAccessInFunc, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class A { int x; int y[5]; }; program {int k; k = funcTest(); put(k); }; int funcTest() { A a; a.y[3] = 1200; return(a.y[3]); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "\n"
            "addi r13,r13,28\n"
            "jl r11,func0\n"
            "subi r13,r13,28\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,28\n"
            "sw 0(r12),r11\n"
            "addi r1,r0,1200\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,3\n"
            "muli r7,r7,5\n"
            "muli r1,r1,24\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,20\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,3\n"
            "muli r7,r7,5\n"
            "muli r1,r1,24\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,20\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "subi r12,r13,28\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

// BONUS SECTION

// passing array/object as parameter

TEST(PassArrayParameter, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x[5]; x[0] = 100; x[3] = count(x); put(x[2]); }; int count(int x[5]) { x[2] = 5; return(4); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,20\n"
            "addi r1,r0,100\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,0\n"
            "muli r7,r7,5\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,20\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "subi r1,r13,20\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,8\n"
            "jl r11,func0\n"
            "subi r13,r13,8\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,3\n"
            "muli r7,r7,5\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,20\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,20\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r11\n"
            "addi r1,r0,5\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,4\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r12,0(r12)\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "addi r1,r0,4\n"
            "subi r12,r13,8\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}

TEST(PassObjectParameter, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class Cord { int x; int y; }; program { int k; Cord cord; cord.x = 100; cord.y = count(cord); put(cord.x); }; int count(Cord cord) { cord.x = 150; return(cord.x); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,12\n"
            "addi r1,r0,100\n"
            "add r5,r0,r1\n"
            "subi r12,r13,8\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "subi r1,r13,8\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,8\n"
            "jl r11,func0\n"
            "subi r13,r13,8\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "add r5,r0,r1\n"
            "subi r12,r13,4\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "subi r12,r13,8\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r11\n"
            "addi r1,r0,150\n"
            "add r5,r0,r1\n"
            "subi r12,r13,4\n"
            "lw r12,0(r12)\n"
            "addi r12,r12,0\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "subi r12,r13,4\n"
            "lw r12,0(r12)\n"
            "addi r12,r12,0\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "subi r12,r13,8\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

// function call stack implementation: recursive/circular function calls

TEST(RecursiveFunctionCall, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = countSum(10); put(x); }; int countSum(int x) { if ( 0 == x) then return(0); else return ( x + countSum(x - 1)); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "addi r1,r0,10\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,8\n"
            "jl r11,func0\n"
            "subi r13,r13,8\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r11\n"
            "\n"
            "addi r1,r0,0\n"
            "add r2,r1,r0\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "ceq r1,r2,r1\n"
            "\n"
            "bz r1,else0\n"
            "addi r1,r0,0\n"
            "subi r12,r13,8\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "j ifend0\n"
            "else0\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,1\n"
            "sub r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,8\n"
            "jl r11,func0\n"
            "subi r13,r13,8\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,8\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "ifend0\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(FibFunctionCall, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = fib(19); put(x); }; int fib(int x) { if ( x <= 2) then return(1); else return ( fib(x-1) + fib(x-2) ); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "addi r1,r0,19\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,8\n"
            "jl r11,func0\n"
            "subi r13,r13,8\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r11\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "add r2,r1,r0\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "addi r1,r0,2\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "cle r1,r2,r1\n"
            "\n"
            "bz r1,else0\n"
            "addi r1,r0,1\n"
            "subi r12,r13,8\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "j ifend0\n"
            "else0\n"
            "\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,1\n"
            "sub r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,8\n"
            "jl r11,func0\n"
            "subi r13,r13,8\n"
            "add r2,r1,r0 % store return value in register\n"
            "\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,2\n"
            "sub r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,8\n"
            "jl r11,func0\n"
            "subi r13,r13,8\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,8\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "ifend0\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 2);
    parser.global_symbol_table_->print(true);
}
TEST(CircularFunctionalCall, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("program { int x; x = count(10); put(x); }; int count(int x) { if ( x <= 0 ) then return (0); else return (x + count2(x - 1));  }; int count2(int x) { if ( x <= 0 ) then return (0); else return (x + count(x - 2)); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,4\n"
            "addi r1,r0,10\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,8\n"
            "jl r11,func0\n"
            "subi r13,r13,8\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "subi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "func0\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r11\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "add r2,r1,r0\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "addi r1,r0,0\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "cle r1,r2,r1\n"
            "\n"
            "bz r1,else0\n"
            "addi r1,r0,0\n"
            "subi r12,r13,8\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "j ifend0\n"
            "else0\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,1\n"
            "sub r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,8\n"
            "jl r11,func1\n"
            "subi r13,r13,8\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,8\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "ifend0\n"
            "func1\n"
            "subi r12,r13,8\n"
            "sw 0(r12),r11\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r1,0(r12)\n"
            "add r2,r1,r0\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "addi r1,r0,0\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "cle r1,r2,r1\n"
            "\n"
            "bz r1,else1\n"
            "addi r1,r0,0\n"
            "subi r12,r13,8\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "j ifend1\n"
            "else1\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r2\n"
            "\n"
            "subi r12,r13,4\n"
            "lw r2,0(r12)\n"
            "addi r1,r0,2\n"
            "sub r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "addi r12,r13,4\n"
            "sw 0(r12),r1\n"
            "\n"
            "addi r13,r13,8\n"
            "jl r11,func0\n"
            "subi r13,r13,8\n"
            "add r1,r1,r0 % store return value in register\n"
            "\n"
            "lw r2,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "add r1,r2,r1\n"
            "subi r8,r8,4\n"
            "sw topaddr(r8),r1\n"
            "\n"
            "lw r1,topaddr(r8)\n"
            "addi r8,r8,4\n"
            "subi r12,r13,8\n"
            "lw r11,0(r12)\n"
            "jr r11\n"
            "ifend1\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}

//floating point numbers computation
// method calls


// arrays of objects

TEST(ArrayOfObjects, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class B { int p; int l; }; class A { int k; int y; B b; int p; }; program { A a[5];  a[2].b.p = 100; put(a[2].b.p); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,100\n"
            "addi r1,r0,100\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,20\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,92\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,20\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,92\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}
TEST(ArrayOfObjectsInObjects, CodeGeneration)
{
    vector<Token*> tokens;
    Scanner scanner;
    tokens = scanner.generate_tokens("class B { int p; int l; }; class A { int k; int y; B b[10][5]; int p; }; program { A a[5];  a[2].b[2][2].p = 100; put(a[2].b[2][2].p); };", false);

    Parser parser;
    parser.enable_double_pass_parse_ = true;
    parser.code_generator_->enable_comments_ = false;
    string result = "program entry\n"
            "addi r13,r0,stack\n"
            "addi r13,r13,2060\n"
            "addi r1,r0,100\n"
            "add r5,r0,r1\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,412\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "add r7,r0,r0\n"
            "addi r1,r0,2\n"
            "muli r7,r7,10\n"
            "muli r1,r1,412\n"
            "add r7,r7,r1\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,412\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,2052\n"
            "add r12,r12,r9\n"
            "sw 0(r12),r5\n"
            "\n"
            "add r9,r0,r0\n"
            "add r7,r0,r0\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,412\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "add r7,r0,r0\n"
            "addi r1,r0,2\n"
            "muli r7,r7,10\n"
            "muli r1,r1,412\n"
            "add r7,r7,r1\n"
            "addi r1,r0,2\n"
            "muli r7,r7,5\n"
            "muli r1,r1,412\n"
            "add r7,r7,r1\n"
            "add r9,r9,r7\n"
            "\n"
            "subi r12,r13,2052\n"
            "add r12,r12,r9\n"
            "lw r1,0(r12)\n"
            "jl r15,putint\n"
            "addi r1,r0,10\n"
            "putc r1\n"
            "hlt\n"
            "putint   add    r2,r0,r0         % c := 0 (character)\n"
            "         add    r3,r0,r0         % s := 0 (sign)\n"
            "         addi   r4,r0,endbuf     % p is the buffer pointer\n"
            "         cge    r5,r1,r0\n"
            "         bnz    r5,putint1       % branch if n >= 0\n"
            "         addi   r3,r0,1          % s := 1\n"
            "         sub    r1,r0,r1         % n := -n\n"
            "putint1  modi   r2,r1,10         % c := n mod 10\n"
            "         addi   r2,r2,48         % c := c + '0'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "         divi   r1,r1,10         % n := n div 10\n"
            "         bnz    r1,putint1       % do next digit\n"
            "         bz     r3,putint2       % branch if n >= 0\n"
            "         addi   r2,r0,45         % c := '-'\n"
            "         subi   r4,r4,1          % p := p - 1\n"
            "         sb     0(r4),r2         % buf[p] := c\n"
            "putint2  lb     r2,0(r4)         % c := buf[p]\n"
            "         putc   r2               % write c\n"
            "         addi   r4,r4,1          % p := p + 1\n"
            "         cgei   r5,r4,endbuf\n"
            "         bz     r5,putint2       % branch if more digits\n"
            "         jr     r15              % return\n"
            "\n"
            "         res    20               % digit buffer\n"
            "endbuf\t \n"
            "stack";
    EXPECT_EQ(parser.parse(tokens), true);
    EXPECT_EQ(parser.semantic_errors_.size(), 0);
    EXPECT_EQ(parser.code_generator_->generate_code(), result);

    EXPECT_EQ(parser.global_symbol_table_->symbol_records_.size(), 3);
    parser.global_symbol_table_->print(true);
}