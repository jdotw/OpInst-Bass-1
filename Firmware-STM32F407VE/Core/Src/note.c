/*
 * note.c
 *
 *  Created on: 1 May 2021
 *      Author: jwilson
 */

#include "note.h"
#include "i2c.h"
#include "midi.h"
#include <string.h>

note_t note = {
    .changed = {0}, .value = {.note_number = MIDI_NOTE_A3, 0}, .trig = {0}};

note_t *note_get_active(void) { return &note; }

void note_set_on(uint8_t note_number) {
  if (note.value.note_number != note_number) {
    note.value.note_number = note_number;
    note.changed.note_number = true;
  }
  if (!note.value.note_on) {
    note.trig.triggered_at = 0;
    note.trig.ping_trigger = true;
    note.value.note_on = true;
    note.changed.note_on = true;
    note.event_ticks = HAL_GetTick();
  }

  // Reset i2c bus handlers to ensure
  // that the high-priority tasks are
  // performed on the next cycle
  i2c_resume_left_bus_reset();
  i2c_resume_right_bus_reset();
}

void note_set_off() {
  if (note.value.note_on) {
    note.value.note_on = false;
    note.changed.note_on = true;
    note.event_ticks = HAL_GetTick();
  }
  // Reset i2c bus handlers to ensure
  // that the high-priority tasks are
  // performed on the next cycle
  i2c_resume_left_bus_reset();
  i2c_resume_right_bus_reset();
}

void note_changed_reset() {
  memset(&note.changed, 0, sizeof(struct note_changed_struct));
}
