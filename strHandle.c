#include "strHandle.h"

/* this function initialize an ind variable and returns the first nonBlank char ind */
int nonBlankInd(char *buffer) {
    int firstLetterInd;
    for (firstLetterInd = 0; isspace(buffer[firstLetterInd]); firstLetterInd++) {
        /* looping through all tabs and spaces that my come in the beginning in order to point the first nonBlank char ind*/}
    return firstLetterInd;
}

/* this int function checks if the line is a blank line */
int isBlank(char *buffer) {
    int i;
    for (i = 0; buffer[i] != '\n'; i++) {
        if (isspace(buffer[i]) == 0)
            return 0;/* return 0 in case it is not */
    }
    return 1;/* return's 1 if it is blank */
}

/* this int function checks if the line is a comment line */
int isComment(char *buffer) {
    int i;
    for (i = 0; buffer[i] != '\n'; i++) {
        if (isspace(buffer[i]) == 0) {
            if (buffer[i] == semiColonAscii)
                return 1;/* return's 1 if it is a comment */
            else
                return 0;/* return 0 in case it is not */
        }
    }
    return 0;/* return 0 in case it is not */
}

/* start Label analyze function */
/* Checks if there is an option for a label in current line */
void isLabel(line *linePointer, label *labelPointer, char *buffer, int lineInd, int labelInd) {
    char labelBuffer[maxLabelLen]; /* a constant array that will get the string we suspect to be a label */
    int firstLetterInd, labelBufInd;
    for (firstLetterInd = 0; isspace(buffer[firstLetterInd]); firstLetterInd++) {
        /* looping through all tabs and spaces that my come in the beginning in order to point the first nonBlank char ind*/}

    for (labelBufInd = 0; isspace(buffer[firstLetterInd]) == 0 && buffer[firstLetterInd] != '\n'; labelBufInd++) {
        /* If statement that checks if label length is bigger than 30*/
        if (firstLetterInd > firstLetterInd + maxLabelLen) {
            raiseLabelError(linePointer, lineInd);
            return; /* Rethink about an exit strategy */
        }
        labelBuffer[labelBufInd] = buffer[firstLetterInd++];
    }
    labelBuffer[labelBufInd] = '\0';/* declaring string ending in order to prevent garbage value appearance */

    /* check if the label is valid and raise relevant flag */
    labelTest(linePointer, labelPointer, lineInd, labelInd, labelBuffer, labelBufInd);
    if (linePointer[lineInd]._isError == TRUE)
        return; /* return to assembler.c an error message will be printed */

    labelBuffer[labelBufInd - 1] = '\0';/* deleting the colon in order to check if the label is a reserved word */
    if (isCommand(labelBuffer) || isInstruction(labelBuffer)) {
        raiseLabelError(linePointer, lineInd);
        return;
    }
    linePointer[lineInd].validLabel = TRUE;
    /* coping the label to the labelBuffer inside the label struct */
    strcpy(labelPointer[labelInd].labelBuff, labelBuffer);
}/* end of isLabel */

/* this function check if the label is valid and raise an error flag in case its not */
void labelTest(line *linePointer, label *labelPointer, int lineInd, int labelInd, char *labelBuffer, int labelBufInd) {
    /* if statement that checks if the current label is already exist and if the label contains a colon */
    if (labelDuplicationCheck(labelPointer, labelInd, labelBuffer) || labelColonCheck(labelBuffer, labelBufInd) == 0) {
        raiseLabelError(linePointer, lineInd);
        return;/* return to isLabel*/
    }
    for (labelBufInd = 0; labelBuffer[labelBufInd] != '\0'; labelBufInd++) {
        if (labelBufInd == 0) {
            if (!(labelCharTest(labelBuffer, labelBufInd))) {/* label must start with a letter */
                raiseLabelError(linePointer, lineInd);
            }
        }
        if (!(labelCharTest(labelBuffer, labelBufInd) || labelDigitsCheck(labelBuffer, labelBufInd))) {
            raiseLabelError(linePointer, lineInd);
        }
    }
}

/* this function check if current label char is valid by her ascii*/
int labelCharTest(char *buffer, int i) {
    if ((buffer[i] >= lowCap_a_ascii && buffer[i] <= lowCap_z_ascii) ||
        (buffer[i] >= capitalAascii && buffer[i] <= capitalZascii))
        return 1;
    else
        return 0;
}

/* this function check if current label digit is valid by her ascii */
int labelDigitsCheck(char *buffer, int i) {
    if (buffer[i] >= digitsStart && buffer[i] <= digitsEnd)
        return 1;
    else if (buffer[i] == colonAscii) {
        if (buffer[i + 1] == '\0')
            return 1;
        else
            return 0;
    } else {
        return 0;
    }
}

/* this function check if the label was already defined */
int labelDuplicationCheck(label *labelPointer, int labelInd, char *givenLabel) {
    int ind;
    for (ind = 0; ind <= labelInd; ind++) {
        if (strcmp(labelPointer[ind].labelBuff, givenLabel) == 0) {
            if (labelPointer[ind].isExt == TRUE || labelPointer[ind].isEnt == TRUE)
                return 0;
            else {
                return 1;
            }
        }
    }
    return 0;
}

/* this function check if label has colon in the end */
int labelColonCheck(char *buffer, int labelLength) {
    if (buffer[labelLength - 1] == colonAscii)
        return 1;
    return 0;
}

void raiseLabelError(line *linePointer, int lineInd) {/* this function raise a label error in case one was found */
    linePointer[lineInd]._isError = TRUE;
    linePointer[lineInd].invalid_label = TRUE;
}

/* end of label functions */

/* First function called from first pass */
/* Checks if the user chose to enter a line without a label in the beginning */
void validFirstWord(line *linePointer, int lineInd, char *buffer, int *IC, int *DC) {
    int firstLetterInd, firstWordInd;
    char firstWordBuff[maxLabelLen]; /* it might be a label */

    firstWordInd = 0; /* initialize to zero */

    /* looping through all tabs and spaces that may come in the beginning in order to point the first nonBlank char index */
    for (firstLetterInd = 0; isspace(buffer[firstLetterInd]); firstLetterInd++) {}

    for (; isspace(buffer[firstLetterInd]) == 0 && firstLetterInd < maxLineLen &&
                         buffer[firstLetterInd] != '\n'; firstLetterInd++) {
        if (firstWordInd > maxLabelLen) {/* an if statement that checks if label length is bigger than 30*/
            raiseLabelError(linePointer, lineInd);
            return; /* rethink about an exit strategy */
        }
        firstWordBuff[firstWordInd++] = buffer[firstLetterInd]; /* Gets the current non-blank letter */
    }

    /* putting a string ending char in the end of the string in order to prevent garbage chars appearance */
    firstWordBuff[firstWordInd] = '\0';

    /* raising all relevant flag based on the first word given */
    firstWordRelevantFlags(linePointer, lineInd, firstWordBuff, IC, DC);

    /* Checks if it is an entry */
    if (linePointer[lineInd].isEntry == TRUE) {
        return; /* the assembler will take care of .entry instruction in the second pass */
    }

    /* Checks if it is a command */
    if (linePointer[lineInd].command == TRUE) {
        /* set the number of words to 1 till checking */
        linePointer[lineInd].numOfWords = 1;
        /* coding the relevant opcode according to the command reserved word */
        getOpcode(linePointer, lineInd, firstWordBuff);
    }
}

