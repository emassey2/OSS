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
 *
 *
 *	6 audio ch
 *	pf0 - m1-4 - m1_2A - ch0
 *	pf1 - m1-5 - m1_2B - ch1
 *	pb4 - m0-2 - m0_1A - ch2
 *	pb5 - m0-3 - m0_1B - ch3
 *	pb6 - m0-0 - m0_0A - ch4
 *	pb7 - m0-1 - m0_0B - ch5
 ******************************************************************************************************/

void initPWM() {
	uint8_t i;
	
	SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R0;				// enable PWM CGR
	for(i = 0; i < 10; i++);	
	SYSCTL_RCGCPWM_R |= SYSCTL_RCGCPWM_R1;				// enable PWM CGR
	for(i = 0; i < 10; i++);	

	// Pins enabled in GPIO.c
	
	// enable PB6 and PB7
	PWM0_0_CTL_R	&= ~PWM_0_CTL_ENABLE;					// disable timers in PWM0_0
	PWM0_0_CTL_R	 =  PWM_0_CTL_DBFALLUPD_I;		// enable immedidate updates to parameters
	PWM0_0_GENA_R |=  PWM_0_GENA_ACTLOAD_ONE;		// set output high (1) on load A
	PWM0_0_GENA_R |=  PWM_0_GENA_ACTCMPAD_ZERO;	// set output low (0) on match A
	PWM0_0_GENB_R |=  PWM_0_GENB_ACTLOAD_ONE;		// set output high (1) on load B
	PWM0_0_GENB_R |=  PWM_0_GENB_ACTCMPBD_ZERO;	// set output low (0) on match B
	PWM0_0_LOAD_R |=  PWM_LOAD_VAL;							// set our reload value
	PWM0_0_CMPA_R |=  PWM_CPM_DEFAULT;					// default our duty cycle 0/100
	PWM0_0_CMPB_R |=  PWM_CPM_DEFAULT;					// default our duty cycle 0/100
	PWM0_0_CTL_R 	|=  PWM_0_CTL_ENABLE;					// enable timers in PWM0_0
	
	// enable PB4 and PB5
	PWM0_1_CTL_R	&= ~PWM_1_CTL_ENABLE;					// disable timers in PWM0_1
	PWM0_1_CTL_R	 =  PWM_1_CTL_DBFALLUPD_I;		// enable immedidate updates to parameters
	PWM0_1_GENA_R |=  PWM_1_GENA_ACTLOAD_ONE;		// set output high (1) on load A
	PWM0_1_GENA_R |=  PWM_1_GENA_ACTCMPAD_ZERO; // set output low (0) on match A
	PWM0_1_GENB_R |=  PWM_1_GENB_ACTLOAD_ONE;		// set output high (1) on load B
	PWM0_1_GENB_R |=  PWM_1_GENB_ACTCMPBD_ZERO;	// set output low (0) on match B
	PWM0_1_LOAD_R |=  PWM_LOAD_VAL;							// set our reload value
	PWM0_1_CMPA_R |=  PWM_CPM_DEFAULT;					// default our duty cycle 0/100
	PWM0_1_CMPB_R |=  PWM_CPM_DEFAULT;					// default our duty cycle 0/100
	PWM0_1_CTL_R 	|=  PWM_1_CTL_ENABLE;					// enable timers in PWM0_1
	
	// enable PF0 and PF1
	PWM1_2_CTL_R	&= ~PWM_2_CTL_ENABLE;					// disable timers in PWM1_2
	PWM1_2_CTL_R	 =  PWM_2_CTL_DBFALLUPD_I;		// enable immedidate updates to parameters
	PWM1_2_GENA_R |=  PWM_2_GENA_ACTLOAD_ONE;		// set output high (1) on load A
	PWM1_2_GENA_R |=  PWM_2_GENA_ACTCMPAD_ZERO; // set output low (0) on match A
	PWM1_2_GENB_R |=  PWM_2_GENB_ACTLOAD_ONE;		// set output high (1) on load B
	PWM1_2_GENB_R |=  PWM_2_GENB_ACTCMPBD_ZERO;	// set output low (0) on match B
	PWM1_2_LOAD_R |=  PWM_LOAD_VAL;							// set our reload value
	PWM1_2_CMPA_R |=  PWM_CPM_DEFAULT;					// default our duty cycle 0/100
	PWM1_2_CMPB_R |=  PWM_CPM_DEFAULT;					// default our duty cycle 0/100
	PWM1_2_CTL_R 	|=  PWM_2_CTL_ENABLE;					// enable timers in PWM1_2
	
	
	PWM0_ENABLE_R |=  PWM_ENABLE_PWM0EN;				// enable PWM output
	PWM0_ENABLE_R |=  PWM_ENABLE_PWM1EN;
	PWM0_ENABLE_R |=  PWM_ENABLE_PWM2EN;
	PWM0_ENABLE_R |=  PWM_ENABLE_PWM3EN;
	PWM1_ENABLE_R |=  PWM_ENABLE_PWM4EN;
	PWM1_ENABLE_R |=  PWM_ENABLE_PWM5EN;
	
	
}
