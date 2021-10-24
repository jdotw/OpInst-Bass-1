/*
 * midi.c
 *
 *  Created on: Feb 16, 2020
 *      Author: jwilson
 */

#include "midi.h"
#include "main.h"
#include "stdbool.h"

static midi_state _currentState;

midi_state *_midi_state_ptr() { return &_currentState; }

void midi_init() {
  _currentState.note_on = 0;
  _currentState.prev_note_number = MIDI_NOTE_NUMBER_NONE;
  _currentState.prev_note_velocity = MIDI_NOTE_VELOCITY_NONE;
  _currentState.cur_note_number = MIDI_NOTE_NUMBER_NONE;
  _currentState.cur_note_velocity = MIDI_NOTE_VELOCITY_NONE;
  _currentState.cur_note_time_ticks = 0;

  _currentState.pitch_bend_amount = MIDI_PITCH_BEND_MID_POINT;
}

midi_state midi_get_state() { return _currentState; }
