//*****************************************************************************
//
//*****************************************************************************
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "inc/defines.h"
#include "inc/UART.h"
#include "inc/GPIO.h"
#include "inc/timers.h"
#include "inc/PWM.h"
#include "inc/scanningMatrix.h"
#include "inc/tm4c123gh6pm.h"


/******************************************************************************
 * Defines
 *****************************************************************************/


/******************************************************************************
 * Global Variables
 *****************************************************************************/
 extern volatile bool alertScan;
	 
 bool scanningMatrix[NUM_ROWS][NUM_COLS];


//*****************************************************************************
// External Functions
//*****************************************************************************
extern void PLL_Init(void);
extern void EnableInterrupts(void);
extern void DisableInterrupts(void);


//*****************************************************************************
//*****************************************************************************
int main(void) {
	//TEMP PLS REMOVE LATER//
	uint8_t i, j;
	//TEMP PLS REMOVE LATER//
  PLL_Init();
	
	initPollingUART0();
	UART0_TxPoll("\n\r\n\r\n\r*****Open Source Synth V.1*****");
	
	UART0_TxPoll("\n\rInitializing GPIO Ports...");
	initGPIOPorts();
	
					GPIO_PORTD_DATA_R &= ~ROWS;
				if (!GPIO_PORTE_DATA_R & 1) {
					UART0_TxPoll("\n\rWTF?!?!?!?");
				}
		
	UART0_TxPoll("\n\rInitializing PWM Modules...");
	initPWM();
				
								GPIO_PORTD_DATA_R &= ~ROWS;
				if (!GPIO_PORTE_DATA_R & 1) {
					UART0_TxPoll("\n\rWTF?!?!?!?");
				}
	
	UART0_TxPoll("\n\rInitializing Timers...");
	initTimers();
				
								GPIO_PORTD_DATA_R &= ~ROWS;
				if (!GPIO_PORTE_DATA_R & 1) {
					UART0_TxPoll("\n\rWTF?!?!?!?");
				}

	
	UART0_TxPoll("\n\rEntering Main Loop");
  while(1) {
		if (alertScan) {
				GPIO_PORTD_DATA_R &= ~ROWS;
				if (!GPIO_PORTE_DATA_R & 1) {
					UART0_TxPoll("\n\rWTF?!?!?!?");
				}
			alertScan = false;
			scanMatrix(scanningMatrix);
			
			UART0_TxPoll("\n\rScanning Matrix Results...");
			for (i = 0; i < NUM_ROWS; i++) {
				UART0_TxPoll("\n\r[ ");
				for (j = 0; j < NUM_COLS; j++) {
					if (scanningMatrix[i][j]) {
						UART0_TxPoll("1 ");
					} else {
						UART0_TxPoll("0 ");
					}
				}
				UART0_TxPoll("]");
			}
		}
  };

  
}

