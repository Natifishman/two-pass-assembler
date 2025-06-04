#include "assemblerHelper.h"

/* Checks if the input file from argv has a valid .as ending */
int validInputFile(char *inputFileName) {
    char buffer[maxFileEndingLen];
    char *currFileEnding;
    strcpy(buffer, ".as");
    buffer[maxFileEndingLen - 1] = '\0';
    currFileEnding = strchr(inputFileName, pointAscii);
    if (currFileEnding) {
        if (strcmp(currFileEnding, buffer) == 0) {/* comparing the file ending with a valid one */
            return 1;/* return 1 - "true" if the program ending  is valid */
        }
    }
    return 0;/* Return 0 - "false" if the program ending is invalid */
} /* End of valid input function */

/* Checks if the input file from argv has a valid .as ending */
int validMacroFile(char *inputFileName) {
    char buffer[maxFileEndingLen];
    char *currFileEnding;
    strcpy(buffer, ".am");
    buffer[maxFileEndingLen - 1] = '\0';
    currFileEnding = strchr(inputFileName, pointAscii);
    if (currFileEnding) {
        if (strcmp(currFileEnding, buffer) == 0) {/* comparing the file ending with a valid one */
            return 1;/* return 1 - "true" if the program ending  is valid */
        }
    }
    return 0;/* Return 0 - "false" if the program ending is invalid */
} /* End of valid input function */

/* this function creates the ps.ob output file */
char *createDotAmFile( char *inputFileName) {
    /* variable declaration */
    char amEndingbuffer[dotAmEndingLen];
    /* creating the .ob File name buffer that will be based on the input file */
    strcpy(amEndingbuffer, ".am");
    amEndingbuffer[dotAmEndingLen - 1] = '\0';
    inputFileName[strlen(inputFileName) - (dotAmEndingLen - 1)] = '\0';
    inputFileName = strcat(inputFileName, amEndingbuffer);
    return inputFileName;
    }/* end of creating the .am File name buffer */

/* Void function that checks if an error flag is raised, if it is it print's a specific error message */
void errorSpecifier(line *linePointer, int lineInd, int lineNum) {
    if (linePointer[lineInd]._isError == TRUE) { /* declaring error found while the last line was parsed */
        if (linePointer[lineInd].invalid_reserved == TRUE)
            printf("invalid command or instruction in line #%d.\n", lineNum);
        else if (linePointer[lineInd].invalid_label == TRUE)
            printf("invalid label in line #%d.\n", lineNum);
        else if (linePointer[lineInd].invalid_register == TRUE)
            printf("invalid register argument in line #%d.\n", lineNum);
        else if (linePointer[lineInd].invalid_labelArg == TRUE)
            printf("invalid label argument in line #%d.\n", lineNum);
        else if (linePointer[lineInd].invalid_number == TRUE)
            printf("invalid number in line #%d\n", lineNum);
        else if (linePointer[lineInd].invalid_string_stringArg == TRUE)
            printf("invalid .string argument- the argument is not a valid string in line #%d.\n", lineNum);
        else if (linePointer[lineInd].invalid_rang_dotD_arg == TRUE)
            printf("invalid .data instruction argument- the amount of bites the argument takes is out of range in line #%d.\n",lineNum);
        else if (linePointer[lineInd].undefined_label_arg == TRUE)
            printf("The label that was given as argument is not defined in line #%d.\n", lineNum);
        else if (linePointer[lineInd].allocationError == TRUE)
            printf("no available memory for a dynamic data structure reallocation in line #%d.\n", lineNum);
        else if (linePointer[lineInd].missing_Operand == TRUE)
            printf("Missing operand, the command in line #%d received less operands than needed.\n", lineNum);
        else if (linePointer[lineInd].extraneous_Operand == TRUE)
            printf("extraneous operand, the command in line #%d received more operands than needed.\n", lineNum);
    }
}

