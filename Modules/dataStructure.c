//
// Created by misa on 11/24/19.
//
#include "../headers/dataStructure.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



void insert(ListNodePtr *sPtr, char *value) {
    ListNodePtr newPtr = malloc(sizeof(ListNode)); // create node

    if (newPtr != NULL) { // is space available
        newPtr->data = malloc(sizeof(15));
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