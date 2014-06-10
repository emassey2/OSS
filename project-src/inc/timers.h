#include "board_config.h"


/******************************************************************************
 * Defines
 *****************************************************************************/
#define SYS_TICK_RELOAD_VAL 10121-1	// 80mhz / 2000 = 126.5us
#define TIMER0A_RELOAD_VAL 256-1	// 80mhz / 256  = 3.2us This is a default val


/******************************************************************************
 * Function Prototypes
 *****************************************************************************/
void initTimers(void);
void initSysTick(void);
void initTimer0PWM(void);

void SYSTICKIntHandler(void);
