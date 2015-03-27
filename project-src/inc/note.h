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
	Effects* effects;
	int8_t* waveTable;
	int8_t* workingWaveTable;
	bool stillPlaying;			// if we need to continue playing after a key is released
	bool isNoise;
}Note;

void updateKey(Note* self, int8_t keyNumber);
void updateTuningWord(Note* self, volatile uint32_t* tuningWord);
void updateWaveTable(Note* self, int8_t* waveTable);
void updateEffects(Note* self, int8_t* refTable);
void adjustVolume(Note* self, int8_t* refTable);
void updateNoiseTWord(uint8_t keyOct, int8_t keyLetter, volatile uint32_t* tuningWord);
void updateVolume(Note* self);
