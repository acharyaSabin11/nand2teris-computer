@256
D=A
@SP
M=D
@return1
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@SP
M=M+1
A=M-1
M=D
@ARG
D=M
@SP
M=M+1
A=M-1
M=D
@THIS
D=M
@SP
M=M+1
A=M-1
M=D
@THAT
D=M
@SP
M=M+1
A=M-1
M=D
@5
D=A
@0
D=A+D
@SP
A=M
D=A-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@sys.init
0;JMP
(return1)
// Starter boiler code for conditional checks and return
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
(RETURN)
@LCL
D=M
@endFrame
M=D
@5
A=D-A
D=M
@retAddr
M=D
@SP
M=M-1
A=M
D=M
@ARG
A=M
M=D
@ARG
D=M+1
@SP
M=D
@endFrame
A=M-1
D=M
@THAT
M=D
@2
D=A
@endFrame
A=M-D
D=M
@THIS
M=D
@3
D=A
@endFrame
A=M-D
D=M
@ARG
M=D
@4
D=A
@endFrame
A=M-D
D=M
@LCL
M=D
@retAddr
A=M
0;JMP
// function main.fibonacci 0
(main.fibonacci)
@SP
A=M
D=A
@SP
M=D
// push argument 0
@0
D=A
@ARG
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 2
@2
D=A
@SP
A=M
M=D
@SP
M=M+1
// lt
@168
D=A
@R13
M=D
@LT
0;JMP
// if-goto if_true
@SP
M=M-1
A=M
D=M
@if_true
D;JNE
// goto if_false
@if_false
0;JMP
// label if_true
(if_true)
// push argument 0
@0
D=A
@ARG
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
// return
@RETURN
0;JMP
// label if_false
(if_false)
// push argument 0
@0
D=A
@ARG
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 2
@2
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
//Call main.fibonacci
@return2
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@SP
M=M+1
A=M-1
M=D
@ARG
D=M
@SP
M=M+1
A=M-1
M=D
@THIS
D=M
@SP
M=M+1
A=M-1
M=D
@THAT
D=M
@SP
M=M+1
A=M-1
M=D
@5
D=A
@1
D=A+D
@SP
A=M
D=A-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@main.fibonacci
0;JMP
(return2)
// push argument 0
@0
D=A
@ARG
A=M+D
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 1
@1
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
//Call main.fibonacci
@return3
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@SP
M=M+1
A=M-1
M=D
@ARG
D=M
@SP
M=M+1
A=M-1
M=D
@THIS
D=M
@SP
M=M+1
A=M-1
M=D
@THAT
D=M
@SP
M=M+1
A=M-1
M=D
@5
D=A
@1
D=A+D
@SP
A=M
D=A-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@main.fibonacci
0;JMP
(return3)
// add
@SP
M=M-1
A=M
D=M
A=A-1
M=M+D
// return
@RETURN
0;JMP
// function sys.init 0
(sys.init)
@SP
A=M
D=A
@SP
M=D
// push constant 4
@4
D=A
@SP
A=M
M=D
@SP
M=M+1
//Call main.fibonacci
@return4
D=A
@SP
M=M+1
A=M-1
M=D
@LCL
D=M
@SP
M=M+1
A=M-1
M=D
@ARG
D=M
@SP
M=M+1
A=M-1
M=D
@THIS
D=M
@SP
M=M+1
A=M-1
M=D
@THAT
D=M
@SP
M=M+1
A=M-1
M=D
@5
D=A
@1
D=A+D
@SP
A=M
D=A-D
@ARG
M=D
@SP
D=M
@LCL
M=D
@main.fibonacci
0;JMP
(return4)
// label while
(while)
// goto while
@while
0;JMP
