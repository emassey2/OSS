#ifndef __BOARD_CONFIG__
#define __BOARD_CONFIG__

#include <stdint.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include "hw_types.h"

/******************************************************************************
 * Defines
 *****************************************************************************/

#define PIN_0     (1 << 0)
#define PIN_1     (1 << 1)
#define PIN_2     (1 << 2)
#define PIN_3     (1 << 3)
#define PIN_4     (1 << 4)
#define PIN_5     (1 << 5)
#define PIN_6     (1 << 6)
#define PIN_7     (1 << 7)


#define PA0_U0_RX      PIN_0
#define PA1_U0_TX      PIN_1
#define PA2_SPI0_CLK   PIN_2
#define PA3_SPI0_CS    PIN_3
#define PA4_SPI0_MISO  PIN_4
#define PA5_SPI0_MOSI  PIN_5
#define PA6_I2C1_SCL   PIN_6
#define PA7_I2C1_SDA   PIN_7

#define PB0_U1_RX      PIN_0
#define PB1_U1_TX      PIN_1

#define PB0_TRIG_0      PIN_0
#define PB1_PS2_BUTTON  PIN_1
#define PB2_MOTOR_0_SA  PIN_2
#define PB3_MOTOR_0_SB  PIN_3
#define PB4_PS2_YPOS    PIN_4
#define PB5_PS2_XPOS    PIN_5
#define PB6_MOTOR_1_SA  PIN_6
#define PB7_MOTOR_1_SB  PIN_7

#define PC0_TCK       PIN_0
#define PC1_TMS       PIN_1
#define PC2_TDI       PIN_2
#define PC3_TDO       PIN_3

#define PD2_LINE0     PIN_2
#define PD3_LINE1     PIN_3
#define PD6_LINE2     PIN_6
#define PD7_LINE3     PIN_7

#define PE0_RANGE_0_IN PIN_0
#define PE1_ECHO_0    PIN_1
#define PE2_TRIG_1    PIN_2
#define PE3_ECHO_1    PIN_3
#define PE4_U5_RX     PIN_4
#define PE5_U5_TX     PIN_5


#define PF1_MOTOR_0_DIR    PIN_1
#define PF2_MOTOR_0_EN     PIN_2
#define PF3_MOTOR_1_DIR    PIN_3
#define PF4_MOTOR_1_EN     PIN_4


#define DISABLED  0
#define NONE      0
#define PORT_CONTROL_DEFAULT

#endif
