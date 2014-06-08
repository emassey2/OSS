#ifndef __GPIO_H__
#define __GPIO_H__

#include <stdint.h>
#include <stdbool.h>
#include "lm4f120h5qr.h"

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

#define PIN_0     (1 << 0)
#define PIN_1     (1 << 1)
#define PIN_2     (1 << 2)
#define PIN_3     (1 << 3)
#define PIN_4     (1 << 4)
#define PIN_5     (1 << 5)
#define PIN_6     (1 << 6)
#define PIN_7     (1 << 7)

#define SW2       PIN_6
#define SW3       PIN_7
#define SW4       PIN_2
#define SW5       PIN_3
#define SW6       PIN_0
#define SW7       PIN_1

#define UART0_RX  PIN_0
#define UART0_TX  PIN_1

#define SPI0_CLK   PIN_2
#define SPI0_CS    PIN_3
#define SPI0_MISO  PIN_4
#define SPI0_MOSI  PIN_5

#define DATA0     PIN_0
#define DATA1     PIN_1
#define DATA2     PIN_2
#define DATA3     PIN_3
#define DATA4     PIN_4
#define DATA5     PIN_5
#define DATA6     PIN_6
#define DATA7     PIN_7

#define RED_EN    PIN_4
#define GRN_EN    PIN_5
#define BLU_EN    PIN_6
#define OUT_EN_B  PIN_4

#define ADC_0     PIN_3
#define ADC_1     PIN_2


#define NONE      0
#define PORT_CONTROL_DEFAULT  0

#endif
