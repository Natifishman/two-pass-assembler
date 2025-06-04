#include "outputPrint.h"

/* Produce the final output by creating the needed output files based on the input */
void
createOutput(line *linePointer, int lineInd, label *labelPointer, int labelInd, int IC, int DC, int *numOfPrintedBites,
             char *inputFileName, int inputFileLen) {
    /* a pointer that will contain a copy of the current input file we got from argv, this copy will prevent changes in the argv value that will effect programs efficiency */
    char currArgVCopy[maxLineLen];
    strcpy(currArgVCopy, inputFileName); /* coping the current input file from argv to his copy buffer) */
    /* calling the function that will create the .ob output file */
    createDotObFile(linePointer, lineInd, labelPointer, labelInd, IC, DC, numOfPrintedBites, currArgVCopy);
    /* calling the function that will create the .ent file if there is a need for one */
    createDotEnt(labelPointer, labelInd, currArgVCopy);
    /* calling the function that will create the .ext file if there is a need for one */
    createDotExt(linePointer, lineInd, currArgVCopy);
}

/* this function creates the ps.ob output file */
void createDotObFile(line *linePointer, int lineInd, label *labelPointer, int labelInd, int IC, int DC,
                     int *numOfPrintedByte, char *inputFileName) {
    /* variable declaration */
    int numOfCodeBytes, numOfDataBytes, currLineInd;
    long address;
    char obEndingbuffer[DotObEndingLen];
    FILE *obPointer;
    /* creating the .ob File name buffer that will be based on the input file */
    strcpy(obEndingbuffer, ".ob");
    obEndingbuffer[DotObEndingLen - 1] = '\0';
    inputFileName[strlen(inputFileName) - (DotObEndingLen - 1)] = '\0';
    inputFileName = strcat(inputFileName, obEndingbuffer);
    address = 100;

    obPointer = fopen(inputFileName, "w");
    /* end of creating the .ob File name buffer */

    /* calculating and printing the amount of code and data bytes in the beggining of the .ob file */
    numOfCodeBytes = IC - 100;
    numOfDataBytes = DC;
    /* printing the amount of bytes that were used by commands and instructions in the beginning of the files */
    fprintf(obPointer, "   %d    %d\n", numOfCodeBytes, numOfDataBytes);
    /* a for loop that goes through all commands and print's them */
    for (currLineInd = 0; currLineInd <= lineInd; currLineInd++) {
        /* giving the command line the right printing function based on her specific type */
        if (linePointer[currLineInd].isOne == TRUE) {
            printOneToOb(obPointer, linePointer, currLineInd, &address);
        } else if (linePointer[currLineInd].isTwo == TRUE) {
            printTwoToOb(obPointer, linePointer, currLineInd, &address);
        } else if (linePointer[currLineInd].isThree == TRUE) {
            printThreeToOb(obPointer, linePointer, currLineInd, &address);
        } else if (linePointer[currLineInd].isFour == TRUE) {
            printFourToOb(obPointer, linePointer, currLineInd, &address);
        }
    }
    /* a for loop that goes through directives only and print's their data image based on their type */
    for (currLineInd = 0; currLineInd <= lineInd; currLineInd++) {
        if (linePointer[currLineInd].is_string == TRUE) {
            printStringToOB(obPointer, linePointer, currLineInd, &address, numOfPrintedByte);
        } else if (linePointer[currLineInd].is_data == TRUE) {
            printDotD(obPointer, linePointer, currLineInd, &address, numOfPrintedByte);
        }
    }
}

void printOneToOb(FILE *obPointer, line *linePointer, int currLineInd, long *address) {
    unsigned int
    fourteenBitVariable, tempVal;  /* a 14 bit variable that will be filled the 14 struct */
    int ind;
    fourteenBitVariable = 0;

    /* filling the 14 bit variable, entered by moving the right amount of bytes left,after the that the next most significant is being filled the same way*/
    tempVal = linePointer[currLineInd].word.opcode;
    fourteenBitVariable |= (tempVal << 6); /* 6-9 bits - opcode*/

    /* printing the address first */
    fprintf(obPointer, "%04ld ", *address);
    fourteenBitVariable = *convertBinary(fourteenBitVariable);

    fprintf(obPointer, "%X", (fourteenBitVariable));

    fprintf(obPointer, "\n");
    *address += 1;/* increasing address by the number of words */
}

