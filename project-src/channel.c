#include "inc/channel.h"

void updateNoteKey(Channel* self, int8_t keyNumber) {
	updateKey(self->note, keyNumber);
}

void updateNoteTuningWord(Channel* self, volatile uint32_t* tuningWord) {
	updateTuningWord(self->note, tuningWord);
}
