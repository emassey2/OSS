#include "inc/timers.h"
#include "inc/defines.h"
#include "inc/waves.h"
#include <stdint.h>

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


/******************************************************************************
 * Global Variables
 *****************************************************************************/
 
volatile uint32_t tword0 = 54336705;

void initTimers() {
	initSysTick();
	initTimer0PWM();
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
	// defaults to output state to not inverted
	// defaults to prescaler to 0
	TIMER0_TAMR_R |= TIMER_TAMR_TAPWMIE;				// enable interrupts
	TIMER0_TAILR_R = TIMER0A_RELOAD_VAL;					// load in starting value
	TIMER0_TAMATCHR_R = TIMER0A_RELOAD_VAL >> 1; 	// duty cycle of 50%
	TIMER0_CTL_R |= TIMER_CTL_TAEN;								//enable timer
}

void SYSTICKIntHandler() {
	static uint32_t phaseAccum0 = 0;
	static uint8_t offset0 = 0;
	phaseAccum0 += tword0;						// Increment the Phase Accumulator0
	offset0 = phaseAccum0 >> 24;  		// use only the upper 8 bits from the phaseAccum0 (256)
	TIMER0_TAILR_R = wave[offset0];					// set new period
	TIMER0_TAMATCHR_R = wave[offset0] >> 1;	// make sure duty cycle is still 50%
}
