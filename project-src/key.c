#include "inc/key.h"
#include <stdlib.h>

Key* getKey(Key* self, int8_t keyNumber) {
	uint8_t oct;
	oct = MIN_OCTAVE;
	// determine which octave we are in
	while (keyNumber > KEYS_PER_OCT) {
		keyNumber -= KEYS_PER_OCT;
		oct++;
	}
	
	self->octave = oct;
	self->letter = keyNumber;
	
	return self;
}


Key* initKey() {
	Key* self = malloc(sizeof(Key));
	self->letter = NO_NOTE;
	self->octave = MIN_OCTAVE;
	
	return self;
}
