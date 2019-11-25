//
// Created by misa on 11/24/19.
//
#include "../headers/Utils.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

char *findPathVariables(char *envp[]) {

    char str[] = "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games";
    int init_size = strlen(str);
    char delim[] = ":";

    char *ptr = strtok(str, delim);

    while (ptr != NULL) {
        printf("'%s'\n", ptr);
        ptr = strtok(NULL, delim);
    }

}

char *findCommand(char *command) {
    pid_t childpid;
    char *binaryPath = "/usr/bin/find";
    char *arguments[] = {binaryPath, "/usr/bin", "-name", "find", NULL};/* child code */
    char *path[] = {"$PATH", "$PATH", NULL};
    childpid = fork();
    if (childpid == -1) {
        perror("Failed to fork");
        return 1;
    }
    if (childpid == 0) {

        execv("$PATH", path);
        perror("Child failed to execv the command");
        return 1;
    }
    if (childpid != wait(NULL)) {                          /* parent code */
        perror("Parent failed to wait");
        return 1;
    }

}


