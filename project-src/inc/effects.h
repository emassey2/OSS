#include "board_config.h"
#include "defines.h"
#include <stdbool.h>

/******************************************************************************
 * Defines
 *****************************************************************************/
 #define LOOP -2
 #define RELEASE_PT -3
 #define INDEX_OF_RELEASE 126
 #define END -4
 #define EFFECT_SIZE 256
 
 
 
typedef struct Effects {
	bool enabled;					// if effects as a whole are enabled
	bool volumeEnabled;		// if volume effects are enabled
	bool arpeggioEnabled;	// if arpeggio effects are enabled
	
	uint8_t volumeCounter;
	uint8_t arpeggioCounter;
	
	uint8_t volumeLoopPos;
	uint8_t arpeggioLoopPos;
	
	uint8_t volumeReleasePos;
	uint8_t arpeggioReleasePos;
	
	uint8_t volumeEndPos;
	uint8_t arpeggioEndPos;
	
	float volume[EFFECT_SIZE];
	int16_t arpeggio[EFFECT_SIZE];
	

	
}Effects;
