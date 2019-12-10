//
// Created by misa on 12/6/19.
//

#ifndef MYSHELL_SETUP_H
#define MYSHELL_SETUP_H

#endif //MYSHELL_SETUP_H

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <dirent.h>
#include "findCommand.h"
#include "execCommand.h"

#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */

//#define debug 1 //enable for debugging

char *setup(char inputBuffer[], char *args[], int *background, int *mode,int *doubleMode) {
    int length, /* # of characters in the command line */
            i,      /* loop index for accessing inputBuffer array */
            start,  /* index where beginning of next command parameter is */
            ct;     /* index of where to place the next parameter into args[] */

    ct = 0;

    /* read what the user enters on the command line */
    length = read(STDIN_FILENO, inputBuffer, MAX_LINE);
//    fflush(stdout);
    /* 0 is the system predefined file descriptor for stdin (standard input),
       which is the user's screen in this case. inputBuffer by itself is the
       same as &inputBuffer[0], i.e. the starting address of where to store
       the command that is read, and length holds the number of characters
       read in. inputBuffer is not a null terminated C-string. */

    start = -1;
    if (length == 0)
        exit(0);            /* ^d was entered, end of user command stream */

/* the signal interrupted the read system call */
/* if the process is in the read() system call, read returns -1
  However, if this occurs, errno is set to EINTR. We can check this  value
  and disregard the -1 value */
    if ((length < 0) && (errno != EINTR)) {
        perror("error reading the command");
        exit(-1);           /* terminate with error code of -1 */
    }
#ifdef debug
    printf(">>%s<<", inputBuffer);
#endif
    for (i = 0; i < length; i++) { /* examine every character in the inputBuffer */

        switch (inputBuffer[i]) {
            case ' ':
            case '\t' :               /* argument separators */
                if (start != -1) {
                    args[ct] = &inputBuffer[start];    /* set up pointer */
                    ct++;
                }
                inputBuffer[i] = '\0'; /* add a null char; make a C string */
                start = -1;
                break;

            case '\n':                 /* should be the final char examined */
                if (start != -1) {
                    args[ct] = &inputBuffer[start];
                    ct++;
                }
                inputBuffer[i] = '\0';
                args[ct] = NULL; /* no more arguments to this command */
                break;

            default :             /* some other character */
                if (start == -1)
                    start = i;
                if (inputBuffer[i] == '&') {
                    *background = 1;
                    inputBuffer[i - 1] = '\0';
                }
        } /* end of switch */
    }    /* end of for */
    args[ct] = NULL; /* just in case the input line was > 80 */
#ifdef debug
    for (i = 0; i < ct; i++) {
        printf("args %d = %s\n", i, args[i]);
    }
#endif
    char *arguments[ct];
    int index = 0;
    char *filename = NULL;
    //  finding current redirecton mode
    while (args[index] != NULL) {
        if (strcmp(args[index], "<") == 0 || strcmp(args[index], "0>") == 0) { // standard input
            *mode = 0;
            if (args[index + 1] != NULL) {
                filename = strdup(args[index + 1]);
            } else {
                fprintf(stderr, "Enter file direction.");
            }
        } else if (strcmp(args[index], ">") == 0 || strcmp(args[index], "1>") == 0) { //standard output
            *mode = 1;
            if (args[index + 1] != NULL) {
                filename = strdup(args[index + 1]);
            } else {
                fprintf(stderr, "Enter file direction.");
            }
        } else if (strcmp(args[index], "2>") == 0) { // standard error
            *mode = 2;
            if (args[index + 1] != NULL) {
                filename = strdup(args[index + 1]);
            } else {
                fprintf(stderr, "Enter file direction.");
            }
        } else if (strcmp(args[index], ">>") == 0) { // append mode
            *mode = 3;
            if (args[index + 1] != NULL) {
                filename = strdup(args[index + 1]);
            } else {
                fprintf(stderr, "Enter file direction.");
            }
        }
        index++;
    }

    int k = 0;
    while(args[k]!=NULL){
        if (strcmp(args[k], ";") == 0){
            *doubleMode=1;
        }
        k++;
    }
    return filename; // return redirection filename
} /* end of setup routine */