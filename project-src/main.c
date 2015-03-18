//*****************************************************************************
// Includes
//*****************************************************************************
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "inc/channel.h"
#include "inc/defines.h"
#include "inc/GPIO.h"
#include "inc/linkedList.h"
#include "inc/PWM.h"
#include "inc/scanningMatrix.h"
#include "inc/timers.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/UART.h"

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

list testList;
bool recordLoop;
uint32_t recordLoopNum;
node *cur;

int8_t keyNumber;
uint32_t waveCount = 0;

uint16_t i;
int8_t* keyNumberPtr;

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
	
	recordLoopNum = 0;
  while(1) { 

		if (alertScan) {
			alertScan = false;
			if (recordLoopNum < 100) {
				recordLoopNum++;
			} else if (!recordLoop) {
				recordLoop = true;
				cur = testList.head;
				UART0_TxPoll("\n\rBegin Record Loop");
			}
			if (!recordLoop) {
				keyNumber = scanMatrix(scanningMatrix);
				keyNumberPtr = malloc(sizeof(int8_t));
				*keyNumberPtr = keyNumber;
				add(&testList, keyNumberPtr);
			} else {
				keyNumber = *((int8_t*) cur->data);
				if (cur == testList.tail) {
					cur = testList.head;
				} else {
					cur = cur->next;
				}
			}
			updateKey(testChannel.note, keyNumber);
			updateTuningWord(testChannel.note, &tword3);
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
	channel->waveTable_ref = &noise[0];
	
	channel->note = &testNote;
	channel->note->waveTable = testWaveTable;
	channel->note->isNoise = true;
	
	channel->note->key = &testKey;
	channel->note->key->letter = NO_NOTE;
	channel->note->key->octave = MIN_OCTAVE + 2;
	
	channel->note->effects.enabled = true;
	channel->note->effects.volumeEnabled = true;
	channel->note->effects.volumeCounter = 0; 
	channel->note->effects.volumeLoopPos = 0;

	initADSREnvelope(channel);
}
