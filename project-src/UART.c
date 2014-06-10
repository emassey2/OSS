#include "inc/UART.h"

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
