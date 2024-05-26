#ifndef MAMAN14V10_STRHANDLE_H
#define MAMAN14V10_STRHANDLE_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "reservedWord.h"
#include "dataStructures.h"

#define maxLabelLen 31
#define maxLineLen 81
#define capitalAascii 65
#define hashtagAscii 35
#define bracketAscii 40
#define endBracketAscii 41
#define capitalZascii 90
#define lowCap_a_ascii 97
#define lowCap_z_ascii 122
#define digitsStart 48
#define digitsEnd 57
#define colonAscii 58
#define maxInstOrCommLen 8
#define highestValue +2147483647
#define lowestValue -2147483648
#define plusAscii 43
#define minusAscii 45
#define regLen 3
#define numMaxLen 7

#define jmpOpcode 30
#define tabAscii 9
#define spaceAscii 32
#define semiColonAscii 59
#define parenthesesAscii 34
#define stopOpCode 63
#define commaAscii 44

#define dataMaxArgLen 7
#define dataMax 16383
#define dataMin -16384

enum operandTypes {
    ERROR, LABEL, NUM, REG
} operandTypes;

#include "strHandle.h"

/* this function initialize an ind variable and returns the first nonBlank char ind */
int nonBlankInd(char *buffer);

/* this int function checks if the line is a blank line */
int isBlank(char *buffer);

/* this int function checks if the line is a comment line */
int isComment(char *buffer);

/* start Label analyze function */
/* Checks if there is an option for a label in current line */
void isLabel(line *linePointer, label *labelPointer, char *buffer, int lineInd, int labelInd);

/* this function check if the label is valid and raise an error flag in case its not */
void labelTest(line *linePointer, label *labelPointer, int lineInd, int labelInd, char *labelBuffer, int labelBufInd);

/* this function check if current label char is valid by her ascii*/
int labelCharTest(char *buffer, int i);

/* this function check if current label digit is valid by her ascii */
int labelDigitsCheck(char *buffer, int i);

/* this function check if the label was already defined */
int labelDuplicationCheck(label *labelPointer, int labelInd, char *givenLabel);

/* this function check if label has colon in the end */
int labelColonCheck(char *buffer, int labelLength);

void raiseLabelError(line *linePointer, int lineInd);

/* end of label functions */

/* First function called from first pass */
/* Checks if the user chose to enter a line without a label in the beginning */
void validFirstWord(line *linePointer, int lineInd, char *buffer, int *IC, int *DC);

/* Checks if the first word has a colon, if there is, its suspected as a label */
int lastCharColonChecker(char *buffer);

/* a function that raise first the Word Relevant Flags */
void firstWordRelevantFlags(line *linePointer, int lineInd, char *buffer, int *IC, int *DC);

/* this function will help to detect the exact type of instruction */
void raiseInstructionFlag(line *linePointer, int lineInd,
                          char *buffer); /* end of function's that checks if the user chose to enter a line without a label in the beginning. */

/*the start of getting command or instruction from a line that contains a label functions*/
/* this function is responsible to check the reserved word in line without a label */
void
reservedWithLabel(line *linePointer, label *labelPointer, char *buffer, int lineInd, int labelInd, int *IC, int *DC);

/* this function returns command or instruction starting index */
int reserved_Ind_WLabel(char *buffer);

/* a function that checks if the word is reserved and raise a relevant flag */
void
ReservedRelevantFlagsWLabel(line *linePointer, label *labelPointer, int lineInd, int labelInd, char *buffer, int *IC,
                            int *DC);

/* Raise the relevant type flag when a line holds a valid command reserved word */
void raiseTypeFlags(line *linePointer, int lineInd, char *buffer); /* End of raiseTypeFlags */

/* valid arguments functions */

/* a function that checks if label that was given as an argument is valid */
int argLabelCheck(char *buffer);

int firstArgIndWLabel(char *buffer);

int firstArgIndNoLabel(char *buffer);

/* command argFunctions */
/* this function checks if there are more non-blank chars between the operand and the comma */
int iterateUntilComma(line *linePointer, int lineInd, char *buffer, int ind);

/* this function checks if there are more non-blank chars between the operand and the bracket */
int iterateUntilBrackets(line *linePointer, int lineInd, char *buffer, int ind);

/* this function checks if there are more non-blank chars between the operand and the bracket */
int iterateUntilEndBrackets(line *linePointer, int lineInd, char *buffer, int ind);

