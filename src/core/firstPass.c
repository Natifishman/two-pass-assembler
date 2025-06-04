#include "firstPass.h"

/* first, we check if the user entered a line that starts with a label or a line that starts with an instruction\command */
void firstPass(line *linePointer, label *labelPointer, char *buffer, int lineInd, int labelInd, int *IC, int *DC) {
    validFirstWord(linePointer, lineInd, buffer, IC, DC);
    if (linePointer[lineInd].isEntry == TRUE) {
        /* this function creates a dynamic buffer and copies the argument to it in order to check it in the second pass */
        handleEntryArg(linePointer, lineInd, buffer);
        /* Lines that contain .entry instruction's will be taken care in the second pass */
        return;
    }

    /* if the first word is not a command or an instruction, and it does not have a colon at her end there is a syntax error,return to main */
    if (linePointer[lineInd]._isError == TRUE) {
        return;
    }

    /* if the first word is not a command or an instruction we need to check if it has a valid label */
    if (linePointer[lineInd].command == FALSE && linePointer[lineInd].instruction == FALSE &&
        linePointer[lineInd].isExtern == FALSE && linePointer[lineInd].isEntry == FALSE) {
        /* a function that checks if there is a valid label */
        isLabel(linePointer, labelPointer, buffer, lineInd, labelInd);

        /* if the label is valid, check if there is a reserved word after */
        if (linePointer[lineInd].validLabel == TRUE && linePointer[lineInd]._isError == FALSE) {
            reservedWithLabel(linePointer, labelPointer, buffer, lineInd, labelInd, IC, DC);

            /* if the word is not reserved (or entry - who gets treated in the second pass) */
            if (linePointer[lineInd]._isError == TRUE || linePointer[lineInd].isEntry == TRUE) {
                return; /* Return to assembler.c and print error */
            }
        } else {    /* If the label is invalid */
            return; /* Return to main, the error will be printed there */
        }
    }

    /* line's argument test function based on the line reserved word type */
    if (linePointer[lineInd].isExtern == TRUE) {
        extArgTest(linePointer, labelPointer, lineInd, labelInd, buffer);
    } else if (linePointer[lineInd].command == TRUE) {
        commandArgsTest(linePointer, lineInd, labelPointer, labelInd, buffer);
    } else if (linePointer[lineInd].instruction == TRUE) {
        instructionsArgsTest(linePointer, lineInd, buffer, DC);
    }
}/* end of first pass */





