#include "inc/channel.h"

void initADSREnvelope(Channel* self) {
	uint32_t i;
	
	// A  D S      R
	//    ^ 
 	//   / \
 	//  /   -------
	// /            \
	
	for (i = 0; i < EFFECT_SIZE; i++){
		if (i < ATTACK) {
			//increase from 0 to 1
			self->note->effects.volume[i] = 1 / (ATTACK / (float)i);
		} else if (i < DECAY) {
			//Decrease from 1 to our sustainable volume
			self->note->effects.volume[i] = 1 - ((1 - SUSTAIN_VOL) * (((float)i - ATTACK)) / (DECAY - ATTACK));
		} else if (i < SUSTAIN) {
			//remain at our our sustainable volume
			self->note->effects.volume[i] = SUSTAIN_VOL;
		} else if (i < RELEASE) {
			// decrease from our sustainable volume to 0
			self->note->effects.volume[i] = SUSTAIN_VOL - (SUSTAIN_VOL * (((float)i - SUSTAIN)/(RELEASE - SUSTAIN)));
		} else {
			self->note->effects.volume[i] = 0;
		}	
	}
	
	self->note->effects.volumeLoopPos = DECAY;				// loop in the sustainable volume area
	self->note->effects.volume[DECAY] = LOOP;
	self->note->effects.volume[SUSTAIN] = RELEASE_PT;	// release point is right after the end of the sustain
	self->note->effects.volumeReleasePos = SUSTAIN;
	self->note->effects.volume[DECAY] = END;
}
