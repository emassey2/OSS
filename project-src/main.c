//*****************************************************************************
// Includes
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
#include "inc/channel.h"
#include "inc/tm4c123gh6pm.h"

/******************************************************************************
 * Forward Decleration
 *****************************************************************************/
void initChannels(Channel* channel);

void hackyUpdate(void);
 
/******************************************************************************
 * Global Variables
 *****************************************************************************/
extern volatile bool alertScan;
extern volatile uint32_t tword0;
extern volatile uint32_t tword1;
extern volatile uint32_t tword2;
extern volatile uint32_t tword3;
bool scanningMatrix[NUM_ROWS][NUM_COLS];

Channel testChannel;
Note testNote;
Key testKey;

int8_t keyNumber;

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
	UART0_TxPoll("\n\r\n\r\n\r*****Chiposizer V.2*****");

	UART0_TxPoll("\n\rInitializing GPIO Ports...");
	initGPIOPorts();	
	
	UART0_TxPoll("\n\rInitializing PWM Modules...");
	initPWM();
	
	UART0_TxPoll("\n\rInitializing Timers...");
	initTimers();
	
	UART0_TxPoll("\n\rInitializing Channels...");
	initChannels(&testChannel);
	
	UART0_TxPoll("\n\rEntering Main Loop");
  while(1) {
		if (alertScan) {
			alertScan = false;
			keyNumber = scanMatrix(scanningMatrix);
			//printMatrix(scanningMatrix);
			updateKey(testChannel.note, keyNumber);
			updateTuningWord(testChannel.note, &tword0);
			//hackyUpdate();
		}
	}
};

void initChannels(Channel* channel) {
	channel = &testChannel;
	channel->waveTable = square50;
	
	channel->note = &testNote;
	channel->note->waveTable_ref = square50;
	
	channel->note->key = &testKey;
	channel->note->key->letter = NO_NOTE;
	channel->note->key->octave = MIN_OCTAVE + 2;
}
