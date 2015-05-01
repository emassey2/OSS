#include "inc/uart_nev.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>


#include "driverlib/fpu.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#define PART_TM4C123GH6PM
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
//#include "grlib/grlib.h"
#include "inc/inc_lib/hw_gpio.h"
#include "inc/inc_lib/hw_ints.h"
#include "inc/inc_lib/hw_types.h"
#include "inc/inc_lib/hw_memmap.h"
#include "inc/inc_lib/tm4c123gh6pm.h"
#include "utils/uartstdio.h"
#include "inc/circular_buffer.h"
#include "inc/lcd_defines.h"

#define GPIO_PA0_U0RX           0x00000001
#define GPIO_PA1_U0TX           0x00000401
#define GPIO_PC7_U3TX           0x00021C01
#define GPIO_PC6_U3RX           0x00021801
#define GPIO_PB1_U1TX           0x00010401
#define GPIO_PB0_U1RX           0x00010001

//volatile bool alertUART3Data = false;
//volatile bool alertUART1Data = false;
extern CircularBuffer RXbuf;
extern CircularBuffer TXbuf;
//char myMsgBuffer[CB_SIZE];


void configureUART(void) {
    //configUART0();
    configUART1();
}

//*************************** Do NOT USE THIS ONE ***************************
//void configUART0() {
//    // Enable the GPIO Peripheral used by the UART.
//  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

//	// Enable UART0_BASE
//	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);

//	// Configure GPIO Pins for UART mode.
//	GPIOPinConfigure(GPIO_PA0_U0RX);
//	GPIOPinConfigure(GPIO_PA1_U0TX);
//	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

//	// Initialize the UART for console I/O.
//	UARTStdioConfig(0, 115200, SysCtlClockGet());
//}

////////////////////////////////////////////////////////////////////////////////

void configUART1() {
	bool bufferInit = true;
	uint64_t  clk = 80000000;
	
	
    // Enable GPIO for UART1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    // Enable UART1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);

    // Configure GPIO Pins for UART mode.
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    // configure UART baud, data bits, parity, stop bits, and flow control
    UARTConfigSetExpClk(UART1_BASE, clk, 115200, (UART_CONFIG_PAR_NONE | UART_CONFIG_STOP_ONE | UART_CONFIG_WLEN_8));
    UARTFlowControlSet(UART1_BASE, UART_FLOWCONTROL_NONE);

	// init our buffers
	if (cBufInit(&TXbuf, CB_SIZE) < 0) {
		bufferInit = false;
	}

	if (cBufInit(&RXbuf, CB_SIZE) < 0) {
		bufferInit = false;
	}

	if (!bufferInit) {
		UARTprintf("ALERT! BUFFER INIT FAIL! SPINNING!");
		while(1);
	}

    // Enable interrupts from UART1
    IntEnable(INT_UART1);
    UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT | UART_INT_TX);
}


///****************************************************************************
// * This function returns a single character from the RX circular buffer.
// * It takes one argument which indicates if the function will wait until
// * data is found.
// ****************************************************************************/


///////////////////////////////////////////// UART 1 Code////////////////////////////////////////




//void uart1TX(int data) {
//	if ((UART1_FR_R & UART_FR_TXFF) != UART_FR_TXFF) {
//		//if the fifo isn't full write to it
//		UART1_DR_R = (data & 0x00FF);
//	} else {
//		while (0 == cBufGetFreeCount(&TXbuf)); // busy wait if buffer is full
//		//if the fifo is full add to the circular one
//		cBufAddChar(&TXbuf, (data & 0x00FF));
//	}
//}

//void UART1Send(uint32_t UARTBase, unsigned char *buffer) {
//	uint32_t i = 0;
//    // Loop while there are more characters to send.
//    while(buffer[i] != '\0') {
//    	if (UARTBase == UART1_BASE) {
//    		// write character to our CB
//    		uartTX((int)buffer[i++]);
//    	} else {
//			// Write the next character to the UART.
//			UARTCharPutNonBlocking(UARTBase, buffer[i++]);
//    	}
//    }
//}




