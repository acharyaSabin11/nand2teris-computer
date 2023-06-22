// Starter boiler code for conditional checks
@START
0; JMP
(setTrue)
@SP
A=M-1
M=-1
@R13
A=M
0;JMP
(setFalse)
@SP
A=M-1
M=0
@R13
A=M
0;JMP
(EQ)
@SP
M=M-1
A=M
D=M
A=A-1
D=M-D
@setTrue
D;JEQ
@setFalse
0;JMP
(GT)
@SP
M=M-1
A=M
D=M
A=A-1
D=M-D
@setTrue
D;JGT
@setFalse
0;JMP
(LT)
@SP
M=M-1
A=M
D=M
A=A-1
D=M-D
@setTrue
D;JLT
@setFalse
0;JMP
(START)
// push constant 111
@111
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 333
@333
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 888
@888
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop static 8
@SP
M=M-1
A=M
D=M
@fileName.8
M=D
// pop static 3
@SP
M=M-1
A=M
D=M
@fileName.3
M=D
// pop static 1
@SP
M=M-1
A=M
D=M
@fileName.1
M=D
// push static 3
@fileName.3
D=M
@SP
A=M
M=D
@SP
M=M+1
// push static 1
@fileName.1
D=M
@SP
A=M
M=D
@SP
M=M+1
// sub
@SP
M=M-1
A=M
D=M
A=A-1
M=M-D
// push static 8
@fileName.8
D=M
@SP
A=M
M=D
@SP
M=M+1
// add
@SP
M=M-1
A=M
D=M
A=A-1
M=M+D
