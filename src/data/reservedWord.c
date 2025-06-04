#include "reservedWord.h"
#include "dataStructures.h"

/* All Registers */
const char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};
int registers_length;
registers_length = sizeof(registers) / sizeof(registers[0]);

/* creating an array of reserved words in order to find a way to write less code and still  treat all the possible cases that may appear in the input file */
char commands[16][5] = {"mov", "cmp", "add", "sub",
                     "not", "clr", "lea", "inc",
                     "dec", "jmp", "bne", "red",
                     "prn", "jsr", "rts", "stop"};
int commands_length;
commands_length = sizeof(commands) / sizeof(commands[0]);

/* This arrays hold all commands that has a common things - same number arguments for example. */
/* Zero operands */
const char *zero_operands[] = {"rts", "stop"};
int zero_operands_length;
zero_operands_length = sizeof(zero_operands) / sizeof(zero_operands[0]);

/* One operand */
const char *one_operand[] = {"not", "clr", "inc", "dec", "red", "prn"};
int one_operand_length;
one_operand_length = sizeof(one_operand) / sizeof(one_operand[0]);

/* One operand special*/
const char *one_operand_special[] = {"jmp", "bne", "jsr"};
int one_operand_special_length;
one_operand_special_length = sizeof(one_operand_special) / sizeof(one_operand_special[0]);

/* Two operands */
const char *two_operands[] = {"move", "cmp", "add", "sub", "lea"};
int two_operands_length;
two_operands_length = sizeof(two_operands) / sizeof(two_operands[0]);

/* end of array's that divide all commands based on their type */

const char *instructions[] = {".data", ".string", ".entry", ".extern"};
int instruction_list_length;
instruction_list_length = sizeof(instructions) / sizeof(instructions[0]);

/* end of function's that categorize a reserved word opcode */

/* this int function check if the reserved word is a stop reserved and retiurn's 1 in case it is, zero if not  */
int isStop(char *buffer) {
    if (strcmp(buffer, "stop") == 0)
        return 1;
    else
        return 0;
}

/* Checks if the reserved word is a command. Return 1 if is, 0 if not */
int isCommand(char *buffer) {
    int i;
    for (i = 0; i < numOfCommands; i++) {
        if (strcmp(buffer, commands[i]) == 0)
            return 1;
    }
    return 0;
}

/* Checks if the reserved word is an instruction and returns 1 if it is, and zero if not */
int isInstruction(char *buffer) {
    int i;
    for (i = 0; i < instruction_list_length; i++) {
        if (strcmp(buffer, instructions[i]) == 0)
            return 1;
    }
    return 0;
}

/* Checks if the reserved word is a .extern instruction and returns 1 if it is, and zero if not */
int isExt(char *buffer) {
    if (strcmp(".extern", buffer) == 0)
        return 1;
    return 0;
}

/* this function checks if the reserved word is a .entry instruction and returns 1 if it is, and zero if not */
int isEnt(char *buffer) {
    if (strcmp(".entry", buffer) == 0)
        return 1;
    return 0;
}

/* Checks if the reserved word is a max one word command. */
int isRegister(char *buffer) {
    int ind;
    for (ind = 0; ind < registers_length; ind++) {
        if (strcmp(buffer, registers[ind]) == 0)
            return 1;
    }
    return 0;
}

/* Checks if the reserved word is a max one word command. */
int isOneType(char *buffer) {
    int ind;
    for (ind = 0; ind < zero_operands_length; ind++) {
        if (strcmp(buffer, zero_operands[ind]) == 0)
            return 1;
    }
    return 0;
}

/* Checks if the reserved word is a max two words command. */
int isTwoType(char *buffer) {
    int ind;
    for (ind = 0; ind < one_operand_length; ind++) {
        if (strcmp(buffer, one_operand[ind]) == 0)
            return 1;
    }
    return 0;
}

/* Checks if the reserved word is a max three words command. */
int isThreeType(char *buffer) {
    int ind;
    for (ind = 0; ind < two_operands_length; ind++) {
        if (strcmp(buffer, two_operands[ind]) == 0)
            return 1;
    }
    return 0;
}

/* Checks if the reserved word is a max four words command. */
int isFourType(char *buffer) {
    int ind;
    for (ind = 0; ind < one_operand_special_length; ind++) {
        if (strcmp(buffer, one_operand_special[ind]) == 0)
            return 1;
    }
    return 0;
}

/* Checks if the reserved word is a max two words command. */
void getOpcode(line *linePointer, int lineInd, char *buffer) {
    int ind;
    for (ind = 0; ind < commands_length; ind++) {
        if (strcmp(buffer, commands[ind]) == 0)
            linePointer[lineInd].word.opcode = ind;
    }
    printf("Error value = %d", ind);
    return;
}
