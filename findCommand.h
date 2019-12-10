//
// Created by misa on 12/6/19.
//

#ifndef MYSHELL_FINDCOMMAND_H
#define MYSHELL_FINDCOMMAND_H

#endif //MYSHELL_FINDCOMMAND_H


#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "dataStructure.h"

ListNodePtr pathPtr = NULL;
ListNodePtr pathPtrDup = NULL;

char *splitString(char str[], char delimeter[]) {
    //parsing path variable respect to : and insert stack
    char temp[300];
    strcpy(temp, str);
    char *ptr = strtok(temp, delimeter);
    int index = 0;
    while (ptr != NULL) {
        insert(&pathPtr, ptr, "");
        insert(&pathPtrDup, ptr, "");
        ptr = strtok(NULL, delimeter);
    }
}

char *findPath(char command[]) {
    // find command run path and return
//     if list is empty
    if (isEmpty(pathPtr)) {
        puts("List is empty.\n");
        return NULL;
    } else {
        // while not the end of the list
        ListNodePtr currentPtr = pathPtr;
        while (currentPtr != NULL) {
            int result = readDir(currentPtr->data, command);
            if (result == 0) {
                char *path = malloc(50);
                strcpy(path, currentPtr->data);
                return path;
            }
            currentPtr = currentPtr->nextPtr;
        }
    }
    return NULL;
}

char *getCommand(char command[]) {
    // union of the previous functions
//    char delim[] = ":";
//    char *ptr = getenv("PATH"); // get path variable
//
//    splitString(ptr, delim);
    char *commandPath = findPath(command);
    if (commandPath == NULL) {
        return NULL;
    }
    strcat(commandPath, "/");
    strcat(commandPath, command);
    return commandPath;
}

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
        } else if (!strcmp(in_file->d_name, ".")) {
            continue;
        } else if (!strcmp(in_file->d_name, "..")) {
            continue;
        } else if (!strcmp(in_file->d_name, command)) { // command found in folder
            close(FD);
            return 0;
        }

    }
    close(FD);
    return -1;
}