void printTwoToOb(FILE *obPointer, line *linePointer, int currLineInd, long *address) {
    unsigned int
    fourteenBitVariable, tempVal;/* a 14 bit variable that will be filled the 14 struct */
    int ind;
    fourteenBitVariable = 0;
    /* filling the 14 bit variable */
    tempVal = linePointer[currLineInd].word.opcode;
    fourteenBitVariable |= (tempVal << 6); /* 6-9 bits */

    /* Checks source type */
    if (linePointer[currLineInd].isDestNum == TRUE) {
        newDestNumberWord(obPointer, linePointer,currLineInd,*address+1);
    } else if (linePointer[currLineInd].isDestLabel == TRUE) {
        tempVal = 1;
        fourteenBitVariable |= (tempVal << 2); /* 2-3 bits */
    } else if (linePointer[currLineInd].isDestReg == TRUE) {
        newDestRegisterWord(obPointer, linePointer,currLineInd,*address+1);
        tempVal = 3;
        fourteenBitVariable |= (tempVal << 2); /* 2-3 bits */
    }

    /* printing the address first */
    fprintf(obPointer, "%04ld ", *address);

    /* Convert to wanted form */
    fourteenBitVariable = *convertBinary(fourteenBitVariable);
    fprintf(obPointer, "%X", (fourteenBitVariable));

    fprintf(obPointer, "\n");
    *address += 2;/* increasing address by the number of words */
}

void printThreeToOb(FILE *obPointer, line *linePointer, int currLineInd, long *address) {
    unsigned int
    fourteenBitVariable;/* a 14 bit variable that will be filled the 14 struct */
    unsigned int tempVal;
    int ind;
    fourteenBitVariable = 0;
    /* filling the 14 bit variable */
    tempVal = linePointer[currLineInd].word.opcode;
    fourteenBitVariable |= (tempVal << 6); /* 6-9 bits */

    /* Checks source type */
    if (linePointer[currLineInd].isSrcReg == TRUE && linePointer[currLineInd].isDestReg == TRUE) {
        tempVal = linePointer[currLineInd].srcRegValue;
        newCombinedRegisterWord(obPointer, linePointer,currLineInd,*address+1);
    } else if (linePointer[currLineInd].isSrcReg == TRUE) {
        newSrcRegisterWord(obPointer, linePointer,currLineInd,*address+1);
        tempVal = linePointer[currLineInd].srcRegValue;
    } else if (linePointer[currLineInd].isSrcNum == TRUE) {
        newSrcNumberWord(obPointer, *linePointer,currLineInd,*address+1);
        tempVal = linePointer[currLineInd].srcNumValue;
    } else if (linePointer[currLineInd].isSrcLabel == TRUE) {
        tempVal = linePointer[currLineInd].srcLabelName;
    }
    fourteenBitVariable |= (tempVal << 4); /* 4-5 bits */

    /* Checks source type */
    if (linePointer[currLineInd].isDestReg == TRUE&&linePointer[currLineInd].isSrcReg == FALSE) {
        newDestRegisterWord(obPointer, linePointer,currLineInd,*address+1);
        tempVal = linePointer[currLineInd].destRegValue;
    } else if (linePointer[currLineInd].isDestNum == TRUE) {
        newDestNumberWord(obPointer, linePointer,currLineInd,*address+1);
        tempVal = linePointer[currLineInd].destNumValue;
    } else if (linePointer[currLineInd].isDestLabel == TRUE) {
        tempVal = linePointer[currLineInd].destLabelName;
    }
    fourteenBitVariable |= (tempVal << 2); /* 2-3 bits */

    /* printing the address first */
    fprintf(obPointer, "%04ld ", *address);

    /* Convert to wanted form */
    fourteenBitVariable = *convertBinary(fourteenBitVariable);
    fprintf(obPointer, "%X", (fourteenBitVariable));
    fprintf(obPointer, "\n");
    *address += linePointer[currLineInd].numOfWords;/* increasing address by the number of words */
}

