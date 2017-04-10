program entry
addi r13,r0,stack
addi r13,r13,84 % allocate stack memory for program
addi r1,r0,10 % load integer value
add r5,r0,r1
subi r12,r13,44 % load stack class data member offset
add r12,r12,r9
sw 0(r12),r5 % store register value into stack class data member
addi r1,r0,5 % load integer value
subi r12,r13,16 % load stack variable offset
sw 0(r12),r1 % store register value into stack variable
addi r1,r0,5 % load integer value
subi r12,r13,40 % load stack variable offset
sw 0(r12),r1 % store register value into stack variable
addi r1,r0,0 % load integer value
subi r12,r13,8 % load stack variable offset
sw 0(r12),r1 % store register value into stack variable
for0
 % ---evaluating LT expression below ---
subi r12,r13,8
lw r1,0(r12) % load stack variable value in function
add r2,r1,r0
subi r8,r8,4
sw topaddr(r8),r2 % store left hand side of relational expression on stack
addi r1,r0,5 % load integer value
lw r2,topaddr(r8)
addi r8,r8,4
clt r1,r2,r1
 % ---evaluating LT expression over ---
bz r1,forend0
 % ---evaluating ADD expression below ---
subi r12,r13,8
lw r2,0(r12) % load stack variable value in function
addi r1,r0,1 % load integer value
add r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1
 % ---evaluting ADD expression over ---
lw r1,topaddr(r8)
addi r8,r8,4 % remove stack variable from arithmetic expression
subi r12,r13,8 % load stack variable offset
sw 0(r12),r1 % store register value into stack variable
 % ---evaluating ADD expression below ---
subi r12,r13,8
lw r2,0(r12) % load stack variable value in function
addi r1,r0,5 % load integer value
add r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1
 % ---evaluting ADD expression over ---
lw r1,topaddr(r8)
addi r8,r8,4 % remove stack variable from arithmetic expression
add r5,r0,r1
 % --- compute index value ---
add r9,r0,r0 % clear array index register
add r7,r0,r0
subi r12,r13,8
lw r1,0(r12) % load stack variable value in function
muli r7,r7,5
muli r1,r1,4 % multiply last evaluated expression stored in r1 by the arrays type size
add r7,r7,r1 % add new dimension access to current offset counter
add r9,r9,r7 % add array dimension calculation with other properties
 % --- compute index over ---
subi r12,r13,36 % load stack array offset
add r12,r12,r9
sw 0(r12),r5 % store register value in stack array index
subi r12,r13,8
lw r1,0(r12) % load stack variable value in function
add r5,r0,r1
 % --- compute index value ---
add r9,r0,r0 % clear array index register
add r7,r0,r0
subi r12,r13,8
lw r1,0(r12) % load stack variable value in function
muli r7,r7,5
muli r1,r1,44 % multiply last evaluated expression stored in r1 by the arrays type size
add r7,r7,r1 % add new dimension access to current offset counter
add r9,r9,r7 % add array dimension calculation with other properties
 % --- compute index over ---
subi r12,r13,84 % load stack class data member offset
add r12,r12,r9
sw 0(r12),r5 % store register value into stack class data member
j for0
forend0
addi r1,r0,0 % load integer value
subi r12,r13,4 % load stack variable offset
sw 0(r12),r1 % store register value into stack variable
for1
 % ---evaluating LT expression below ---
subi r12,r13,4
lw r1,0(r12) % load stack variable value in function
add r2,r1,r0
subi r8,r8,4
sw topaddr(r8),r2 % store left hand side of relational expression on stack
addi r1,r0,5 % load integer value
lw r2,topaddr(r8)
addi r8,r8,4
clt r1,r2,r1
 % ---evaluating LT expression over ---
bz r1,forend1
 % ---evaluating ADD expression below ---
subi r12,r13,4
lw r2,0(r12) % load stack variable value in function
addi r1,r0,1 % load integer value
add r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1
 % ---evaluting ADD expression over ---
lw r1,topaddr(r8)
addi r8,r8,4 % remove stack variable from arithmetic expression
subi r12,r13,4 % load stack variable offset
sw 0(r12),r1 % store register value into stack variable
 % --- compute index value ---
