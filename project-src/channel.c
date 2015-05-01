#include <stdlib.h>
#include "inc/channel.h"
#include "inc/effects.h"
#include "inc/linkedList.h"

//getting hacky now...
//here we declare pointers to the head of various premade effects
extern Effect* noArpeg;
extern Effect* slowArpeg;
extern Effect* fastArpeg;
extern Effect* octLow;
extern Effect* octHigh;

extern Effect* noPitch;
extern Effect* pitchUp;
extern Effect* pitchDown;
extern Effect* vibrato;
extern Effect* delayedVibrato;

extern Effect* noVolume;
extern Effect* ADSR;
extern Effect* shortNote;
extern Effect* longNote;
extern Effect* crescendo;

void initChannel(Channel** channel, int8_t** outputTable, int8_t* waveTableRef) {
	
	*channel = malloc(sizeof(Channel));
	(*channel)->waveTable_ref = waveTableRef;
	
	(*channel)->note = initNote(false, waveTableRef, 3);
	*outputTable = (*channel)->note->waveTable;
	
	(*channel)->note->key = initKey();
	
	(*channel)->mute = true;
	
	//below should/will be its own method
	
	(*channel)->note->effects = initEffects(true, true, true, true);
	(*channel)->note->effects->volume = noVolume;
	(*channel)->note->effects->arpeggio = noArpeg;
	(*channel)->note->effects->pitch = delayedVibrato;

	//TODO: work on a solution to mark our loop and release pos ?
	
}


