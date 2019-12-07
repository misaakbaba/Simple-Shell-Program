#include <stdio.h>
#include <stdlib.h>
#include "setup.h"

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

    while (1) {
        background = 0;
        printf("myshell: ");
        fflush(stdout);
        /*setup() calls exit() when Control-D is entered */
        setup(inputBuffer, args, &background);
        /** the steps are:
        (1) fork a child process using fork()
        (2) the child process will invoke execv()
        (3) if background == 0, the parent will wait,
        otherwise it will invoke the setup() function again. */
    }
    return 0;
}