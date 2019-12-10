//
// Created by misa on 12/6/19.
//

#ifndef MYSHELL_DATASTRUCTURE_H
#define MYSHELL_DATASTRUCTURE_H

#endif //MYSHELL_DATASTRUCTURE_H

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/types.h>

#define SIZE 100
struct listNode {
    char *data; // each listNode contains a character
    char *commandPath;
    struct listNode *nextPtr; // pointer to next node
};

typedef struct listNode ListNode; // synonym for struct listNode
typedef ListNode *ListNodePtr; // synonym for ListNode*

struct childpid_list {
    pid_t childpid;
    char *name;
    struct childpid_list *nextPtr;
};

typedef struct childpid_list childpid_List;
typedef childpid_List *childpidPtr;

void insert(ListNodePtr *sPtr, char *value, char *commandPath) {
    ListNodePtr newPtr = malloc(sizeof(ListNode)); // create node

    if (newPtr != NULL) {
        newPtr->data = strdup(value);
        newPtr->commandPath = strdup(commandPath);
        newPtr->nextPtr = *sPtr;
        *sPtr = newPtr;
    } else { // no space available
        printf("value not inserted. No memory available.\n");
    }
}

void pushPid(childpidPtr *sPtr, pid_t childpid, char *command) {
    childpidPtr newPtr = malloc(sizeof(childpid_List)); // create node

    if (newPtr != NULL) {
        newPtr->childpid = childpid;
        newPtr->name = strdup(command);
        newPtr->nextPtr = *sPtr;
        *sPtr = newPtr;
    } else { // no space available
        printf("value not inserted. No memory available.\n");
    }
}

void popPid(childpidPtr *sPtr, pid_t childpid) {

    if (*sPtr == NULL) {
        return;
    }
    // delete first node if a match is found
    if (childpid == (*sPtr)->childpid) {
        childpidPtr tempPtr = *sPtr; // hold onto node being removed
        *sPtr = (*sPtr)->nextPtr; // de-thread the node
        free(tempPtr); // free the de-threaded node
    } else {
        childpidPtr previousPtr = *sPtr;
        childpidPtr currentPtr = (*sPtr)->nextPtr;

        // loop to find the correct location in the list
        while (currentPtr != NULL && currentPtr->childpid != childpid) {
            previousPtr = currentPtr; // walk to ...
            currentPtr = currentPtr->nextPtr; // ... next node
        }

        // delete node at currentPtr
        if (currentPtr != NULL) {
            childpidPtr tempPtr = currentPtr;
            previousPtr->nextPtr = currentPtr->nextPtr;
            free(tempPtr);
        }
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

void printHistory(ListNodePtr currentPtr) {
    // if list is empty
    if (isEmpty(currentPtr)) {
        puts("List is empty.\n");
    } else {
        puts("The history is:");
        int index = 0;
        // while not the end of the list
        while (currentPtr != NULL && index < 10) {
            printf("%d %s %s\n", index, currentPtr->commandPath, currentPtr->data);
            currentPtr = currentPtr->nextPtr;
            index++;
        }
    }
}

void printProcesses(childpidPtr currentPtr) {
    // if list is empty
    childpidPtr copy = currentPtr;
    if (isEmpty(currentPtr)) {
        puts("List is empty.\n");
    } else {
        puts("background Processes is:");
        // while not the end of the list
        while (currentPtr != NULL) {
            printf("%d  %s\n", currentPtr->childpid, currentPtr->name);
            currentPtr = currentPtr->nextPtr;
        }
    }
}





