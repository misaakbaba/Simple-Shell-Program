#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <dirent.h>

#define MAX_LINE 80 /* 80 chars per line, per command, should be enough. */

#define SIZE 20

/* STRUCT DEFINITON */
struct listNode {
    char *data; // each listNode contains a character
    struct listNode *nextPtr; // pointer to next node
};

typedef struct listNode ListNode; // synonym for struct listNode
typedef ListNode *ListNodePtr; // synonym for ListNode*

/* FUNCTİON PROTOTYPES START HERE */

char *findPathVariables(char *envp[]);

char *findCommandPath(char path[], char command[]);

char *execCommand(char path[], char command[]);

char *splitString(char str[], char delimeter[]);

void insert(ListNodePtr *sPtr, char *value);

int isEmpty(ListNodePtr sPtr);

void printList(ListNodePtr currentPtr);

int readDir(char dirName[], char command[]);

char *findPath(char command[]);

ListNodePtr pathPtr;

/* The setup function below will not return any value, but it will just: read
in the next command line; separate it into distinct arguments (using blanks as
delimiters), and set the args array entries to point to the beginning of what
will become null-terminated, C-style strings. */

void setup(char inputBuffer[], char *args[], int *background) {
    int length, /* # of characters in the command line */
            i,      /* loop index for accessing inputBuffer array */
            start,  /* index where beginning of next command parameter is */
            ct;     /* index of where to place the next parameter into args[] */

    ct = 0;

    /* read what the user enters on the command line */
    length = read(STDIN_FILENO, inputBuffer, MAX_LINE);

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

    printf(">>%s<<", inputBuffer);
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

    for (i = 0; i <= ct; i++)
        printf("args %d = %s\n", i, args[i]);
} /* end of setup routine */

/*
 * hocanın yazdığı yerler mainde yorumda olan kısımlar
 * program ilk çalıştığında bir kere enter a basmak gerekiyor ondan sonra command okumaya başlıyor.
 * hoca ilk gönderdiğinde de böyleydi sebebini bilmiyorum
 */
int main(int argc, char *argv[]) {
//    char inputBuffer[MAX_LINE]; /*buffer to hold command entered */
//    int background; /* equals 1 if a command is followed by '&' */
//    char *args[MAX_LINE / 2 + 1]; /*command line arguments */

    char sstr[] = "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games";
    char delim[] = ":";
    char *ptr = getenv("PATH"); //path variable ini çeken asıl fonksiyon.
    splitString(ptr, delim);
    findPath("lol");

//    while (1) {
//        background = 0;
//        printf("myshell: ");
//        /*setup() calls exit() when Control-D is entered */
//        setup(inputBuffer, args, &background);
////        findCommand("lol");
//        /** the steps are:
//        (1) fork a child process using fork()
//        (2) the child process will invoke execv()
//        (3) if background == 0, the parent will wait,
//        otherwise it will invoke the setup() function again. */
//    }
    return 0;
}

/*  PROCCESS FUNCTİONS START HERE */
//taslak aşamasında. pipe yçnlendirmesini deniyordum.
char *findCommandPath(char path[], char command[]) {
    pid_t childpid;
    char *findPath = "/usr/bin/find";
    char *arguments[] = {findPath, path, "-name", command, NULL};
    childpid = fork();
    if (childpid == -1) {
        perror("Failed to fork");
        return 1;
    }
    if (childpid == 0) {
        int fd[2];
        if (pipe(fd) == -1) {
            perror("Failed to setup pipeline");
            return 1;
        }
        if (dup2(fd[1], STDOUT_FILENO) == -1) {
            perror("Failed to redirect stdout of ls");
        }
        execv(findPath, arguments);
        char inbuf[20];
//        printf("heloooo");
        read(fd[0], inbuf, 20);
        printf("%s\n", inbuf);
//        perror("Child failed to execv the command");
//        return 1;


    }
    if (childpid != wait(NULL)) {                          /* parent code */
        perror("Parent failed to wait");
        return 1;
    }
}

