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
 * The synth is optimized for the common case of a max frequency of C5 or 523.251hz.
 * Knowing this we can solve for a theoretical minimum Systick Reload Value to satisfy our reference clock
 * Using the equation above: 523.251hz = ((2^32 - 1) * ( (80*10^6) / x)) / 2^32
 * Solving for x we find our reload value to be 598.
 * Thus SysTick has a reload value of 598 and our clk is 80mhz. 80mhz / 598 = 133779.2642Hz = REFCLK
 *
 * By changing the tuning word we can achieve different frequencies
 * Solving for M we get the equation M = 2^32 * Fout / 133779.2642
 *
 * Fout is controlled by the user but we precalculate 2^N / REFCLK to save time
 * Thus our turning word M = 2^32 / 133779.2642 * Fout therfore M = 32104.9 * Fout
 *
 * 32104.9 will from here on be refered to as the Tuning Constant or TUNING_CONST
 * 
 * Our DAC is provided using PWM modules in conjunction with the alternate function of pin(s)
 ******************************************************************************************************/

void initPWM() {
	uint8_t i;
	
	SYSCTL_RCGC0_R |= SYSCTL_RCGC0_PWM0;				// enable PWM CGR
	for(i = 0; i < 10; i++);	

	// Pin6 on portB is enabled in GPIO.c
	
	//SYSCTL_RCC_R |= SYSCTL_RCC_USEPWMDIV;			// enable the PWM divider
	//for(i = 0; i < 10; i++);	
	//SYSCTL_RCC_R |= SYSCTL_RCC_PWMDIV_2;			// set PWM divider to 2
	//for(i = 0; i < 10; i++);	
	
	PWM0_0_CTL_R	&= ~PWM_0_CTL_ENABLE;				// disable timers in PWM0
	PWM0_0_CTL_R	= PWM_0_CTL_DBFALLUPD_I;		// enable immedidate updates to parameters
	PWM0_0_GENA_R |= PWM_0_GENA_ACTLOAD_ONE;	// set output high (1) on load A
	PWM0_0_GENA_R |= PWM_0_GENA_ACTCMPAD_ZERO;// set output low (0) on match A
	PWM0_0_GENB_R |= PWM_0_GENB_ACTLOAD_ONE;	// set output high (1) on load B
	PWM0_0_GENB_R |= PWM_0_GENB_ACTCMPBD_ZERO;// set output low (0) on match B
	PWM0_0_LOAD_R |= PWM_0_LOAD_VAL;					// set our reload value
	PWM0_0_CMPA_R |= PWM_CPM_DEFAULT;					// default our duty cycle 0/100
	PWM0_0_CMPB_R |= PWM_CPM_DEFAULT;					// default our duty cycle 0/100
	PWM0_0_CTL_R 	|= PWM_0_CTL_ENABLE;				// enable timers in PWM0
	PWM0_ENABLE_R |= PWM_ENABLE_PWM0EN;				// enable PWM output
	PWM0_ENABLE_R |= PWM_ENABLE_PWM1EN;
}
