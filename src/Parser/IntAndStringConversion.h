//
// Created by Harley on 4/6/2017.
//
#include <string>
using namespace std;
#ifndef COMPILER_INTANDSTRINGCONVERSION_H
#define COMPILER_INTANDSTRINGCONVERSION_H

class IntAndStringConversion {
public:


    string put_int = "putint   add    r2,r0,r0         % c := 0 (character)\n"
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
            "endbuf\t \n";

    string get_int = "getint   add    r1,r0,r0         % n := 0 (result)\n"
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
            "getint6  jr     r15              % return\n";

    bool put_int_required = false;
    bool get_int_required = false;

    string generate_required_functions();


};


#endif //COMPILER_INTANDSTRINGCONVERSION_H
