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

/******************************************************************************
 * Global Variables
 *****************************************************************************/
extern volatile bool alertScan;
extern volatile bool alertEffect;
extern volatile uint32_t tword0;
extern volatile uint32_t tword1;
extern volatile uint32_t tword2;
extern volatile uint32_t tword3;
extern uint8_t* wave0;
extern uint8_t* wave1;
extern uint8_t* wave2;
extern uint8_t* wave3;
bool scanningMatrix[NUM_ROWS][NUM_COLS];

Channel testChannel;
Note testNote;
Key testKey;
uint8_t testWaveTable[WAVE_TABLE_SIZE];
Effects testEffect;

int8_t keyNumber;
uint32_t waveCount = 0;

uint16_t i;

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
	UART0_TxPoll("\n\r\n\r\n\r*****Chiposizer V.3****");

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
			updateKey(testChannel.note, keyNumber);
			updateTuningWord(testChannel.note, &tword0);
		}
		if (alertEffect) {
			alertEffect = false;
			updateEffects(testChannel.note, testChannel.waveTable_ref);
			wave0 = testChannel.note->waveTable;
		}
	}
};

void initChannels(Channel* channel) {
	channel = &testChannel;
	channel->waveTable_ref = &sine[0];
	
	channel->note = &testNote;
	channel->note->waveTable = testWaveTable;
	
	channel->note->key = &testKey;
	channel->note->key->letter = NO_NOTE;
	channel->note->key->octave = MIN_OCTAVE + 2;
	
	channel->note->effects.enabled = true;
	channel->note->effects.volumeEnabled = true;
	channel->note->effects.volumeCounter = 0; 
	channel->note->effects.volumeLoopPos = 0;

	initADSREnvelope(channel);
}
