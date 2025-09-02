// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

// R0 - current screen pos in RAM
// R1 - screen color (black: -1, white: 0)

// Infinite loop
(LOOP)
@SCREEN // A=RAM[16384]
D=A
@R0 // A=RAM[0]
M=D

// Default to black
@R1
M=-1

// Check input
@KBD // A=RAM[24576]
D=M
@FILL
D;JNE // Key not pressed

// Set to white
@R1
M=0

// Fill/clear screen
(FILL)
// Fetch the color to fill
@R1
D=M

// Inc screen counter, set color
@R0 // A=RAM[0]
A=M // A=RAM[RAM[0]]
M=D

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