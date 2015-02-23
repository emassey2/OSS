#include "board_config.h"
#include "key.h"
#include "waves.h"
#include "defines.h"

/******************************************************************************
 * Defines
 *****************************************************************************/
typedef struct Note {
	Key* key;
	uint8_t* waveTable_ref;
}Note;

void updateKey(Note* self, int8_t keyNumber);

void updateTuningWord(Note* self, volatile uint32_t* tuningWord);
