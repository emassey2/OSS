

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "inc/UART.h"
#include "inc/circular_buffer.h"
#include "inc/lcd_defines.h"
#include "inc/uart_nev.h"
#include "utils/uartstdio.h"
#include "driverlib/uart.h"

#define UART1_BASE                      0x4000D000

volatile bool alertUART3Data = false;
volatile bool alertUART1Data = false;
CircularBuffer RXbuf;
CircularBuffer TXbuf;
char myMsgBuffer[CB_SIZE];

void initPollingUART0(){
	uint32_t delay;
  
  // *******************************************
  // Configure GPIO PA0 and PA1 to be UART Pins
  // *******************************************
  
  // Turn on the clock gating register for GPIO port A
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;
  delay = SYSCTL_RCGCGPIO_R;
	
	// Now for GPIO port E
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;
  delay = SYSCTL_RCGCGPIO_R;
  
	// Set the Direction 0 is IN 1 is OUT
	GPIO_PORTA_DIR_R &= ~PA0_U0_RX;
	
	GPIO_PORTA_DIR_R |= PA1_U0_TX;

	
  // Set the Digital Enable
  GPIO_PORTA_DEN_R |= PA0_U0_RX | PA1_U0_TX;
  
  // Set the Alternate Function
  GPIO_PORTA_AFSEL_R |= PA0_U0_RX | PA1_U0_TX;
	
  // Set the Port Control Register 
  GPIO_PORTA_PCTL_R |= GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX;
	
  
  // *******************************
  // Set up the UART registers
  // *******************************
  
  // Enable the clock gating register
  SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
  delay = SYSCTL_RCGCUART_R;
	
	// disable uart
	UART0_CTL_R &= ~UART_CTL_UARTEN;
  
  // Set the baud rate to 9600
  UART0_IBRD_R = 520;
  UART0_FBRD_R = 53;

  // Configure the Line Control for 8-n-1
  UART0_LCRH_R |= UART_LCRH_WLEN_8 | UART_LCRH_FEN;
  
  // Enable the UART - Need to enabel both TX and RX
  UART0_CTL_R |= UART_CTL_TXE | UART_CTL_RXE | UART_CTL_UARTEN;
  
  // Wait until the UART is avaiable
  while( !(SYSCTL_PRUART_R & SYSCTL_PRUART_R0 ));
  
  delay = 500;
  while( delay != 0) {
    delay--;
  }
}

char UART0_RxPoll() {
/****************************************************************************
 * This routine returns a character received from the UART/COM port.
 * This routine blocks until a character is received
 ****************************************************************************/
  while ( ((UART0_FR_R) & (UART_FR_RXFE)) != 0) {
		// wait
	}
	return UART0_DR_R & 0x00FF;;
}

void UART0_TxPoll(char *data) {
/****************************************************************************
 * This routine transmits a character out to the UART / COM port.
 * Only the lower 8 bits of the 'data' variable are transmitted.
 ****************************************************************************/
  
  if ( data != 0)
  {
    while(*data != '\0') {
      while( ((UART0_FR_R) & (UART_FR_TXFF)) != 0 );
      UART0_DR_R = *data & 0x00FF;;
      data++;
    }
  }
  return;
}

///////////////////////////////////////////////// UART1 /////////////////////////////////////////////////////////////////////
void initPollingUART1(){
	
	bool bufferInit = true;
	uint32_t delay;
  
  // *******************************************
  // Configure GPIO PA0 and PA1 to be UART Pins
  // *******************************************
  
  // Turn on the clock gating register for GPIO port B
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R1;
  delay = SYSCTL_RCGCGPIO_R;
	
	// Now for GPIO port E
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4;
  delay = SYSCTL_RCGCGPIO_R;
  
	// Set the Direction 0 is IN 1 is OUT
	GPIO_PORTB_DIR_R &= ~PB0_U1_RX;
	
	GPIO_PORTB_DIR_R |= PB1_U1_TX;

	
  // Set the Digital Enable
  GPIO_PORTB_DEN_R |= PB0_U1_RX | PB1_U1_TX;
  
  // Set the Alternate Function
  GPIO_PORTB_AFSEL_R |= PB0_U1_RX | PB1_U1_TX;
	
  // Set the Port Control Register 
  GPIO_PORTB_PCTL_R |= GPIO_PCTL_PB1_U1TX | GPIO_PCTL_PB0_U1RX;
	
  
  // *******************************
  // Set up the UART registers
  // *******************************
  
  // Enable the clock gating register
  SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R1;
  delay = SYSCTL_RCGCUART_R;
	
	// disable uart
	UART1_CTL_R &= ~UART_CTL_UARTEN;
  
  // Set the baud rate to 115200
	//BRD = 43.4028 
  UART1_IBRD_R = 43;
  UART1_FBRD_R = 26;

  // Configure the Line Control for 8-n-1
  UART1_LCRH_R |= UART_LCRH_WLEN_8 | UART_LCRH_FEN;
  
	UARTIntEnable(UART1_BASE, UART_INT_RX | UART_INT_RT | UART_INT_TX);
  // Enable the UART - Need to enable both TX and RX
  UART1_CTL_R |= UART_CTL_TXE | UART_CTL_RXE | UART_CTL_UARTEN;
  
  // Wait until the UART is avaiable
  while( !(SYSCTL_PRUART_R & SYSCTL_PRUART_R1 ));
  
  delay = 500;
  while( delay != 0) {
    delay--;
  }
	
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
	
	
}

