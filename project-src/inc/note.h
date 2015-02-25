#include "board_config.h"
#include "key.h"
#include "waves.h"
#include "effects.h"
#include "defines.h"

/******************************************************************************
 * Defines
 *****************************************************************************/
typedef struct Note {
	Key* key;
	Effects effects;
	uint8_t* waveTable;
	bool stillPlaying;
}Note;

void updateKey(Note* self, int8_t keyNumber);
void updateTuningWord(Note* self, volatile uint32_t* tuningWord);
void updateWaveTable(Note* self, uint8_t* waveTable);
void updateEffects(Note* self, uint8_t* refTable);
void adjustVolume(Note* self, uint8_t* refTable);
