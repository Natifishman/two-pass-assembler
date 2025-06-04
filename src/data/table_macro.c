#include "table_macro.h"

void addMacroNode(MacroNode **head, char *name) {
    MacroNode *newNode = malloc(sizeof(MacroNode));
    newNode->name = name;
    newNode->commands = NULL;
    newNode->next = NULL;

    if (*head == NULL) {
        /* the list is empty, so set the new node as the head */
        *head = newNode;
    } else {
        /* iterate over the list to find the last node */
        MacroNode *currentNode = *head;
        while (currentNode->next != NULL) {
            currentNode = currentNode->next;
        }

        /* update the last node's next field to point to the new node */
        currentNode->next = newNode;
    }
}

void addCommandToMacro(MacroNode* head, char* name, char* command) {
    MacroNode* current = head;
CommandNode* newCommand;
char *new_string;
    /* search for macroNode with matching name */
    while (current != NULL && strcmp(current->name, name) != 0) {
        current = current->next;
    }

    /* Creates a new Macro if there are no macro like this */
    if (current == NULL) {
        /* if the macroNode does not exist, create a new one */
        current = (MacroNode*)malloc(sizeof(MacroNode));

new_string = malloc(strlen(name) + 1);
if (new_string == NULL) {
    printf("error, Macro table!");
}
strcpy(new_string, name);
current->name = new_string;

        current->commands = NULL;
        current->next = head;
        head = current;
        /* write error */
    }

    /* add the command to the end of the command list */

newCommand = (CommandNode*)malloc(sizeof(CommandNode));
newCommand->command = (char*)malloc(strlen(command) + 1);
strcpy(newCommand->command, command);
    newCommand->next = NULL;

    /* If there are no commands */
    if (current->commands == NULL) {
        current->commands = newCommand;
    } else { /* if there are already commands */
        CommandNode* lastCommand = current->commands;
        while (lastCommand->next != NULL) {
            lastCommand = lastCommand->next;
        }
        lastCommand->next = newCommand;
    }
}

/* Checks if a given name is already exist */
myBool isMacroName(MacroNode *head, char *macro_name) {
    MacroNode *current = head;
    while (current != NULL) {
        if (strcmp(current->name, macro_name) == 0) {
            return TRUE;
        }
        current = current->next;
    }
    return FALSE;
}

void printCommands(MacroNode *head, char *macro_name, FILE *file_ptr) {
    MacroNode *current = head;
    while (current != NULL) {
        if (strcmp(current->name, macro_name) == 0) {
            CommandNode *command = current->commands;
            while (command != NULL) {
                fprintf(file_ptr, "%s\n", command->command);
                command = command->next;
            }
            return;
        }
        current = current->next;
    }
    fprintf(file_ptr, "Macro not found\n");
}
