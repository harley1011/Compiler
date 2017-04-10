program entry
addi r13,r0,stack
addi r13,r13,420 % allocate stack memory for program
jl r15,getint
subi r12,r13,420 % load stack variable offset
sw 0(r12),r1 % store register value into stack variable
 % ---evaluating LT expression below ---
subi r12,r13,420
lw r1,0(r12) % load stack variable value in function
add r2,r1,r0
subi r8,r8,4
sw topaddr(r8),r2 % store left hand side of relational expression on stack
addi r1,r0,100 % load integer value
lw r2,topaddr(r8)
addi r8,r8,4
clt r1,r2,r1
 % ---evaluating LT expression over ---
bz r1,else0
addi r1,r0,99 % load integer value
subi r12,r13,420 % load stack variable offset
sw 0(r12),r1 % store register value into stack variable
j ifend0
else0
ifend0
addi r1,r0,0 % load integer value
subi r12,r13,16 % load stack variable offset
sw 0(r12),r1 % store register value into stack variable
for0
 % ---evaluating LT expression below ---
subi r12,r13,16
lw r1,0(r12) % load stack variable value in function
add r2,r1,r0
subi r8,r8,4
sw topaddr(r8),r2 % store left hand side of relational expression on stack
subi r12,r13,420
lw r1,0(r12) % load stack variable value in function
lw r2,topaddr(r8)
addi r8,r8,4
clt r1,r2,r1
 % ---evaluating LT expression over ---
bz r1,forend0
 % ---evaluating ADD expression below ---
subi r12,r13,16
lw r2,0(r12) % load stack variable value in function
addi r1,r0,1 % load integer value
add r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1
 % ---evaluting ADD expression over ---
lw r1,topaddr(r8)
addi r8,r8,4 % remove stack variable from arithmetic expression
subi r12,r13,16 % load stack variable offset
sw 0(r12),r1 % store register value into stack variable
jl r15,getint
add r5,r0,r1
subi r12,r13,416 % load stack array offset
add r12,r12,r9
sw 0(r12),r5 % store register value in stack array index
j for0
forend0
jl r15,getint
subi r12,r13,12 % load stack variable offset
sw 0(r12),r1 % store register value into stack variable
subi r1,r13,416 % load stack array address
addi r12,r13,4 % loading parameter offset
sw 0(r12),r1 % storing value in function parameter stack
subi r12,r13,420
lw r1,0(r12) % load stack variable value in function
addi r12,r13,404 % loading parameter offset
sw 0(r12),r1 % storing value in function parameter stack
subi r12,r13,12
lw r1,0(r12) % load stack variable value in function
addi r12,r13,408 % loading parameter offset
sw 0(r12),r1 % storing value in function parameter stack
 % ---- function call code beg----
addi r13,r13,20 % allocate stack memory for function calls variables and parameters
jl r11,func0
subi r13,r13,20 % remove declared stack memory for function
add r1,r1,r0 % store return value in register
 % ---- function call code end ----
subi r12,r13,8 % load stack variable offset
sw 0(r12),r1 % store register value into stack variable
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
subi r12,r13,420
lw r1,0(r12) % load stack variable value in function
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
subi r12,r13,8
lw r1,0(r12) % load stack variable value in function
muli r7,r7,100
muli r1,r1,4 % multiply last evaluated expression stored in r1 by the arrays type size
add r7,r7,r1 % add new dimension access to current offset counter
add r9,r9,r7 % add array dimension calculation with other properties
 % --- compute index over ---
subi r12,r13,416 % load stack array  offset
add r12,r12,r9
lw r1,0(r12) % load stack array index value in function
jl r15,putint
addi r1,r0,10
putc r1
j for1
forend1
hlt
func0 % func addValueToArray entry point
subi r12,r13,20 % store the return address in stack memory
sw 0(r12),r11
addi r1,r0,0 % load integer value
subi r12,r13,-392 % load stack variable offset
sw 0(r12),r1 % store register value into stack variable
for2
 % ---evaluating LT expression below ---
subi r12,r13,-392
lw r1,0(r12) % load stack variable value in function
add r2,r1,r0
subi r8,r8,4
sw topaddr(r8),r2 % store left hand side of relational expression on stack
subi r12,r13,-384
lw r1,0(r12) % load stack variable value in function
lw r2,topaddr(r8)
addi r8,r8,4
clt r1,r2,r1
 % ---evaluating LT expression over ---
bz r1,forend2
 % ---evaluating ADD expression below ---
subi r12,r13,-392
lw r2,0(r12) % load stack variable value in function
addi r1,r0,1 % load integer value
add r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1
 % ---evaluting ADD expression over ---
lw r1,topaddr(r8)
addi r8,r8,4 % remove stack variable from arithmetic expression
subi r12,r13,-392 % load stack variable offset
sw 0(r12),r1 % store register value into stack variable
 % ---evaluating ADD expression below ---
 % --- compute index value ---
add r9,r0,r0 % clear array index register
add r7,r0,r0
subi r12,r13,-392
lw r1,0(r12) % load stack variable value in function
muli r7,r7,100
muli r1,r1,4 % multiply last evaluated expression stored in r1 by the arrays type size
add r7,r7,r1 % add new dimension access to current offset counter
add r9,r9,r7 % add array dimension calculation with other properties
 % --- compute index over ---
subi r12,r13,16 % load stack array  offset
add r12,r12,r9
lw r2,0(r12) % load stack array index value in function
subi r12,r13,-388
lw r1,0(r12) % load stack variable value in function
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
subi r12,r13,-392
lw r1,0(r12) % load stack variable value in function
muli r7,r7,100
muli r1,r1,4 % multiply last evaluated expression stored in r1 by the arrays type size
add r7,r7,r1 % add new dimension access to current offset counter
add r9,r9,r7 % add array dimension calculation with other properties
 % --- compute index over ---
subi r12,r13,16 % load stack array offset
lw r12,0(r12) % load array address
add r12,r12,r9
sw 0(r12),r5 % store register value in stack array index
j for2
forend2
addi r1,r0,0 % load integer value
subi r12,r13,20 % load return address in r12
lw r11,0(r12)
jr r11
getint   add    r1,r0,r0         % n := 0 (result)
         add    r2,r0,r0         % c := 0 (character)
         add    r3,r0,r0         % s := 0 (sign)
getint1  getc   r2               % read c
         ceqi   r4,r2,32
         bnz    r4,getint1       % skip blanks
         ceqi   r4,r2,43
         bnz    r4,getint2       % branch if c is '+'
         ceqi   r4,r2,45
         bz     r4,getint3       % branch if c is not '-'
         addi   r3,r0,1          % s := 1 (number is negative)
getint2  getc   r2               % read c
getint3  ceqi   r4,r2,10
         bnz    r4,getint5       % branch if c is \n
         cgei   r4,r2,48
         bz     r4,getint4       % c < 0
         clei   r4,r2,57
         bz     r4,getint4       % c > 9
         muli   r1,r1,10         % n := 10 * n
         add    r1,r1,r2         % n := n + c
         subi   r1,r1,48         % n := n - '0'
         j      getint2
getint4  addi   r2,r0,63         % c := '?'
         putc   r2               % write c
         j      getint           % Try again
getint5  bz     r3,getint6       % branch if s = 0 (number is positive)
         sub    r1,r0,r1         % n := -n
getint6  jr     r15              % return
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