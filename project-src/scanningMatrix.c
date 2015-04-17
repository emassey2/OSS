#include "inc/scanningMatrix.h"
#include "inc/UART.h"

// send output on each row one at a time
// for ever output scan every input to detect key presses
int8_t scanMatrix(bool matrix[NUM_ROWS][NUM_COLS]) {
	uint8_t i, j, k, col, row;
	int8_t keyPressed;
	bool keyIsPressed;
	keyIsPressed	= false;
	keyPressed = NO_NOTE;
	for (i = 0; i < NUM_ROWS; i++) {
		switch(i) {
			case 0: 
				row = ROW1;
				break;
			case 1: 
				row = ROW2;
				break;
			case 2: 
				row = ROW3;
				break;
			case 3: 
				row = ROW4;
				break;
			case 4: 
				row = ROW5;
				break;
		}
		
		//set all pins low then set our new one high
		GPIO_PORTE_DATA_R |= ROWS;
		GPIO_PORTE_DATA_R &= ~row;
		
		
		// wait for a little bit to allow voltage to build
		for (k = 0; k < 20; k++);
			
		for (j = 0; j < NUM_COLS; j++) {
					switch(j) {
						case 0: 
							col = COL1;
							break;
						case 1: 
							col = COL2;
							break;
						case 2: 
							col = COL3;
							break;
						case 3: 
							col = COL4;
							break;
						case 4: 
							col = COL5;
							break;
					}
					
					// COL5 is in portE not portC and thus is a special case
					if (col != COL5) {
						if (GPIO_PORTC_DATA_R & col) {
							keyIsPressed = true;
						}
					} else {
						if (GPIO_PORTE_DATA_R & col) {
							keyIsPressed = true;
						}
					}
					
					if (keyIsPressed) {
						matrix[i][j] = true;
						keyPressed = (i * 5) + j;
					} else {
						matrix[i][j] = false;
					}
					keyIsPressed = false;
		}
	}
	
	return keyPressed;
}
	
void printMatrix(bool matrix[NUM_ROWS][NUM_COLS]) {
	uint8_t i, j;
	UART0_TxPoll("\n\rScanning Matrix Results...");
		for (i = 0; i < NUM_ROWS; i++) {
			UART0_TxPoll("\n\r[ ");
			for (j = 0; j < NUM_COLS; j++) {
				if (matrix[i][j]) {
					UART0_TxPoll("1 ");
				} else {
					UART0_TxPoll("0 ");
				}
			}
			UART0_TxPoll("]");
		}
}

