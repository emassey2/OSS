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
#include "inc/lm4f120h5qr.h"


/******************************************************************************
 * Defines
 *****************************************************************************/


/******************************************************************************
 * Global Variables
 *****************************************************************************/



//*****************************************************************************
// External Functions
//*****************************************************************************
extern void PLL_Init(void);
extern void EnableInterrupts(void);
extern void DisableInterrupts(void);


//*****************************************************************************
//*****************************************************************************
int main(void) {
  PLL_Init();
	
	initPollingUART0();
	UART0_TxPoll("\n\r\n\r\n\r*****Open Source Synth V.02*****");
	
	UART0_TxPoll("\n\rInitializing GPIO Ports...");
	initGPIOPorts();
	
	UART0_TxPoll("\n\rInitializing Timers...");
	initTimers();
	
	
	UART0_TxPoll("\n\rEntering Main Loop");
  while(1) {
		
  };

  
}

