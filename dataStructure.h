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

#define SIZE 100

struct listNode {
    char *data; // each listNode contains a character
    struct listNode *nextPtr; // pointer to next node
};

typedef struct listNode ListNode; // synonym for struct listNode
typedef ListNode *ListNodePtr; // synonym for ListNode*

void insert(ListNodePtr *sPtr, char *value) {
    ListNodePtr newPtr = malloc(sizeof(ListNode)); // create node

    if (newPtr != NULL) { // is space available
        newPtr->data = strdup(value);
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


