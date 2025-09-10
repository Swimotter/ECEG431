// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/FillStatic.asm

// Blackens the screen, i.e. writes "black" in every pixel. 
// Key presses are ignored.
// This is an intermediate step added to help you out.

// Infinite loop
(LOOP)
@SCREEN // A=RAM[16384]
D=A
@R0 // A=RAM[0]
M=D

(FILL)
// Inc screen counter, set color
@R0 // A=RAM[0]
A=M // A=RAM[RAM[0]]
M=-1

@R0 // A=RAM[0]
M=M+1 // Where to update next tick
D=M

// Check if screen filled
@KBD // A=RAM[24576]
D=A-D

//if R0==@KBD, the screen was completely filled
@FILL
D;JGT

@LOOP
0;JMP