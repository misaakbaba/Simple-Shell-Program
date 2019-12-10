#include <stdio.h>
#include <stdlib.h>
#include "setup.h"
#include "redirection.h"
#include <signal.h>

#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */


/*
 * hocanın yazdığı yerler mainde yorumda olan kısımlar
 * program ilk çalıştığında bir kere enter a basmak gerekiyor ondan sonra command okumaya başlıyor.
 * hoca ilk gönderdiğinde de böyleydi sebebini bilmiyorum
 */
int main(int argc, char *argv[]) {
    char inputBuffer[MAX_LINE]; /*buffer to hold command entered */
    int background; /* equals 1 if a command is followed by '&' */
    char *args[MAX_LINE / 2 + 1]; /*command line arguments */
    int redirectionMode;
    // -1 for default
    // 0 for < or 0> = input
    // 1 for > or 1> = out
    // 2 for 2> = error
    // 3 for >> =out append

    while (1) {
        redirectionMode = -1;

        background = 0;
        printf("myshell: ");
        fflush(stdout);
        /*setup() calls exit() when Control-D is entered */
        char *filename = setup(inputBuffer, args, &background, &redirectionMode);
        if (args[0] == NULL) { //argüman yoksa
            continue;
        }
        char *command = getCommand(args[0]);
        if (command != NULL && redirectionMode == -1) {//normal mode
            run(args, command, background, inputBuffer);
        } else if (command != NULL && redirectionMode != -1) { // redirection mode
            int j = 1;
            char *commandArguments[25];
            commandArguments[0] = command;
            char commandforHistory[80] = "";
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
        printf("redirection mode is %d\n", redirectionMode);
        fflush(stdout);
        if (strcmp(args[0], "exit") == 0) {
            printf("exit is worked.\n");
            exitProgram();
        } else if (strcmp(args[0], "history") == 0 && args[1] == NULL) {
            printHistory(historyPtr);
        } else if (strcmp(args[0], "history") == 0 && strcmp(args[1], "-i") == 0 && args[2] != NULL) {
            runFromHistory(historyPtr, 0);
        } else if (strcmp(args[0], "jobs") == 0) {
            printProcesses(bg_processes);
        } else if (strcmp(args[0], "fg") == 0 && args[1] != NULL) {
//            waitBgChilds(bg_processes, 0,);
            printf("%s\n", args[1]);
            waitpid(atoi(args[1]), NULL, 0);
        }
        struct sigaction act;
        int count = 0;
        double sum = 0;
        double x;

        act.sa_handler = childSignalHandler;            /* set up signal handler */
        act.sa_flags = 0;
        if ((sigemptyset(&act.sa_mask) == -1) ||
            (sigaction(SIGCHLD, &act, NULL) == -1)) {
            perror("Failed to set SIGINT handler");
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