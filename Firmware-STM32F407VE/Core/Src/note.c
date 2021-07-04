/*
 * note.c
 *
 *  Created on: 1 May 2021
 *      Author: jwilson
 */

#include "note.h"
#include "midi.h"
#include <string.h>

note_value_t note_value = {
  .note_number = MIDI_NOTE_A3,
  0
};

note_changed_t note_changed = {
  0
};

note_trig_t note_trig = {
	0
};

void note_set_on(uint8_t note_number) {
  if (note_value.note_number != note_number) {
    note_value.note_number = note_number;
    note_changed.note_number = true;
  }
	if (!note_value.note_on) {
		note_trig.triggered_at = 0;
		note_trig.ping_trigger = true;
	  note_value.note_on = true;
    note_changed.note_on = true;
	}
}

void note_set_off() {
  if (note_value.note_on) {
    note_value.note_on = false;
    note_changed.note_on = true;
  }
}

void note_changed_reset() {
  memset(&note_changed, 0, sizeof(note_changed_t));
}