/* this function checks if there are more non-blank chars between the operand and the string ending char */
void iterateUntilEnd(line *linePointer, int lineInd, char *buffer, int ind);

/* a function that was made to raise the extraneous_operand flag in case of entering operands needed */
void raiseExtraneousOperandFlags(line *linePointer, int lineInd);

void raiseMissingOperandFlags(line *linePointer, int lineInd);

/* Get the reserved word index when there is no label */
int reservedIndNoLabel(char *buffer);

/* Raise all command's argument flags in order to handle multiple number of commands in one function */
void setCommandType(line *linePointer, int lineInd, char *buffer);

/* Check all the command's arguments and code them. if there are an error,raise an error flag */
void commandArgsTest(line *linePointer, int lineInd, label *labelPointer, int labelInd, char *buffer);

/* this function check the One word commands(zero operands) */
void checkAndEncodeOneTypeArgs(line *linePointer, int lineInd, char *buffer);

/* Check and code 1-operand argument with max 4 words */
void checkAndEncodeFourTypeArgs(line *linePointer, int lineInd, label *labelPointer, int labelInd, char *buffer);

/* Codes a valid label */
void isValidLabel(line *linePointer, int lineInd, label *labelPointer, int labelInd, char *buffer);

int is_valid_register(char *str);

/* Checks by Operand's length */
int CheckOperand(line *linePointer, int lineInd, char *buffer, int firstArgInd);

/* this function fillUp a buffer with the requested content */
int fillUpOpBuffer(line *linePointer, int lineInd, char *buffer, char *opBuffer, int bufferLen, int firstArgInd, int isReg, int isNum, int isLabel
);

/* Code the valid number, raise an error flag if it isn't valid */
myBool isNumberOneOp(line * linePointer, int
lineInd,
char *buffer
);

/* Code the valid number, raise an error flag if it isn't valid */
myBool isNumberTwoOp(line * linePointer, int
lineInd,
char *buffer
);

void checkStop(line * linePointer, int
lineInd,
char *buffer,
int firstArgInd
);

/* valid extern functions */
/* checking an extern line with a label */
void extArgCheckWLabel(line * linePointer, label * labelPointer, int
lineInd,
int labelInd,
char *buffer
);

/* checking an extern line without a label */
void extArgCheckNoLabel(line * linePointer, label * labelPointer, int
lineInd,
int labelInd,
char *buffer
);

int extArgDuplication(label *labelPointer, int labelInd, char *buffer);

/* this function check .extern argument */
void extArgTest(line * linePointer, label * labelPointer, int
lineInd,
int labelInd,
char *buffer
);

/* entry label saving functions */
/* Copies entry's argument in order to check if it is valid in the second pass */
void handleEntryArg(line * linePointer, int
lineInd,
char *buffer
);

/* instructions args functions */
/* Raise an error flags in case of invalid .String char argument */
void raiseStringErrorFlags(line * linePointer, int
lineInd);

/* this function raise a error flags in case of invalid .d argument */
void raiseDotDErrorFlags(line * linePointer, int
lineInd);

/* Handle the instruction's arguments based on their kind */
void instructionsArgsTest(line * linePointer, int
lineInd,
char *buffer,
int *DC
);

/* this function check and code the argument in an String type instruction line */
void checkAndCodeString(line * linePointer, int
lineInd,
char *buffer,
int firstArgInd,
int *DC
);

/* this function get a .d instruction and points it to the right function */
void CodeDot_d(line * linePointer, int
lineInd,
char *buffer,
int firstArgInd,
int *DC
);

/* Check all .d directives argument's and copies them into the data image if they are valid */
void checkAndCode_dot_d(line * linePointer, int
lineInd,
char *buffer,
int firstArgInd,
int argLen,
int numOfBites
);

/* Get an ind variable and a buffer pointer and filling buffer with the argument until it reaches a new line or a comma, and return's the updates index */
int fillUpInsArgBuffer(line * linePointer, int
lineInd,
char *buffer,
char *argBuffer,
int maxLen,
int firstArgInd
);

/* Checks if the argument that was given to a .d instruction is valid based on her type */
int valid_dot_d_argVal(line * linePointer, int
lineInd,
char *buffer
);

void raiseNumFlag(line *linePointer, int lineInd);

void raiseRegErrorFlags(line *linePointer, int lineInd);

/* Raise a memory allocation error flag */
void raiseAllocationError(line * linePinter, int
lineInd);

/* Raise an invalid label argument error flag */
void raiseLabelArgError(line * linePointer, int
lineInd);

#endif
