#include <stdbool.h>
#include "board_config.h"
#include "defines.h"
#include "linkedList.h"

/******************************************************************************
 * Defines
 *****************************************************************************/
#define EFFECT_SIZE 256
 
#define LOOP_MARKER 		'L'	// marks the point to return to when note is being held
#define RELEASE_MARKER	'R'	// marks the point to go to on release
#define END_MARKER			'E'	// marks the end of a note
#define NO_MARKER				'\0'// blank ie means nothing

/* Basic loop structure for effects
 * Start when key is pressed
 * While key is held 
 * 	Loop to L when R is reached 
 * Go to R when key is released
 * Stop playing when E is reached
 *       --------
 *       |      ^
 * >-----L------R------E
 */
 
typedef struct Effects {
	bool enabled;						// if effects as a whole are enabled
	bool volumeEnabled;			// if volume effects are enabled
	bool released;					// if the key is no longer being pressed
	List* volumeList;				// list of our volume effects
	Node* volumeCur;				// ptr to our current volumeEff node
	Node* volumeLoopPos;		// ptr to the loop point
	Node* volumeReleasePos;	// ptr to the release point
	
}Effects;

typedef struct VolumeEff {
	float 		volume;				// volume at the current state
	uint32_t	duration;			// how long this state lasts
	char 			marker;				// if this state marks a special point (see markers above)
}VolumeEff;

typedef struct ArpeggioEff {
	int8_t 		distance;			// distance relative to current note (the key being pressed)
	uint32_t	duration;			// how long this state lasts
	char 			marker;				// if this state marks a special point (see markers above)
}ArpeggioEff;

// determines when to move to the next state
bool updateVolumeState(Effects* self);

// basically a constructor - returns a pointer to an effect with given params
VolumeEff* newVolumeEff(float volume, uint32_t duration, char marker);
