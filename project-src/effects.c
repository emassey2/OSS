#include "inc/effects.h"
#include <stdlib.h>

// returns a pointer to the new VolumeEff
VolumeEff* newVolumeEff(float volume, uint32_t duration, char marker) {
	VolumeEff* volumeEff = malloc(sizeof(VolumeEff));
	volumeEff->volume = volume;
	volumeEff->duration = duration;
	volumeEff->marker = marker;
	
	return volumeEff;
}

// returns a pointer to the new ArpeggioEff
ArpeggioEff* newArpeggioEff(int8_t distance, uint32_t duration, char marker) {
	ArpeggioEff* arpeggioEff = malloc(sizeof(ArpeggioEff));
	arpeggioEff->distance = distance;
	arpeggioEff->duration = duration;
	arpeggioEff->marker = marker;
	
	return arpeggioEff;
}

bool updateVolumeState(Effects* self) {
	// keeps track how long we have been in a given state
	static uint32_t currentDuration = 0;
	
	if (((VolumeEff*)self->volumeCur->data)->duration > currentDuration) {
		currentDuration++;
		return false;
	} else {
		currentDuration = 0;
		return true;
	}
}

