


#include <stdio.h>
#include <stdlib.h>
#include "inc/qei.h"
#include "inc/tm4c123gh6pm.h"


void initQei1(void){
	
	uint32_t delay;

 SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2; // enable clk for port c
	
	// PC5, PC6
	SYSCTL_RCGC1_R |= SYSCTL_RCGC1_QEI1;  // enable clk for qei
	//delay = SYSCTL_RCGC1_R;
	

	//delay = SYSCTL_RCGCGPIO_R;
	
	//GPIO_PORTC_DIR_R  |= 0x01100000; // output
	
	//GPIO_PORTC_DEN_R |= 0x01100000;
	
	GPIO_PORTC_AFSEL_R &= 0x10011111; // no alternate func
	
	GPIO_PORTC_PDR_R |= 0x01100000;  // pulldown 
	
	// PC5 = PhA1, PCTL 6; PC6 = PhB1, PCTL 6
	GPIO_PORTC_PCTL_R |=  (GPIO_PCTL_PC5_PHA1 | GPIO_PCTL_PC6_PHB1);
	
	QEI1_CTL_R |= 0x00000018;
	QEI1_MAXPOS_R |= 0x00000F9F;
	
	QEI1_CTL_R |= QEI_CTL_ENABLE; // enable the quadrature encoder by setting bit 0
	
}


uint32_t getPos(void){
	uint32_t pos;
	pos = QEI1_POS_R;
	
	return pos;
	
}