/* Checks if the first word has a colon, if there is, its suspected as a label */
int lastCharColonChecker(char *buffer) {
    int i;
    for (i = 0; buffer[i] != '\0'; i++) {
        if (buffer[i + 1] == '\0') {
            if (buffer[i] == colonAscii)
                return 1;
        }
    }
    return 0;
}

/* a function that raise first the Word Relevant Flags */
void firstWordRelevantFlags(line *linePointer, int lineInd, char *buffer, int *IC, int *DC) {
    if (isCommand(buffer)) {
        linePointer[lineInd].command = TRUE;
        linePointer[lineInd].noLabel = TRUE;
        linePointer[lineInd].IC = *IC;/* giving the line her final address */
        raiseTypeFlags(linePointer, lineInd, buffer); /* Get the type of the command(num of operands). */
    } else if (isInstruction(buffer)) {
        if (isEnt(buffer))
            linePointer[lineInd].isEntry = TRUE;
        else if (isExt(buffer))
            linePointer[lineInd].isExtern = TRUE;
        else if (linePointer[lineInd].isExtern == FALSE && linePointer[lineInd].isEntry == FALSE) {
            linePointer[lineInd].instruction = TRUE;
            /* giving the instruction line her temporary address that get an updated in the second pass */
            linePointer[lineInd].DC = *DC;
            raiseInstructionFlag(linePointer, lineInd, buffer);
            linePointer[lineInd].noLabel = TRUE;
        }
    }
        /* checking if the first word was meant to be used as a label by checking if it contains a colon in end */
    else if (lastCharColonChecker(buffer) == 0) {
        linePointer[lineInd].invalid_reserved = TRUE;
        linePointer[lineInd]._isError = TRUE;
    }
}

/* this function will help to detect the exact type of instruction */
void raiseInstructionFlag(line *linePointer, int lineInd, char *buffer) {
    if (strcmp(buffer, ".String") == 0)
        linePointer[lineInd].is_string = TRUE;
    else if (strcmp(buffer, ".data") == 0)
        linePointer[lineInd].is_data = TRUE;
} /* end of function's that checks if the user chose to enter a line without a label in the beginning. */

/*the start of getting command or instruction from a line that contains a label functions*/
/* this function is responsible to check the reserved word in line without a label */
void
reservedWithLabel(line *linePointer, label *labelPointer, char *buffer, int lineInd, int labelInd, int *IC, int *DC) {
    char commOrInstBuff[maxInstOrCommLen];/* the maximum length of a valid command or an instruction is 7 plus a string ending char ('\0') */
    int commOrInstInd;
    int newBuffInd;
    commOrInstInd = reserved_Ind_WLabel(buffer); /* Getting the command or instruction stating Ind */

    newBuffInd = 0;
    for (; buffer[commOrInstInd] != '\n' && isspace(buffer[commOrInstInd]) == 0; commOrInstInd++) {
        if (newBuffInd > maxInstOrCommLen) {
            linePointer[lineInd].invalid_reserved = TRUE;
            linePointer[lineInd]._isError = TRUE;
            return;
        }
        commOrInstBuff[newBuffInd++] = buffer[commOrInstInd];
    }
    commOrInstBuff[newBuffInd] = '\0';/*in order to prevent garbage chars in the end of the string we want to check */

    /* raising the relevant flags based on the word that was founded */
    ReservedRelevantFlagsWLabel(linePointer, labelPointer, lineInd, labelInd, commOrInstBuff, IC, DC);
    if (linePointer[lineInd].command == TRUE) {
        getOpcode(linePointer, lineInd, buffer);
    }
}

/* this function returns command or instruction starting index */
int reserved_Ind_WLabel(char *buffer) {
    int afterLabelInd;
    for (afterLabelInd = 0; buffer[afterLabelInd] != '\n' && buffer[afterLabelInd] != colonAscii; afterLabelInd++) {
    }/* creating a for loop that will stop when it will reach the label's colon */
    afterLabelInd += 2; /* in order to skip the colon that the loop stopped on plus the needed space after the label */

    for (; isspace(buffer[afterLabelInd]) && buffer[afterLabelInd] != '\n'; afterLabelInd++) {
        /* this loop skips all tabs and spaces that might appear between the label and to command/instruction */
    }
    return afterLabelInd;
}

/* a function that checks if the word is reserved and raise a relevant flag */
void
ReservedRelevantFlagsWLabel(line *linePointer, label *labelPointer, int lineInd, int labelInd, char *buffer, int *IC,
                            int *DC) {
    /* if a command is found raise relevant flags */
    if (isCommand(buffer)) {
        linePointer[lineInd].command = TRUE;
        labelPointer[labelInd].isCode = TRUE;
        raiseTypeFlags(linePointer, lineInd, buffer);
        linePointer[lineInd].IC = *IC;
        labelPointer[labelInd].ic = *IC;

        /* if extern raise flag */
    } else if (isExt(buffer)) {
        linePointer[lineInd].validLabel = FALSE;/* turning of the valid label flag by iving it the value "false" because an extern label should be ignored */
        linePointer[lineInd].isExtern = TRUE;
        linePointer[lineInd].ignoreLabel = TRUE;/* turning on this flag in order to prevent the labels dynamic array from extending */
        strcpy(labelPointer[labelInd].labelBuff, "");/* there is no need in keeping an extern instruction label*/

        /* if entry raise flag */
    } else if (isEnt(buffer)) {
        linePointer[lineInd].validLabel = FALSE;/* turning of the valid label flag by giving it the value "false" because an entry label should be ignored */
        linePointer[lineInd].isEntry = TRUE;
        linePointer[lineInd].ignoreLabel = TRUE;/* turning on this flag in order to prevent the labels dynamic array from extending */
        strcpy(labelPointer[labelInd].labelBuff, "");/* there is no need in keeping an entry instruction label*/

        /* if an instruction is found raise relevant flags */
    } else if (isInstruction(buffer)) {/* if an instruction is found raise relevant flags */
        raiseInstructionFlag(linePointer, lineInd, buffer);/* this function will detect the exact type of instruction */
        linePointer[lineInd].DC = *DC;/* giving the line her unfinished address (will get updated in the second pass */
        labelPointer[labelInd].dc = *DC;/* giving the label her unfinished address (will get updated in the second pass */
        linePointer[lineInd].instruction = TRUE;
        labelPointer[labelInd].isData = TRUE;

        /* not a reserved word */
    } else {
        linePointer[lineInd]._isError = TRUE;
        linePointer[lineInd].invalid_reserved = TRUE;
    }
}

