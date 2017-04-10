program entry
addi r13,r0,stack
addi r13,r13,4 % allocate stack memory for program
addi r1,r0,19 % load integer value
addi r12,r13,4 % loading parameter offset
sw 0(r12),r1 % storing value in function parameter stack
 % ---- function call code beg----
addi r13,r13,8 % allocate stack memory for function calls variables and parameters
jl r11,func0
subi r13,r13,8 % remove declared stack memory for function
add r1,r1,r0 % store return value in register
 % ---- function call code end ----
subi r12,r13,4 % load stack variable offset
sw 0(r12),r1 % store register value into stack variable
subi r12,r13,4
lw r1,0(r12) % load stack variable value in function
jl r15,putint
addi r1,r0,10
putc r1
hlt
func0 % func fib entry point
subi r12,r13,8 % store the return address in stack memory
sw 0(r12),r11
 % ---evaluating LTEQ expression below ---
subi r12,r13,4
lw r1,0(r12) % load stack variable value in function
add r2,r1,r0
subi r8,r8,4
sw topaddr(r8),r2 % store left hand side of relational expression on stack
addi r1,r0,2 % load integer value
lw r2,topaddr(r8)
addi r8,r8,4
cle r1,r2,r1
 % ---evaluating LTEQ expression over ---
bz r1,else0
addi r1,r0,1 % load integer value
subi r12,r13,8 % load return address in r12
lw r11,0(r12)
jr r11
j ifend0
else0
 % ---evaluating ADD expression below ---
 % ---evaluating SUB expression below ---
subi r12,r13,4
lw r2,0(r12) % load stack variable value in function
addi r1,r0,1 % load integer value
sub r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1
 % ---evaluting SUB expression over ---
lw r1,topaddr(r8)
addi r8,r8,4 % remove stack variable from arithmetic expression
addi r12,r13,4 % loading parameter offset
sw 0(r12),r1 % storing value in function parameter stack
 % ---- function call code beg----
addi r13,r13,8 % allocate stack memory for function calls variables and parameters
jl r11,func0
subi r13,r13,8 % remove declared stack memory for function
add r2,r1,r0 % store return value in register
 % ---- function call code end ----
subi r8,r8,4
sw topaddr(r8),r2
 % ---evaluating SUB expression below ---
subi r12,r13,4
lw r2,0(r12) % load stack variable value in function
addi r1,r0,2 % load integer value
sub r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1
 % ---evaluting SUB expression over ---
lw r1,topaddr(r8)
addi r8,r8,4 % remove stack variable from arithmetic expression
addi r12,r13,4 % loading parameter offset
sw 0(r12),r1 % storing value in function parameter stack
 % ---- function call code beg----
addi r13,r13,8 % allocate stack memory for function calls variables and parameters
jl r11,func0
subi r13,r13,8 % remove declared stack memory for function
add r1,r1,r0 % store return value in register
 % ---- function call code end ----
lw r2,topaddr(r8)
addi r8,r8,4
add r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1
 % ---evaluting ADD expression over ---
lw r1,topaddr(r8)
addi r8,r8,4 % remove stack variable from arithmetic expression
subi r12,r13,8 % load return address in r12
lw r11,0(r12)
jr r11
ifend0
putint   add    r2,r0,r0         % c := 0 (character)
         add    r3,r0,r0         % s := 0 (sign)
         addi   r4,r0,endbuf     % p is the buffer pointer
         cge    r5,r1,r0
         bnz    r5,putint1       % branch if n >= 0
         addi   r3,r0,1          % s := 1
         sub    r1,r0,r1         % n := -n
putint1  modi   r2,r1,10         % c := n mod 10
         addi   r2,r2,48         % c := c + '0'
         subi   r4,r4,1          % p := p - 1
         sb     0(r4),r2         % buf[p] := c
         divi   r1,r1,10         % n := n div 10
         bnz    r1,putint1       % do next digit
         bz     r3,putint2       % branch if n >= 0
         addi   r2,r0,45         % c := '-'
         subi   r4,r4,1          % p := p - 1
         sb     0(r4),r2         % buf[p] := c
putint2  lb     r2,0(r4)         % c := buf[p]
         putc   r2               % write c
         addi   r4,r4,1          % p := p + 1
         cgei   r5,r4,endbuf
         bz     r5,putint2       % branch if more digits
         jr     r15              % return

         res    20               % digit buffer
endbuf	 
stack