#include "secondPassHelper.h"

/* Search for errors across the current file, if there are, return 1. else, returns 0. */
int errorsScanner(line *linePointer, int currlLineInd, int ignoredLines) {
    int ind = 0;
    /* looping through all cells that was created for lines in the lines array */
    while (ind <= currlLineInd) {
        if (linePointer[ind]._isError == TRUE) {
            return 1;
        }
        ind++;
    }
    return 0;
} /* end of errorScanner */

/* an int  function that checks if a label argument is defined as a valid label if it is, it returns 1, else it returns 0 */
int is_definedLabel(label *labelPointer, int curLabelInd, const char *labelArgBuffer) {
    return (strcmp(labelPointer[curLabelInd].labelBuff, labelArgBuffer) == 0 &&
            labelPointer[curLabelInd].isExt != TRUE);
} /* end of is_definedLabel */

/* this function updates all .data labels address value by adding the amount of bites the commands holds (ICF) */
void updateLabelDC(label *labelPointer, int labelInd, int *ICF) {
    int ind;
    for (ind = 0; ind <= labelInd; ind++) {
        if (labelPointer[ind].isData == TRUE) {
            labelPointer[ind].dc += *ICF;
        }
    }
}/* end of updateLabelDC */

/* this function raise an entry flag if the .entry argument equal to a valid label */
void handleEntry(line *linePointer, int curLineInd, label *labelPointer, int labelInd, int ignoreLinesNum) {
    int currLabelInd, definedFlag;
    definedFlag = 0;
    for (currLabelInd = 0; currLabelInd <= labelInd; currLabelInd++) {/* scanning the label's dynamic array */
        /* this function check if the .entry argument is a valid label */
        if (is_definedLabel(labelPointer, currLabelInd, linePointer[curLineInd].entryArgument)) {
            labelPointer[currLabelInd].isEnt = TRUE;/* if it is, raise the entry flag in order it will be identified in the printing time */
            definedFlag = 1;
            free(linePointer[curLineInd].entryArgument);/* free the entry arg pointer */

        }
    }

    /* if there wasn't any defined label that equals the one that was given as an argument raise a relevant error flag */
    if (definedFlag == 0) {
        raiseLabelArgError(linePointer, curLineInd);
        errorSpecifier(linePointer, curLineInd, ignoreLinesNum + curLineInd + 1);/* raise and print an error message */
        free(linePointer[curLineInd].entryArgument);/* free the entry arg pointer */
    }
}/* end of handle entry */


/* this function search for .entry lines and takes care of them as part of the  second pass */
void scanForEntrys(line *linePointer, int lineInd, label *labelPointer, int labelInd, int ignoredLinesNum) {
    int currLineInd;
    for (currLineInd = 0; currLineInd <= lineInd; currLineInd++) {
        if (linePointer[currLineInd].isEntry == TRUE) {
            /* calling handle entry in order to check the entry argument */
            handleEntry(linePointer, currLineInd, labelPointer, labelInd, ignoredLinesNum);
        }
    }
}/* end of scan for entry's */

/* function that are made to handle lines that has a missed Label argument from the first pass */
/* this void function check if the label arg that was not found in the first pass is a valid argument and code it.else, raise an error */
void codeMissedLabel(line *linePointer, int currLineInd, label *labelPointer, int lineInd, int labelInd,int ignoredLinesNum) {
    int currLabelInd, missedFlag;/* defining variable's that will track the looping through the label array and help us tell if an identical label was found */
    missedFlag = 0;/* creating a flag that will help us notice if the label argument  have been defined already and raise an error in case its needed */
    /*looping through the label array to first find out if the missed label arg is a data or a code type valid label */
    for (currLabelInd = 0; currLabelInd <= labelInd; currLabelInd++) {
        if (!(strcmp(linePointer[currLineInd].missedLabel, labelPointer[currLabelInd].labelBuff))) {
            if (labelPointer[currLabelInd].isCode != FALSE) {
                linePointer[currLineInd].address = labelPointer[currLabelInd].ic;
                missedFlag = 1;
            } else if (labelPointer[currLabelInd].isData != FALSE) {
                missedFlag = 1;
                linePointer[currLineInd].address = labelPointer[currLabelInd].dc;
            }
        }
    }
    if (missedFlag == 0) {/* if the missed label wasn't found at the label array search in the extern array */
        /* calling a function that checks if a label is an out source extern argument */
        codeExternMissed(linePointer, currLineInd, lineInd, &missedFlag);
    }

    if (missedFlag == 0) {/* if the label that was given as an argument is not defined */
        /* it means the un-founded label arg from the first pass does not exist and this situation requires a rising of an error flag */
        raiseLabelArgError(linePointer, currLineInd);
        /* calling the error Specifier in order to get the specific input that is needed for the problem */
        errorSpecifier(linePointer, currLineInd, currLineInd + ignoredLinesNum + 1);
    }
    free(linePointer[currLineInd].missedLabel);
}/* end of  J type label */


