#include "inc/PWM.h"
#include "inc/defines.h"
#include <stdint.h>
#include <stdbool.h>
#include "inc/UART.h"

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
 * Our DAC is provided using PWM modules in conjunction with the alternate function of pin(s)
 ******************************************************************************************************/

// above is out of date as I am currently trying diffrent values to find best one empirically

void initPWM() {
	uint8_t i;
	
	GPIO_PORTD_DATA_R &= ~ROWS;
	if (!GPIO_PORTE_DATA_R & 1) {
		UART0_TxPoll("\n\rPWM START");
	}
	
	SYSCTL_RCGC0_R |= SYSCTL_RCGC0_PWM0;				// enable PWM CGR
	for(i = 0; i < 10; i++);	
	
	GPIO_PORTD_DATA_R &= ~ROWS;
	if (!GPIO_PORTE_DATA_R & 1) {
		UART0_TxPoll("\n\rPWM CGR");
	}
	
	SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB;			// enable port B for our output
	for(i = 0; i < 10; i++);	
	
	GPIO_PORTD_DATA_R &= ~ROWS;
	if (!GPIO_PORTE_DATA_R & 1) {
		UART0_TxPoll("\n\rPORT B");
	}
	
	// Pin6 on portB is enabled in GPIO.c
	
	//SYSCTL_RCC_R |= SYSCTL_RCC_USEPWMDIV;			// enable the PWM divider
	//for(i = 0; i < 10; i++);	
	//SYSCTL_RCC_R |= SYSCTL_RCC_PWMDIV_2;			// set PWM divider to 2
	//for(i = 0; i < 10; i++);	
	
	PWM0_0_CTL_R	&= ~PWM_0_CTL_ENABLE;				// disable timers in PWM0
	PWM0_0_CTL_R	&= ~PWM_0_CTL_DBFALLUPD_I;	// disable immedidate updates to parameters
	PWM0_0_GENA_R |= PWM_0_GENA_ACTLOAD_ONE;	// set output high (1) on load
	PWM0_0_GENA_R |= PWM_0_GENA_ACTCMPAD_ZERO;// set output low (0) on match?
	PWM0_0_GENB_R |= PWM_0_GENB_ACTLOAD_ONE;	// set output high (1) on load
	PWM0_0_GENB_R |= PWM_0_GENB_ACTCMPAD_ZERO;// set output low (0) on match?
	PWM0_0_LOAD_R |= PWM_0_LOAD_VAL;					// set our reload value
	PWM0_0_CMPA_R |= PWM_CPM_DEFAULT;					// default our duty cycle 0/100
	PWM0_0_CMPB_R |= PWM_CPM_DEFAULT;					// default our duty cycle 0/100
	PWM0_0_CTL_R 	|= PWM_0_CTL_ENABLE;				// enable timers in PWM0
	PWM0_ENABLE_R |= PWM_ENABLE_PWM0EN;				// enable PWM output
}
