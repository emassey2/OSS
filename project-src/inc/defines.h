
#define PIN_0     (1 << 0)
#define PIN_1     (1 << 1)
#define PIN_2     (1 << 2)
#define PIN_3     (1 << 3)
#define PIN_4     (1 << 4)
#define PIN_5     (1 << 5)
#define PIN_6     (1 << 6)
#define PIN_7     (1 << 7)

#define CLOCK_RATE 80000000

#define AUDIO_OUT PIN_6

// Below we define GPIO for scanning matrix 
// PD[0:3] & PD[6] are rows and will be the outputs
// PE[0:4] are cols and will be the inputs
// the lower pins match the earlier cols/rows ie PE0 is col 1
#define ROW1 PIN_0
#define ROW2 PIN_1
#define ROW3 PIN_2
#define ROW4 PIN_3
#define ROW5 PIN_6
#define ROWS ROW1 | ROW2 | ROW3 | ROW4 | ROW5

#define COL1 PIN_0
#define COL2 PIN_1
#define COL3 PIN_2
#define COL4 PIN_3
#define COL5 PIN_4
#define COLS COL1 | COL2 | COL3 | COL4 | COL5

#define NUM_COLS 5
#define NUM_ROWS 5
