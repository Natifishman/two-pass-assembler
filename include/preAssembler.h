#ifndef OPENU_2PASS_ASSEMBLER_MAIN_PREASSEMBLER_H
#define OPENU_2PASS_ASSEMBLER_MAIN_PREASSEMBLER_H

#include "assemblerHelper.h"
#include "strHandle.h"
#include "dataStructures.h"
#include "table_macro.h"

#define amEndingLen 4
#define LINE_LENGTH 81

char getExistMcrName(char line);

myBool isExistMcr(MacroNode *macroPointer, char mcr_name);

myBool isMcr(char line);

char getMcrName(char line);

/* This function checks if the given line contains only the string "endmcr" */
myBool isEndMcr(char *line);

void preAssembler(FILE *srcFile, FILE *mcrFile);

#endif /* OPENU_2PASS_ASSEMBLER_MAIN_PREASSEMBLER_H */
