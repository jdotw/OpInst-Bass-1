/*
 * midi.c
 *
 *  Created on: Feb 16, 2020
 *      Author: jwilson
 */

#include "midi.h"
#include "midi_internal.h"
#include "main.h"
#include "stdbool.h"
#include "note.h"

// MARK: - Note On / Note Off Handling

#define NOTE_TRANSITION_THRESHOLD_TICKS 40

void handle_midi_note_on_msg(uint8_t status_msg, uint8_t *data) {
	midi_state *midi = _midi_state_ptr();
	uint8_t note = data[0];
	uint8_t velocity = data[1];

	if (velocity == 0) {
		// MIDI spec allows for velocity=0 to be equivalent
		// to a note-off message
		handle_midi_note_off_msg(status_msg, data);
		return;
	}

	if (midi->note_on) {
		// Note is already on, set up state for a slide

		// LEGACY
		midi->prev_note_number = midi->cur_note_number;
		midi->prev_note_velocity = midi->cur_note_velocity;
	} else {
		// Note is not on, don't use prev note number

		// LEGACY
		midi->prev_note_number = MIDI_NOTE_NUMBER_NONE;
		midi->prev_note_velocity = MIDI_NOTE_VELOCITY_NONE;
	}

	note_trig.ping_trigger = true;
	note_value.note_number = note;
	note_value.note_on = true;

	// LEGACY
	midi->cur_note_time_ticks = HAL_GetTick();
	midi->cur_note_number = note;
	midi->cur_note_velocity = velocity;
	midi->note_on = true;
}

void handle_midi_note_off_msg(uint8_t status_msg, uint8_t *data) {
	midi_state *midi = _midi_state_ptr();
	uint8_t note = data[0];
	if (note == midi->cur_note_number) {
		note_value.note_on = false;
		// LEGACY
		midi->note_on = false;
	} else if (note == midi->prev_note_number) {
		midi->prev_note_number = MIDI_NOTE_NUMBER_NONE;
		midi->prev_note_velocity = MIDI_NOTE_VELOCITY_NONE;
	}
}

void handle_midi_pitch_bend_msg(uint8_t status_msg, uint8_t *data) {
	midi_state *midi = _midi_state_ptr();
	uint16_t bend_amount = ((uint16_t)data[0]) + (((uint16_t)data[1]) << 7);
	midi->pitch_bend_amount = bend_amount; // 14bit uint with 0x2000 mid
}