/* Raise the relevant type flag when a line holds a valid command reserved word */
void raiseTypeFlags(line *linePointer, int lineInd, char *buffer) {
    if (isOneType(buffer)) {
        linePointer[lineInd].isOne = TRUE;
        linePointer[lineInd].numOfWords = 1;
    } else if (isTwoType(buffer)) {
        linePointer[lineInd].isTwo = TRUE;
    } else if (isThreeType(buffer)) {
        linePointer[lineInd].isThree = TRUE;
    } else if (isFourType(buffer)) {
        linePointer[lineInd].isFour = TRUE;
    }
} /* End of raiseTypeFlags */

/* valid arguments functions */

/* a function that checks if label that was given as an argument is valid */
int argLabelCheck(char *buffer) {
    int bufferInd;
    for (bufferInd = 0; buffer[bufferInd] != '\0'; bufferInd++) {
        if (buffer[bufferInd] == colonAscii) {
            return 0;
        }
        if (bufferInd == 0) {
            if (!(labelCharTest(buffer, bufferInd))) {/* label must start with a letter */
                return 0;
            }
        }
        if (!(labelCharTest(buffer, bufferInd) || labelDigitsCheck(buffer, bufferInd))) {
            return 0;
        }
    }
    return 1;
}

int firstArgIndWLabel(char *buffer) {
    int firstArgInd;
    /* giving the address the index of the first reserved word after passing the label and the blank chars */
    firstArgInd = reserved_Ind_WLabel(buffer);
    for (; isspace(buffer[firstArgInd]) == 0 && buffer[firstArgInd] != '\n'; firstArgInd++) {
        /* looping through the reserved word char until it reaches a blank char */
    }
    for (; isspace(buffer[firstArgInd]) && buffer[firstArgInd] != '\n'; firstArgInd++) {
        /*looping through all blank chars between the reserved word and the arg */
    }
    return firstArgInd;
}

int firstArgIndNoLabel(char *buffer) {
    int firstArgInd;
    for (firstArgInd = 0; isspace(buffer[firstArgInd]) && buffer[firstArgInd] != '\n'; firstArgInd++) {
        /* looping through all tabs and spaces that may come in the beginning in order to point the first nonBlank char ind*/}
    for (; isspace(buffer[firstArgInd]) == 0 && buffer[firstArgInd] != '\n'; firstArgInd++) {
        /*looping through all chars of the reserved word */}
    for (; isspace(buffer[firstArgInd]) && buffer[firstArgInd] != '\n'; firstArgInd++) {
        /* one more time looping through all tabs and spaces that come after the reserved word in order to point the first arg*/}
    return firstArgInd;
}

/* command argFunctions */
/* this function checks if there are more non-blank chars between the operand and the comma */
int iterateUntilComma(line *linePointer, int lineInd, char *buffer, int ind) {
    for (; buffer[ind] != '\0'; ind++) {
        if (buffer[ind] == ',') {
            return ind;
        }
    }
    raiseExtraneousOperandFlags(linePointer, lineInd);
    return 0;
}

/* this function checks if there are more non-blank chars between the operand and the bracket */
int iterateUntilBrackets(line *linePointer, int lineInd, char *buffer, int ind) {
    for (; buffer[ind] != '\0'; ind++) {
        if (buffer[ind] == '(') {
            return ind;
        }
    }
    raiseExtraneousOperandFlags(linePointer, lineInd);
    return 0;
}

/* this function checks if there are more non-blank chars between the operand and the bracket */
int iterateUntilEndBrackets(line *linePointer, int lineInd, char *buffer, int ind) {
    for (; buffer[ind] != '\0'; ind++) {
        if (buffer[ind] == ')') {
            return ind;
        }
    }
    raiseExtraneousOperandFlags(linePointer, lineInd);
    return 0;
}

/* this function checks if there are more non-blank chars between the operand and the string ending char */
void iterateUntilEnd(line *linePointer, int lineInd, char *buffer, int ind) {
    for (; buffer[ind] != '\0'; ind++) {
        if ((isspace(buffer[ind]) == 0) && buffer[ind] != '\n') {
            raiseExtraneousOperandFlags(linePointer, lineInd);
            return;
        }
    }
}

/* a function that was made to raise the extraneous_operand flag in case of entering operands needed */
void raiseExtraneousOperandFlags(line *linePointer, int lineInd) {
    linePointer[lineInd].extraneous_Operand = TRUE;
    linePointer[lineInd]._isError = TRUE;
}

void raiseMissingOperandFlags(line *linePointer, int lineInd) {
    linePointer[lineInd].missing_Operand = TRUE;
    linePointer[lineInd]._isError = TRUE;
}

/* Get the reserved word index when there is no label */
int reservedIndNoLabel(char *buffer) {
    int firstLetterInd;
    for (firstLetterInd = 0; isspace(buffer[firstLetterInd]) && buffer[firstLetterInd] != '\n'; firstLetterInd++) {
        /* looping through all tabs and spaces that my come in the beginning in order to point the first nonBlank char ind*/}
    return firstLetterInd;
}

/* Raise all command's argument flags in order to handle multiple number of commands in one function */
void setCommandType(line *linePointer, int lineInd, char *buffer) {
    /* getting the reserved command in order to find out how many args it gets */
    char commandBuffer[maxInstOrCommLen];
    int reservedInd, commBuffInd;
    commBuffInd = 0;
    if (linePointer[lineInd].validLabel == TRUE) {
        reservedInd = reserved_Ind_WLabel(buffer);
    } else {
        reservedInd = reservedIndNoLabel(buffer);
    }
    for (; isspace(buffer[reservedInd]) == 0 && buffer[reservedInd] != '\0'; reservedInd++) {
        /* this loop goes through all the non-blank char from the beginning of the command in order to copy ot to the new buffer */
        commandBuffer[commBuffInd++] = buffer[reservedInd]; /* get command without spaces */
    }
    commandBuffer[commBuffInd] = '\0';

    /* this function check if the command is one of the commands that get three registers and return's her opcode in case of a non-zero return value */
    if (isOneType(commandBuffer)) {
        linePointer[lineInd].isOne = TRUE;
    } else if (isTwoType(commandBuffer)) {
        linePointer[lineInd].isTwo = TRUE;
    } else if (isThreeType(commandBuffer)) {
        linePointer[lineInd].isThree = TRUE;
    } else if (isFourType(commandBuffer)) {
        linePointer[lineInd].isFour = TRUE;
    }
}

