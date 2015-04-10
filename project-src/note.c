#include "inc/note.h"
#include <stdlib.h>

volatile bool volumeUpdate = false;
int8_t arpeggioModifier = 0;

extern uint32_t currentVolumeDuration;
extern uint32_t currentArpeggioDuration;


Note* initNote(bool isNoise, int8_t* waveTableRef) {
	uint16_t i;
	Note* self = malloc(sizeof(Note));
	self->waveTable = malloc(sizeof(int8_t [WAVE_TABLE_SIZE]));
	self->workingWaveTable = malloc(sizeof(int8_t [WAVE_TABLE_SIZE]));
	self->isNoise = false;
	
	for (i = 0; i < EFFECT_SIZE; i++) {
			self->workingWaveTable[i] = waveTableRef[i];
			self->waveTable[i] = waveTableRef[i];
	}
	
	return self;
}


void updateKey(Note* self, int8_t keyNumber) {
	static int8_t lastKeyNumber = NO_NOTE;
	
	// check to see if the keys have changed
	if (lastKeyNumber != keyNumber) {
		if (lastKeyNumber != NO_NOTE && keyNumber != NO_NOTE) {
			// new key is pressed while another key was being held
			resetEffects(self);
			self->stillPlaying = true;
			self->effects->released = false;
		} else if (keyNumber != NO_NOTE) {
			// no keys being pressed to one key being pressed
			resetEffects(self);
			self->stillPlaying = true;
			self->effects->released = false;
		} else {
			// key was pressed and now no keys are being pressed
		}
	}
	lastKeyNumber = keyNumber;
	getKey(self->key, keyNumber);
}

void calculateTuningWord(volatile uint32_t* tuningWord, int8_t key, uint8_t octave, int8_t arpeggioModifier) {
	// convert everything into one giant number know as keyNumber
	int16_t keyNumber;
	keyNumber = (octave + 2) * KEYS_PER_OCT + key + arpeggioModifier;
	if (keyNumber < 0 || keyNumber > (NUM_OCTAVES * KEYS_PER_OCT) - 1 ) {
		// note is too low or too high to play
		*tuningWord = NO_SOUND;
	} else {
		octave = 0;
		//need to calculate our new oct and key
		while (keyNumber > KEYS_PER_OCT) {
		keyNumber -= KEYS_PER_OCT;
		octave++;
		}
		*tuningWord = TUNING_CONST * noteFreq[octave][keyNumber];
	}
}

