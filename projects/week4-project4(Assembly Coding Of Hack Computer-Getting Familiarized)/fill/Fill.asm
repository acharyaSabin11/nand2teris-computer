// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

//NOTE: For this program to work, we need to access the R1 register in ram that must store the value 8192


//Defining an infinite loop that looks for keyboard input throughout the program
(INFINITE)
//Initializing counter to zero
@R1
D=M
@R0
M=D
@counter
M=0
@KBD
D=M;
@LOOPINITBLACK
D;JGT    
@LOOPINITWHITE
0;JMP
//To display black screen
(LOOPINITBLACK)
@SCREEN //loading screen address
D=A;
@counter //setting counter to screen address
M=D;

(LOOPBLACK)
@counter
D=M;    //loading address of current memory location of screen
A=D;    //loading that address into address register
M=-1    //putting content of that memory to -1 which corresponds to 1111111111111111
@counter    
M=M+1   //incrementing counter to get next memory location
@R0
D=M-1   //Decrementing the R0 to get view of whether all the registers of SCREEN block is finished looping. The looping finished is checked below and directed accordingly.
M=D
@LOOPBLACK
D;JGT
@INFINITE   //Going to initial state when the loop is finished
0;JMP

//TO display white screen
(LOOPINITWHITE)
@SCREEN //loading screen address
D=A;
@counter //setting counter to screen address
M=D;

(LOOPWHITE)
@counter
D=M;    //loading address of current memory location of screen
A=D;    //loading that address into address register
M=0    //putting content of that memory to 0 which corresponds to 0000000000000000
@counter
M=M+1   //incrementing counter to get next memory location
@R0
D=M-1   //Decrementing the R0 to get view of whether all the registers of SCREEN block is finished looping. The looping finished is checked below and directed accordingly.
M=D
@LOOPWHITE  
D;JGT
@INFINITE   //Going to initial state when the loop is finished
0;JMP