// taslak halinde. biraz değiştirilip güncel command çalıştırma fonksiyonu olucak.
char *execCommand(char path[], char command[]) {
    pid_t childpid;
    char *findPath = "/usr/bin/find";
    char *arguments[] = {findPath, path, "-name", command, NULL};/* child code */
    childpid = fork();
    if (childpid == -1) {
        perror("Failed to fork");
        return 1;
    }
    if (childpid == 0) {
        execv(findPath, arguments);
        perror("Child failed to execv the command");
        return 1;
    }
    if (childpid != wait(NULL)) {                          /* parent code */
        perror("Parent failed to wait");
        return 1;
    }
}

/* UTILITY FUNCTIONS START HERE */
//verilen string i istenen delimeter ile parçalayıp ll e ekliyor.
char *splitString(char str[], char delimeter[]) {
    char temp[250];
    strcpy(temp, str);
//    char delim[] = ":";
    char *ptr = strtok(temp, delimeter);
    while (ptr != NULL) {
        insert(&pathPtr, ptr);
        ptr = strtok(NULL, delimeter);
    }
}

/* DATA STRUCTURE FUNCTIONS */
// insert ederken oluşturulan ListNodePtr nin adresi yollanıyor parametre olarak!!
void insert(ListNodePtr *sPtr, char *value) {
    ListNodePtr newPtr = malloc(sizeof(ListNode)); // create node

    if (newPtr != NULL) { // is space available
        newPtr->data = malloc(sizeof(200));
        strcpy(newPtr->data, value);
        newPtr->nextPtr = NULL; // node does not link to another node

        ListNodePtr previousPtr = NULL;
        ListNodePtr currentPtr = *sPtr;

        // loop to find the correct location in the list
        while (currentPtr != NULL && value > currentPtr->data) {
            previousPtr = currentPtr; // walk to ...
            currentPtr = currentPtr->nextPtr; // ... next node
        }

        // insert new node at beginning of list
        if (previousPtr == NULL) {
            newPtr->nextPtr = *sPtr;
            *sPtr = newPtr;
        } else { // insert new node between previousPtr and currentPtr
            previousPtr->nextPtr = newPtr;
            newPtr->nextPtr = currentPtr;
        }
    } else {
        printf("%c not inserted. No memory available.\n", value);
    }
}

int isEmpty(ListNodePtr sPtr) {
    return sPtr == NULL;
}

void printList(ListNodePtr currentPtr) {
    // if list is empty
    if (isEmpty(currentPtr)) {
        puts("List is empty.\n");
    } else {
        puts("The list is:");

        // while not the end of the list
        while (currentPtr != NULL) {
            printf("%s --> ", currentPtr->data);
            currentPtr = currentPtr->nextPtr;
        }
        puts("NULL\n");
    }
}

//verilen klasörde istenen command ı aramak için fonksiyon. command ı bulunca break atıp true dönecek.
//taslak aşamasında
int readDir(char dirName[], char command[]) {//function that read file name from folder
    DIR *FD;
    struct dirent *in_file;
    char folder[SIZE];
    strcpy(folder, dirName);
    if (NULL == (FD = opendir(folder))) { // folder can not opened
        fprintf(stderr, "Error : Failed to open input directory\n");
        return 1;
    }
    while ((in_file = readdir(FD)) != NULL) { // to ignore parent directtories
        if (!strcmp(in_file->d_name, ".directory")) {
            continue;
        }
        if (!strcmp(in_file->d_name, ".")) {
            continue;
        }
        if (!strcmp(in_file->d_name, "..")) {
            continue;
        }

        printf("%s\n", in_file->d_name);
    }
}

//bu fonksiyon linked listi gezip sürekli readDir fonksiyonunu çağıracak. readDir den eğer true dönerse o anda denenen path i return edecek
//taslak aşamasında
char *findPath(char command[]) {
    // if list is empty
    if (isEmpty(pathPtr)) {
        puts("List is empty.\n");
    } else {
        puts("The list is:");

        // while not the end of the list
        while (pathPtr != NULL) {
            printf("%s --> ", pathPtr->data);
            pathPtr = pathPtr->nextPtr;
        }
        puts("NULL\n");
    }
}
