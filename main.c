#include <stdio.h>
#include <stdlib.h>
#include "setup.h"
#include "redirection.h"
#include <signal.h>

#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */


int main(int argc, char *argv[]) {
    char inputBuffer[MAX_LINE]; /*buffer to hold command entered */
    int background; /* equals 1 if a command is followed by '&' */
    char *args[MAX_LINE / 2 + 1]; /*command line arguments */
    int redirectionMode;
    int doubleCommandMode;
    // -1 for default
    // 0 for < or 0> = input
    // 1 for > or 1> = out
    // 2 for 2> = error
    // 3 for >> =out append
    char delim[] = ":";
    char *ptr = getenv("PATH"); // get path variable

    splitString(ptr, delim);
    while (1) {
        doubleCommandMode = 0;
        redirectionMode = -1; // default redirection mode is -1

        background = 0;
        printf("myshell: ");
        fflush(stdout);
        /*setup() calls exit() when Control-D is entered */
        char *filename = setup(inputBuffer, args, &background, &redirectionMode, &doubleCommandMode);
        if (args[0] == NULL) { //if there is no argument take another input
            continue;
        }
        if (doubleCommandMode == 1) {
            //double mode
            doubleMode(args);
            continue;
        }
        char *command = getCommand(args[0]); // command finded in path variable and returned its actual path
        if (command != NULL && redirectionMode == -1) {// normal mode
            run(args, command, background); // run command
        } else if (command != NULL && redirectionMode != -1) { // redirection mode
            int j = 1;
            char *commandArguments[25];
            commandArguments[0] = command;
            char commandforHistory[80] = "";
            // clear redirection mark from args parameter and create argv array for run command
            while (args[j] != NULL) {
                if (strcmp(args[j], ">") == 0 || strcmp(args[j], "1>") == 0 || strcmp(args[j], ">>") == 0 ||
                    strcmp(args[j], "<") == 0 || strcmp(args[j], "0>") == 0 || strcmp(args[j], "2>") == 0) {
                    if (args[j + 1] != NULL) {
                        strcat(commandforHistory, args[j]);
                        strcat(commandforHistory, " ");
                        strcat(commandforHistory, args[j + 1]);
                    } else {
                        fprintf(stderr, "invalid redirrection type");
                    }
                    break;
                }
                strcat(commandforHistory, args[j]);
                strcat(commandforHistory, " ");
                commandArguments[j] = args[j];
                j++;
            }
            commandArguments[j] = NULL;
            redirection(commandArguments, command, filename, redirectionMode, commandforHistory);
        }
//        printf("redirection mode is %d\n", redirectionMode);
//        fflush(stdout);
        if (strcmp(args[0], "exit") == 0) { // exit built-in
            printf("exit is worked.\n");
            exitProgram();
        } else if (strcmp(args[0], "history") == 0 && args[1] == NULL) { // history built-in
            printHistory(historyPtr);
        } else if (strcmp(args[0], "history") == 0 && strcmp(args[1], "-i") == 0 && args[2] != NULL) {
            // history -i % build-in
            if (atoi(args[2]) < 10) {
                runFromHistory(historyPtr, atoi(args[2]));
            }
        } else if (strcmp(args[0], "fg") == 0 && args[1] != NULL) { // fg % build-in
            printf("%s\n", args[1]);
            waitpid(atoi(args[1]), NULL, 0);
        } else if (strcmp(args[0], "path") == 0 && args[1] == NULL) {
            //print path
            printPath(pathPtrDup);
        } else if (strcmp(args[0], "path") == 0 && strcmp(args[1], "+") == 0 && args[2] != NULL) {
            // add path
            insert(&pathPtrDup, strdup(args[2]), "");
        } else if (strcmp(args[0], "path") == 0 && strcmp(args[1], "-") == 0 && args[2] != NULL) {
            // delete from path
            pop(&pathPtrDup, strdup(args[2]));
        }
        // handling sigchild signal for understand child is dead or not
        struct sigaction act;

        act.sa_handler = childSignalHandler;
        act.sa_flags = 0;
        if ((sigemptyset(&act.sa_mask) == -1) ||
            (sigaction(SIGCHLD, &act, NULL) == -1)) {
            perror("Failed to set SIGCHLD handler");
            return 1;
        }

        struct sigaction act2;

        act2.sa_handler = ctrlzSignalHandler;
        act2.sa_flags = 0;
        if ((sigemptyset(&act2.sa_mask) == -1) ||
            (sigaction(SIGTSTP, &act2, NULL) == -1)) {
            perror("Failed to set SIGTSTP handler");
            return 1;
        }
        /** the steps are:
        (1) fork a child process using fork()
        (2) the child process will invoke execv()
        (3) if background == 0, the parent will wait,
        otherwise it will invoke the setup() function again. */
    }
    return 0;
}