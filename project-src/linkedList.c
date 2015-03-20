
#include <stddef.h>
#include <stdlib.h>
#include "inc/linkedList.h"

// adds an element to the end of the List
int8_t add(List *list, void *data) {
	Node *newNode;
	
	// validate params
	if (list == NULL) {
		return -1;
	}
	
	// allocate space for our new Node
	newNode = malloc(sizeof(Node));
	newNode->data = data;
	
	// add our new Node to our List
	if (list->head == NULL) {
		// case where there are no elements in List
		list->head = newNode;
		list->tail = newNode;
	} else if (list->tail != NULL) {
		// generic case. i.e. one or more Nodes present
		list->tail->next = newNode;
		list->tail = newNode;
	} else {
		// how could this happen to me!? I've made my mistakes!
		return -1;
	}
	
	return 0;
}

// removes an element from the end of the List
int8_t removeEnd(List *list) {
	Node *cur;
		
	// validate params
	if (list == NULL) {
		return -1;
	} else if (list->head == NULL) {
		return -1;
	}
	
	// edge case where there is only one element in the List
	if (list->head->next == NULL) {
		free(list->head->data);
		free(list->head);
		list->head = NULL;
		list->tail = NULL;
		
	//normal case
	} else {
		//start at head
		cur = list->head;
		
		// two or more Nodes present
		while (cur->next->next != NULL) {
			// loop until we find the second to last Node
			cur = cur->next;
		}
		
		free(cur->next->data);	// free our data
		free(cur->next);				// free our Node
		list->tail = cur;				// set tail to new last Node
		cur->next = NULL;
	}
	
	return 0;
}

// deletes all Nodes in a linked List. returns number of elements deleted else -1.
int8_t clear(List *list) {
	int8_t retVal;
	uint8_t count = 0;
	
	// validate params
	if (list != NULL) {
		retVal = removeEnd(list);
		while (retVal == 0) {
			retVal = removeEnd(list);
			count++;
		}
		return count;
	}
	return -1;
}

