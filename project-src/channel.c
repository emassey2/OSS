#include <stdlib.h>
#include "inc/channel.h"
#include "inc/linkedList.h"


void initChannel(Channel** channel, int8_t** outputTable, int8_t* waveTableRef) {
	EffectState *volumeEff;
	EffectState *arpeggioEff;
	EffectState *pitchEff;
	
	*channel = malloc(sizeof(Channel));
	(*channel)->waveTable_ref = waveTableRef;
	
	(*channel)->note = initNote(false, waveTableRef, 3);
	*outputTable = (*channel)->note->waveTable;
	
	(*channel)->note->key = initKey();
	
	(*channel)->mute = false;
	
	//below should/will be its own method
	
	(*channel)->note->effects = initEffects(true, false, false, true);
	//(*channel)->note->effects = initEffects(false, false, false, false);
	
	//TODO: work on a solution to mark our loop and release pos ?

	

	/*volumeEff = newVolumeEff(1, 50, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(.75, 50, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(.5, 50, LOOP_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(.5, 50, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(.5, 0, RELEASE_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	(*channel)->note->effects->volume->releasePos = (*channel)->note->effects->volume->list->tail;

	volumeEff = newVolumeEff(.5, 88, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(.4, 880, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(.3, 88, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(.2, 88, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(.1, 88, NO_MARKER);							
	add((*channel)->note->effects->volume->list, volumeEff);
	
	volumeEff = newVolumeEff(0, 0, END_MARKER);							//end
	add((*channel)->note->effects->volume->list, volumeEff);
	
		
	arpeggioEff = newArpeggioEff(0, 220, LOOP_MARKER);							
	add((*channel)->note->effects->arpeggio->list, arpeggioEff);
		
	arpeggioEff = newArpeggioEff(4, 220, NO_MARKER);							
	add((*channel)->note->effects->arpeggio->list, arpeggioEff);
		
	arpeggioEff = newArpeggioEff(7, 220, NO_MARKER);							
	add((*channel)->note->effects->arpeggio->list, arpeggioEff);
	
	arpeggioEff = newArpeggioEff(12, 220, NO_MARKER);							
	add((*channel)->note->effects->arpeggio->list, arpeggioEff);

	arpeggioEff = newArpeggioEff(12, 0, RELEASE_MARKER);							
	add((*channel)->note->effects->arpeggio->list, arpeggioEff);
	(*channel)->note->effects->arpeggio->releasePos = (*channel)->note->effects->arpeggio->list->tail;
	
	arpeggioEff = newArpeggioEff(0, 0, END_MARKER);							//end
	add((*channel)->note->effects->arpeggio->list, arpeggioEff);*/
	
	
	pitchEff = newPitchEff(-5000, 50, LOOP_MARKER);							
	add((*channel)->note->effects->pitch->list, pitchEff);
	
	pitchEff = newPitchEff(0, 50, NO_MARKER);							
	add((*channel)->note->effects->pitch->list, pitchEff);

	pitchEff = newPitchEff(0, 0, RELEASE_MARKER);							
	add((*channel)->note->effects->pitch->list, pitchEff);
	(*channel)->note->effects->pitch->releasePos = (*channel)->note->effects->pitch->list->tail;
	
	pitchEff = newPitchEff(0, 0, END_MARKER);							//end
	add((*channel)->note->effects->pitch->list, pitchEff);
	
		
	/*pitchEff = newPitchEff(0, 400, NO_MARKER);							
	add((*channel)->note->effects->pitch->list, pitchEff);
	
	pitchEff = newPitchEff(-5000, 50, LOOP_MARKER);							
	add((*channel)->note->effects->pitch->list, pitchEff);
		
	pitchEff = newPitchEff(0, 50, NO_MARKER);							
	add((*channel)->note->effects->pitch->list, pitchEff);
		
	pitchEff = newPitchEff(5000, 50, NO_MARKER);							
	add((*channel)->note->effects->pitch->list, pitchEff);
	
	pitchEff = newPitchEff(0, 50, NO_MARKER);							
	add((*channel)->note->effects->pitch->list, pitchEff);

	pitchEff = newPitchEff(0, 0, RELEASE_MARKER);							
	add((*channel)->note->effects->pitch->list, pitchEff);
	(*channel)->note->effects->pitch->releasePos = (*channel)->note->effects->pitch->list->tail;
	
	pitchEff = newPitchEff(0, 0, END_MARKER);							//end
	add((*channel)->note->effects->pitch->list, pitchEff);*/
	
}
