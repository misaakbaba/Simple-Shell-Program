//
// Created by misa on 12/10/19.
//

#ifndef MYSHELL_REDIRECTION_H
#define MYSHELL_REDIRECTION_H

#endif //MYSHELL_REDIRECTION_H

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

#define CREATE_FLAGS_APPEND (O_WRONLY | O_CREAT | O_APPEND)
#define CREATE_FLAGS_WRITE (O_WRONLY | O_CREAT | O_TRUNC)
#define CREATE_MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)


int redirection(char *argv[], char *command, char *filename, int mode, char *commandHistory) {
    int fd;

    int inDefault = dup(0);
    int outDefault = dup(1);
    int errDefault = dup(2);
    if (mode == 0) { //standart input
        fd = open(filename, CREATE_FLAGS_WRITE, CREATE_MODE);
        if (fd == -1) {
            perror("Failed to open file");
            return 1;
        }
        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("Failed to redirect standard output");
            return 1;
        }
    } else if (mode == 1) { // standart output
        fd = open(filename, CREATE_FLAGS_WRITE, CREATE_MODE);
        if (fd == -1) {
            perror("Failed to open file");
            return 1;
        }
        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("Failed to redirect standard output");
            return 1;
        }
    } else if (mode == 2) { // standart error
        fd = open(filename, CREATE_FLAGS_WRITE, CREATE_MODE);
        if (fd == -1) {
            perror("Failed to open file");
            return 1;
        }
        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("Failed to redirect standard output");
            return 1;
        }
    } else if (mode == 3) { // append
        fd = open(filename, CREATE_FLAGS_APPEND, CREATE_MODE);
        if (fd == -1) {
            perror("Failed to open file");
            return 1;
        }
        if (dup2(fd, STDOUT_FILENO) == -1) {
            perror("Failed to redirect standard output");
            return 1;
        }
    }
    execCommand(command, argv, 0, commandHistory);
    if (close(fd) == -1) {
        perror("Failed to close the file");
        return 1;
    }
    fflush(stdin);
    fflush(stdout);
    fflush(stderr);
    dup2(inDefault, 0);
    dup2(outDefault, 1);
    dup2(errDefault, 2);
    close(inDefault);
    close(outDefault);
    close(errDefault);

    return 0;
}