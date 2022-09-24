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
  _currentState.pitch_bend_amount = MIDI_PITCH_BEND_MID_POINT;
}

midi_state midi_get_state() { return _currentState; }