void updateTuningWord(Note* effect, volatile uint32_t* tuningWord) {
	static int8_t lastKeyLetter = NO_NOTE;
	static int8_t keyLetter = NO_NOTE;
	static uint8_t keyOct = 0;
	static uint8_t lastKeyOct = 0;

	keyOct = effect->key->octave;
	keyLetter = effect->key->letter;
	
	// check if we need to account for effects
	if (effect->effects->enabled) {
		
		// check to make sure we still need to play a effect
		if (!effect->stillPlaying) {		
				// check if this is a noise channel as there are some special cases here
				if (effect->isNoise) {
					*tuningWord = NO_NOTE;
				} else {
					*tuningWord = NO_SOUND;		
				}
			
			// key isn't held but we need to continue the effect. Reassign values and wait for stillPlaying to be false
		} else if(keyLetter == NO_NOTE) { // loop & release stuff
				keyOct = lastKeyOct;
				keyLetter = lastKeyLetter;	
				
				// check if this is a noise channel as there are some special cases here
				if (effect->isNoise) {
					updateNoiseTWord(keyOct, keyLetter, tuningWord);
				} else {
					calculateTuningWord(tuningWord, lastKeyLetter, lastKeyOct, arpeggioModifier);
				}
		
			// a new key has been pressed	
		} else if (keyLetter != lastKeyLetter || keyOct != lastKeyOct) {	
			// check if this is a noise channel as there are some special cases here
				if (effect->isNoise) {
					updateNoiseTWord(keyOct, keyLetter, tuningWord);
				} else {
					calculateTuningWord(tuningWord, keyLetter, keyOct, arpeggioModifier);
				}
		} else {
			//update Tuningword like normal
			calculateTuningWord(tuningWord, keyLetter, keyOct, arpeggioModifier);
		}
		
	//effects can be ignored
	} else {
		if (keyLetter == NO_NOTE) {
			*tuningWord = NO_SOUND;
		} else {	
			// check if this is a noise channel as there are some special cases here
			if (effect->isNoise) {
				updateNoiseTWord(keyOct, keyLetter, tuningWord);
			} else {
					calculateTuningWord(tuningWord, keyLetter, keyOct, 0);
			}
		}
	}
	
	lastKeyOct = keyOct;	
	lastKeyLetter = keyLetter;
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

void resetEffects(Note* self) {
	currentVolumeDuration = 0;
	currentArpeggioDuration = 0;
	self->effects->volume->curPos = self->effects->volume->list->head;
	self->effects->arpeggio->curPos = self->effects->arpeggio->list->head;
}

void updateEffects(Note* self, int8_t* refTable) {
	if (self->stillPlaying) {
		//first we adjust our volume and then we increment our state
		if (self->effects->arpeggioEnabled) {
			adjustArpeggio(self);
			updateEffect(self, self->effects->arpeggio);
		}		
		if (self->effects->volumeEnabled) {
			adjustVolume(self, refTable);
			updateEffect(self, self->effects->volume);
		}
	}
}

void updateEffect(Note* self, Effect* effect) {
	bool isNoNote = (self->key->letter == NO_NOTE);
	EffectState* state;
	// make sure we still holding the key
	// if there is no note being played and we having marked the key as released
	if (isNoNote && !self->effects->released) {
		// key isn't pressed and we need to skip to release point
		effect->curPos = effect->releasePos;
		self->effects->released = true;
	}
	
	state = (EffectState*)effect->curPos->data;
	
	// in all the below methods we assume the list has proper markers. BEWARE of the NULL(S)!
	switch(state->marker) {
		
		case LOOP_MARKER:	
			// we see the loop marker so we save it off
			effect->loopPos = effect->curPos;
			// update our waveTable and increment the counter by one
			if (updateState(state)) {
				effect->curPos = effect->curPos->next;
			}
			break;
		
		case RELEASE_MARKER: 
			if (!isNoNote) {
				// if effect is still pressed we mark release location and go back to the start of loop
				effect->releasePos = effect->curPos;
				if (updateState(state)) {
					effect->curPos = effect->loopPos;
				}
			} else {
				//proceed as normal
				// update our waveTable and increment the counter by one
				if (updateState(state)) {
					effect->curPos = effect->curPos->next;
				}
			}
			break;
		
		case END_MARKER:
			// in this case we disable the effect and reset the counter
			self->stillPlaying = false;
			effect->curPos = effect->list->head;
			break;
		
		default:
			// make sure we still holding the key
			// or key isn't pressed and we are passed/at the release point so just keep going
			if (!isNoNote || self->effects->released) {
				// update our waveTable and increment the counter by one
				if (updateState(state)) {
					effect->curPos = effect->curPos->next;
				}
			} else {
				// key isn't pressed and we need to skip to release point
				effect->curPos = effect->releasePos;
				self->effects->released = true;
			}
			break;
	}
}

void adjustVolume(Note* self, int8_t* refTable) {
	uint16_t i;
	static float volumeScaler;
	
	// get the value stored at the location pointed by the modifier
	volumeScaler = *(float*) ((EffectState*)self->effects->volume->curPos->data)->modifier;
	
	if (volumeUpdate) {
		// if volume is already updated don't do anything
	} else {
		for (i = 0; i < EFFECT_SIZE; i++) {
			self->workingWaveTable[i] = (int8_t)(volumeScaler * refTable[i]);
		}
		volumeUpdate = true;
	}
}

void adjustArpeggio(Note* self) {
	arpeggioModifier = *(int8_t*) ((EffectState*)self->effects->arpeggio->curPos->data)->modifier;
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

