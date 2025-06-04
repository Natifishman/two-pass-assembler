#include "preAssembler.h"

char getExistMcrName(char line) {
    char mcr_name[77]; // maximum MCR name length is 76, plus one for the null terminator.
    int i, j;

    // skip over any initial spaces or tabs
    i = 0;

    while (i < strlen(line) && (line[i] == ' ' || line[i] == '\t')) {
        i++;
    }

    // look for the MCR name
    j = 0;
    while (j < 76 && i < strlen(line) && line[i] != ' ' && line[i] != '\t') {
        mcr_name[j] = line[i];
        i++;
        j++;
    }
    mcr_name[j] = '\0';
    return mcr_name;
}

myBool isExistMcr(MacroNode *macroPointer, char mcr_name) {
    if (isMacroName(macroPointer, mcr_name)) {
        return true;
    } else {
        return false;
    }
}

myBool isMcr(char line) {
    int i;

    // skip over any initial spaces or tabs
    i = 0;
    while (i < strlen(line) && (line[i] == ' ' || line[i] == '\t')) {
        i++;
    }

    // search for the "mcr" string
    while (i < strlen(line) - 2) {
        if (line[i] == 'm' && line[i + 1] == 'c' && line[i + 2] == 'r' && (line[i + 3] == ' ' || line[i + 3] == '\t')) {
            return true;
        }
        i++;
    }
    return false;
}

char getMcrName(char line) {
    char mcr_name[77]; // maximum MCR name length is 76, plus one for the null terminator.
    int found = 0; // flag to track if we've found the MCR string
    int i, j, max_index;

    // skip over any initial spaces or tabs
    i = 0;
    while (i < strlen(line) && (line[i] == ' ' || line[i] == '\t')) {
        i++;
    }

    // search for the "mcr" string
    max_index = strlen(line) - 3;
    for (int i = 0; i <= max_index; i++) {
        if (strncmp(&line[i], "mcr", 3) == 0 && isspace(line[i + 3])) {
            found = 1;
            i += 3; // skip over the "mcr" string
            break;
        }
    }

    // if we found the "mcr" string, skip over any additional spaces or tabs
    if (found) {
        while (i < strlen(line) && (line[i] == ' ' || line[i] == '\t')) {
            i++;
        }

        // look for the MCR name
        j = 0;
        while (j < 76 && i < strlen(line) && line[i] != ' ' && line[i] != '\t') {
            mcr_name[j] = line[i];
            i++;
            j++;
        }
        mcr_name[j] = '\0';
    }

    // print out the MCR name if we found it
    if (found) {
        return mcr_name;
    } else {
        printf("Error: Did not find MCR.\n");
        return; /* Get out */
    }
}

/* This function checks if the given line contains only the string "endmcr" */
myBool isEndMcr(char *line) {
    /* Remove the trailing newline character */
    char *newline = strchr(line, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }

    /* Remove leading and trailing spaces and tabs */
    const char *start = line;
    const char *end = line + strlen(line) - 1;
    while (*start == ' ' || *start == '\t') {
        start++;
    }
    while (end > start && (*end == ' ' || *end == '\t')) {
        end--;
    }
    char *trimmed = strndup(start, end - start + 1);

    /* Check if the line only contains "endmcr" */
    myBool result = strcmp(trimmed, "endmcr") == 0;
    free(trimmed);
    return result;
}

void preAssembler(FILE *srcFile, FILE *mcrFile) {
    char *macro_name;
    char line[LINE_LENGTH];
    char flag = true;
    MacroNode *macroPointer = NULL;

    /* Check all file's lines */
    while (fgets(line, LINE_LENGTH, srcFile) != 0) {
        if (isComment(buffer) || isBlank(buffer)) { /* if line is empty or comment(;) */
            continue; /* Continue to the next line in the input file */
        }

        if (flag) { /* if flag == true */
            macro_name = getExistMcrName(line);
            if (isExistMcr(macroPointer, macro_name)) {
                printCommands(macroPointer, mcr_name, mcrFile); /* need to add */
                continue; /* Continue to the next line in the input file */
            }

            /* set flag if it is a macro */
            if (isMcr(line)) {
                macro_name = getMcrName(line);
                addMacroNode(macroPointer, macro_name);
                flag = false;
                continue; /* Continue to the next line in the input file */
            }
            fprintf(mcrFile, "%s", line); /* check it */
        } else { /* if flag = false */
            if (!(isEndMcr(line))) {
                addCommandToMacro(macroPointer, macro_name, line);
            } else {
                flag = true;
            }
        }
    }
    /* close source and macro file. */
    fclose(srcFile);
    fclose(mcrFile);
}
