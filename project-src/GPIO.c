#include "inc/GPIO.h"
#include "inc/defines.h"
#include "inc/UART.h"
 
uint8_t delay;
 
void initGPIOPorts() {
	initGPIOPortA();
	initGPIOPortB();
	initGPIOPortD();
	initGPIOPortE();
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
	delay = SYSCTL_RCGCGPIO_R1;

	// Audio Out pin first
	GPIO_PORTB_DIR_R |= AUDIO_OUT;							// DIR is out
	GPIO_PORTB_DEN_R |= AUDIO_OUT;							// enable digital I/O by setting bit high
	GPIO_PORTB_AMSEL_R &= ~AUDIO_OUT;						// disable analog function
	GPIO_PORTB_ODR_R &= ~AUDIO_OUT;							// set open drain to 0
	GPIO_PORTB_AFSEL_R |= AUDIO_OUT;						// enable alternate function (PWM)
	GPIO_PORTB_PCTL_R &= ~GPIO_PCTL_PB6_M;			// mask off any stray bits
	GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB6_M0PWM0;	// accept timer0 pwm on pin B6
}

void initGPIOPortD(){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;		// enable PORTD CLK
	delay = SYSCTL_RCGCGPIO_R3;
	
	//scanning matrix output rows
	GPIO_PORTD_DIR_R |= ROWS;		// DIR is OUT
	GPIO_PORTD_DEN_R |= ROWS;		// enable digital I/O
	GPIO_PORTB_ODR_R &= ~ROWS;	// set open drain to 0
	GPIO_PORTD_AMSEL_R &= ~ROWS;// disable analog function
	GPIO_PORTD_AFSEL_R &= ~ROWS;// disable alternate functions
	GPIO_PORTD_PCTL_R &= ~ROWS;	// mask off any stray bits
}

void initGPIOPortE() {
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;		// enable PORTE CLK
	delay = SYSCTL_RCGCGPIO_R4;
	
	//scanning matrix input cols
	GPIO_PORTE_DIR_R &= ~COLS;	// DIR is IN
	GPIO_PORTE_DEN_R |= COLS;		// enable digital I/O
	GPIO_PORTB_ODR_R &= ~COLS;	// set open drain to 0
	GPIO_PORTE_AMSEL_R &= ~COLS;// disable analog function
	GPIO_PORTE_AFSEL_R &= ~COLS;// disable alternate functions
	GPIO_PORTE_PCTL_R &= ~COLS;	// mask off any stray bits
}