void printFourToOb(FILE *obPointer, line *linePointer, int currLineInd, long *address) {
    unsigned int
    fourteenBitVariable;/* a 14 bit variable that will be filled the 14 struct */
    unsigned int tempVal;
    int ind;
    fourteenBitVariable = 0;
    /* filling the 14 bit variable */
    tempVal = 2;
    fourteenBitVariable |= (tempVal << 2); /* 2-3 bits */

    tempVal = linePointer[currLineInd].word.opcode;
    fourteenBitVariable |= (tempVal << 6); /* 6-9 bits */

    /* Checks source type */
    if (linePointer[currLineInd].isSrcReg == TRUE && linePointer[currLineInd].isDestReg == TRUE) {
        tempVal = linePointer[currLineInd].srcRegValue;
        newCombinedRegisterWord(obPointer, *linePointer,currLineInd,*address+1);
    } else if (linePointer[currLineInd].isSrcReg == TRUE) {
        newSrcRegisterWord(obPointer, *linePointer,currLineInd,*address+1);
        tempVal = linePointer[currLineInd].srcRegValue;
    } else if (linePointer[currLineInd].isSrcNum == TRUE) {
        newSrcNumberWord(obPointer, *linePointer,currLineInd,*address+1);
        tempVal = linePointer[currLineInd].srcNumValue;
    } else if (linePointer[currLineInd].isSrcLabel == TRUE) {
        tempVal = linePointer[currLineInd].srcLabelName;
    }
    fourteenBitVariable |= (tempVal << 12); /* 12-13 bits */

    /* Checks source type */
    if (linePointer[currLineInd].isDestReg == TRUE&&linePointer[currLineInd].isSrcReg == FALSE) {
        newDestRegisterWord(obPointer, linePointer,currLineInd,*address+1);
        tempVal = linePointer[currLineInd].destRegValue;
    } else if (linePointer[currLineInd].isDestNum == TRUE){
	newDestNumberWord(obPointer, linePointer,currLineInd,*address+1);
        tempVal = linePointer[currLineInd].destNumValue;
    } else if (linePointer[currLineInd].isDestLabel == TRUE) {
        tempVal = linePointer[currLineInd].destLabelName;
    }
    fourteenBitVariable |= (tempVal << 10); /* 10-11 bits */

    /* printing the address first */
    fprintf(obPointer, "%04ld ", *address);

    /* Convert to wanted form */
    fourteenBitVariable = *convertBinary(fourteenBitVariable);
    fprintf(obPointer, "%X", (fourteenBitVariable));
    fprintf(obPointer, "\n");
    *address += linePointer[currLineInd].numOfWords;/* increasing address by the number of words */
}

void newSrcNumberWord(FILE *obPointer, line *linePointer, int currLineInd, long *address) {
    unsigned int
    fourteenBitVariable, tempVal;/* a 14 bit variable that will be filled the 14 struct */
    int ind;
    fourteenBitVariable = 0;
    /* filling the 14 bit variable */
    tempVal = linePointer[currLineInd].srcNumValue;
    fourteenBitVariable |= (tempVal << 2); /* 2-13 bits */

    /* printing the address first */
    fprintf(obPointer, "%04ld ", *address);

    /* Convert to wanted form */
    fourteenBitVariable = *convertBinary(fourteenBitVariable);
    fprintf(obPointer, "%X", (fourteenBitVariable));
}

void newDestNumberWord(FILE *obPointer, line *linePointer, int currLineInd, long *address) {
    unsigned int
    fourteenBitVariable, tempVal;/* a 14 bit variable that will be filled the 14 struct */
    int ind;
    fourteenBitVariable = 0;
    /* filling the 14 bit variable */
    tempVal = linePointer[currLineInd].destNumValue;
    fourteenBitVariable |= (tempVal << 2); /* 2-13 bits */

    /* printing the address first */
    fprintf(obPointer, "%04ld ", *address);

    /* Convert to wanted form */
    fourteenBitVariable = *convertBinary(fourteenBitVariable);
    fprintf(obPointer, "%X", (fourteenBitVariable));
}

void newCombinedRegisterWord(FILE *obPointer, line *linePointer, int currLineInd, long *address) {
    unsigned int fourteenBitVariable, tempVal;/* a 14 bit variable that will be filled the 14 struct */
    int ind;
    fourteenBitVariable = 0;
    /* filling the 14 bit variable */
    tempVal = linePointer[currLineInd].destRegValue;
    fourteenBitVariable |= (tempVal << 2); /* 2-7 bits - register*/

    tempVal = linePointer[currLineInd].srcRegValue;
    fourteenBitVariable |= (tempVal << 8); /* 8-13 bits - register*/

    /* printing the address first */
    fprintf(obPointer, "%04ld ", *address);

    /* Convert to wanted form */
    fourteenBitVariable = *convertBinary(fourteenBitVariable);
    fprintf(obPointer, "%X", (fourteenBitVariable));
}

