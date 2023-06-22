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
// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// eq
@64
D=A
@R13
M=D
@EQ
0;JMP
// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 16
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
// eq
@84
D=A
@R13
M=D
@EQ
0;JMP
// push constant 16
@16
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 17
@17
D=A
@SP
A=M
M=D
@SP
M=M+1
// eq
@104
D=A
@R13
M=D
@EQ
0;JMP
// push constant 892
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
// lt
@124
D=A
@R13
M=D
@LT
0;JMP
// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 892
@892
D=A
@SP
A=M
M=D
@SP
M=M+1
// lt
@144
D=A
@R13
M=D
@LT
0;JMP
// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 891
@891
D=A
@SP
A=M
M=D
@SP
M=M+1
// lt
@164
D=A
@R13
M=D
@LT
0;JMP
// push constant 32767
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
// gt
@184
D=A
@R13
M=D
@GT
0;JMP
// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 32767
@32767
D=A
@SP
A=M
M=D
@SP
M=M+1
// gt
@204
D=A
@R13
M=D
@GT
0;JMP
// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 32766
@32766
D=A
@SP
A=M
M=D
@SP
M=M+1
// gt
@224
D=A
@R13
M=D
@GT
0;JMP
// push constant 57
@57
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 31
@31
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 53
@53
D=A
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
// push constant 112
@112
D=A
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
// neg
@SP
A=M-1
M=-M
// and
@SP
M=M-1
A=M
D=M
A=A-1
M=M&D
// push constant 82
@82
D=A
@SP
A=M
M=D
@SP
M=M+1
// or
@SP
M=M-1
A=M
D=M
A=A-1
M=M|D
// not
@SP
A=M-1
M=!M
