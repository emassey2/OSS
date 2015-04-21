
#define PIN_0     (1 << 0)
#define PIN_1     (1 << 1)
#define PIN_2     (1 << 2)
#define PIN_3     (1 << 3)
#define PIN_4     (1 << 4)
#define PIN_5     (1 << 5)
#define PIN_6     (1 << 6)
#define PIN_7     (1 << 7)

#define CLOCK_RATE 80000000

#define AUDIO_OUT_0 PIN_6		//PB6
#define AUDIO_OUT_1 PIN_7		//PB7
#define AUDIO_OUT_2 PIN_4		//PB4
#define AUDIO_OUT_3 PIN_5		//PB5
#define AUDIO_OUT_4 PIN_0		//Pf0	LOCKED!!!!!
#define AUDIO_OUT_5 PIN_1		//Pf1
#define B_AUDIO_OUTS AUDIO_OUT_0 | AUDIO_OUT_1 | AUDIO_OUT_2 | AUDIO_OUT_3
#define F_AUDIO_OUTS AUDIO_OUT_4 | AUDIO_OUT_5

#define B_PCTL GPIO_PCTL_PB4_M0PWM2 | GPIO_PCTL_PB5_M0PWM3 | GPIO_PCTL_PB6_M0PWM0 | GPIO_PCTL_PB7_M0PWM1
#define F_PCTL GPIO_PCTL_PF0_M1PWM4 | GPIO_PCTL_PF1_M1PWM5

#define TUNING_CONST 32104.9
#define NO_SOUND 0
#define MAX_FREQ (TUNING_CONST * noteFreq[NUM_OCTAVES-1][KEYS_PER_OCT-1])

// Below we define GPIO for scanning matrix 
// PE[1:5] are rows and will be the outputs from the uC
// PE0 and PC[4:7] are cols and will be the inputs to the uC
// the lower pins match the earlier cols/rows ie PE0 is col 1
#define ROW1 PIN_1	//PE1
#define ROW2 PIN_2	//PE2
#define ROW3 PIN_3	//PE3
#define ROW4 PIN_4	//PE4
#define ROW5 PIN_5	//PE5
#define ROWS ROW1 | ROW2 | ROW3 | ROW4 | ROW5


#define COL1 PIN_4	//PC4
#define COL2 PIN_5	//PC5
#define COL3 PIN_6	//PC6
#define COL4 PIN_7	//PC7
#define COL5 PIN_0	//PE0
#define COLS COL1 | COL2 | COL3 | COL4 | COL5
#define C_COLS COL1 | COL2 | COL3 | COL4
#define E_COLS COL5

#define NUM_COLS 5
#define NUM_ROWS 5
#define MIN_OCTAVE 0
#define NUM_OCTAVES 9
#define KEYS_PER_OCT 12
#define NOISE_FREQS 16

#define NO_NOTE -1
#define C  0
#define CS 1
#define D  2
#define DS 3
#define E  4
#define F  5
#define FS 6
#define G  7
#define GS 8
#define A  9
#define AS 10
#define B  11
#define BS 12
