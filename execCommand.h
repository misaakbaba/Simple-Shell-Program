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
#include <sys/types.h>

ListNodePtr historyPtr;
childpidPtr fg_processes = NULL;
childpidPtr bg_processes = NULL;

int waitChild(pid_t childPid, int background) {
    if (background == 0) {
        if (waitpid(childPid, NULL, 0) > 0) {
            puts("child waited succesfully");
        }
    } else {
        if (waitpid(childPid, NULL, WNOHANG) > 0) {
            puts("backgound is finished");
        }
    }
}

int *execCommand(char path[], char *arguments[], int background, char *command) {
    pid_t childpid;
    childpid = fork();
    if (background == 0) {
        pushPid(&fg_processes, childpid, command);
    } else {
        pushPid(&bg_processes, childpid, command);
    }
    if (childpid == -1) {
        perror("Failed to fork");
        return 1;
    }
    if (childpid == 0) {
        execv(path, arguments);
        perror("Child failed to execv the command");
        return 1;
    }
    waitChild(childpid, background);
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
    execCommand(command, commandArguments, background, tempCommand);
}

int waitBgChilds(childpidPtr head, int background) {
    // TODO Beklenenler indexe göre yapılacak
    childpidPtr current = head;
    int bgCount = 0;
    while (current != NULL) {
        waitChild(current->childpid, background);
        current = current->nextPtr;
        bgCount++;
    }
}



