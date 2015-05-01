/************************************************************************/
/* FILE NAME    - circular_buffer.c                                     */
/* AUTHOR       - ECE353-Ethan Massey                                   */
/* DATE CREATED - 26-Feb-2014                                           */
/* DESCRIPTION  -                                                       */
/*                                                                      */
/* (c) ECE Department, University of Wisconsin - Madison                */
/************************************************************************/
#include "inc/circular_buffer.h"



/******************************************************************************
 * cBufInit
 *****************************************************************************/
int32_t		cBufInit(CircularBuffer *bufPtr, int16_t size) {
		//validate parameters
		if (!(size > 0 && bufPtr != NULL)) {
			return -1;
		}
		
		//allocate size number bytes
		bufPtr->buffer = malloc(size);
		if (bufPtr->buffer == NULL) {
			return -1;
		}
		
		//initalize variables
		bufPtr->size = size;
		bufPtr->count = 0;
		bufPtr->tailIndex = 0;
    
    return 0; 
}

/******************************************************************************
 * cBufAddChar
 *****************************************************************************/
int32_t		cBufAddChar(CircularBuffer *bufPtr, char c) {
		//validate parameters
		if (bufPtr == NULL) {
			return -1;
		}
		
		//check to see if buffer is full
		if (bufPtr->size == bufPtr->count && bufPtr->count != 0) {
			//overwrite oldest entry
			bufPtr->buffer[bufPtr->tailIndex] = c;
			//we mod with size so we wrap around to the start
			bufPtr->tailIndex = (bufPtr->tailIndex + 1) % bufPtr->size;
			return 1;
			
		} else { //buffer is not full; normal case
			bufPtr->buffer[(bufPtr->count + bufPtr->tailIndex) % bufPtr->size] = c;
			bufPtr->count++;
			return 0;
		}
    
    return -1;
}

/******************************************************************************
 * cBufGetChar:
 *****************************************************************************/
int32_t		cBufGetChar(CircularBuffer *bufPtr, char *retChar) {
		//validate parameters
		if (bufPtr == NULL || retChar == NULL) {
			return -1;
		}
		
		//check if buffer is nonempty
		if (bufPtr->count != 0) {
			//set the data of retChar to the oldest element
			*retChar = bufPtr->buffer[bufPtr->tailIndex];
			//we mod so we wrap around to the start
			bufPtr->tailIndex = (bufPtr->tailIndex + 1) % bufPtr->size;
			bufPtr->count--;
			return 0;
			
		} else { //buffer is empty
			return 1;
		}
		
    return -1;
}

/******************************************************************************
 * cBufGetFreeCount
 *****************************************************************************/
int32_t		cBufGetFreeCount(CircularBuffer *bufPtr) {
		//validate parameters
		if (bufPtr == NULL) {
			return -1;
		}

    return bufPtr->size - bufPtr->count;
}

