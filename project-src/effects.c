#include "inc/effects.h"
#include <stdlib.h>

uint32_t currentVolumeDuration = 0;
uint32_t currentArpeggioDuration = 0;
uint32_t currentPitchDuration = 0;

//here we declare pointers to the head of various premade effects
Effect* noArpeg;
Effect* slowArpeg;
Effect* fastArpeg;
Effect* octLow;
Effect* octHigh;

Effect* noPitch;
Effect* pitchUp;
Effect* pitchDown;
Effect* vibrato;
Effect* delayedVibrato;

Effect* noVolume;
Effect* ADSR;
Effect* shortNote;
Effect* longNote;
Effect* crescendo;


Effects* initEffects(bool enabled, bool volEnabled, bool arpEnabled, bool pitchEnabled) {
	Effects* self = malloc(sizeof(Effects));
	self->enabled = enabled;
	self->volumeEnabled = volEnabled;
	self->arpeggioEnabled = arpEnabled;
	self->pitchEnabled = pitchEnabled;
	
	if (enabled) {
		if (volEnabled) {
			self->volume = malloc(sizeof(Effect));
			initEffect(self->volume);
		}
		if (arpEnabled) {
			self->arpeggio = malloc(sizeof(Effect));
			initEffect(self->arpeggio);
		}
		if (pitchEnabled) {
			self->pitch = malloc(sizeof(Effect));
			initEffect(self->pitch);
		}
	}
	
	return self;
}
// populates a given effects list and nodes
void initEffect(Effect* self) {
	self->list = newList();
	self->curPos = self->list->head;
	self->loopPos = self->list->head;
	self->releasePos = self->list->head;
}

// returns a pointer to the new VolumeEff
EffectState* newVolumeEff(float volume, uint32_t duration, char marker) {
	EffectState* volumeEff = malloc(sizeof(EffectState));
	volumeEff->modifier = malloc(sizeof(float));
	*((float*)volumeEff->modifier) = volume;
	volumeEff->duration = duration;
	volumeEff->marker = marker;
	volumeEff->type = VOLUME;
	
	return volumeEff;
}

// returns a pointer to the new ArpeggioEff
EffectState* newArpeggioEff(int8_t distance, uint32_t duration, char marker) {
	EffectState* arpeggioEff = malloc(sizeof(EffectState));
	arpeggioEff->modifier = malloc(sizeof(int8_t));
	*((int8_t*)arpeggioEff->modifier) = distance;
	arpeggioEff->duration = duration;
	arpeggioEff->marker = marker;
	arpeggioEff->type = ARPEGGIO;
	
	return arpeggioEff;
}

// returns a pointer to the new PitchEff
EffectState* newPitchEff(int32_t rate, uint32_t duration, char marker) {
	EffectState* pitchEff = malloc(sizeof(EffectState));
	pitchEff->modifier = malloc(sizeof(int8_t));
	*((int32_t*)pitchEff->modifier) = rate;
	pitchEff->duration = duration;
	pitchEff->marker = marker;
	pitchEff->type = PITCH;
	
	return pitchEff;
}

bool updateState(EffectState* self) {
	if (self->type == VOLUME) {
		return updateVolumeState(self);
	} else if (self->type == ARPEGGIO) {
		return updateArpeggioState(self);
	} else if (self->type == PITCH) {
		return updatePitchState(self);
	}
	//hopefully don't get here
	return false;
}

bool updateVolumeState(EffectState* self) {
	// keeps track how long we have been in a given state
	
	if ( (self)->duration > currentVolumeDuration) {
		currentVolumeDuration++;
		return false;
	} else {
		currentVolumeDuration = 0;
		return true;
	}
}

bool updateArpeggioState(EffectState* self) {
	// keeps track how long we have been in a given state
	
	if ( (self)->duration > currentArpeggioDuration) {
		currentArpeggioDuration++;
		return false;
	} else {
		currentArpeggioDuration = 0;
		return true;
	}
}

bool updatePitchState(EffectState* self) {
	// keeps track how long we have been in a given state
	
	if ((self)->duration > currentPitchDuration) {
		currentPitchDuration++;
		return false;
	} else {
		currentPitchDuration = 0;
		return true;
	}
}

