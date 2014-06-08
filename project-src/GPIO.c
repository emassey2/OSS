 #include "inc/GPIO.h"
 #include "inc/defines.h"
 
 uint8_t delay;
 
 void initGPIOPorts() {
	 initGPIOPortA();
 }
 
 void initGPIOPortA() {
	// Enable our audio ouput first
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;		// enable PORTA CLK
	delay = SYSCTL_RCGCGPIO_R0;
	 
	GPIO_PORTA_DIR_R |= AUDIO_OUT;			// DIR is out
	GPIO_PORTA_DEN_R |= AUDIO_OUT;			// enable digital I/O by setting bit high
 }
 