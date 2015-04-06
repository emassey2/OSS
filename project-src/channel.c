#include <stdlib.h>
#include "inc/channel.h"
#include "inc/linkedList.h"


void initChannel(Channel** channel, int8_t* waveTableRef) {
	VolumeEff *volumeEff;
	
	*channel = malloc(sizeof(Channel));
	(*channel)->waveTable_ref = waveTableRef;
	
	(*channel)->note = malloc(sizeof(Note));
	(*channel)->note->waveTable = malloc(sizeof(int8_t [WAVE_TABLE_SIZE]));
	(*channel)->note->workingWaveTable = malloc(sizeof(int8_t [WAVE_TABLE_SIZE]));
	(*channel)->note->isNoise = false;
	
	(*channel)->note->key = malloc(sizeof(Key));
	(*channel)->note->key->letter = NO_NOTE;
	(*channel)->note->key->octave = MIN_OCTAVE + 3;
	
	(*channel)->note->effects = malloc(sizeof(Effects));
	(*channel)->note->effects->enabled = true;
	(*channel)->note->effects->volumeEnabled = true;
	(*channel)->note->effects->released = true; 
	(*channel)->note->effects->volumeList = newList();
	(*channel)->note->effects->volumeCur = newNode();
	(*channel)->note->effects->volumeLoopPos = newNode();
	(*channel)->note->effects->volumeReleasePos = newNode();
	
	//TODO: work on a solution to mark our loop and release pos
	
	volumeEff = newVolumeEff(.75, 10, NO_MARKER);							
	add((*channel)->note->effects->volumeList, volumeEff);
	
	volumeEff = newVolumeEff(1, 100, LOOP_MARKER);							
	add((*channel)->note->effects->volumeList, volumeEff);
	
	volumeEff = newVolumeEff(1, 100, NO_MARKER);					
	add((*channel)->note->effects->volumeList, volumeEff);
	
	volumeEff = newVolumeEff(1, 100, RELEASE_MARKER);							
	add((*channel)->note->effects->volumeList, volumeEff);
	
	volumeEff = newVolumeEff(1, 10, NO_MARKER);							
	add((*channel)->note->effects->volumeList, volumeEff);
	
	volumeEff = newVolumeEff(.75, 50, NO_MARKER);							
	add((*channel)->note->effects->volumeList, volumeEff);
	
	volumeEff = newVolumeEff(.5, 50, NO_MARKER);							
	add((*channel)->note->effects->volumeList, volumeEff);	
	
	volumeEff = newVolumeEff(.25, 50, NO_MARKER);							
	add((*channel)->note->effects->volumeList, volumeEff);
	
	volumeEff = newVolumeEff(.1, 50, NO_MARKER);							
	add((*channel)->note->effects->volumeList, volumeEff);
	
	volumeEff = newVolumeEff(0, 0, END_MARKER);							//end
	add((*channel)->note->effects->volumeList, volumeEff);
	

	initADSREnvelope((*channel));
}

void initADSREnvelope(Channel* self) {
	
}



/* Sad to see an old function go

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
*/
