#include <stdbool.h>
#include <stdint.h>

typedef struct node {
    void *data;
    struct node *next;
} node;

typedef struct list {
	struct node *head;
	struct node *tail;
} list;

// adds an element to the end of the list. returns -1 on fail
int8_t add(list *list, void *newObject);

// remvoes an element from the end of the list. returns -1 on fail
int8_t removeEnd(list *list);

// deletes all nodes in a linked list. returns -1 on fail
int8_t clear(list *list);
