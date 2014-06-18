#include "inc/timers.h"
#include "inc/defines.h"
#include "inc/waves.h"
#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************************************
 * This is a DDS Frequency Generator and thus we use the equation Fout = (M (REFCLK)) / 2^N
 * Where Fout is desired frequency out, M is the tuning word, REFCLK is the frequency of the reference
 * clock which in our case is the frequesncy of SysTick. and N is the length (bit-width) of 
 * the phase accumulator (32 bits).
 * 
 * The synth is designed to have an 8 octave max range. Therefore our highest frequency will be 7902hz
 * Knowing this we can solve for a theoretical minimum Reload Value to satisfy our reference value
 * Using the equation above: 7902hz = ((2^32 - 1) * ( (80*10^6) / x)) / 2^32
 * Solving for x we find our reload value to be 10124.
 * Thus SysTick has a reload value of 10124 and our clk is 80mhz. 80mhz / 10124 = 7902.02Hz = REFCLK
 *
 * By changing the tuning word we can achive diffrent frequencies
 * Solving for M we get the equation M = 2^32 * Fout / 7902.02
 *
 * Fout is controlled by the user but we precalculate 2^N / REFCLK to save time
 * Thus our turning word M = 2^32 / 7902.02 * Fout = 543528.1113088 * Fout
 * 
 * Our DAC is provided using Timer0 in conjunction with the alternate function of pin B6
 ******************************************************************************************************/

// above is out of date as I am currently trying diffrent values to find best one empirically 

/******************************************************************************
 * Global Variables
 *****************************************************************************/
 
volatile uint32_t tword0 = (136902 * 60);
volatile uint32_t tword1 = (543528 * 350);
volatile uint16_t lfsr = 1;

void initTimers() {
	initSysTick();
	initTimer0PWM();
	initTimer1Noise();
}

void initSysTick() {
	NVIC_ST_RELOAD_R = SYS_TICK_RELOAD_VAL;		// load in our reload value
	NVIC_ST_CURRENT_R = 0;										// clear the current value
	NVIC_ST_CTRL_R = NVIC_ST_CTRL_ENABLE	| 	// set clksrc and enable high
									 NVIC_ST_CTRL_INTEN 	|
									 NVIC_ST_CTRL_CLK_SRC;
}

void initTimer0PWM() {
	uint8_t delay;
	SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R0;	// enable clock gating reg for timer0
	delay = SYSCTL_RCGCTIMER_R0;								// wait a cycle for the CGR
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
	static uint32_t phaseAccum0 = 0;
	static uint8_t offset0 = 0;
	static uint32_t phaseAccum1 = 0;
	static uint8_t offset1 = 0;
	
	/*update noise register using a linear feedback shift register (this one is based off the Noise APU of the NES)
	feedBack = ((noiseReg & bit0) ^ ((noiseReg & bit1) >> 1)) << 15;
	//feeback becomes bit 15 of noiseReg
	noiseReg = feedBack | (noiseReg >> 1);*/
	
	
	
	//voice 1
	phaseAccum0 += tword0;						// Increment the Phase Accumulator0
	offset0 = phaseAccum0 >> 24;  		// use only the upper 8 bits from the phaseAccum0 (256)
	
	//voice 2
	phaseAccum1 += tword1;						// Increment the Phase Accumulator0
	offset1 = phaseAccum1 >> 24;  		// use only the upper 8 bits from the phaseAccum0 (256)
	
	//combine voices (add and shift right one to get a rough average)
	TIMER0_TAMATCHR_R = ((lfsr >> 8));//+ sine[offset1]) >> 1;	// change duty cycle
}

void TIMER1IntHandler() {
	static uint16_t bit = 0;
	TIMER1_ICR_R = TIMER_ICR_TATOCINT;	// acknowledge timer1A
	bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
  lfsr =  (lfsr >> 1) | (bit << 15);
}
