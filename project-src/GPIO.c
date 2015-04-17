#include "inc/GPIO.h"
#include "inc/defines.h"
 
uint8_t delay;
 
void initGPIOPorts() {
	//initGPIOPortA();
	initGPIOPortB();
	initGPIOPortC();
	initGPIOPortD();
	initGPIOPortE();
	initGPIOPortF();
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

	// Audio Out
	GPIO_PORTB_AFSEL_R 	|=   B_AUDIO_OUTS;	// enable alternate function (PWM)
	GPIO_PORTB_DIR_R		|=   B_AUDIO_OUTS;
	GPIO_PORTB_ODR_R 		&= ~(B_AUDIO_OUTS);	// set open drain to 0
	GPIO_PORTB_DEN_R 		|=   B_AUDIO_OUTS;	// enable digital I/O by setting bit high
	GPIO_PORTB_AMSEL_R 	&= ~(B_AUDIO_OUTS);	// disable analog function
	GPIO_PORTB_PCTL_R 	&= ~(B_PCTL);				// mask off any stray bits
	GPIO_PORTB_PCTL_R 	|= B_PCTL;					// accept pwms
}

void initGPIOPortC() {
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2;		// enable PORTD CLK
	delay = SYSCTL_RCGCGPIO_R2;
		
	//scanning matrix input cols
	GPIO_PORTC_DIR_R 		&= ~C_COLS;	// DIR is IN
	GPIO_PORTC_DEN_R 		|=  C_COLS;	// enable digital I/O
	GPIO_PORTC_ODR_R 		&= ~C_COLS;	// set open drain to 0
	GPIO_PORTC_AMSEL_R 	&= ~C_COLS;	// disable analog function
	GPIO_PORTC_AFSEL_R 	&= ~C_COLS;	// disable alternate functions
}

void initGPIOPortD(){
	//SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;		// enable PORTD CLK
	//delay = SYSCTL_RCGCGPIO_R3;
	
	//unlock PD7
	//GPIO_PORTD_LOCK_R = GPIO_LOCK_KEY;	// unlock GPIO Port D
	//GPIO_PORTD_CR_R |= ROW1;
}


void initGPIOPortE() {
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;		// enable PORTE CLK
	delay = SYSCTL_RCGCGPIO_R4;
	
	//scanning matrix output rows
	GPIO_PORTE_DIR_R 		|=  ROWS;	// DIR is OUT
	GPIO_PORTE_DEN_R 		|=  ROWS;	// enable digital I/O
	GPIO_PORTE_ODR_R 		&= ~ROWS;	// set open drain to 0
	GPIO_PORTE_AMSEL_R 	&= ~ROWS;	// disable analog function
	GPIO_PORTE_AFSEL_R 	&= ~ROWS;	// disable alternate functions
	
	//scanning matrix input cols
	GPIO_PORTE_DIR_R 		&= ~E_COLS;	// DIR is IN
	GPIO_PORTE_DEN_R 		|=  E_COLS;	// enable digital I/O
	GPIO_PORTE_ODR_R 		&= ~E_COLS;	// set open drain to 0
	GPIO_PORTE_AMSEL_R 	&= ~E_COLS;	// disable analog function
	GPIO_PORTE_AFSEL_R 	&= ~E_COLS;	// disable alternate functions
}

void initGPIOPortF() {
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;		// enable PORTF CLK
	delay = SYSCTL_RCGCGPIO_R5;
	
	//unlock PF0 and PF1
	GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;	// unlock GPIO Port F
	GPIO_PORTF_CR_R |= F_AUDIO_OUTS;
	
	// Audio Out
	GPIO_PORTF_AFSEL_R 	|=   F_AUDIO_OUTS;	// enable alternate function (PWM)
	GPIO_PORTF_DIR_R		|=   F_AUDIO_OUTS;
	GPIO_PORTF_ODR_R 		&= ~(F_AUDIO_OUTS);	// set open drain to 0
	GPIO_PORTF_DEN_R 		|=   F_AUDIO_OUTS;	// enable digital I/O By setting Bit high
	GPIO_PORTF_AMSEL_R 	&= ~(F_AUDIO_OUTS);	// disable analog function
	GPIO_PORTF_PCTL_R 	&= ~(F_PCTL);				// mask off any stray Bits
	GPIO_PORTF_PCTL_R 	|= F_PCTL;					// accept pwms
}