/* Check all the command's arguments and code them. if there are an error,raise an error flag */
void commandArgsTest(line *linePointer, int lineInd, label *labelPointer, int labelInd, char *buffer) {
    /* Set the type of command(number of arguments in it). */
    setCommandType(linePointer, lineInd, buffer);

    /* Encode according to the type of command */
    if (linePointer[lineInd].isOne == TRUE)
        checkAndEncodeOneTypeArgs(linePointer, lineInd, buffer);
    else if (linePointer[lineInd].isTwo == TRUE) {
        checkAndEncodeTwoTypeArgs(linePointer, lineInd, labelPointer, labelInd, buffer);
    } else if (linePointer[lineInd].isThree == TRUE) {
        checkAndEncodeThreeTypeArgs(linePointer, lineInd, labelPointer, labelInd, buffer);
    } else if (linePointer[lineInd].isFour == TRUE) {
        checkAndEncodeFourTypeArgs(linePointer, lineInd, labelPointer, labelInd, buffer);
    }
}/* end commandArgsTest  function */

/* this function check the One word commands(zero operands) */
void checkAndEncodeOneTypeArgs(line *linePointer, int lineInd, char *buffer) {}

/* Check and code 1-operand argument with max 4 words */
void checkAndEncodeFourTypeArgs(line *linePointer, int lineInd, label *labelPointer, int labelInd, char *buffer) {
    char op1Buff[regLen], op2Buff[regLen], op3Buff[maxLabelLen];
int destType,regValue,srcType,firstArgInd;
firstArgInd=0;
    /* this function copies the current index argument into a compatible buffer */
    firstArgInd = fillUpOpBuffer(linePointer, lineInd, buffer, op1Buff, regLen, firstArgInd, 1, 0, 0);
    if (firstArgInd == -1){
        return;}
    /* this function checks if the register is valid and encodes the register */
    isValidLabel(linePointer, lineInd, labelPointer, labelInd, op3Buff);
    if (linePointer[lineInd]._isError == TRUE){
        return;}


    /* this function passes all blank chars until it meets a bracket */
    firstArgInd = iterateUntilBrackets(linePointer, lineInd, buffer, firstArgInd);
    firstArgInd++;/* in order to pass the brackets */


    /* Copies the current index argument into a compatible buffer */
    srcType = CheckOperand(linePointer, lineInd, buffer, firstArgInd);
    /* If first operand is a register */
    if (srcType == REG) {
        firstArgInd = fillUpOpBuffer(linePointer, lineInd, buffer, op1Buff, numMaxLen, firstArgInd, 1, 0, 0);
        if (firstArgInd != -1) {
            regValue = is_valid_register(op1Buff);
            linePointer[lineInd].srcRegValue = regValue;
            linePointer[lineInd].isSrcReg = TRUE;
        }
    }
    /* If first operand is not a register check if it is num(2nd smallest word) */
    if ((srcType == REG && firstArgInd == -1)) {
        srcType = NUM;
    }
    /* If first operand is a number */
    if (srcType == NUM) {
        firstArgInd = fillUpOpBuffer(linePointer, lineInd, buffer, op1Buff, numMaxLen, firstArgInd, 0, 1, 0);
        if (firstArgInd != -1)
            isNumberTwoOp(linePointer, firstArgInd, firstArgInd);
    }
    /* If first operand is not a number check if it is label(our biggest word) */
    if ((srcType == NUM && firstArgInd == -1)) {
        srcType = LABEL;
    }
    /* If first operand is a label */
    if (srcType == LABEL) {
        firstArgInd = fillUpOpBuffer(linePointer, lineInd, buffer, op1Buff, numMaxLen, firstArgInd, 0, 0, 1);
        if (firstArgInd != -1)
            isValidLabel(linePointer, lineInd, labelPointer, labelInd, op1Buff);
    }


    firstArgInd = iterateUntilComma(linePointer, lineInd, buffer, firstArgInd);
    firstArgInd++;/* in order to pass the comma */;


    /* Copies the current index argument into a compatible buffer */
    destType = CheckOperand(linePointer, lineInd, buffer, firstArgInd);
    /* If first operand is a register */
    if (destType == REG) {
        firstArgInd = fillUpOpBuffer(linePointer, lineInd, buffer, op2Buff, numMaxLen, firstArgInd, 1, 0, 0);
        if (firstArgInd != -1) {
            regValue = is_valid_register(op2Buff);
            linePointer[lineInd].srcRegValue = regValue;
            linePointer[lineInd].isSrcReg = TRUE;
        }
    }
    /* If first operand is not a register check if it is num(2nd smallest word) */
    if ((destType == REG && firstArgInd == -1)) {
        destType = NUM;
    }
    /* If first operand is a number */
    if (destType == NUM) {
        firstArgInd = fillUpOpBuffer(linePointer, lineInd, buffer, op2Buff, numMaxLen, firstArgInd, 0, 1, 0);
        if (firstArgInd != -1)
            isNumberTwoOp(linePointer, firstArgInd, firstArgInd);
    }
    /* If first operand is not a number check if it is label(our biggest word) */
    if ((destType == NUM && firstArgInd == -1)) {
        destType = LABEL;
    }
    /* If first operand is a label */
    if (destType == LABEL) {
        firstArgInd = fillUpOpBuffer(linePointer, lineInd, buffer, op2Buff, numMaxLen, firstArgInd, 0, 0, 1);
        if (firstArgInd != -1)
            isValidLabel(linePointer, lineInd, labelPointer, labelInd, op2Buff);
    }

    /* Checks if there are closing brackets */
    iterateUntilEndBrackets(linePointer, lineInd, buffer, firstArgInd);
    /* Checks if there are more parameters than needed */
    iterateUntilEnd(linePointer, lineInd, buffer, firstArgInd);
}/* end of four type args function */

/* Check and code 1-operand argument */
void checkAndEncodeTwoTypeArgs(line *linePointer, int lineInd, label *labelPointer, int labelInd, char *buffer) {
    int destType, regValue,firstArgInd;
    char op1Buff[MaxLabelSize];
    /* Copies the current index argument into a compatible buffer */
    destType = CheckOperand(linePointer, lineInd, buffer, lineInd);
    /* If first operand is a register */
    if (destType == REG) {
        firstArgInd = fillUpOpBuffer(linePointer, lineInd, buffer, op1Buff, numMaxLen, lineInd, 1, 0, 0);
        if (is_valid_register(op1Buff) != -1) {
            regValue = is_valid_register(op1Buff);
            linePointer[lineInd].destRegValue = regValue;
            linePointer[lineInd].isDestReg = TRUE;
        }
    }
    /* If first operand is a number */
    if (destType == NUM) {
        firstArgInd = fillUpOpBuffer(linePointer, lineInd, buffer, op1Buff, numMaxLen, firstArgInd, 0, 1, 0);
        isNumberOneOp(linePointer, firstArgInd, firstArgInd);
    }
    /* If first operand is a label */
    if (destType == LABEL) {
        firstArgInd = fillUpOpBuffer(linePointer, lineInd, buffer, op1Buff, numMaxLen, firstArgInd, 0, 0, 1);
        isValidLabel(linePointer, lineInd, labelPointer, 1, op1Buff);
    }

    /* this function checks if there are more parameters than needed */
    iterateUntilEnd(linePointer, lineInd, buffer, firstArgInd);

    /* Set the number of words */
    linePointer[lineInd].numOfWords = 2;
}

