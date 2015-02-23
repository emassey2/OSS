#include "inc/timers.h"
#include "inc/defines.h"
#include "inc/waves.h"
#include <stdbool.h>
#include <stdint.h>


/******************************************************************************
 * Global Variables
 *****************************************************************************/
 
volatile uint32_t tword0 = 0;
volatile uint32_t tword1 = 0;
volatile uint32_t tword2 = 0;
volatile uint32_t tword3 = 0;
volatile uint16_t lfsr = 1;
volatile bool alertScan;

void initTimers() {
	initSysTick();
	//initTimer0PWM();
	//initTimer1Noise();
}

void initSysTick() {
	NVIC_ST_RELOAD_R = SYS_TICK_RELOAD_VAL;		// load in our reload value 126.5ms
	NVIC_ST_CURRENT_R = 0;										// clear the current value
	NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE	| 	// set clksrc and enable high
									 NVIC_ST_CTRL_INTEN 	|
									 NVIC_ST_CTRL_CLK_SRC;
}

void initTimer0PWM() {
	uint8_t i;
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;	// enable clock gating reg for timer0
	for (i = 0; i < 1; i++);										// wait a cycle for the CGR
	TIMER0_CTL_R &= ~TIMER_CTL_TAEN;						// disable timer during setup
	TIMER0_CFG_R |= TIMER_CFG_16_BIT;						// configure for 16bit mode
	TIMER0_TAMR_R |= TIMER_TAMR_TAAMS;					// enable timer0 alternate mode
	TIMER0_TAMR_R &= ~TIMER_TAMR_TACMR;					// disable capture mode
	TIMER0_TAMR_R |= TIMER_TAMR_TAMR_PERIOD;		// configure timer0 for periodic mode
	TIMER0_TAMR_R |= TIMER_TAMR_TAILD;					// enable loading on interval (start of every cycle)
	TIMER0_TAMR_R |= TIMER_TAMR_TAMRSU;					// same thing but with match register
	// defaults to output state to not inverted
	// defaults to prescaler to 0
	// TIMER0_TAMR_R |= TIMER_TAMR_TAPWMIE;				// enable interrupts
	TIMER0_TAILR_R = TIMER0A_RELOAD_VAL;					// set period to 256 (0-255)
	TIMER0_TAMATCHR_R = TIMER0A_RELOAD_VAL >> 1; 	// default duty cycle of 50%
	TIMER0_CTL_R |= TIMER_CTL_TAEN;								//enable timer
}

void initTimer1Noise() {	
	int i;
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;	// activate timer1
	for(i = 0; i < 1; i++);									// delay
	TIMER1_CTL_R &= ~TIMER_CTL_TAEN;				// disable Timer1 during setup
	TIMER1_CFG_R = TIMER_CFG_16_BIT;				// configure for 16bit timer mode
	TIMER1_TAMR_R = TIMER_TAMR_TAMR_PERIOD;	// configure for periodic mode
	TIMER1_TAILR_R = TIMER1A_RELOAD_VAL;		// reload value
	TIMER1_TAPR_R = TIMER1A_PRESCALE;				// set timer period
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;			// clear timer1A timeout flag
	TIMER1_IMR_R |= TIMER_IMR_TATOIM;				// arm timeout interrupt
	NVIC_PRI5_R = (NVIC_PRI5_R & 0xFFFF00FF) | 0x00000000;// set priority to 0 as it is interrupting at only 20hz
	NVIC_EN0_R = NVIC_EN0_INT21;						// enable interrupt in NVIC
	TIMER1_CTL_R |= TIMER_CTL_TAEN;					// enable Timer1A
}


void SYSTICKIntHandler() {
	static uint32_t tickCount;					// increments ever time in the loop. Resets at 80 ~= 10ms
	static uint32_t phaseAccum0 = 0;
	static uint8_t offset0 = 0;
	static uint32_t phaseAccum1 = 0;
	static uint8_t offset1 = 0;	
	static uint32_t phaseAccum2 = 0;
	static uint8_t offset2 = 0;	
	static uint32_t phaseAccum3 = 0;
	static uint8_t offset3		= 0;
	
	if (SYS_TICK_RESET_VAL > tickCount) {
		tickCount++;
	} else {
		alertScan = true;
		tickCount = 0;
	}

	//voice 0
	phaseAccum0 += tword0;						// Increment the Phase Accumulator0
	offset0 = phaseAccum0 >> 24;  		// use only the upper 8 bits from the phaseAccum0 (256)
	
	//voice 1
	phaseAccum1 += tword1;						// Increment the Phase Accumulator0
	offset1 = phaseAccum1 >> 24;  		// use only the upper 8 bits from the phaseAccum0 (256)
	
	//voice 2
	phaseAccum2 += tword2;						// Increment the Phase Accumulator2
	offset2 = phaseAccum2 >> 24;  		// use only the upper 8 bits from the phaseAccum2 (256)
	
	//voice 3
	phaseAccum3 += tword3;						// Increment the Phase Accumulator3
	offset3 = phaseAccum3 >> 24;  		// use only the upper 8 bits from the phaseAccum3 (256)
	
	//combine voices (add and shift right one to get a rough average)
	PWM0_0_CMPA_R = (saw[offset0] + square50[offset1]) >> 1;
	PWM0_0_CMPB_R = saw[offset0];// + square50[offset1]) >> 1;
}

void TIMER1IntHandler() {
	static uint16_t fbb = 0;
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;						// acknowledge timer1A
	fbb = (lfsr & 0x0001) ^ ((lfsr & 0x0002) >> 1);	// Get LSB (i.e., the output bit).
	lfsr = lfsr >> 1;
	lfsr = lfsr | (fbb << 13);
	//TIMER0_TAMATCHR_R	= lfsr >> 7;
}