char UART1_RxPoll() {
/****************************************************************************
 * This routine returns a character received from the UART/COM port.
 * This routine blocks until a character is received
 ****************************************************************************/
  while ( ((UART1_FR_R) & (UART_FR_RXFE)) != 0) {
		// wait
	}
	return UART1_DR_R & 0x00FF;;
}

void UART1_TxPoll(char *data) {
/****************************************************************************
 * This routine transmits a character out to the UART / COM port.
 * Only the lower 8 bits of the 'data' variable are transmitted.
 ****************************************************************************/
  
  if ( data != 0)
  {
    while(*data != '\0') {
      while( ((UART1_FR_R) & (UART_FR_TXFF)) != 0 );
      UART1_DR_R = *data & 0x00FF;;
      data++;
    }
  }
  return;
}




int uart1RX(bool block) {
	char retChar1, retChar2, retChar3, retChar4, retChar5, retChar6;
	int retVal1, retVal2, retVal3, retVal4, retVal5, retVal6;

	// if circular buffer is NOT empty we return oldest char
	if (cBufGetFreeCount(&RXbuf) != CB_SIZE) {
		cBufGetChar(&RXbuf, &retChar1);
		
		return (int)retChar1;

		// it IS empty so check if block is true
	} else if (block && cBufGetFreeCount(&RXbuf) == CB_SIZE){
		//block is true
		while(cBufGetFreeCount(&RXbuf) == CB_SIZE) {
			// we busy wait until we can get a char
		}
		retVal1 = cBufGetChar(&RXbuf, &retChar1);
		retVal2 =	cBufGetChar(&RXbuf, &retChar2);
	  retVal3 = cBufGetChar(&RXbuf, &retChar3);
		retVal4 = cBufGetChar(&RXbuf, &retChar4);
		retVal5 = cBufGetChar(&RXbuf, &retChar5);
		retVal6 = cBufGetChar(&RXbuf, &retChar6);
		
		//retChar3 = BASIC_SQUARE;
		
 		//handleLCDinput(retChar3, retVal6);
		
		if (retVal1 == 0) {
			return (int)retChar3;
		} else {
			return -1;
		}

	} else if (!block && cBufGetFreeCount(&RXbuf) == CB_SIZE) {
		// if block is false we return -1
		return -1;
	}

	//cBufGetChar(&RXbuf, &retChar);
	return -1;//(int)retChar;
}

// note - return buffer must at least CB_SIZE large
bool checkMsgs(char *msgBuf) {
	bool msgReceived = false;
	uint8_t i = 0;

	memset(msgBuf, '\0', CB_SIZE);	// clear out the array
	UART1Read(UART1_BASE, msgBuf);	// read into our buffer

	// loop to see if we got any msgs
	while (i < CB_SIZE && !msgReceived) {
		if (msgBuf[i++] != NULL) {
			msgReceived = true;
		}
	}

	return msgReceived;
}


void UART1Read(uint32_t UARTBase, char *buffer) {
	uint32_t index = 0;
	uint32_t bufferSize = (char) sizeof(buffer);
	int retChar;
	memset(buffer, '\0', bufferSize);


	if (UARTBase == UART1_BASE && index < UART_BUF_SIZE) {
		// get characters from our CB
		retChar = uart1RX(false);	// blocking
		while (retChar != -1) {
			buffer[index++] = retChar;
			retChar = uart1RX(false);
		}
	} else {
		while(UARTCharsAvail(UARTBase) && index < UART_BUF_SIZE) {
		// Read the next character and put it into the buffer
		buffer[index++] = UARTCharGetNonBlocking(UARTBase);
		}
	}
}


void UART1IntHandler() {
	uint32_t status;
	char retChar;

	status = UARTIntStatus(UART1_BASE, true);	// Save status of MCU before continuing

	// determine the nature of this intrusion!
	if (status & (UART_INT_RX | UART_INT_RT)) {
		//if data recieved then we take it from RX FIFO and place it into RX CB
		while (!(UART1_FR_R & UART_FR_RXFE)) {
			cBufAddChar(&RXbuf, (UART1_DR_R & 0x00FF));
		}
		//clear the interrupt
		UARTIntClear(UART1_BASE, UART_INT_RX | UART_INT_RT);

	} else if (status & UART_INT_TX) {
		//if TX FIFO empty we take data from TX CB and place it into TX FIFO
		while (!(UART0_FR_R & UART_FR_TXFF) && cBufGetFreeCount(&TXbuf) != CB_SIZE) {
			cBufGetChar(&TXbuf, &retChar);
			UART1_DR_R = (uint32_t)(retChar & 0x00FF);
		}
		//clear the interrupt
		UARTIntClear(UART1_BASE, UART_INT_TX);
	}
	alertUART1Data = true;
}