/* Check and code 2-operands argument */
void checkAndEncodeThreeTypeArgs(line *linePointer, int lineInd, label *labelPointer, int labelInd, char *buffer) {
    int srcType, destType, regValue,firstArgInd;
    char op1Buff[MaxLabelSize], op2Buff[MaxLabelSize];
firstArgInd=0;
    /* Copies the current index argument into a compatible buffer */
    srcType = CheckOperand(linePointer, lineInd, buffer, lineInd);
    /* If first operand is a register */
    if (srcType == REG) {
        firstArgInd = fillUpOpBuffer(linePointer, lineInd, buffer, op1Buff, numMaxLen, firstArgInd, 1, 0, 0);
        if (is_valid_register(op1Buff) != -1) {
            regValue = is_valid_register(op1Buff);
            linePointer[lineInd].srcRegValue = regValue;
            linePointer[lineInd].isSrcReg = TRUE;
        }
    }
    /* If first operand is a number */
    if (srcType == NUM) {
        firstArgInd = fillUpOpBuffer(linePointer, lineInd, buffer, op1Buff, numMaxLen, firstArgInd, 0, 1, 0);
        isNumberTwoOp(linePointer, firstArgInd, firstArgInd);
    }
    /* If first operand is a label */
    if (srcType == LABEL) {
        firstArgInd = fillUpOpBuffer(linePointer, lineInd, buffer, op1Buff, numMaxLen, firstArgInd, 0, 0, 1);
    }

    /* Passes all blank chars until it meets a comma and skip one after the comma */
    firstArgInd = iterateUntilComma(linePointer, lineInd, buffer, firstArgInd);
    firstArgInd++;

    /* Copies the current index argument into a compatible buffer */
    destType = CheckOperand(linePointer, lineInd, buffer, firstArgInd);
    /* If first operand is a register */
    if (destType == REG) {
        firstArgInd = fillUpOpBuffer(linePointer, lineInd, buffer, op2Buff, numMaxLen, firstArgInd, 1, 0, 0);
        if (is_valid_register(op2Buff) != -1) {
            regValue = is_valid_register(op1Buff);
            linePointer[lineInd].srcRegValue = regValue;
            linePointer[lineInd].isSrcReg = TRUE;
        }
    }
    /* If first operand is a number */
    if (destType == NUM) {
        firstArgInd = fillUpOpBuffer(linePointer, lineInd, buffer, op2Buff, numMaxLen, firstArgInd, 0, 1, 0);
        isNumberTwoOp(linePointer, firstArgInd, firstArgInd);
    }
    /* If first operand is a label */
    if (destType == LABEL) {
        firstArgInd = fillUpOpBuffer(linePointer, lineInd, buffer, op2Buff, numMaxLen, firstArgInd, 0, 0, 1);
    }

    /* this function checks if there are more parameters than needed */
    iterateUntilEnd(linePointer, lineInd, buffer, firstArgInd);

    /* Set the number of words */
    linePointer[lineInd].numOfWords = 3;
    /* If first and 2nd operands are registers */
    if (linePointer[lineInd].isSrcReg == TRUE && linePointer[lineInd].isDestReg == TRUE) {
        linePointer[lineInd].numOfWords = 2;
    }
}


/* valid I-type args functions */
/* Codes a valid label */
void isValidLabel(line *linePointer, int lineInd, label *labelPointer, int labelInd, char *buffer) {
    int ind, codedNumFlag;
    codedNumFlag = 0;
    if (argLabelCheck(buffer) == 0) {
        raiseLabelArgError(linePointer, lineInd);
        return;
    }
    for (ind = 0; ind <= labelInd; ind++) {
        if (strcmp(buffer, labelPointer[ind].labelBuff) == 0) {
            if (labelPointer[ind].isCode == TRUE) {
                codedNumFlag = 1;
            }
        }
    }
    if (codedNumFlag == 0) {
        /* if a valid equal label was not found, raise a missed label flag , and copy the label argument in order to search her easily in the second pass */
        linePointer[lineInd].isMissedLabel = TRUE;/* this flag will help to detect the uncoded field in the second pass */
        /* Creating a dynamic array that will contain the missed in label for the second pass */
        linePointer[lineInd].missedLabel = (char *) calloc(maxLabelLen, sizeof(char));
        strcpy(linePointer[lineInd].missedLabel, buffer);/* copying the label argument into the dynamic buffer */
    }
}/* end of code I-type valid label function */

int is_valid_register(char *str) {
    int reg_num;
    if (sscanf(str, "r%d", &reg_num) != 1) {
        return -1;
    }
    if (reg_num < 0 || reg_num > 7) {
        return -1;
    }
    return reg_num;
}

/* Checks by Operand's length */
int CheckOperand(line *linePointer, int lineInd, char *buffer, int firstArgInd) {
int nonBlankCounter;
nonBlankCounter = 0;
    /*looping through blank chars that may appear after comma */
    for (firstArgInd; buffer[firstArgInd] == tabAscii || buffer[firstArgInd] == spaceAscii; firstArgInd++);

    /* stops when it reaches a comma or new line */
    for (firstArgInd; buffer[firstArgInd] != '\n' &&
                      (buffer[firstArgInd] != tabAscii && buffer[firstArgInd] != spaceAscii); firstArgInd++) {
        if (buffer[firstArgInd] == commaAscii) {
            break;
        }
        nonBlankCounter++;
    }

    /* Check what is the value */
    if (nonBlankCounter > maxLabelLen - 1) {
        return ERROR;
    }
    if (nonBlankCounter > numMaxLen - 1) {
        return LABEL;
    }
    if (nonBlankCounter > regLen - 1) {
        return NUM;
    }
    if (nonBlankCounter > 0) {
        return REG;
    }
    /* if there is no argument to fill up the buffer with*/
    if (nonBlankCounter == 0) {
        raiseMissingOperandFlags(linePointer, lineInd);
        return ERROR;
    }
    return ERROR;
}