void initPremadeEffects(void) {
	EffectState *effPtr;
	
	/*** ARPEGGIO EFFECTS ***/
	
	noArpeg = malloc(sizeof(Effect));
	initEffect(noArpeg);
	effPtr = newArpeggioEff(0, 1, LOOP_MARKER);
	add(noArpeg->list, effPtr);
	
	effPtr = newArpeggioEff(0, 1, RELEASE_MARKER);
	add(noArpeg->list, effPtr);
	noArpeg->releasePos = noArpeg->list->tail;

	effPtr = newArpeggioEff(0, 0, END_MARKER);
	add(noArpeg->list, effPtr);
	
	
	slowArpeg = malloc(sizeof(Effect));
	initEffect(slowArpeg);
	effPtr = newArpeggioEff(0, 220, LOOP_MARKER);
	add(slowArpeg->list, effPtr);

	effPtr = newArpeggioEff(4, 220, NO_MARKER);
	add(slowArpeg->list, effPtr);

	effPtr = newArpeggioEff(7, 220, NO_MARKER);
	add(slowArpeg->list, effPtr);

	effPtr = newArpeggioEff(12, 220, NO_MARKER);
	add(slowArpeg->list, effPtr);

	effPtr = newArpeggioEff(12, 0, RELEASE_MARKER);
	add(slowArpeg->list, effPtr);
	slowArpeg->releasePos = slowArpeg->list->tail;

	effPtr = newArpeggioEff(0, 0, END_MARKER);
	add(slowArpeg->list, effPtr);
	
	
	fastArpeg = malloc(sizeof(Effect));
	initEffect(fastArpeg);
	effPtr = newArpeggioEff(0, 20, LOOP_MARKER);
	add(fastArpeg->list, effPtr);

	effPtr = newArpeggioEff(4, 20, NO_MARKER);
	add(fastArpeg->list, effPtr);

	effPtr = newArpeggioEff(7, 20, NO_MARKER);
	add(fastArpeg->list, effPtr);

	effPtr = newArpeggioEff(12, 20, NO_MARKER);
	add(fastArpeg->list, effPtr);

	effPtr = newArpeggioEff(12, 0, RELEASE_MARKER);
	add(fastArpeg->list, effPtr);
	fastArpeg->releasePos = fastArpeg->list->tail;

	effPtr = newArpeggioEff(0, 0, END_MARKER);
	add(fastArpeg->list, effPtr);
	
		
	octLow = malloc(sizeof(Effect));
	initEffect(octLow);
	effPtr = newArpeggioEff(0, 66, NO_MARKER);
	add(octLow->list, effPtr);
	
	effPtr = newArpeggioEff(-12, 1, LOOP_MARKER);
	add(octLow->list, effPtr);

	effPtr = newArpeggioEff(-12, 0, RELEASE_MARKER);
	add(octLow->list, effPtr);
	octLow->releasePos = octLow->list->tail;

	effPtr = newArpeggioEff(0, 0, END_MARKER);
	add(octLow->list, effPtr);	
	
		
	octHigh = malloc(sizeof(Effect));
	initEffect(octHigh);
	effPtr = newArpeggioEff(0, 66, NO_MARKER);
	add(octHigh->list, effPtr);
	
	effPtr = newArpeggioEff(12, 1, LOOP_MARKER);
	add(octHigh->list, effPtr);

	effPtr = newArpeggioEff(12, 0, RELEASE_MARKER);
	add(octHigh->list, effPtr);
	octHigh->releasePos = octHigh->list->tail;

	effPtr = newArpeggioEff(0, 0, END_MARKER);
	add(octHigh->list, effPtr);
	
	
	/*** PITCH EFFECTS ***/
	
	noPitch = malloc(sizeof(Effect));
	initEffect(noPitch);
	effPtr = newPitchEff(0, 1, LOOP_MARKER);
	add(noPitch->list, effPtr);

	effPtr = newPitchEff(0, 1, RELEASE_MARKER);
	add(noPitch->list, effPtr);
	noPitch->releasePos = noPitch->list->tail;

	effPtr = newPitchEff(0, 0, END_MARKER);
	add(noPitch->list, effPtr);
	

	pitchUp = malloc(sizeof(Effect));
	initEffect(pitchUp);
	effPtr = newPitchEff(10000, 5, LOOP_MARKER);
	add(pitchUp->list, effPtr);

	effPtr = newPitchEff(10000, 0, RELEASE_MARKER);
	add(pitchUp->list, effPtr);
	pitchUp->releasePos = pitchUp->list->tail;

	effPtr = newPitchEff(0, 0, END_MARKER);
	add(pitchUp->list, effPtr);


	pitchDown = malloc(sizeof(Effect));
	initEffect(pitchDown);
	effPtr = newPitchEff(-10000, 5, LOOP_MARKER);
	add(pitchDown->list, effPtr);

	effPtr = newPitchEff(-10000, 0, RELEASE_MARKER);
	add(pitchDown->list, effPtr);
	pitchDown->releasePos = pitchDown->list->tail;

	effPtr = newPitchEff(0, 0, END_MARKER);
	add(pitchDown->list, effPtr);
	
	
	vibrato = malloc(sizeof(Effect));
	initEffect(vibrato);
	
	effPtr = newPitchEff(-5000, 50, LOOP_MARKER);
	add(vibrato->list, effPtr);
	
	effPtr = newPitchEff(0, 50, NO_MARKER);
	add(vibrato->list, effPtr);
	
	effPtr = newPitchEff(5000, 50, NO_MARKER);
	add(vibrato->list, effPtr);
	
	effPtr = newPitchEff(0, 50, NO_MARKER);
	add(vibrato->list, effPtr);

	effPtr = newPitchEff(0, 0, RELEASE_MARKER);
	add(vibrato->list, effPtr);
	vibrato->releasePos = vibrato->list->tail;

	effPtr = newPitchEff(0, 0, END_MARKER);
	add(vibrato->list, effPtr);
	
		
	delayedVibrato = malloc(sizeof(Effect));
	initEffect(delayedVibrato);
	
	effPtr = newPitchEff(0, 600, NO_MARKER);
	add(delayedVibrato->list, effPtr);
	
	effPtr = newPitchEff(-5000, 50, LOOP_MARKER);
	add(delayedVibrato->list, effPtr);
	
	effPtr = newPitchEff(0, 50, NO_MARKER);
	add(delayedVibrato->list, effPtr);
	
	effPtr = newPitchEff(5000, 50, NO_MARKER);
	add(delayedVibrato->list, effPtr);
	
	effPtr = newPitchEff(0, 50, NO_MARKER);
	add(delayedVibrato->list, effPtr);

	effPtr = newPitchEff(0, 0, RELEASE_MARKER);
	add(delayedVibrato->list, effPtr);
	delayedVibrato->releasePos = delayedVibrato->list->tail;

	effPtr = newPitchEff(0, 0, END_MARKER);
	add(delayedVibrato->list, effPtr);
	
	
	/*** VOLUME EFFECTS ***/
	
	noVolume = malloc(sizeof(Effect));
	initEffect(noVolume);
	
	effPtr = newVolumeEff(1, 1, LOOP_MARKER);
	add(noVolume->list, effPtr);
	
	effPtr = newVolumeEff(1, 1, RELEASE_MARKER);
	add(noVolume->list, effPtr);
	noVolume->releasePos = noVolume->list->tail;

	effPtr = newVolumeEff(0, 0, END_MARKER);
	add(noVolume->list, effPtr);
	

	ADSR = malloc(sizeof(Effect));
	initEffect(ADSR);
	
	effPtr = newVolumeEff(.25, 33, NO_MARKER);
	add(ADSR->list, effPtr);
	
	effPtr = newVolumeEff(.5, 33, NO_MARKER);
	add(ADSR->list, effPtr);
	
	effPtr = newVolumeEff(.75, 33, NO_MARKER);
	add(ADSR->list, effPtr);
	
	effPtr = newVolumeEff(1, 33, NO_MARKER);
	add(ADSR->list, effPtr);
	
	effPtr = newVolumeEff(.75, 33, NO_MARKER);
	add(ADSR->list, effPtr);
	
	effPtr = newVolumeEff(.5, 16, LOOP_MARKER);
	add(ADSR->list, effPtr);

	effPtr = newVolumeEff(.5, 16, RELEASE_MARKER);
	add(ADSR->list, effPtr);
	ADSR->releasePos = ADSR->list->tail;
	
	effPtr = newVolumeEff(.4, 33, NO_MARKER);
	add(ADSR->list, effPtr);
	
	effPtr = newVolumeEff(.3, 33, NO_MARKER);
	add(ADSR->list, effPtr);
	
	effPtr = newVolumeEff(.2, 33, NO_MARKER);
	add(ADSR->list, effPtr);

	effPtr = newVolumeEff(.1, 33, NO_MARKER);
	add(ADSR->list, effPtr);

	effPtr = newVolumeEff(0, 0, END_MARKER);
	add(ADSR->list, effPtr);
	
				
	shortNote = malloc(sizeof(Effect));
	initEffect(shortNote);
	
	effPtr = newVolumeEff(.25, 10, NO_MARKER);
	add(shortNote->list, effPtr);
	
	effPtr = newVolumeEff(.5, 10, NO_MARKER);
	add(shortNote->list, effPtr);
	
	effPtr = newVolumeEff(1, 10, NO_MARKER);
	add(shortNote->list, effPtr);
	
	effPtr = newVolumeEff(.75, 10, NO_MARKER);
	add(shortNote->list, effPtr);
	
	effPtr = newVolumeEff(.25, 10, NO_MARKER);
	add(shortNote->list, effPtr);
	
	effPtr = newVolumeEff(0, 1, LOOP_MARKER);
	add(shortNote->list, effPtr);
	
	effPtr = newVolumeEff(0, 1, RELEASE_MARKER);
	add(shortNote->list, effPtr);
	shortNote->releasePos = shortNote->list->tail;

	effPtr = newVolumeEff(0, 0, END_MARKER);
	add(shortNote->list, effPtr);
	
				
	longNote = malloc(sizeof(Effect));
	initEffect(longNote);
	
	effPtr = newVolumeEff(.25, 50, NO_MARKER);
	add(longNote->list, effPtr);
	
	effPtr = newVolumeEff(.5, 50, NO_MARKER);
	add(longNote->list, effPtr);
	
	effPtr = newVolumeEff(1, 50, NO_MARKER);
	add(longNote->list, effPtr);
	
	effPtr = newVolumeEff(.75, 50, NO_MARKER);
	add(longNote->list, effPtr);
	
	effPtr = newVolumeEff(.25, 50, NO_MARKER);
	add(longNote->list, effPtr);
	
	effPtr = newVolumeEff(0, 1, LOOP_MARKER);
	add(longNote->list, effPtr);
	
	effPtr = newVolumeEff(0, 1, RELEASE_MARKER);
	add(longNote->list, effPtr);
	longNote->releasePos = longNote->list->tail;

	effPtr = newVolumeEff(0, 0, END_MARKER);
	add(longNote->list, effPtr);


	crescendo = malloc(sizeof(Effect));
	initEffect(crescendo);
	
	effPtr = newVolumeEff(.1, 63, NO_MARKER);
	add(crescendo->list, effPtr);
	
	effPtr = newVolumeEff(.2, 63, NO_MARKER);
	add(crescendo->list, effPtr);
	
	effPtr = newVolumeEff(.3, 63, NO_MARKER);
	add(crescendo->list, effPtr);
	
	effPtr = newVolumeEff(.4, 63, NO_MARKER);
	add(crescendo->list, effPtr);
	
	effPtr = newVolumeEff(.5, 63, NO_MARKER);
	add(crescendo->list, effPtr);
	
	effPtr = newVolumeEff(.6, 63, NO_MARKER);
	add(crescendo->list, effPtr);
	
	effPtr = newVolumeEff(.7, 63, NO_MARKER);
	add(crescendo->list, effPtr);
	
	effPtr = newVolumeEff(.8, 63, NO_MARKER);
	add(crescendo->list, effPtr);
	
	effPtr = newVolumeEff(.9, 63, NO_MARKER);
	add(crescendo->list, effPtr);

	effPtr = newVolumeEff(1, 63, NO_MARKER);
	add(crescendo->list, effPtr);
	
	effPtr = newVolumeEff(1, 10, LOOP_MARKER);
	add(crescendo->list, effPtr);

	effPtr = newVolumeEff(1, 10, RELEASE_MARKER);
	add(crescendo->list, effPtr);
	crescendo->releasePos = crescendo->list->tail;

	effPtr = newVolumeEff(0, 0, END_MARKER);
	add(crescendo->list, effPtr);
}



void setEffect(char effectType, Effects* self, Effect* premadeEffect ){
	
	if (effectType == VOLUME){
		self->volume = premadeEffect;
	}
	else if (effectType == ARPEGGIO){
		self->arpeggio = premadeEffect;
	}
	else if (effectType == PITCH){
		self->pitch = premadeEffect;
	}
	else {
	 // do nothing
	}
	
}
