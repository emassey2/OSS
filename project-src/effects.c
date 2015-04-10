#include "inc/effects.h"
#include <stdlib.h>

Effects* initEffects(bool enabled, bool volEnabled, bool arpEnabled) {
	Effects* self = malloc(sizeof(Effects));
	self->enabled = enabled;
	self->volumeEnabled = volEnabled;
	self->arpeggioEnabled = arpEnabled;
	
	if (enabled) {
		if (volEnabled) {
			self->volume = malloc(sizeof(Effect));
			initEffect(self->volume);
		}
		if (arpEnabled) {
			self->arpeggio = malloc(sizeof(Effect));
			initEffect(self->arpeggio);
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

bool updateState(EffectState* self) {
	if (self->type == VOLUME) {
		return updateVolumeState(self);
	} else if (self->type == ARPEGGIO) {
		return updateArpeggioState(self);
	}
	//hopefully don't get here
	return false;
}

bool updateVolumeState(EffectState* self) {
	// keeps track how long we have been in a given state
	static uint32_t currentDuration = 0;
	
	if ( (self)->duration > currentDuration) {
		currentDuration++;
		return false;
	} else {
		currentDuration = 0;
		return true;
	}
}

bool updateArpeggioState(EffectState* self) {
	// keeps track how long we have been in a given state
	static uint32_t currentDuration = 0;
	
	if ( (self)->duration > currentDuration) {
		currentDuration++;
		return false;
	} else {
		currentDuration = 0;
		return true;
	}
}
