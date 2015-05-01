
#include <stdint.h>
#include <stdbool.h>

#define UART_BUF_SIZE	16
#define CB_SIZE 	32
 
void configureUART(void);
void configUART0(void);
void configUART3(void);
int uartRX(bool block);
void uartTX(int data);
void UARTSend(uint32_t UARTBase, unsigned char *pucBuffer);
void UARTRead(uint32_t UARTBase, unsigned char *buffer);
void UART3IntHandler(void);

int uart1RX(bool block);
void uart1TX(int data);
void UART1IntHandler(void);
void configUART1(void);
void UART1Send(uint32_t UARTBase, unsigned char *pucBuffer);
void UART1Read(uint32_t UARTBase, char *buffer);
bool checkMsgs(char *msgBuf);

static unsigned char UART3_BUFFER[UART_BUF_SIZE];		// 16 is the uart buffer size
static unsigned char UART1_BUFFER[UART_BUF_SIZE];		// 16 is the uart buffer size
