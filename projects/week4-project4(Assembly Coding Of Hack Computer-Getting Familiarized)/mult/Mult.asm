// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

// Put your code here.

//Initializing sum variable to zero.(Initialization to zero may be optional but i prefer to add it)
@sum
M=0

//Jumping to end if the difference of the register R1 and counter is less than or equal to zero.
//looping for addition
(LOOP)
@R1
D=M
//if Decremented counter = 0;
@END
D;JEQ


//loading register R0 From RAM
@R0
D=M
//summing
@sum
M=D + M
//decrementing counter
@R1
M=M-1
//unconditional jump
@LOOP
0;JMP


//End: infinite loop after saving data to R2 in RAM

(END)
//putting the sum to register R2 in RAM
@sum
D=M
@R2
M=D
//Ending the process via infinite loop
(INFINITE)
@INFINITE
0;JMP

