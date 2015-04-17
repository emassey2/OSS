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
	uint8_t octaveMod;
}Note;

void adjustArpeggio(Note* self);
void adjustPitch(Note* self);
void adjustVolume(Note* self, int8_t* refTable);
Note* initNote(bool isNoise, int8_t* waveTableRef, uint8_t octaveMod);
void calculateTuningWord(volatile uint32_t* tuningWord, Note* self);
void resetEffects(Note* self);
void updateEffect(Note* self, Effect* effect);
void updateEffects(Note* self, int8_t* refTable);
void updateKey(Note* self, int8_t keyNumber);
void updateNoiseTWord(uint8_t keyOct, int8_t keyLetter, volatile uint32_t* tuningWord);
void updateTuningWord(Note* self, volatile uint32_t* tuningWord);
