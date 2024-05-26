# OpenU_Assembler
2-PASS Assembler, wrriten in C language based on mastering bit-field methods, memory management and division into many functions and files.


Project file’s guid:

assembler.c - the file that contain’s the main function,
“executePasses” that run’s the entire project.

assemblerHelper.c & assemblerHelper.h  - the file that contains the functions that check the argument line, and the loops that go trough the first and the second pass.

dataStructure.c & dataStructure.h - the file’s that contains the line and label struct’s, their flags
and the function’s that initialize them.

reservedWords.c & reservedWords.h - the file’s that contains all the function’s and array’s that help to find out line attribute’s by their reserved word- such as their  opcode, num Of Arguments, type of argument etc..


strHandle.c & strHandle.h - the files that contains 
all the parsing functions.

firstPass.c & firstPass.h - the file’s that control’s the first pass loop and calls the relevant function’s.
 
secondPassHelper.c & secondPassHelper.h - the file’s that contains all second pass function’s that are called from assemblerHelper.c .

outputPrint.c & outputPrint.h - the file’s that contain all printing function’s for every line type and resposible for creating the output file’s.



