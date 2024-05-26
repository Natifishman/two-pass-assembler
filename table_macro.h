#ifndef OPENU_2PASS_ASSEMBLER_MAIN_TABLE_MACRO_H
#define OPENU_2PASS_ASSEMBLER_MAIN_TABLE_MACRO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dataStructures.h"

/* Macro's commands */
typedef struct CommandNode {
    char *command;
    struct CommandNode *next;
} CommandNode;

/* Macros */
typedef struct MacroNode {
    char *name;
    CommandNode *commands;
    struct MacroNode *next;
} MacroNode;

void addMacroNode(MacroNode **head, char *name);

void addCommandToMacro(MacroNode *head, char *name, char *command);

/* Checks if a given name is already exist */
myBool isMacroName(MacroNode *head, char *macro_name);

void printCommands(MacroNode *head, char *macro_name, FILE *file_ptr);

#endif
