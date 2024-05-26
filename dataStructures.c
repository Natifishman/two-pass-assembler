#include "dataStructures.h"

/* Initializes the line type struct flags and interns struct */
void initialize_line_structures(line *linePointer, int lineInd) {
    /* Initialize all members of linePointer struct to default values */
    linePointer[lineInd].IC = 0;
    linePointer[lineInd].DC = 0;
    linePointer[lineInd].address = 0;
    linePointer[lineInd].entryArgument = NULL;


    linePointer[lineInd].IC = 0;
    linePointer[lineInd].DC = 0;
    linePointer[lineInd].address = 0;
    linePointer[lineInd].entryArgument = NULL;


    linePointer[lineInd].word.encoding = 0;
    linePointer[lineInd].word.srcValue = 0;
    linePointer[lineInd].word.destValue = 0;
    linePointer[lineInd].word.opcode = 0;
    linePointer[lineInd].word.parameterOne = 0;
    linePointer[lineInd].word.parameterTwo = 0;

    linePointer[lineInd].labelWord.encoding = 0;
    linePointer[lineInd].labelWord.adress = 0;

    linePointer[lineInd].numberWord.encoding = 0;
    linePointer[lineInd].numberWord.adress = 0;

    linePointer[lineInd].registerWord.encoding = 0;
    linePointer[lineInd].registerWord.destAdress = 0;
    linePointer[lineInd].registerWord.srcAdress = 0;

    linePointer[lineInd].srcRegValue = 0;
    linePointer[lineInd].srcNumValue = 0;
    linePointer[lineInd].destRegValue = 0;
    linePointer[lineInd].destNumValue = 0;
    linePointer[lineInd].destLabelValue = 0;
    strcpy(linePointer[lineInd].srcLabelName, ""); /* emptying the label buff */
    strcpy(linePointer[lineInd].destLabelName, ""); /* emptying the label buff */
    /* Boleans */
    linePointer[lineInd].isSrcReg = FALSE;
    linePointer[lineInd].isSrcNum = FALSE;
    linePointer[lineInd].isSrcLabel = FALSE;
    linePointer[lineInd].isDestReg = FALSE;
    linePointer[lineInd].isDestNum = FALSE;
    linePointer[lineInd].isDestLabel = FALSE;
    linePointer[lineInd].isMissedLabel = FALSE;
    strcpy(linePointer[lineInd].missedLabel, ""); /* emptying the label buff */
    /*flags*/
    linePointer[lineInd].numOfWords = 0;
    linePointer[lineInd].validLabel = FALSE;
    linePointer[lineInd].noLabel = FALSE;
    linePointer[lineInd].command = FALSE;
    linePointer[lineInd].instruction = FALSE;
    linePointer[lineInd].isOne = FALSE;
    linePointer[lineInd].isTwo = FALSE;
    linePointer[lineInd].isThree = FALSE;
    linePointer[lineInd].isFour = FALSE;
    /* error flags based by error types*/
    linePointer[lineInd]._isError = FALSE;
    linePointer[lineInd].invalid_reserved = FALSE;
    linePointer[lineInd].invalid_label = FALSE;
    linePointer[lineInd].invalid_register = FALSE;
    linePointer[lineInd].invalid_labelArg = FALSE;
    linePointer[lineInd].invalid_number = FALSE;
    linePointer[lineInd].invalid_string_stringArg = FALSE;
    linePointer[lineInd].invalid_rang_dotD_arg = FALSE;
    linePointer[lineInd].undefined_label_arg = FALSE;
    linePointer[lineInd].allocationError = FALSE;
    linePointer[lineInd].missing_Operand = FALSE;
    linePointer[lineInd].extraneous_Operand = FALSE;
    /* end of error flags */
    linePointer[lineInd].ignoreLabel = FALSE;
    linePointer[lineInd].unrecognized_directive = FALSE;

    linePointer[lineInd].isEntry = FALSE;
    linePointer[lineInd].absolute = FALSE;
    linePointer[lineInd].isRelocatable = FALSE;
    linePointer[lineInd].isExtern = FALSE;
    linePointer[lineInd].is_data = FALSE;
    linePointer[lineInd].is_string = FALSE;

    strcpy(linePointer[lineInd].ext.extArg, ""); /* emptying the label buff */
    linePointer[lineInd].ext.extAddress = 0;
    linePointer[lineInd].ext.addressInd = 0;

    strcpy(linePointer[lineInd].dataImage.argumentsArray, ""); /* emptying the label buff */
    linePointer[lineInd].dataImage.biteSize = 0;

    strcpy(linePointer[lineInd].stringDataImage.stringArray, ""); /* emptying the label buff */
    linePointer[lineInd].stringDataImage.biteSize = 0;
    linePointer[lineInd].stringDataImage.dc = 0;
}   /* end of line struct initialize  function  */

/* Initialize the new label array when a new cell is created */
void initialize_label_structures(label *labelPointer, int labelInd) {
    strcpy(labelPointer[labelInd].labelBuff, ""); /* emptying the label buff */
    labelPointer[labelInd].ic = 0;
    labelPointer[labelInd].dc = 0;
    labelPointer[labelInd].isCode = FALSE;
    labelPointer[labelInd].isData = FALSE;
    labelPointer[labelInd].isExt = FALSE;
    labelPointer[labelInd].isEnt = FALSE;
    labelPointer[labelInd].ignoreLabel = FALSE;
} /* end of label struct initialize  function  */
