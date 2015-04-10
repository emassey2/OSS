#include "board_config.h"
#include "defines.h"
#include <stdbool.h>

/******************************************************************************
 * Defines
 *****************************************************************************/
typedef struct Key {
	int8_t letter;	// ex C is 0 and 3 is D#/Eb see defines for more
	uint8_t octave;
}Key;


/******************************************************************************
 * Function Prototypes
 *****************************************************************************/

/******************************************************************************
 * getkey
 *
 * Description:
 *	Finds the appropriate letter and octave depending on the current state of
 * 	the scanning matrix. In the event of more than one key press only last key
 *	is counted currently;
 *
 *  Params: key - the key we wish to modify
 *  				keyNumber - number of the key on the keyboard that is pressed
 */
Key* getKey(Key* self, int8_t keyNumber);
Key* initKey(void);
