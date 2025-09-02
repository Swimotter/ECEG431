// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
// The algorithm is based on repetitive addition.

// zero the output
@R2
M=0

// load, R0 check if 0 mult
@R0
D=M
@END
D;JLE

// load, R1 check if 0 mult
@R1
D=M
@END
D;JLE

(MULLOOP)
@R1
D=M
@R2
M=D+M
@R0
M=M-1
D=M
@MULLOOP
D;JGT

(END)
@END
0;JMP
