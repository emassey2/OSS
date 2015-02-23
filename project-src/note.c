#include "inc/note.h"

void updateKey(Note* self, int8_t keyNumber) {
	getKey(self->key, keyNumber);
}

void updateTuningWord(Note* self, volatile uint32_t* tuningWord) {
	static int8_t lastKeyLetter = NO_NOTE;
	static int8_t keyLetter = NO_NOTE;
	static uint8_t keyOct = 0;
	static uint8_t lastKeyOct = 0;
	
	lastKeyOct = keyOct;
	keyOct = self->key->octave;
	
	lastKeyLetter = keyLetter;
	keyLetter = self->key->letter;
	
	if (keyLetter == NO_NOTE) {
		*tuningWord = NO_SOUND;
	} else if (keyLetter != lastKeyLetter || keyOct != lastKeyOct) {
		*tuningWord = TUNING_CONST * noteFreq[self->key->octave + 2][self->key->letter];
	}
}
