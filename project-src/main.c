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


/******************************************************************************
 * Global Variables
 *****************************************************************************/
extern volatile bool volumeUpdate;


extern volatile bool alertScan;
extern volatile bool alertEffect;
extern volatile uint8_t offset0;
extern volatile uint32_t tword0;
extern volatile uint32_t tword1;
extern volatile uint32_t tword2;
extern volatile uint32_t tword3;
extern volatile uint32_t tword4;
extern volatile uint32_t tword5;
extern int8_t* wave0;
extern int8_t* wave1;
extern int8_t* wave2;
extern int8_t* wave3;
extern int8_t* wave4;
extern int8_t* wave5;





Channel* ch0;
Channel* ch1;
Channel* ch2;
Channel* ch3;
Channel* ch4;
Channel* ch5;

bool scanningMatrix[NUM_ROWS][NUM_COLS];

Channel* testChannel;
Note* testNote;
Key* testKey;
uint8_t testWaveTable[WAVE_TABLE_SIZE];
Effects testEffect;

List testList;
bool recordLoop;
uint32_t recordLoopNum;
Node *cur;

int8_t keyNumber;

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
	UART0_TxPoll("\n\r\n\r\n\r*****Chiposizer V.5****");

	UART0_TxPoll("\n\rInitializing GPIO Ports...");
	initGPIOPorts();	
	
	UART0_TxPoll("\n\rInitializing PWM Modules...");
	initPWM();
	
	UART0_TxPoll("\n\rInitializing Timers...");
	initTimers();
	
	UART0_TxPoll("\n\rInitializing Channels...");
	initChannel(&testChannel, &wave0, saw);
	
	UART0_TxPoll("\n\rEntering Main Loop");
	
	recordLoopNum = 0;
  while(1) { 
		if (alertEffect) {
			alertEffect = false;
			updateEffects(testChannel->note, testChannel->waveTable_ref);
		}
		if (alertScan) {
			alertScan = false;
			/*if (recordLoopNum < 1000) {
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
			}*/
			
			keyNumber = scanMatrix(scanningMatrix);
			updateKey(testChannel->note, keyNumber);
		}
		updateTuningWord(testChannel->note, &tword0);

	}
};
