#include "inc/note.h"

void updateKey(Note* self, int8_t keyNumber) {
	static int8_t lastKeyNumber = NO_NOTE;
	
	// check to see if the keys have changed
	if (lastKeyNumber != keyNumber) {
		if (lastKeyNumber != NO_NOTE && keyNumber != NO_NOTE) {
			// new key is pressed while another key was being held
			self->effects.volumeCounter = self->effects.volumeLoopPos;
			self->stillPlaying = true;
		} else if (keyNumber != NO_NOTE) {
			// no keys being pressed to one is pressed
			self->effects.volumeCounter = 0;
			self->stillPlaying = true;
		} else {
			// key was pressed and now no keys are being pressed
		}
	}
	lastKeyNumber = keyNumber;
	getKey(self->key, keyNumber);
}

void updateTuningWord(Note* self, volatile uint32_t* tuningWord) {
	static int8_t lastKeyLetter = NO_NOTE;
	static int8_t keyLetter = NO_NOTE;
	static uint8_t keyOct = 0;
	static uint8_t lastKeyOct = 0;

	keyOct = self->key->octave;
	keyLetter = self->key->letter;
	
	// check if we need to account for effects
	if (self->effects.enabled) {
		
		// check to make sure we still need to play a note
		if (!self->stillPlaying) {		
				// check if this is a noise channel as there are some special cases here
				if (self->isNoise) {
					*tuningWord = NO_NOTE;	
				} else {
					*tuningWord = NO_SOUND;		
				}
			
			// key isn't held but we need to continue the note. Reassign values and wait for stillPlaying to be false
		} else if(keyLetter == NO_NOTE) { // loop & release stuff
				keyOct = lastKeyOct;
				keyLetter = lastKeyLetter;	
				
				// check if this is a noise channel as there are some special cases here
				if (self->isNoise) {
					updateNoiseTWord(keyOct, keyLetter, tuningWord);
				} else {
					*tuningWord = TUNING_CONST * noteFreq[lastKeyOct + 4][lastKeyLetter];
				}
		
			// a new key has been pressed	
		} else if (keyLetter != lastKeyLetter || keyOct != lastKeyOct) {	
			// check if this is a noise channel as there are some special cases here
				if (self->isNoise) {
					updateNoiseTWord(keyOct, keyLetter, tuningWord);
				} else {
					*tuningWord = TUNING_CONST * noteFreq[self->key->octave + 4][self->key->letter];
				}
		}
		
	//effects can be ignored
	} else {
		if (keyLetter == NO_NOTE) {
			*tuningWord = NO_SOUND;
		} else {	
			// check if this is a noise channel as there are some special cases here
			if (self->isNoise) {
				updateNoiseTWord(keyOct, keyLetter, tuningWord);
			} else {
					*tuningWord = TUNING_CONST * noteFreq[self->key->octave + 4][self->key->letter];
			}
		}
	}

	
	lastKeyOct = keyOct;	
	lastKeyLetter = keyLetter;
}

void updateWaveTable(Note* self, uint8_t* waveTable) {
	waveTable = self->waveTable;
}

void updateEffects(Note* self, uint8_t* refTable) {
	int16_t volumeScaler;
	
	if (self->effects.volumeEnabled && self->stillPlaying) {
		volumeScaler = (int16_t)self->effects.volume[self->effects.volumeCounter];
		
		switch(volumeScaler) {
			case LOOP:	
				// we see the loop marker so we save it and increment the counter
				self->effects.volumeLoopPos = self->effects.volumeCounter;
				self->effects.volumeCounter = self->effects.volumeCounter + 1;
				break;
			
			case RELEASE_PT: 
				if (self->key->letter != NO_NOTE) {
					// if note is pressed still we mark release location and go back to the start of loop
					self->effects.volumeReleasePos = self->effects.volumeCounter;
					self->effects.volumeCounter = self->effects.volumeLoopPos;
				} else {
					// if the key is no longer pressed go forward with the release
					self->effects.volumeCounter = self->effects.volumeCounter + 1;
				}
				break;
			
			case END:
				// in this case we disable the note and reset the counter
				self->stillPlaying = false;
				self->effects.volumeCounter = 0;
				break;
			
			default:
				// make sure we still holding the key
				if (self->key->letter != NO_NOTE) {
					// update our waveTable and increment the counter by one
					adjustVolume(self, refTable);
					self->effects.volumeCounter = self->effects.volumeCounter + 1;
				} else if (self->effects.volumeCounter > self->effects.volumeReleasePos) {
					// key isn't pressed and we are passed the release point so just keep going
					adjustVolume(self, refTable);
					self->effects.volumeCounter = self->effects.volumeCounter + 1;
				} else {
					// key isn't pressed and we need to skip to release point
					self->effects.volumeCounter = self->effects.volumeReleasePos;
				}
				break;
		}
	}
	
	if (self->effects.arpeggioEnabled) {
		// do nothing right now. Maybe Nev will fix this?
	}
}

void adjustVolume(Note* self, uint8_t* refTable) {
	uint16_t i;
	float volumeScaler;
	volumeScaler = self->effects.volume[self->effects.volumeCounter];
	
	for (i = 0; i < EFFECT_SIZE; i++) {
		self->waveTable[i] = (uint8_t)(volumeScaler * refTable[i]);
	}
}

void updateNoiseTWord(uint8_t keyOct, int8_t keyLetter, volatile uint32_t* tuningWord) {
	// check to see if any key is pressed
	if (keyLetter != NO_NOTE) {
		// tuning word is repurposed in the case where we want to generate random noise
		*tuningWord = ((keyLetter * (keyOct+1)) % NOISE_FREQS);
	} else {
		*tuningWord = NO_NOTE;
	}
}
