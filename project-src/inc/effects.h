#include <stdbool.h>
#include "board_config.h"
#include "defines.h"
#include "linkedList.h"

/******************************************************************************
 * Defines
 *****************************************************************************/
// These might need to be variables some day
#define ATTACK 0
#define DECAY 50
#define SUSTAIN 100//150
#define RELEASE 150//254
#define SUSTAIN_VOL .5
#define EFFECT_SIZE 256
 
#define LOOP_MARKER 		'L'	// marks the point to return to when note is being held
#define RELEASE_MARKER	'R'	// marks the point to go to on release
#define END_MARKER			'E'	// marks the end of a note

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

// search a list to find a given marker
// returns the first node with marker or null
Node* findMarker(List *list, char marker);

// determines when to move to the next state
bool updateVolumeState(Effects* self);
