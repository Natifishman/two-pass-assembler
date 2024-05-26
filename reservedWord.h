#ifndef MAMAN14V10_RESERVEDWORDS_H
#define MAMAN14V10_RESERVEDWORDS_H

#include <string.h>
#include "dataStructures.h"

#define numOfCommands 27
#define maxCommandLen 5

/* These are function prototypes for checking the type of instructions and commands in a program. */

/* this int function check if the reserved word is a stop reserved and retiurn's 1 in case it is, zero if not  */
int isStop(char *buffer);

/* Checks if the reserved word is a command. Return 1 if is, 0 if not */
int isCommand(char *buffer);

/* Checks if the reserved word is an instruction and returns 1 if it is, and zero if not */
int isInstruction(char *buffer);

/* Checks if the reserved word is a .extern instruction and returns 1 if it is, and zero if not */
int isExt(char *buffer);

/* this function checks if the reserved word is a .entry instruction and returns 1 if it is, and zero if not */
int isEnt(char *buffer);

/* Checks if the reserved word is a max one word command. */
int isRegister(char *buffer);

/* Checks if the reserved word is a max one word command. */
int isOneType(char *buffer);

/* Checks if the reserved word is a max two words command. */
int isTwoType(char *buffer);

/* Checks if the reserved word is a max three words command. */
int isThreeType(char *buffer);

/* Checks if the reserved word is a max four words command. */
int isFourType(char *buffer);

/* Checks if the reserved word is a max two words command. */
void getOpcode(line *linePointer, int lineInd, char *buffer);

#endif
