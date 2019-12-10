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

ListNodePtr historyPtr; // history head

int waitChild(pid_t childPid, int background) {
    // wait child processes function
    if (background == 0) {
        if (waitpid(childPid, NULL, 0) > 0) { // bekleme durumu
//            puts("child waited succesfully");
        }
    } else {
        if (waitpid(childPid, NULL, WNOHANG) > 0) {
//            puts("backgound is finished");
        }
    }
}

int *execCommand(char path[], char *arguments[], int background) {
    // execute command with found path and arguments
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
    waitChild(childpid, background);
}


void exitProgram() {
    pid_t child_pid = waitpid(-1, NULL, WNOHANG);

    // If there is any child process, wait for it until it is terminated
    if (child_pid == 0) {
        printf("Before exit, you must close background processes!\n");
        return;
        // If there is no child process, finish
    } else {
        printf("exit success");
        exit(EXIT_SUCCESS);

    }
}

void run(char *args[], char *command, int background) {
    // union of the previous functions
#ifdef debug
    printf("%s\n", command);
#endif
    int j = 1;
    char *commandArguments[25];
    commandArguments[0] = command;
    char tempCommand[80] = "";
    while (args[j] != NULL) {
        // clear the redirection mark from args
//        printf("%s\n", args[j]);
        if (strcmp(args[j], ">") == 0 || strcmp(args[j], "1>") == 0 || strcmp(args[j], ">>") == 0 ||
            strcmp(args[j], "<") == 0 || strcmp(args[j], "0>") == 0 || strcmp(args[j], "2>") == 0) {
            break;
        }
        commandArguments[j] = args[j];
        j++;
    }
    commandArguments[j] = NULL;
    int index = 0;
    while (args[index] != NULL) { // history e insert etmek için
        strcat(tempCommand, args[index]);
        strcat(tempCommand, " ");
        index++;
    }
    insert(&historyPtr, tempCommand, command); // insert history
    execCommand(command, commandArguments, background);
}

static void childSignalHandler() { // handling with sigchild signal and wait them
//    printf("\nchild is dead\n");
    pid_t deadpid = waitpid(-1, NULL, WNOHANG);
    printf("dead pid is %d\n", deadpid);
    fflush(stdout);
}

static void ctrlzSignalHandler() {
    pid_t childpid = waitpid(-1, NULL, WNOHANG);
    printf("%d", childpid);
    fflush(stdout);
}

int runFromHistory(ListNodePtr head, int index) {
    // run from history with -i parameter
    ListNodePtr current = head;
    int currentindex = 0;
    char *argv[25];
    while (current != NULL) {
        if (currentindex == index) { // found specified index
            argv[0] = strdup(current->commandPath);
            int i = 1;
            char *str = strdup(current->data);
            char *ptr = strtok(str, " ");
            ptr = strtok(NULL, " ");
            int index = 0;
            while (ptr != NULL) {
                argv[i] = strdup(ptr);
                ptr = strtok(NULL, " ");
                i++;
            }
            argv[i] = NULL;
            execCommand(current->commandPath, argv, 0);
        }
        current = current->nextPtr;
        currentindex++;
    }
}




