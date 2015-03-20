#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include <stdbool.h>
#include <stdint.h>

typedef struct Node {
    void *data;
    struct Node *next;
} Node;

typedef struct List {
	struct Node *head;
	struct Node *tail;
} List;

// adds an element to the end of the List. returns -1 on fail
int8_t add(List *list, void *newObject);

// remvoes an element from the end of the List. returns -1 on fail
int8_t removeEnd(List *list);

// deletes all Nodes in a linked List. returns -1 on fail
int8_t clear(List *list);

#endif // __LINKEDLIST_H__
