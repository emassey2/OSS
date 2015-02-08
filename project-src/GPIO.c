#include "inc/GPIO.h"
#include "inc/defines.h"
 
uint8_t delay;
 
void initGPIOPorts() {
	initGPIOPortA();
	initGPIOPortB();
}
 
void initGPIOPortA() {
	// Enable our audio ouput first
	/*SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;		// enable PORTA CLK
	delay = SYSCTL_RCGCGPIO_R0;

	GPIO_PORTA_DIR_R |= AUDIO_OUT;			// DIR is out
	GPIO_PORTA_DEN_R |= AUDIO_OUT;			// enable digital I/O by setting bit high*/
}
 
void initGPIOPortB() {
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;		// enable PORTB CLK
	delay = SYSCTL_RCGCGPIO_R3;

	GPIO_PORTB_DIR_R |= AUDIO_OUT;							// DIR is out
	GPIO_PORTB_DEN_R |= AUDIO_OUT;							// enable digital I/O by setting bit high
	GPIO_PORTB_AMSEL_R &= ~AUDIO_OUT;						// disable analog function
	GPIO_PORTB_ODR_R &= ~AUDIO_OUT;							// set open drain to 0
	GPIO_PORTB_AFSEL_R |= AUDIO_OUT;						// enable alternate function (PWM)
	GPIO_PORTB_PCTL_R &= ~GPIO_PCTL_PB6_M;			// mask off any stray bits
	GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB6_M0PWM0;	// accept timer0 pwm on pin B6
}
 