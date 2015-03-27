#include "inc/note.h"

void updateKey(Note* self, int8_t keyNumber) {
	static int8_t lastKeyNumber = NO_NOTE;
	
	// check to see if the keys have changed
	if (lastKeyNumber != keyNumber) {
		if (lastKeyNumber != NO_NOTE && keyNumber != NO_NOTE) {
			// new key is pressed while another key was being held
			//self->effects->volumeCur = findMarker(self->effects->volumeList, 'L');
			self->stillPlaying = true;
			self->effects->released = false;
		} else if (keyNumber != NO_NOTE) {
			// no keys being pressed to one key being pressed
			self->effects->volumeCur = self->effects->volumeList->head;
			self->stillPlaying = true;
			self->effects->released = false;
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
	if (self->effects->enabled) {
		
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

void updateWaveTable(Note* self, int8_t* waveTable) {
	waveTable = self->waveTable;
}

/* Basic loop structure for effects
 * Start when key is pressed
 * While key is held 
 * 	Loop to L when R is reached 
 * Go to R when key is released
 * Stop playing when E is reached
 *       --------
 *       |      ^
 * >-----L------R------E
 */
void updateEffects(Note* self, int8_t* refTable) {
	if (self->effects->volumeEnabled && self->stillPlaying) {
		//first we adjust our volume and then we increment our state
		adjustVolume(self, refTable);
		updateVolume(self);
	}
}
	
void updateVolume(Note* self) {
	// in all the below methods we assume the list has proper markers. BEWARE of the NULL(S)!
	switch(((VolumeEff*)self->effects->volumeCur->data)->marker) {
		
		case LOOP_MARKER:	
			// we see the loop marker so we save it off
			self->effects->volumeLoopPos = self->effects->volumeCur;
			// update our waveTable and increment the counter by one
			if (updateVolumeState(self->effects)) {
				self->effects->volumeCur = self->effects->volumeCur->next;
			}
			break;
		
		case RELEASE_MARKER: 
			if (self->key->letter != NO_NOTE) {
				// if note is still pressed we mark release location and go back to the start of loop
				self->effects->volumeReleasePos = self->effects->volumeCur;
				self->effects->volumeCur = self->effects->volumeLoopPos;
			} else {
				//proceed as normal
				// update our waveTable and increment the counter by one
				if (updateVolumeState(self->effects)) {
					self->effects->volumeCur = self->effects->volumeCur->next;
				}
			}
			break;
		
		case END_MARKER:
			// in this case we disable the note and reset the counter
			self->stillPlaying = false;
			self->effects->volumeCur = self->effects->volumeList->head;
			break;
		
		default:
			// make sure we still holding the key
			// or key isn't pressed and we are passed/at the release point so just keep going
			if (self->key->letter != NO_NOTE || self->effects->released) {
				// update our waveTable and increment the counter by one
				if (updateVolumeState(self->effects)) {
					self->effects->volumeCur = self->effects->volumeCur->next;
				}
			} else {
				// key isn't pressed and we need to skip to release point
				self->effects->volumeCur = self->effects->volumeReleasePos;
				self->effects->released = true;
			}
			break;
	}
}

void adjustVolume(Note* self, int8_t* refTable) {
	uint16_t i;
	static float volumeScaler;
	
	volumeScaler = ((VolumeEff*)self->effects->volumeCur->data)->volume;
	
	for (i = 0; i < EFFECT_SIZE; i++) {
		self->workingWaveTable[i] = (uint8_t)(volumeScaler * refTable[i]);
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

