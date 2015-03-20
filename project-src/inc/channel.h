#include "board_config.h"
#include "defines.h"
#include "note.h"

/******************************************************************************
 * Defines
 *****************************************************************************/
typedef struct Channel {
	Note* note;
	uint8_t* waveTable_ref;
} Channel;


/******************************************************************************
 * Function Prototypes
 *****************************************************************************/
void updateNoteKey(Channel* self, int8_t keyNumber);

void updateNoteTuningWord(Channel* self, volatile uint32_t* tuningWord);

void initChannel(Channel** channel, uint8_t* waveTableRef);
	
void initADSREnvelope(Channel* self);
