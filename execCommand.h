//
// Created by misa on 12/6/19.
//

#ifndef MYSHELL_EXECCOMMAND_H
#define MYSHELL_EXECCOMMAND_H

#endif //MYSHELL_EXECCOMMAND_H

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <dirent.h>

ListNodePtr historyPtr;

int *execCommand(char path[], char *arguments[], int background) {
    pid_t childpid;
    childpid = fork();
    if (childpid == -1) {
        perror("Failed to fork");
        return 1;
    }
    if (childpid == 0) {
        execv(path, arguments);
        perror("Child failed to execv the command");
        return 1;
    }
    if (background == 0) {
        if (childpid != wait(NULL)) {                          /* parent code */
            perror("Parent failed to wait");
            return 1;
        }
    }
}

void exitProgram() {
    // To hold child process ID
    pid_t child_pid = waitpid(-1, NULL, WNOHANG);

    // If there is any child process, wait for it until it is terminated
    if (child_pid == 0) {
        printf("Before exit, you must close background processes!\n");
        return;
        // If there is no child process, finish the program successfully
    } else {
        printf("See you later |(o_o)|\n");
        exit(EXIT_SUCCESS);

    }
}

void run(char *args[], char *command, int background, char inputBuffer[]) {
#ifdef debug
    printf("%s\n", command);
#endif
    int j = 1;
    char *commandArguments[25];
    commandArguments[0] = command;
    char tempCommand[80] = "";

    while (args[j] != NULL) {
        printf("%s\n", args[j]);
        commandArguments[j] = args[j];
        j++;
    }
    commandArguments[j] = NULL;
    int index = 0;
    while (args[index] != NULL) {
        strcat(tempCommand, args[index]);
        strcat(tempCommand, " ");
        index++;
    }
    insert(&historyPtr, tempCommand);
    execCommand(command, commandArguments, background);
}




