/*
 * midi.c
 *
 *  Created on: Feb 16, 2020
 *      Author: jwilson
 */

#include "i2c.h"
#include "main.h"
#include "midi.h"
#include "midi_internal.h"
#include "note.h"
#include "stdbool.h"

// MARK: - Note On / Note Off Handling

#define NOTE_TRANSITION_THRESHOLD_TICKS 40

void handle_midi_note_on_msg(uint8_t status_msg, uint8_t *data) {
  uint8_t note = data[0];
  uint8_t velocity = data[1];

  if (velocity == 0) {
    // MIDI spec allows for velocity=0 to be equivalent
    // to a note-off message
    handle_midi_note_off_msg(status_msg, data);
    return;
  }

  note_set_on(note);
}

void handle_midi_note_off_msg(uint8_t status_msg, uint8_t *data) {
  note_t *note = note_get_active();
  uint8_t midi_note = data[0];
  if (midi_note == note->value.note_number) {
    note_set_off();
  }
}

void handle_midi_pitch_bend_msg(uint8_t status_msg, uint8_t *data) {
  midi_state *midi = _midi_state_ptr();
  uint16_t bend_amount = ((uint16_t)data[0]) + (((uint16_t)data[1]) << 7);
  midi->pitch_bend_amount = bend_amount; // 14bit uint with 0x2000 mid
}
