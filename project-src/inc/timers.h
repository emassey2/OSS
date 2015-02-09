#include "board_config.h"
#include <stdbool.h>


/******************************************************************************
 * Defines
 *****************************************************************************/
#define SYS_TICK_RELOAD_VAL (2550-1)	// 80mhz / 2000 = 126.5us
#define SYS_TICK_RESET_VAL 80
#define TIMER0A_RELOAD_VAL (256-1)	// 80mhz / 256  = 3.2us This is a default val
#define TIMER1A_RELOAD_VAL 10000 / (60)	// number in () is the freq in hz 
#define TIMER1A_PRESCALE 8000
#define bit0 (1 << 0)
#define bit1 (1 << 1)
#define bit15M ~(1 << 15)


/******************************************************************************
 * Function Prototypes
 *****************************************************************************/
void initTimers(void);
void initSysTick(void);
void initTimer0PWM(void);
void initTimer1Noise(void);

void SYSTICKIntHandler(void);
void TIMER1IntHandler(void);
