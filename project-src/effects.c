#include "inc/effects.h"

// may not need this method anymore
Node* findMarker(List *list, char marker) {
	Node *curNode;
	
	//validate params
	if (list == NULL) {
		return NULL;
	} else if (list->head == NULL) {
		return NULL;
	}
	
	// get a pointer to the head of our list
	curNode = list->head;
	
	// iterate over our list to find marker
	while (curNode->next != NULL) {
		if (((VolumeEff*)curNode->data)->marker != marker) {
			// marker found! return our node
			return curNode;
		} else {
			// increment
			curNode = curNode->next;
		}
	}
	
	return NULL;
}

bool updateVolumeState(Effects* self) {
	// keeps track how long we have been in a given state
	static uint32_t currentDuration = 0;
	
	if (((VolumeEff*)self->volumeCur->data)->duration > currentDuration) {
		currentDuration++;
		return false;
	} else {
		currentDuration = 0;
		return true;
	}
}

