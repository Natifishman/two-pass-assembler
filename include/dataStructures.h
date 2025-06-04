#ifndef MAMAN14V10_DATASTRUCTURES_H
#define MAMAN14V10_DATASTRUCTURES_H
#define MaxLabelSize 31 /* it says that the max length of label is 30 + the '\0' string ending char */

#include <string.h>

/* booleans */
typedef enum {
    TRUE,
    FALSE
} myBool;
/* lines */
typedef struct line {
    int IC;
    int DC;
    int address;
    char *entryArgument;

    struct word {
        unsigned int encoding: 2;
        unsigned int srcValue: 2;
        unsigned int destValue: 2;
        unsigned int opcode: 4;
        unsigned int parameterOne: 2;
        unsigned int parameterTwo: 2;
    } word;

    struct labelWord {
        unsigned int encoding: 2;
        unsigned int adress: 12;
    } labelWord;

    struct numberWord {
        unsigned int encoding: 2;
        unsigned int adress: 12;
    } numberWord;

    struct registerWord {
        unsigned int encoding: 2;
        unsigned int destAdress: 6;
        unsigned int srcAdress: 6;
    } registerWord;

     unsigned int srcRegValue: 3;
        unsigned int srcNumValue: 14;
        unsigned int destRegValue: 3;
        unsigned int destNumValue: 3;
        unsigned int destLabelValue: 9;
    char *destLabelName;
    char *srcLabelName;

    /* Boleans */
    myBool isSrcReg;
    myBool isSrcNum;
    myBool isSrcLabel;
    myBool isDestReg;
    myBool isDestNum;
    myBool isDestLabel;
    myBool isMissedLabel;
    char* missedLabel;

    struct ext {
        char *extArg;
        int *extAddress;
        int addressInd;
    } ext;


    struct dot_d_dataImage {
        char *argumentsArray;
        int biteSize;
    } dataImage;


    struct stringDataImage {
        char *stringArray;
        int biteSize;
        int dc;
    } stringDataImage;


    /*flags*/
    int numOfWords;
    myBool validLabel;

    myBool noLabel;
    myBool command;
    myBool instruction;

    /* Num of words */
    myBool isOne;
    myBool isTwo;
    myBool isThree;
    myBool isFour;

    /* error flags based by error type's*/
    myBool _isError;
    myBool invalid_reserved;
    myBool invalid_label;
    myBool invalid_register;
    myBool invalid_labelArg;
    myBool invalid_number;
    myBool invalid_string_stringArg;
    myBool invalid_rang_dotD_arg;
    myBool undefined_label_arg;
    myBool allocationError;
    myBool missing_Operand;
    myBool extraneous_Operand;
    /* end of error flags */

    myBool ignoreLabel;
    myBool unrecognized_directive;
    myBool two_operands_two_registers;
    myBool one_operand_label;

    myBool isEntry;
    myBool absolute;
    myBool isRelocatable;
    myBool isExtern;

    /* instructions flags */
    myBool is_data;
    myBool is_string;
} line;

/* labels */
typedef struct label {
    char labelBuff[MaxLabelSize];
    int ic;
    int dc;
    myBool isCode;
    myBool isData;
    myBool isExt;
    myBool isEnt;
    myBool ignoreLabel;
} label;

/* initialize line */
void initialize_line_structures(line *, int);

/* initialize label */
void initialize_label_structures(label *, int);

#endif
