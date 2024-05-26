#ifndef MAMAN14V10_ASSEMBLERHELPER_H
#define MAMAN14V10_ASSEMBLERHELPER_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "assemblerHelper.h"
#include "strHandle.h"
#include "firstPass.h"
#include "dataStructures.h"
#include "secondPassHelper.h"
#include "outputPrint.h"
#include "preAssembler.h"
#include "table_macro.h"

#define tabAscii 9
#define spaceAscii 32
#define maxLineLen 81
#define maxFileEndingLen 4
#define pointAscii 46
#define dotAmEndingLen 4

void execute(char **, int);

char *createDotAmFile( char *inputFileName);

int validInputFile(char *);

int validMacroFile(char *);

void errorSpecifier(line * , int, int);

#endif