/* this function fillUp a buffer with the requested content */
int
fillUpOpBuffer(line *linePointer, int lineInd, char *buffer, char *opBuffer, int bufferLen, int firstArgInd,
               int isReg,
               int isNum, int isLabel) {
    int nonBlankCounter, bufferInd,stoppedOnComma;
    bufferInd = 0, nonBlankCounter = 0;
    /*looping through blank chars that may appear after comma */
    for (; buffer[firstArgInd] == tabAscii || buffer[firstArgInd] == spaceAscii; firstArgInd++);
    /* a for loop that fills the first arg buffer */
    /* this for loop copies the arguments into another in order to make it easier to check, stops when it reaches a comma or new line */
    for (; buffer[firstArgInd] != '\n' &&
                      (buffer[firstArgInd] != tabAscii && buffer[firstArgInd] != spaceAscii); firstArgInd++) {
        if (buffer[firstArgInd] == commaAscii) {
            stoppedOnComma = 1;
            break;
        }
        if (nonBlankCounter > regLen - 1 && isReg == 1) {
            raiseRegErrorFlags(linePointer, lineInd);
            return -1;
        } else if (isNum && nonBlankCounter > numMaxLen - 1) {
            raiseNumFlag(linePointer, lineInd);
            return -1;
        } else if (isLabel && nonBlankCounter > maxLabelLen - 1) {
            raiseLabelArgError(linePointer, lineInd);
            return -1;
        }
        nonBlankCounter++;
        opBuffer[bufferInd++] = buffer[firstArgInd];
    }
    if (nonBlankCounter == 0) {/* if there is no argument to fill up the buffer with*/
        raiseMissingOperandFlags(linePointer, lineInd);
        return -1;
    }
    opBuffer[bufferInd] = '\0';
    return firstArgInd;/* this function return the updated index in order to make arguments tracking easier */
}

/* Code the valid number, raise an error flag if it isn't valid */
myBool isNumberOneOp(line *linePointer, int lineInd, char *buffer) {
    int ind, numIntVal;
    for (ind = 0; buffer[ind] != '\0'; ind++) {
        if (isdigit(buffer[ind] == 0)) {
            if (ind == 0 && (buffer[ind] == hashtagAscii)) {
                continue;
            } else if (ind == 1 && (buffer[ind] == plusAscii || buffer[ind] == minusAscii)) {
                continue;
            } else {
                raiseNumFlag(linePointer, lineInd);
                return FALSE;
            }
        }
    }
    numIntVal = atoi(buffer);
    /* checking if the number operand can be store inside a 16bit variable */
    if (numIntVal > (unsigned) highestValue || numIntVal < (unsigned) lowestValue) {
        raiseNumFlag(linePointer, lineInd);
        return FALSE;
    }

    /* Set flags */
    linePointer[lineInd].destNumValue = numIntVal;
    linePointer[lineInd].isDestNum = TRUE;
    return TRUE;
}

/* Code the valid number, raise an error flag if it isn't valid */
myBool isNumberTwoOp(line *linePointer, int lineInd, char *buffer) {
    int ind, numIntVal;
    for (ind = 0; buffer[ind] != '\0'; ind++) {
        if (isdigit(buffer[ind] == 0)) {
            if (ind == 0 && (buffer[ind] == hashtagAscii)) {
                continue;
            } else if (ind == 1 && (buffer[ind] == plusAscii || buffer[ind] == minusAscii)) {
                continue;
            } else {
                raiseNumFlag(linePointer, lineInd);
                return FALSE;
            }
        }
    }
    numIntVal = atoi(buffer);
    /* checking if the number operand can be store inside a 16bit variable */
if (numIntVal > (unsigned) highestValue || numIntVal < (unsigned) lowestValue) {
        raiseNumFlag(linePointer, lineInd);
        return FALSE;
    }

    /* If it is a valid number and first index is not initialized */
    if (linePointer[lineInd].isSrcNum == FALSE && linePointer[lineInd].isSrcReg == FALSE &&
        linePointer[lineInd].isSrcLabel == FALSE) {
        linePointer[lineInd].srcNumValue = numIntVal;
        linePointer[lineInd].isSrcNum = TRUE;
    }

    /* If it is a valid number and first index already initialized */
    if (linePointer[lineInd].isSrcNum == TRUE || linePointer[lineInd].isSrcReg == TRUE ||
        linePointer[lineInd].isSrcLabel == TRUE) {
        linePointer[lineInd].destNumValue = numIntVal;
        linePointer[lineInd].isDestNum = TRUE;
    }
    return TRUE;
}

void checkStop(line *linePointer, int lineInd, char *buffer, int firstArgInd) {
    iterateUntilEnd(linePointer, lineInd, buffer,
                    firstArgInd);/* in case of a valid stop command the only non-blank char that comes  after stop is a  new line char,this function check's it */
    /* the opcode was coded in other function */
}


/* valid extern functions */
/* checking an extern line with a label */
void extArgCheckWLabel(line *linePointer, label *labelPointer, int lineInd, int labelInd, char *buffer) {
    char argBuffer[maxLabelLen];/* extern argument needs to be a valid label */
    int argInd, argBufferInd;
    argInd = firstArgIndWLabel(buffer);
    argBufferInd = 0;
    for (; isspace(buffer[argInd]) == 0 && buffer[argInd] != '\n'; argInd++) {
        argBuffer[argBufferInd++] = buffer[argInd];
    }
    argBuffer[argBufferInd] = '\0';
    /* this function checks if there is a non-blank char that comes after .extern label argument */
    iterateUntilEnd(linePointer, lineInd, buffer, argInd);

    if (argLabelCheck(argBuffer) == 0) {
        raiseLabelArgError(linePointer, lineInd);
        return;
    }
    if (extArgDuplication(labelPointer, labelInd, argBuffer)) {
        raiseLabelArgError(linePointer, lineInd);
        return;
    }

    linePointer[lineInd].validLabel = FALSE;/* giving the valid label flag a false value in order the program will ignore the extern label and wont create an extra cell in the label array */
    linePointer[lineInd].ext.extArg = (char *) calloc(maxLabelLen, sizeof(char));
    strcpy(linePointer[lineInd].ext.extArg, argBuffer);
}

/* checking an extern line without a label */
void extArgCheckNoLabel(line *linePointer, label *labelPointer, int lineInd, int labelInd, char *buffer) {
    char argBuffer[maxLabelLen];/* extern argument needs to be a valid label */
    int argInd, argBufferInd;
    argInd = firstArgIndNoLabel(buffer);
    argBufferInd = 0;
    for (; isspace(buffer[argInd]) == 0 && buffer[argInd] != '\n'; argInd++) {
        argBuffer[argBufferInd++] = buffer[argInd];
    }
    argBuffer[argBufferInd] = '\0';
    argInd++;
    /* this function checks if there is a non-blank char that comes after .extern label argument */
    iterateUntilEnd(linePointer, lineInd, buffer, argInd);
    if (argLabelCheck(argBuffer) == 0) {/* if the label arg s not a valid label raise an error flag */
        raiseLabelArgError(linePointer, lineInd);
        return;
    }
    if (extArgDuplication(labelPointer, labelInd, argBuffer)) {
        raiseLabelArgError(linePointer, lineInd);
        return;
    }
    linePointer[lineInd].ext.extArg = (char *) calloc(maxLabelLen, sizeof(char));
    strcpy(linePointer[lineInd].ext.extArg, argBuffer);
}

int extArgDuplication(label *labelPointer, int labelInd, char *buffer) {
    int ind;
    for (ind = 0; ind < labelInd; ind++) {
        if (strcmp(labelPointer[ind].labelBuff, buffer) == 0) {
            /* if the extern arg has been given to a non extern reserved word and this label is not declared as an ignored label return an error */
            if (labelPointer[ind].isExt == FALSE && labelPointer[labelInd].ignoreLabel == FALSE) {
                return 1;/* there is an error label duplication */
            }
        }
    }
    return 0;
}

