#include "inc/scanningMatrix.h"

// send output on each row one at a time
// for ever output scan every input to detect key presses
void scanMatrix(bool matrix[NUM_ROWS][NUM_COLS]) {
	uint8_t i, j, col, row;
	for (i = 0; i < NUM_ROWS; i++) {
		// janky right now because pins aren't all in series 
		// this will be fixed when we have a real circuit board
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
		//GPIO_PORTD_DATA_R &= ~ROWS;
		//GPIO_PORTD_DATA_R |= row;
			
		for (j =0; j < NUM_COLS; j++) {
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
					if (GPIO_PORTE_DATA_R & col) {
						matrix[i][j] = true;
					} else {
						matrix[i][j] = false;
					}
		}
	}
}