add r9,r0,r0 % clear array index register
add r7,r0,r0
subi r12,r13,4
lw r1,0(r12) % load stack variable value in function
muli r7,r7,5
muli r1,r1,4 % multiply last evaluated expression stored in r1 by the arrays type size
add r7,r7,r1 % add new dimension access to current offset counter
add r9,r9,r7 % add array dimension calculation with other properties
 % --- compute index over ---
subi r12,r13,36 % load stack array  offset
add r12,r12,r9
lw r1,0(r12) % load stack array index value in function
jl r15,putint
addi r1,r0,10
putc r1
j for1
forend1
 % ---evaluating ADD expression below ---
addi r2,r0,5 % load integer value
subi r8,r8,4
sw topaddr(r8),r2
addi r1,r0,10 % load integer value
addi r12,r13,4 % loading parameter offset
sw 0(r12),r1 % storing value in function parameter stack
addi r1,r0,5 % load integer value
addi r12,r13,8 % loading parameter offset
sw 0(r12),r1 % storing value in function parameter stack
 % ---- function call code beg----
addi r13,r13,12 % allocate stack memory for function calls variables and parameters
jl r11,func0
subi r13,r13,12 % remove declared stack memory for function
add r1,r1,r0 % store return value in register
 % ---- function call code end ----
lw r2,topaddr(r8)
addi r8,r8,4
add r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1
 % ---evaluting ADD expression over ---
 % ---evaluating ADD expression below ---
 % --- compute index value ---
add r9,r0,r0 % clear array index register
add r7,r0,r0
addi r1,r0,2 % load integer value
muli r7,r7,5
muli r1,r1,44 % multiply last evaluated expression stored in r1 by the arrays type size
add r7,r7,r1 % add new dimension access to current offset counter
add r9,r9,r7 % add array dimension calculation with other properties
 % --- compute index over ---
subi r12,r13,84 % load stack class data member offset
add r12,r12,r9
lw r2,0(r12) % load stack class data member into register
lw r1,topaddr(r8)
addi r8,r8,4 % remove stack variable from arithmetic expression
add r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1
 % ---evaluting ADD expression over ---
 % ---evaluating ADD expression below ---
 % --- compute index value ---
add r9,r0,r0 % clear array index register
add r7,r0,r0
 % ---evaluating SUB expression below ---
 % --- compute index value ---
add r9,r0,r0 % clear array index register
add r7,r0,r0
addi r1,r0,0 % load integer value
muli r7,r7,5
muli r1,r1,4 % multiply last evaluated expression stored in r1 by the arrays type size
add r7,r7,r1 % add new dimension access to current offset counter
add r9,r9,r7 % add array dimension calculation with other properties
 % --- compute index over ---
subi r12,r13,36 % load stack array  offset
add r12,r12,r9
lw r2,0(r12) % load stack array index value in function
addi r1,r0,2 % load integer value
sub r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1
 % ---evaluting SUB expression over ---
lw r1,topaddr(r8)
addi r8,r8,4 % remove stack variable from arithmetic expression
muli r7,r7,5
muli r1,r1,4 % multiply last evaluated expression stored in r1 by the arrays type size
add r7,r7,r1 % add new dimension access to current offset counter
add r9,r9,r7 % add array dimension calculation with other properties
 % --- compute index over ---
subi r12,r13,36 % load stack array  offset
add r12,r12,r9
lw r2,0(r12) % load stack array index value in function
lw r1,topaddr(r8)
addi r8,r8,4 % remove stack variable from arithmetic expression
add r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1
 % ---evaluting ADD expression over ---
lw r1,topaddr(r8)
addi r8,r8,4 % remove stack variable from arithmetic expression
subi r12,r13,40 % load stack variable offset
sw 0(r12),r1 % store register value into stack variable
subi r12,r13,40
lw r1,0(r12) % load stack variable value in function
jl r15,putint
addi r1,r0,10
putc r1
hlt
func0 % func divide entry point
subi r12,r13,12 % store the return address in stack memory
sw 0(r12),r11
 % ---evaluating DIV expression below ---
subi r12,r13,8
lw r2,0(r12) % load stack variable value in function
subi r12,r13,4
lw r1,0(r12) % load stack variable value in function
div r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1
 % ---evaluting DIV expression over ---
lw r1,topaddr(r8)
addi r8,r8,4 % remove stack variable from arithmetic expression
subi r12,r13,12 % load return address in r12
lw r11,0(r12)
jr r11
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