/* this function check .extern argument */
void extArgTest(line *linePointer, label *labelPointer, int lineInd, int labelInd, char *buffer) {
    if (linePointer[lineInd].validLabel == TRUE) {
        extArgCheckWLabel(linePointer, labelPointer, lineInd, labelInd, buffer);
    } else if (linePointer[lineInd].validLabel == FALSE) {
        extArgCheckNoLabel(linePointer, labelPointer, lineInd, labelInd, buffer);
    }
}/* end of valid extern functions */

/* entry label saving functions */
/* Copies entry's argument in order to check if it is valid in the second pass */
void handleEntryArg(line *linePointer, int lineInd, char *buffer) {
    int firstArgInd, labelBuffInd, nonBlankCounter;
    char labelBuff[maxLabelLen];
    labelBuffInd = 0, nonBlankCounter = 0;
    if (linePointer[lineInd].validLabel ==
        TRUE)/* the index value of the first argument changes in case of a label */
        firstArgInd = firstArgIndWLabel(buffer);
    else
        firstArgInd = firstArgIndNoLabel(buffer);
    /* looping from the begging of the argument until the buffer end's or until it gets a blank char */
    for (; buffer[firstArgInd] != '\n' && isspace(buffer[firstArgInd]) == 0; firstArgInd++) {
        /* if th argument length is bigger than the max size of a label it must be an invalid one */
        if (labelBuffInd > maxLabelLen) {
            raiseLabelArgError(linePointer, lineInd);
            return;
        }
        nonBlankCounter++;
        labelBuff[labelBuffInd++] = buffer[firstArgInd];
    }
    if (nonBlankCounter == 0) {/* it means that there wasn't an argument after the entry reserved word */
        /* raise the missing argument flag in order to print a valid error input */
        raiseMissingOperandFlags(linePointer, lineInd);
        return;
    }
    /* a function that checks if there is another extraneous argument, if there is - it raises a relevant flag */
    iterateUntilEnd(linePointer, lineInd, buffer, firstArgInd);
    labelBuff[labelBuffInd] = '\0';
    /* initializing dynamic memory in order to save the entry argument for a smoother experience in the second pass */
    linePointer[lineInd].entryArgument = (char *) calloc(maxLabelLen, sizeof(char));
    strcpy(linePointer[lineInd].entryArgument, labelBuff);
}/* end of entry label saving functions */

/* instructions args functions */
/* Raise an error flags in case of invalid .String char argument */
void raiseStringErrorFlags(line *linePointer, int lineInd) {
    linePointer[lineInd].invalid_string_stringArg = TRUE;
    linePointer[lineInd]._isError = TRUE;
}

/* this function raise a error flags in case of invalid .d argument */
void raiseDotDErrorFlags(line *linePointer, int lineInd) {
    linePointer[lineInd].invalid_rang_dotD_arg = TRUE;
    linePointer[lineInd]._isError = TRUE;
}

/* Handle the instruction's arguments based on their kind */
void instructionsArgsTest(line *linePointer, int lineInd, char *buffer, int *DC) {
    int firstArgInd;
    /* The first argument index value depends on whether the line contains a label or not */
    if (linePointer[lineInd].validLabel == TRUE)
        firstArgInd = firstArgIndWLabel(buffer);
    else
        firstArgInd = firstArgIndNoLabel(buffer);
/* giving the instruction the right function */
    if (linePointer[lineInd].is_string) {
        checkAndCodeString(linePointer, lineInd, buffer, firstArgInd, DC);
    } else
        CodeDot_d(linePointer, lineInd, buffer, firstArgInd, DC);
}

/* this function check and code the argument in an String type instruction line */
void checkAndCodeString(line *linePointer, int lineInd, char *buffer, int firstArgInd, int *DC) {
    int numOfChars;
    numOfChars = 0;
    if (buffer[firstArgInd] != parenthesesAscii) {/* valid .String argument must start with parentheses */
        raiseStringErrorFlags(linePointer, lineInd);
        return;
    }
    firstArgInd++;
    for (; buffer[firstArgInd] != parenthesesAscii && buffer[firstArgInd] !=
                                                                 '\n'; firstArgInd++) {/* looping the argument until a new line char of prenthesses */
        if (isprint(buffer[firstArgInd])) {
            numOfChars++;
            if (numOfChars == 1) {
                linePointer[lineInd].stringDataImage.stringArray = calloc(numOfChars, sizeof(char));
                linePointer[lineInd].stringDataImage.stringArray[numOfChars - 1] = buffer[firstArgInd];
                linePointer[lineInd].stringDataImage.biteSize += 1;
            } else {
                linePointer[lineInd].stringDataImage.stringArray = realloc(
                        linePointer[lineInd].stringDataImage.stringArray, numOfChars);
                if (!(linePointer[lineInd].stringDataImage.stringArray)) {/* in case of an allocation error raise relevant flags and return */
                    raiseAllocationError(linePointer, lineInd);
                    free(linePointer[lineInd].stringDataImage.stringArray);
                    return;
                }
                linePointer[lineInd].stringDataImage.stringArray[numOfChars - 1] = buffer[firstArgInd];
                linePointer[lineInd].stringDataImage.biteSize += 1;/* increasing the bite size in order to track the arguments array */
            }
        } else {/* printable chars are the only ones allowed */
            raiseStringErrorFlags(linePointer, lineInd);
            free(linePointer[lineInd].stringDataImage.stringArray);
            return;
        }
    }
    if (buffer[firstArgInd] == parenthesesAscii) {/* it means the argument ended in a valid way */
        numOfChars++;
        linePointer[lineInd].stringDataImage.stringArray = realloc(linePointer[lineInd].stringDataImage.stringArray,
                                                                   numOfChars);/* extending the array in order to put a string ending char in the end */
        if (!(linePointer[lineInd].stringDataImage.stringArray)) {
            raiseAllocationError(linePointer, lineInd);
            free(linePointer[lineInd].stringDataImage.stringArray);
            return;
        }
        linePointer[lineInd].stringDataImage.stringArray[numOfChars -
                                                         1] = '\0';/* a null char is needed in strings */
        linePointer[lineInd].stringDataImage.biteSize += 1;/* increasing the bite size in order to track the arguments array */
        *DC += linePointer[lineInd].stringDataImage.biteSize;/* extending dc size by adding the amount of bites the String instruction takes in order to track the next instruction address */

    } else {
        raiseStringErrorFlags(linePointer, lineInd);
        free(linePointer[lineInd].stringDataImage.stringArray);
    }
    firstArgInd++;/* in to pass the comma it stopped on */
    iterateUntilEnd(linePointer, lineInd, buffer, firstArgInd);
}

