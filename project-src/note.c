#include "inc/note.h"
#include <stdlib.h>

volatile bool volumeUpdate = false;
int8_t arpeggioModifier = 0;
int32_t pitchModifier = 0;

extern uint32_t currentVolumeDuration;
extern uint32_t currentArpeggioDuration;
extern uint32_t currentPitchDuration;


Note* initNote(bool isNoise, int8_t* waveTableRef, uint8_t octaveMod) {
	uint16_t i;
	Note* self = malloc(sizeof(Note));
	self->waveTable = malloc(sizeof(int8_t [WAVE_TABLE_SIZE]));
	self->workingWaveTable = malloc(sizeof(int8_t [WAVE_TABLE_SIZE]));
	self->isNoise = isNoise;
	self->octaveMod = octaveMod;
	
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

void calculateTuningWord(volatile uint32_t* tuningWord, Note* self) {
	// convert everything into one giant number know as keyNumber
	int16_t keyNumber;
	uint8_t workingOct = 0;
	uint32_t tempTword = 0;
	
	keyNumber = (self->key->octave + self->octaveMod) * KEYS_PER_OCT + self->key->letter + arpeggioModifier;
	if (keyNumber < 0 || keyNumber > (NUM_OCTAVES * KEYS_PER_OCT) - 1 ) {
		// note is too low or too high to play
		*tuningWord = NO_SOUND;
	} else {
		workingOct = 0;
		//need to calculate our new oct and key
		while (keyNumber > KEYS_PER_OCT) {
		keyNumber -= KEYS_PER_OCT;
		workingOct++;
		}
		
		if (self->effects->pitchEnabled) {
			// make sure we don't over or underflow
			if ( (pitchModifier < 0 && (*tuningWord + pitchModifier) > *tuningWord) 
				|| (pitchModifier > 0 && (*tuningWord + pitchModifier) < *tuningWord)
				|| (pitchModifier > 0 && (*tuningWord + pitchModifier) > MAX_FREQ)) {
					// overflow or underflow - do nothing
					*tuningWord = NO_SOUND;
				} else {
							if (self->isNoise) {
								updateNoiseTWord(workingOct, keyNumber, tuningWord + pitchModifier);
							} else {
								tempTword = (TUNING_CONST * noteFreq[workingOct][keyNumber]) + pitchModifier;
								*tuningWord = tempTword;
							}
			}
		} else {
					if (self->isNoise) {
						updateNoiseTWord(workingOct, keyNumber, tuningWord);
					} else {
						*tuningWord = (TUNING_CONST * noteFreq[workingOct][keyNumber]);
					}
		}
		

	}
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
		
		// check to make sure we still need to play a self
		if (!self->stillPlaying) {		
				// check if this is a noise channel as there are some special cases here
				*tuningWord = NO_SOUND;

			// key isn't held but we need to continue the self. Reassign values and wait for stillPlaying to be false
		} else if(keyLetter == NO_NOTE) { // loop & release stuff
			
				self->key->letter = lastKeyLetter;
				self->key->octave = lastKeyOct;
				
				calculateTuningWord(tuningWord, self);
			
				self->key->letter = keyLetter;
				self->key->octave = keyOct;
			
				keyOct = lastKeyOct;
				keyLetter = lastKeyLetter;
			
			// a new key has been pressed	
		} else if (keyLetter != lastKeyLetter || keyOct != lastKeyOct) {	
				calculateTuningWord(tuningWord, self);
				pitchModifier = 0;
		} else {
				//update Tuningword like normal
				calculateTuningWord(tuningWord, self);
		}
		
	//effects can be ignored
	} else {
		if (keyLetter == NO_NOTE) {
			*tuningWord = NO_SOUND;
		} else {	
				arpeggioModifier = 0;
				calculateTuningWord(tuningWord, self);
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
	currentPitchDuration = 0;
	if (self->effects->volumeEnabled) {
		self->effects->volume->curPos = self->effects->volume->list->head;
	}
	if (self->effects->arpeggioEnabled) {
		self->effects->arpeggio->curPos = self->effects->arpeggio->list->head;
		arpeggioModifier = 0;
	}
	if (self->effects->pitchEnabled) {
		self->effects->pitch->curPos = self->effects->pitch->list->head;
	}
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
		if (self->effects->pitchEnabled) {
			adjustPitch(self);
			updateEffect(self, self->effects->pitch);
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

void adjustPitch(Note* self) {
	static int32_t rate = 0;
	rate = (*(int32_t*) ((EffectState*)self->effects->pitch->curPos->data)->modifier);
	
	// make sure we don't over or underflow
	if ( (rate < 0 && pitchModifier < 0 && (rate + pitchModifier) > 0) 
		|| (rate > 0 && pitchModifier > 0 && (rate + pitchModifier) < 0)) {
			//do nothing
			pitchModifier = pitchModifier;
		} else {
			pitchModifier += rate;
		}
		
}

void updateNoiseTWord(uint8_t keyOct, int8_t keyLetter, volatile uint32_t* tuningWord) {
	// check to see if any key is pressed
	if (keyLetter != NO_NOTE) {
		// tuning word is repurposed in the case where we want to generate random noise
		*tuningWord = ((keyLetter  + (KEYS_PER_OCT*keyOct)) % NOISE_FREQS)+1;
	} else {
		*tuningWord = NO_SOUND;
	}
}

