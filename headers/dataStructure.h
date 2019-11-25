//
// Created by misa on 11/24/19.
//

#ifndef MYSHELL_DATASTRUCTURE_H
#define MYSHELL_DATASTRUCTURE_H
struct listNode {
    char *data; // each listNode contains a character
    struct listNode *nextPtr; // pointer to next node
};

typedef struct listNode ListNode; // synonym for struct listNode
typedef ListNode *ListNodePtr; // synonym for ListNode*
#endif //MYSHELL_DATASTRUCTURE_H
