program entry
addi r13,r0,stack
addi r13,r13,4

addi r2,r0,10
addi r1,r0,12
add r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1


addi r2,r0,5
lw r1,topaddr(r8)
addi r8,r8,4
mul r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1


lw r2,topaddr(r8)
addi r8,r8,4
addi r1,r0,200
sub r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1


addi r2,r0,112
addi r1,r0,21
add r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1


lw r2,topaddr(r8)
addi r8,r8,4
addi r1,r0,32
mul r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1


lw r2,topaddr(r8)
addi r8,r8,4
lw r1,topaddr(r8)
addi r8,r8,4
add r1,r2,r1
subi r8,r8,4
sw topaddr(r8),r1

lw r1,topaddr(r8)
addi r8,r8,4
subi r12,r13,4
sw 0(r12),r1
subi r12,r13,4
lw r1,0(r12)
jl r15,putint
addi r1,r0,10
putc r1
hlt
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