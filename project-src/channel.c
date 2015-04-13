#include <stdlib.h>
#include "inc/channel.h"
#include "inc/linkedList.h"


void initChannel(Channel** channel, int8_t** outputTable, int8_t* waveTableRef) {
	EffectState *volumeEff;
	EffectState *arpeggioEff;
	
	*channel = malloc(sizeof(Channel));
	(*channel)->waveTable_ref = waveTableRef;
	
	(*channel)->note = initNote(false, waveTableRef, 3);
	*outputTable = (*channel)->note->waveTable;
	
	(*channel)->note->key = initKey();
	
	//below should/will be its own method
	
	(*channel)->note->effects = initEffects(true, false, true);
	
	//TODO: work on a solution to mark our loop and release pos ?

	

	
	/*volumeEff = newVolumeEff(.25, 88, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(.5, 88, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(.75, 50, LOOP_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(1, 50, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);	
	
	volumeEff = newVolumeEff(.75, 50, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(.5, 50, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(.5, 0, RELEASE_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	(*channel)->note->effects->volume->releasePos = (*channel)->note->effects->volume->list->tail;

	volumeEff = newVolumeEff(.5, 88, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(.4, 88, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(.3, 88, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(.2, 88, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(.1, 88, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(0, 0, END_MARKER);							//end
	add((*channel)->note->effects->volume->list, volumeEff);*/
	
		
	arpeggioEff = newArpeggioEff(0, 22, LOOP_MARKER);							
	add((*channel)->note->effects->arpeggio->list, arpeggioEff);
		
	arpeggioEff = newArpeggioEff(4, 22, NO_MARKER);							
	add((*channel)->note->effects->arpeggio->list, arpeggioEff);
		
	arpeggioEff = newArpeggioEff(7, 22, NO_MARKER);							
	add((*channel)->note->effects->arpeggio->list, arpeggioEff);
	
	arpeggioEff = newArpeggioEff(12, 22, NO_MARKER);							
	add((*channel)->note->effects->arpeggio->list, arpeggioEff);

	arpeggioEff = newArpeggioEff(12, 0, RELEASE_MARKER);							
	add((*channel)->note->effects->arpeggio->list, arpeggioEff);
	(*channel)->note->effects->arpeggio->releasePos = (*channel)->note->effects->arpeggio->list->tail;
	
	arpeggioEff = newArpeggioEff(0, 0, END_MARKER);							//end
	add((*channel)->note->effects->arpeggio->list, arpeggioEff);
	
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