void codeExternMissed(line *linePointer,int currLineInd,int lineInd,int *missedFlag){/* a function that checks if the missed label is an outsource extern argument */
    int lineLoopInd; /* a variable that will help track the looping trough the line index and search for an extern label args that will match the missed label */
    for (lineLoopInd = 0; lineLoopInd <= lineInd; lineLoopInd++){/* search true the saved valid extern label args that are saved inside the line array */
        if (linePointer[lineLoopInd].isExtern != FALSE) {
            if (strcmp(linePointer[lineLoopInd].ext.extArg, linePointer[currLineInd].missedLabel) == 0) {
                if(linePointer[lineLoopInd].ext.addressInd==0){/* it means the current external label is in use for the first time */
                    linePointer[lineLoopInd].ext.extAddress = calloc(1,sizeof(int));
                    linePointer[lineLoopInd].ext.extAddress[0] = linePointer[currLineInd].IC;
                    linePointer[lineLoopInd].ext.addressInd++;
                    *missedFlag =1;/*updating the missed flag value with a pointer in order to chenge the value of the the varible that his value was sent*/
                } else {/* it means there is already one line that uses the outer label arg and the address dynamic array is have been defined */
                    linePointer[lineLoopInd].ext.extAddress = realloc(linePointer[lineLoopInd].ext.extAddress,++linePointer[lineLoopInd].ext.addressInd);
                    if (!(linePointer[lineLoopInd].ext.extAddress)){
                        raiseAllocationError(linePointer,lineInd);/* raise an error flag in case of allocation error in order to prevent segmentation fault */
                        free(linePointer[lineLoopInd].ext.extArg);
                        free(linePointer[lineLoopInd].ext.extAddress);
                    } else {
                        linePointer[lineLoopInd].ext.extAddress[linePointer[lineLoopInd].ext.addressInd -1] = linePointer[currLineInd].IC;
                        *missedFlag =1;/*updating the missed flag value with a pointer in order to chenge the value of the the varible that his value was sent*/
                    }
                }/* end of else */
            }/* and of strcmp if */
        }/* end of if */
    }/*end of for loop */
}/* end of codeJTypeExternMissed */
/* a function that checks if the missed label is an outsource extern argument */


void searchMissedLabel(line *linePointer, int lineInd, label *labelPointer, int labelInd, int ignoredLinesNum) {
    int currLineInd;/* a variable that will help track the current line we want to send to the functions */

    for (currLineInd = 0; currLineInd <= lineInd; currLineInd++) {
        if (linePointer[currLineInd].isMissedLabel == TRUE) {
            /* call the function that will help handle all j type lines  that has a missed arg label */
            codeMissedLabel(linePointer, currLineInd, labelPointer, lineInd, labelInd, ignoredLinesNum);
        }/* end of if jMissed */
    }/* end of for */
}/* end of searchMissedLabel */

/* this function will contain all second pass function's and execute them */
void secondPass(line *linePointer, int lineInd, label *labelPointer, int labelInd, int ignoredLinesNum) {
    /* handle the .entry instruction's that were ignored in the first pass */
    scanForEntrys(linePointer, lineInd, labelPointer, labelInd, ignoredLinesNum);
    /* search for the missed labels who were given as arguments that haven't been checked im the first pass */
    searchMissedLabel(linePointer, lineInd, labelPointer, labelInd, ignoredLinesNum);
}
