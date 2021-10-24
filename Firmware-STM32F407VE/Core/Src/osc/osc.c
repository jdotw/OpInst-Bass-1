/*
 * osc.c
 *
 *  Created on: Apr 25, 2021
 *      Author: jwilson
 */

#include "osc.h"
#include "main.h"
#include "midi.h"

uint16_t osc_dac_offset[2];

uint16_t _scaled_dac_value_for_midi_note(uint8_t note) {
  // This instruments range is from C1= (1v) up to C4 (4v)
  if (note < MIDI_NOTE_MINIMUM || note > MIDI_NOTE_MAXIMUM) {
    return MIDI_NOTE_OUT_OF_RANGE;
  }

  // 5v vref version
  // Scale up to 1v/oct across a 5v/4096bit range
  // Each midi note is 1v / 12steps = 83.3333mv per step
  // Scaling 5000mv across 12bits (4096) means each bit is 0.8192mv
  // Therefore scaled dac value is steps * (83.3333mv per step * 0.8192mv per
  // bit scale)
  uint16_t dacValue = (note - MIDI_NOTE_MINIMUM) * 68.2663936;

  return dacValue;
}

uint16_t osc_dac_value_for_note(osc_t osc, uint8_t note_number) {
  // Returns the scaled *AND* calibrated DAC value
  uint16_t val = _scaled_dac_value_for_midi_note(note_number);
  if (val != MIDI_NOTE_OUT_OF_RANGE) {
    val += osc_dac_offset[osc];
  }
  return val;
}
