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
void updateEffects(Note* self, int8_t* refTable);
void updateEffect(Note* self, Effect* effect);
void adjustVolume(Note* self, int8_t* refTable);
void adjustArpeggio(Note* self);
void updateNoiseTWord(uint8_t keyOct, int8_t keyLetter, volatile uint32_t* tuningWord);
void calculateTuningWord(volatile uint32_t* tuningWord, int8_t key, uint8_t octave, int8_t arpeggioModifier);
Note* initNote(bool isNoise);
