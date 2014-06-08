/************************************************************************/
/* FILE NAME    - UART.h												*/
/* AUTHOR       - ECE353 Staff                                          */
/* DATE CREATED - 03-Jun-2013                                           */
/* DESCRIPTION  - Header file for UART.c 								*/
/* 														                */
/* (c) ECE Department, University of Wisconsin - Madison                */
/************************************************************************/
#ifndef __UART_H__
#define __UART_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <errno.h>
#include "board_config.h"


#define	UART0 0x4000C000
#define UART1 0x4000D000
#define UART2 0x4000E000
#define UART3 0x4000F000
#define UART4 0x40010000
#define UART5 0x40011000
#define UART6 0x40012000
#define UART7 0x40013000

#define UART0_IRQ_NUM   5
#define UART1_IRQ_NUM   6
#define UART2_IRQ_NUM   33
#define UART3_IRQ_NUM   59
#define UART4_IRQ_NUM   60
#define UART5_IRQ_NUM   61
#define UART6_IRQ_NUM   62
#define UART7_IRQ_NUM   63

#define	_8BITS		0x3	
#define	ENABLED		1
#define	DISABLED	0

/*************************************************************************
 * RX FIFO LENGTHS (RxIntFIFO)
 ************************************************************************/
#define	RX_ONE_EIGHTH  	0x0
#define	RX_ONE_FOURTH	0x1
#define	RX_ONE_HALF		0x2
#define	RX_THREE_FOURTH	0x3
#define	RX_SEVEN_EIGHTH	0x4

/*************************************************************************
 * TX FIFO LENGTHS (TxIntFIFO)
 ************************************************************************/
#define	TX_ONE_EIGHTH  	0x4
#define	TX_ONE_FOURTH	0x3
#define	TX_ONE_HALF		0x2
#define	TX_THREE_FOURTH	0x1
#define	TX_SEVEN_EIGHTH	0x0

/*************************************************************************
 * Function Prototypes
 ************************************************************************/
void initPollingUART0(void);
char UART0_RxPoll(void);
void UART0_TxPoll(char *data);

/*************************************************************************
 ************************************************************************/
#define EN_TX_FIFO		1
 
#endif
