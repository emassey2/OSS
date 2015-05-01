#ifndef __EFFECTS_H__
#define __EFFECTS_H__


#include <stdbool.h>
#include <stdint.h>
#include "board_config.h"
#include "defines.h"
#include "linkedList.h"

/******************************************************************************
 * Defines
 *****************************************************************************/
#define EFFECT_SIZE 256
 
#define LOOP_MARKER			'L'	// marks the point to return to when note is being held
#define RELEASE_MARKER	'R'	// marks the point to go to on release
#define END_MARKER			'E'	// marks the end of a note
#define NO_MARKER				'\0'// blank ie means nothing
#define VOLUME					'V'	// volume type
#define ARPEGGIO				'A'	// arpeggio type
#define PITCH						'P'	// pitch type



/* Each channel holds an Effects struct. Inse this struct there are pointers to
	each possible Effect and a bool if that effect is enabled or not. Each Effect
	is a linked list of effectStates that hold the duration of a given effect,
	what it does for that duration, and if it marks a special location in the 
	linked list. Information on the special locations are shown below.*/



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

// an effect of a single type eg volume or arpeggio
typedef struct Effect {
	List* list;
	Node* curPos;			// ptr to our current node
	Node* loopPos;		// ptr to the loop node
	Node* releasePos;	// ptr to the release node
}Effect;

// a struct that holds all of our effects
typedef struct Effects {
	bool enabled;						// if effects as a whole are enabled
	bool volumeEnabled;			// if volume effects are enabled
	bool arpeggioEnabled;		// if arpeggio effects are enabled
	bool pitchEnabled;			// if pitch effects are enabled
	bool released;					// if the key is no longer being pressed
	Effect* volume;					// ptr to our volume Effects
	Effect* arpeggio;				// ptr to our arpeggio Effects
	Effect* pitch;
}Effects;

// a given state in the list of an effect
typedef struct EffectState {
	void*			modifier;		// this is the actual affect eg float from 0-1 for volume or int8 for arpeggios
	uint32_t	duration;			// how long this state lasts
	char 			marker;				// if this state marks a special point (see markers above)
	char			type;					// the type of effect eg volume or arpeggio
}EffectState;

Effects* initEffects(bool enabled, bool volEnabled, bool arpEnabled, bool pitchEnabled);
bool updateState(EffectState* self);

void initEffect(Effect* self);
bool updateVolumeState(EffectState* self);
bool updateArpeggioState(EffectState* self);
bool updatePitchState(EffectState* self);

// basically a constructor - returns a pointer to an effect with given params
EffectState* newVolumeEff(float volume, uint32_t duration, char marker);
EffectState* newArpeggioEff(int8_t distance, uint32_t duration, char marker);
EffectState* newPitchEff(int32_t rate, uint32_t duration, char marker);

void initPremadeEffects(void);

void setEffect(char effectType, Effects* self, Effect* premadeEffect);

#endif // __EFFECTS_H__