void newSrcRegisterWord(FILE *obPointer, line *linePointer, int currLineInd, long *address) {
    unsigned int
    fourteenBitVariable, tempVal;/* a 14 bit variable that will be filled the 14 struct */
    int ind;
    fourteenBitVariable = 0;
    /* filling the 14 bit variable */
    tempVal = linePointer[currLineInd].srcRegValue;
    fourteenBitVariable |= (tempVal << 8); /* 8-13 bits - register*/

    /* printing the address first */
    fprintf(obPointer, "%04ld ", *address);

    /* Convert to wanted form */
    fourteenBitVariable = *convertBinary(fourteenBitVariable);
    fprintf(obPointer, "%X", (fourteenBitVariable));
}

void newDestRegisterWord(FILE *obPointer, line *linePointer, int currLineInd, long *address) {
/* a 14 bit variable that will be filled the 14 struct */
    unsigned int fourteenBitVariable, tempVal;
    int ind;
    fourteenBitVariable = 0;
    /* filling the 14 bit variable */
    tempVal = linePointer[currLineInd].destNumValue;
    fourteenBitVariable |= (tempVal << 2); /* 2-7 bits - register*/

    /* printing the address first */
    fprintf(obPointer, "%04ld ", *address);

    /* Convert to wanted form */
    fourteenBitVariable = *convertBinary(fourteenBitVariable);
    fprintf(obPointer, "%X", (fourteenBitVariable));
}

/*  Gets a ptr to array of 14 bit chars, each char is '0' or '1'
	return pointer to array of 14 bit chars of the special base:
	replacing each '0' with '.' and replacing each '1' with '/'  */
char *convertBinary(char *binary_word_p) {
    char *ret_p = binary_word_p;/* allocating memory for weird word,the array pointer to be returned*/
    char *runner_p; /* a temp runner to assign values in ret_p */
    runner_p = ret_p; /* initialize runner to point to first char of result */

    /* assign each cell with appropriate char */
    while (*binary_word_p != '\0') {
        if (*binary_word_p == '0')
            *runner_p = '.';
        else
            *runner_p = '/';
        ++runner_p;
        ++binary_word_p;
    }
    *runner_p = '\0'; /* assign end of string flag*/
    return ret_p;
}

/* this function goes true the string type instruction line and print the data it contains */
void printStringToOB(FILE *obPointer, line *linePointer, int currLineInd, long *address, int *numOfPrintedByte) {
    int ind;
    if (*numOfPrintedByte == 0) {
        fprintf(obPointer, "%04ld ", *address);/* printing the address  before the new line starts */
    }
    for (ind = 1; ind <= linePointer[currLineInd].stringDataImage.biteSize; ind++) {
        *numOfPrintedByte += 1;
        fprintf(obPointer, "%X", (linePointer[currLineInd].stringDataImage.stringArray[ind - 1] >> 4) &
                                 0x0F);/* high nibble, masking it with 0x0F */
        fprintf(obPointer, "%X ", linePointer[currLineInd].stringDataImage.stringArray[ind - 1] &
                                  0x0F);/* low nibble, masking it with 0x0F */
        if (*numOfPrintedByte % 4 == 0) {
            *address += 4;
            fprintf(obPointer, "\n");/*a new line is needed after printing 4 bites */
            fprintf(obPointer, "%04ld ", *address);/* printing the address  before the new line starts */
        }
    }
    /* freeing dynamic memory after it was used */
    free(linePointer[currLineInd].stringDataImage.stringArray);
}

/* this function print's all .d lines argument in hex */
void printDotD(FILE *obPointer, line *linePointer, int currLineInd, long *address, int *numOfPrintedByte) {
    int ind, argumentBiteSize;
    if (*numOfPrintedByte == 0) {
        fprintf(obPointer, "%04ld ", *address);/* printing the address  before the new line starts */
    }
    for (ind = 1; ind <= linePointer[currLineInd].dataImage.biteSize; ind++) {
        *numOfPrintedByte += 1;
        fprintf(obPointer, "%X", (linePointer[currLineInd].dataImage.argumentsArray[ind - 1] >> 4) &
                                 0x0F);/* high nibble, masking it with 0x0F */
        fprintf(obPointer, "%X ", linePointer[currLineInd].dataImage.argumentsArray[ind - 1] &
                                  0x0F);/* low nibble, masking it with 0x0F */
        if (*numOfPrintedByte % 4 == 0) {
            *address += 4;/* increasing address value by 4 */
            fprintf(obPointer, "\n");/*a new line is needed after printing 4 bites */
            fprintf(obPointer, "%04ld ", *address);/* printing the address  before the new line starts */
        }
    }
    /* freeing dynamic memory after it was used */
    free(linePointer[currLineInd].dataImage.argumentsArray);
}

