#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

/******************************************************************************
 * Defines
 *****************************************************************************/

#ifdef AHB
#define PORTA  0x40058000
#define PORTB  0x40059000
#define PORTC  0x4005A000
#define PORTD  0x4005B000
#define PORTE  0x4005C000
#define PORTF  0x4005D000
#error
#else
#define PORTA  0x40004000
#define PORTB  0x40005000
#define PORTC  0x40006000
#define PORTD  0x40007000
#define PORTE  0x40024000
#define PORTF  0x40025000
#endif


#define UART0_RX  PIN_0
#define UART0_TX  PIN_1


#endif


/******************************************************************************
 * Function Prototypes
 *****************************************************************************/
 void initGPIOPorts(void);
 void initGPIOPortA(void);
 void initGPIOPortB(void);
