#include "inc/GPIO.h"
#include "inc/defines.h"
 
uint8_t delay;
 
void initGPIOPorts() {
	//initGPIOPortA();
	initGPIOPortD();
	//GPIO_PORTD_DATA_R &= ~COL2;
	initGPIOPortB();
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

	// Audio Out 0 and 1
	GPIO_PORTB_AFSEL_R 	|=   AUDIO_OUT_0 | AUDIO_OUT_1;										// enable alternate function (PWM)
	GPIO_PORTB_DIR_R		|=   AUDIO_OUT_0 | AUDIO_OUT_1;
	GPIO_PORTB_ODR_R 		&= ~(AUDIO_OUT_0 | AUDIO_OUT_1);									// set open drain to 0
	GPIO_PORTB_DEN_R 		|=   AUDIO_OUT_0 | AUDIO_OUT_1;										// enable digital I/O by setting bit high
	GPIO_PORTB_AMSEL_R 	&= ~(AUDIO_OUT_0 | AUDIO_OUT_1);									// disable analog function
	GPIO_PORTB_PCTL_R 	&= ~(GPIO_PCTL_PB6_M | GPIO_PCTL_PB7_M);					// mask off any stray bits
	GPIO_PORTB_PCTL_R 	|=   GPIO_PCTL_PB6_M0PWM0 | GPIO_PCTL_PB7_M0PWM1;	// accept pwm0_0 on pin B6 & pwm0_1 on B7
}

void initGPIOPortD(){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;		// enable PORTD CLK
	delay = SYSCTL_RCGCGPIO_R3;
	
	//unlock PD7
	GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;	// unlock GPIO Port D
	GPIO_PORTD_CR_R |= ROW1;
	
	//scanning matrix output rows
	GPIO_PORTD_DIR_R |= ROWS;		// DIR is OUT
	GPIO_PORTD_DEN_R |= ROWS;		// enable digital I/O
	GPIO_PORTD_ODR_R &= ~ROWS;	// set open drain to 0
	GPIO_PORTD_AMSEL_R &= ~ROWS;// disable analog function
	GPIO_PORTD_AFSEL_R &= ~ROWS;// disable alternate functions
}

void initGPIOPortE() {
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;		// enable PORTE CLK
	delay = SYSCTL_RCGCGPIO_R4;
	
	//scanning matrix input cols
	GPIO_PORTE_DIR_R &= ~COLS;	// DIR is IN
	GPIO_PORTE_DEN_R |= COLS;		// enable digital I/O
	GPIO_PORTE_ODR_R &= ~COLS;	// set open drain to 0
	GPIO_PORTE_AMSEL_R &= ~COLS;// disable analog function
	GPIO_PORTE_AFSEL_R &= ~COLS;// disable alternate functions
}