/* Void type function that execute the assembler passes */
void execute(char *argv[], int argc) {
    /* declaring all the assembler's necessary variables */
    int lineNum, lineInd, labelInd, argcInd, ignoredLinesNum;
    int IC, DC;/* code and data memory trackers */
    int numOfPrintedDataBites;/* an int variable that will help to keep the form of the .ob output file */
    int label_initialize_flag;

    /* declaring the file pointer, and both of the pointer's that will point the dynamic array that will contain  all line's and label's information */
    line *linePointer;
    label *labelPointer;
    FILE *currentFilePointer;
    FILE *macroFilePointer;
    char buffer[maxLineLen];/* a buffer that will contain each line of the entered input file by order */
    char* macroFileName;

    /* Loop that goes through all the arguments starting from argc=1 in order to get only the input files */
    for (argcInd = 1; argcInd < argc; argcInd++) {
        numOfPrintedDataBites = 0;
        IC = 100;
        DC = 0;
        lineNum = 1, lineInd = 0, ignoredLinesNum = 0;
        labelInd = 0;

        /* If the file is not valid continue to the next one */
        if (!(validInputFile(argv[argcInd]))) {
            printf("Unreadable file, check the entered file format.\n");
            continue;
        }

        /* Open current file for reading */
        currentFilePointer = fopen(argv[argcInd], "r");
        /* create macro file for writing */
	macroFileName = (char*) createDotAmFile(argv[argcInd]);
        macroFilePointer = fopen(macroFileName, "w");


 	/* If the file is not valid continue to the next one */
        if (!(validInputFile(argv[argcInd]))) {
            printf("Unreadable file, check the entered file format.\n");
            break;
        }

        /* Call pre assembler, argv[argcInd] - Name of file */
        preAssembler(currentFilePointer, macroFilePointer);
        /* Open macro file for reading and set as current */
	currentFilePointer = fopen(macroFileName, "r");

        /* While the current input file is not fully read */
        while (fgets(buffer, maxLineLen, currentFilePointer) != 0) {
            /* This flag will help us to know if there is a need in initializing the label's dynamic array new cell */
            label_initialize_flag = 0;
            /* Means no space was located for the dynamic array */
            if (lineNum == 1) {
                linePointer = (line *) calloc(1, sizeof(line));
                labelPointer = (label *) calloc(1, sizeof(label));
            } else { /* linePointer is already pointing a dynamic memory, reallocate the linePointer in order to handle the current new line */
                linePointer = (line *) realloc(linePointer, lineNum * sizeof(line));
            }

            /* if it is not the first line */
            if (lineNum > 1) {
                /* if a valid label was found, there is a need to extend the label dynamic array in order to handle the next valid label */
                if (linePointer[lineInd - 1].validLabel == TRUE) {
                    label_initialize_flag = 1; /* turn on the label initialize flag */
                    labelInd++;                /* increase index */
                    /* extend the label array based on the num of label we got so far */
                    labelPointer = (label *) realloc(labelPointer, (labelInd + 1) * sizeof(label));
                }
            }

            /* Initializing all of line's structure structs and flags */
            initialize_line_structures(linePointer, lineInd);
            /* if a new label cell was created in the label's dynamic array */
            if (label_initialize_flag) {
                initialize_label_structures(labelPointer, labelInd);  /* initializing all of label's structure  flags */
            }

            /* this function execute the first pass on the current line */
            firstPass(linePointer, labelPointer, buffer, lineInd, labelInd, &IC, &DC);

            /* calling error specifier in order to get an output in case of an error in the current line */
            errorSpecifier(linePointer, lineInd, lineNum + ignoredLinesNum);

            /* increasing lineNum and line index variables in order to handle the new line */
            lineNum++;
            lineInd++;
        }/* end of while */

        /* calling error scanner in order to see if there is a point of getting to the second pass */
        if (!(errorsScanner(linePointer, lineInd, ignoredLinesNum))) {
            /* it means no errors were found during the first pass */
            updateLabelDC(labelPointer, labelInd, &IC);      /* Update all data type labels dc by adding IC to it */
            secondPass(linePointer, lineInd, labelPointer, labelInd, ignoredLinesNum); /* Execute the second pass */
        }

        /* calling error scanner, if an error was found during the first or second pass, there is no need in creating the output file */
        if (!(errorsScanner(linePointer, lineInd, ignoredLinesNum))) {
            createOutput(linePointer, lineInd, labelPointer, labelInd, IC, DC, &numOfPrintedDataBites, argv[argcInd],
                         strlen(argv[argcInd]));
        }
        free(linePointer);  /* Free linePointer, all internal structs sent to free during the first and second pass */
        free(labelPointer); /* Free labelPointer */
    }/* End of for */
}/* End of execution */