/* this function creates the ps.ext outputFile only if there are valid .extern instruction lines in the input file */
void createDotExt(line *linePointer, int lineInd, char *inputFileName) {
    int currLineInd, currExtAddressInd;/* creating index variables for the iterations */
    FILE *extPointer;/* creating a file type pointer for the ext file */
    char extEndingbuffer[DotExtEndingLen];
    if (!(externTypeLinesScanner(linePointer, lineInd)))
        return;
    strcpy(extEndingbuffer, ".ext");
    extEndingbuffer[DotExtEndingLen - 1] = '\0';
    inputFileName[(strlen(inputFileName)) - (DotExtEndingLen - 1)] = '\0';
    inputFileName = strcat(inputFileName, extEndingbuffer);
    extPointer = fopen(inputFileName, "w");/* creating the ps.ext file */
    for (currLineInd = 0; currLineInd <=
                          lineInd; currLineInd++) {/* looping true the valid lines in order to check if there are valid .extern lines */
        if (linePointer[currLineInd].isExtern == TRUE) {
            /* looping through the external addresses array - each external label can hold more than one address it was used in */
            for (currExtAddressInd = 0;
                 currExtAddressInd < linePointer[currLineInd].ext.addressInd; currExtAddressInd++) {
                fprintf(extPointer, "%s ", linePointer[currLineInd].ext.extArg);/* printing the external label */
                /* printing the address of the line that used the external label from the external addresses dynamic array */
                fprintf(extPointer, "%04d\n", linePointer[currLineInd].ext.extAddress[currExtAddressInd]);
            }
            /* freeing dynamic memory after it was used */
            free(linePointer[currLineInd].ext.extArg);
            free(linePointer[currLineInd].ext.extAddress);
        }
    }

}

int externTypeLinesScanner(line *linePointer, int lineInd) {
    int currLineInd;
    for (currLineInd = 0; currLineInd <= lineInd; currLineInd++) {
        if (linePointer[currLineInd].isExtern == TRUE)
            return 1;
    }
    return 0;
}

/* Creates the ps.ent outputFile only if there are valid .entry instruction lines in the input file */
void createDotEnt(label *labelPointer, int labelInd, char *inputFileName) {
    int currLabelInd;/* creating index variable for the iteration */
    FILE *entPointer;/* creating a file type pointer for the .ent file */
    char entEndingbuffer[DotEntEndingLen];
    /* calling this function in order to realize if if a .ent file is needed */
    if (!(entryTypeLabelScanner(labelPointer, labelInd)))
        return;
    strcpy(entEndingbuffer, ".ent");

    entEndingbuffer[DotEntEndingLen - 1] = '\0';
    inputFileName[strlen(inputFileName) - (DotEntEndingLen - 2)] = '\0';
    inputFileName = strcat(inputFileName, entEndingbuffer);
    entPointer = fopen(inputFileName, "w");
    for (currLabelInd = 0; currLabelInd <=
                           labelInd; currLabelInd++) {/* scanning the label's table in order to find valid entry type label's */
        if (labelPointer[currLabelInd].isEnt == TRUE) {
            fprintf(entPointer, "%s ", labelPointer[currLabelInd].labelBuff);/* printing the .entry label */
            if (labelPointer[currLabelInd].isCode == TRUE) {
                fprintf(entPointer, "%04d\n",
                        labelPointer[currLabelInd].ic);/* printing the address of the .entry label based on her other type */
            } else if (labelPointer[currLabelInd].isData == TRUE) {
                fprintf(entPointer, "%04d\n",
                        labelPointer[currLabelInd].dc);/* printing the address of the .entry label based on her other type */
            }
        }
    }
}

/* Help us know if a .ent file is needed */
int entryTypeLabelScanner(label *labelPointer, int labelInd) {
    int currLabelInd;/* creating index variable for the iteration */
    /* scanning the label's table in order to find valid entry type label's that will make us to open a .ent output file */
    for (currLabelInd = 0; currLabelInd <= labelInd; currLabelInd++) {
        if (labelPointer[currLabelInd].isEnt == TRUE) {
            return 1;
        }/* end of if */
    }/* end of for */
    return 0;
}/* end of entryTypeLabelScanner*/
