myProg: assembler.o assemblerHelper.o dataStructures.o firstPass.o outputPrint.o reservedWord.o secondPassHelper.o strHandle.o table_macro.o
	gcc -ansi -Wall -pedantic -g assembler.o assemblerHelper.o dataStructures.o firstPass.o outputPrint.o reservedWord.o secondPassHelper.o strHandle.o table_macro.o -o myProg
assembler.o: assembler.c 
	gcc -c -ansi -Wall -pedantic -g  assembler.c -o assembler.o
assemblerHelper.o: assemblerHelper.c assemblerHelper.h
	gcc -c -ansi -Wall -pedantic -g assemblerHelper.c -o assemblerHelper.o	
strHandle.o: strHandle.c strHandle.h
	gcc -c -ansi -Wall -pedantic -g strHandle.c -o strHandle.o	
dataStructures.o: dataStructures.c dataStructures.h
	gcc -c -ansi -Wall -pedantic -g dataStructures.c -o dataStructures.o
firstPass.o: firstPass.c firstPass.h
	gcc -c -ansi -Wall -pedantic -g firstPass.c -o firstPass.o
outputPrint.o: outputPrint.c outputPrint.h
	gcc -c -ansi -Wall -pedantic -g outputPrint.c -o outputPrint.o
reservedWord.o: reservedWord.c reservedWord.h
	gcc -c -ansi -Wall -pedantic -g reservedWord.c -o reservedWord.o
secondPassHelper.o: secondPassHelper.c secondPassHelper.h
	gcc -c -ansi -Wall -pedantic -g secondPassHelper.c -o secondPassHelper.o
table_macro.o: table_macro.c table_macro.h
	gcc -c -ansi -Wall -pedantic -g table_macro.c -o table_macro.o