/* this function get a .data instruction and points it to the right function */
void CodeDot_d(line *linePointer, int lineInd, char *buffer, int firstArgInd, int *DC) {
    if (linePointer[lineInd].is_data == TRUE) {
        checkAndCode_dot_d(linePointer, lineInd, buffer, firstArgInd, dataMaxArgLen, 1);
        *DC += linePointer[lineInd].dataImage.biteSize;/* extending dc by adding the amount of bites given as arguments in order to track the next line address */
    }
}

/* Check .data arguments and copies them into the data image if they are valid */
void checkAndCode_dot_d(line *linePointer, int lineInd, char *buffer, int firstArgInd, int argLen, int numOfBites) {
    int argVal, numOfArgs, argIndStart;
    char argBuffer[dataMaxArgLen]; /* initializing the argBuffer to the size of the highest argument the instruction can get*/
    numOfArgs = 0, argIndStart = 0;
    while (firstArgInd > 0) {/* -2 means that all the lines was checked, -1 means that an error has occurred */
        firstArgInd = fillUpInsArgBuffer(linePointer, lineInd, buffer, argBuffer, argLen, firstArgInd);

        if (firstArgInd == -1)/* -1 means that an error has occurred */
            return;
        argVal = atoi(argBuffer);
        /* checking if the num of bites each argument can get is able to hold the argument value */
        if (valid_dot_d_argVal(linePointer, lineInd, argBuffer) == 0) {
            raiseDotDErrorFlags(linePointer, lineInd);/* raising a dot d directive "argument out of range" flag */
            free(linePointer[lineInd].dataImage.argumentsArray);
            return;
        }
        numOfArgs++;/* increasing the number of arguments by one */
        if (numOfArgs == 1) {
            linePointer[lineInd].dataImage.argumentsArray = (char *) calloc(numOfBites, sizeof(char));
            /* coping argument checked value into argument array ina specific num of bites*/
            memcpy(&linePointer[lineInd].dataImage.argumentsArray[argIndStart], &argVal, numOfBites);
            /* increasing bite size in order to track the num of bites we got from arguments */
            linePointer[lineInd].dataImage.biteSize += numOfBites;
            /* increasing the bite index in order to track the arguments array */
            argIndStart += numOfBites;
        } else { /* when the pointer was already initialized with memory, realloc for more */
            linePointer[lineInd].dataImage.argumentsArray = (char *) realloc(
                    linePointer[lineInd].dataImage.argumentsArray, numOfArgs * numOfBites * sizeof(char));
            if (!(linePointer[lineInd].dataImage.argumentsArray)) {/* if there are no enough space for memory reallocation */
                raiseAllocationError(linePointer, lineInd);/* raise allocation error flag */
                free(linePointer[lineInd].dataImage.argumentsArray);
                return;
            }
            memcpy(&linePointer[lineInd].dataImage.argumentsArray[argIndStart], &argVal, numOfBites);
            linePointer[lineInd].dataImage.biteSize += numOfBites;/* increasing bite size in order to track the num of bites we got from arguments */
            argIndStart += numOfBites;/* increasing the bite index in order to track the arguments array */
        }
        strcpy(argBuffer, "");/* this line cleans up the buffer before it gets the new argument */


    }
    if (numOfArgs == 0) {
        raiseMissingOperandFlags(linePointer, lineInd);
        free(linePointer[lineInd].dataImage.argumentsArray);
    }
}

/* Get an ind variable and a buffer pointer and filling buffer with the argument until it reaches a new line or a comma, and return's the updates index */
int fillUpInsArgBuffer(line *linePointer, int lineInd, char *buffer, char *argBuffer, int maxLen, int firstArgInd) {
    int blankFlag, argBufferInd, argLen, curArgStart;
    blankFlag = 0, argBufferInd = 0, curArgStart = firstArgInd;
    argLen = 0; /* a variable that is meant to track the arg length */
    /* a loop that was created in order to pass all blank chars that may appear after comma */
    for (; buffer[firstArgInd] != '\n' && isspace(buffer[firstArgInd]); firstArgInd++);

    /* a loop that checks if the argument is valid and fills up his buffer */
    for (; buffer[firstArgInd] != '\n' && buffer[firstArgInd] != commaAscii; firstArgInd++) {
        if (isspace(buffer[firstArgInd])) {
            blankFlag = 1;/* "turning on" the blank char flag */
            continue;/* there is no point in getting the blank chars inside the argument buffer */
        }
        if (blankFlag == 1 && isspace(buffer[firstArgInd]) == 0) {
            raiseExtraneousOperandFlags(linePointer,
                                        lineInd);/* blank chars are not allowed between argument numbers */
            return -1;
        }
        if (isdigit(buffer[firstArgInd] == 0)) {
            if (!(firstArgInd == curArgStart && (buffer[firstArgInd] == plusAscii || buffer[firstArgInd] ==
                                                                                     minusAscii))) {/* excluding the minus and the plus signs that may can in the beginning */
                raiseDotDErrorFlags(linePointer, lineInd);/* blank chars are not allowed between argument numbers */
                return -1;
            }
        }
        if (argLen == maxLen) {/* ensuring that we dont get an argument that cant be valid by his length */
            raiseDotDErrorFlags(linePointer, lineInd);;/* blank chars are not allowed between argument numbers */
            return -1;
        }
        argBuffer[argBufferInd++] = buffer[firstArgInd];
        argLen++;
    }
    argBuffer[argBufferInd] = '\0';
    if (buffer[firstArgInd] == commaAscii) {
        firstArgInd++;/* in order to pass the comma */
    } else if (buffer[firstArgInd] == '\n') {
        return -2;/* it means that there are no more arguments to check */
    }
    return firstArgInd;
} /* end of fillUpInsArgBuffer */

/* Checks if the argument that was given to a .d instruction is valid based on her type */
int valid_dot_d_argVal(line *linePointer, int lineInd, char *buffer) {
    int argVal;
    argVal = atoi(buffer);

    if (linePointer[lineInd].is_data == TRUE) {
        if (argVal < dataMin || argVal > dataMax)
            return 0;
        else
            return 1;
    }
return 0;
}/* end of valid .d argument value function */

/* Raise a memory allocation error flag */
void raiseAllocationError(line *linePinter, int lineInd) {
    linePinter[lineInd].allocationError = TRUE;
    linePinter[lineInd]._isError = TRUE;
}

/* Raise an invalid label argument error flag */
void raiseLabelArgError(line *linePointer, int lineInd) {
    linePointer[lineInd].invalid_labelArg = TRUE;
    linePointer[lineInd]._isError = TRUE;
}


/* Raise an invalid label argument error flag */
void raiseRegErrorFlags(line *linePointer, int lineInd) {
    linePointer[lineInd].invalid_register = TRUE;
    linePointer[lineInd]._isError = TRUE;
}

/* Raise an invalid label argument error flag */
void raiseNumFlag(line *linePointer, int lineInd) {
    linePointer[lineInd].invalid_number = TRUE;
    linePointer[lineInd]._isError = TRUE;
}
