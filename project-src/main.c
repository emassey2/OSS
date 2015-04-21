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
#include "inc/qei.h"
#include "inc/scanningMatrix.h"
#include "inc/timers.h"
#include "inc/tm4c123gh6pm.h"
#include "inc/UART.h"

/******************************************************************************
 * Forward Decleration
 *****************************************************************************/
void initChannels(void);
void updateAllEffects(void);
void updateAllTuningWords(void);
void setCurrentChannel(Channel* newCurrentCh);

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

Channel* currentCh;

Channel* ch0;
Channel* ch1;
Channel* ch2;
Channel* ch3;
Channel* ch4;
Channel* ch5;

bool scanningMatrix[NUM_ROWS][NUM_COLS];

List testList;
bool recordLoop;
uint32_t recordLoopNum;
Node *cur;

int8_t keyNumber;

uint32_t i, pos;
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
	char posArray[40];
  PLL_Init();
	
	initPollingUART0();
	UART0_TxPoll("\n\r\n\r\n\r*****Chiposizer V.7****");

	UART0_TxPoll("\n\rInitializing GPIO Ports...");
	initGPIOPorts();	
	
	UART0_TxPoll("\n\rInitializing PWM Modules...");
	initPWM();
	
	UART0_TxPoll("\n\rInitializing Timers...");
	initTimers();
	
	UART0_TxPoll("\n\rInitializing Channels...");
	initChannels();
	setCurrentChannel(ch0);

	//initQei1();
	
	UART0_TxPoll("\n\rEntering Main Loop");
	
	recordLoopNum = 0;

  while(1) { 
		/*while(1){

			pos = getPos();
			
			//delay
			for(i = 0; i < 8000000 ; i++ ){}

			//	pos = getPos();
				sprintf(posArray, "\n\rPos was: %d", pos);
		}*/
		
		if (alertEffect) {
			alertEffect = false;
			updateAllEffects();
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
			updateKey(currentCh->note, keyNumber);
			//printMatrix(scanningMatrix);
		}
		updateAllTuningWords();
	}
};

void initChannels(void) {
		initChannel(&ch0, &wave0, square50);
	/*initChannel(&ch1, &wave1, triangle);
	initChannel(&ch2, &wave2, saw);
	initChannel(&ch3, &wave3, noise);
	initChannel(&ch4, &wave4, sine);
	initChannel(&ch5, &wave5, square12);*/
}

void updateAllEffects(void) {
	updateEffects(ch0->note, ch0->waveTable_ref);
	/*updateEffects(ch1->note, ch1->waveTable_ref);
	updateEffects(ch2->note, ch2->waveTable_ref);
	updateEffects(ch3->note, ch3->waveTable_ref);
	ch3->note->isNoise = true;
	updateEffects(ch4->note, ch4->waveTable_ref);
	updateEffects(ch5->note, ch5->waveTable_ref);*/
}

void updateAllTuningWords(void) {
	updateTuningWord(ch0->note, &tword0);
	/*updateTuningWord(ch1->note, &tword1);
	updateTuningWord(ch2->note, &tword2);
	updateTuningWord(ch3->note, &tword3);
	updateTuningWord(ch4->note, &tword4);
	updateTuningWord(ch5->note, &tword5);*/
}

void setCurrentChannel(Channel* newCurrentCh) {
	currentCh = newCurrentCh;
}