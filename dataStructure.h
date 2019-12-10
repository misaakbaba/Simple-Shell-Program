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

void pop(ListNodePtr *sPtr, char *deletePath) {

    if (*sPtr == NULL) {
        return;
    }
    // delete first node if a match is found
    if (strcmp(deletePath, (*sPtr)->data) == 0) {
        ListNodePtr tempPtr = *sPtr; // hold onto node being removed
        *sPtr = (*sPtr)->nextPtr; // de-thread the node
        free(tempPtr); // free the de-threaded node
    } else {
        ListNodePtr previousPtr = *sPtr;
        ListNodePtr currentPtr = (*sPtr)->nextPtr;

        // loop to find the correct location in the list
        while (currentPtr != NULL && strcmp(deletePath, (*sPtr)->data) != 0) {
            previousPtr = currentPtr; // walk to ...
            currentPtr = currentPtr->nextPtr; // ... next node
        }

        // delete node at currentPtr
        if (currentPtr != NULL) {
            ListNodePtr tempPtr = currentPtr;
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

void printPath(ListNodePtr head) {
    ListNodePtr currentPtr = head;
    // if list is empty
    if (isEmpty(currentPtr)) {
        puts("List is empty.\n");
    } else {
        puts("The path variable is:");
        // while not the end of the list
        while (currentPtr != NULL) {
            printf("%s:", currentPtr->data);
            currentPtr = currentPtr->nextPtr;
        }
        puts("");
    }
}





