/*
 * dac_note.c
 *
 *  Created on: 17 Aug 2020
 *      Author: jwilson
 */

#include "dac.h"
#include "midi.h"

#define SLIDE_TICKS 250	// 250ms slide default

uint16_t scaled_dac_value_for_midi_note(uint8_t note)
{
	// This instruments range is from C1= (1v) up to C4 (4v)
	if (note < MIDI_NOTE_MINIMUM || note > MIDI_NOTE_MAXIMUM)
	{
		return MIDI_NOTE_OUT_OF_RANGE;
	}

	// 5.33v vRef version
	//	// Scale up from 1v (64*12) by semitones (64 steps) above the minimum note
	//	uint16_t dacValue = (64*12) + ((note - MIDI_NOTE_MINIMUM) * 64);
	//	return dacValue;

	//	// 5v vref version
	//	// Scale up to 1v/oct across a 5v/4096bit range
	//	uint16_t dacValue = (note - MIDI_NOTE_MINIMUM) * 68.26666666667;

	// 4.096 internal vRef (MCP4728) version
	// Scale up to 1v/oct across a 4.096v/4096bit range
	uint16_t dacValue = (note - MIDI_NOTE_MINIMUM) * 83.33333;

	return dacValue;
}



uint16_t dac_value_for_midi_state(midi_state midi, uint8_t transpose_steps, bool allow_pitch_bend) {
	// Examine the MIDI state and determine the note DAC
	// control voltage to set the oscillator pitch based
	// on the current note value as well as the previous
	// if we're sliding/gliding between notes
	uint32_t now_ticks = HAL_GetTick();
	uint32_t time_elapsed = 0;
	if (now_ticks >= midi.cur_note_time_ticks) {
		time_elapsed = now_ticks - midi.cur_note_time_ticks;
	} else {
		// now_ticks has wrapped... bummer
		time_elapsed = (UINT32_MAX - midi.cur_note_time_ticks) + now_ticks;
	}

	uint16_t dacValue = 0;
	if (midi.prev_note_number != MIDI_NOTE_NUMBER_NONE
			&& time_elapsed < SLIDE_TICKS) {
		// Calculate an interpolated voltage between the prev and cur notes
		double interpolation = time_elapsed / (double)SLIDE_TICKS;
		uint16_t prevNoteDACValue = scaled_dac_value_for_midi_note(midi.prev_note_number+transpose_steps);
		uint16_t curNoteDACValue = scaled_dac_value_for_midi_note(midi.cur_note_number+transpose_steps);
		uint16_t interpolatedDACValue = prevNoteDACValue + ((curNoteDACValue - prevNoteDACValue) * interpolation);
		dacValue = interpolatedDACValue;
	} else {
		// Just use the cur note value
		dacValue = scaled_dac_value_for_midi_note(midi.cur_note_number+transpose_steps);
	}

	if (allow_pitch_bend) {
		// Factor in the pitch bend which is a 14 bit unsigned number
		// with a midpoint of 0x2000. We convert it to a signed 16 bit int,
		// then subtract the midpoint, and then shift it 2 bits to
		// the right to reduce its resolution to -1024 <-> +1024
		int16_t pitch_bend_modulation = (((int16_t)midi.pitch_bend_amount - MIDI_PITCH_BEND_MID_POINT) >> 3);
		int32_t modulated_dac_value = ((int32_t)dacValue) + ((int32_t)pitch_bend_modulation);
		if (modulated_dac_value < DAC_VALUE_MIN) {
			return DAC_VALUE_MIN;
		} else if (modulated_dac_value > DAC_VALUE_MAX) {
			return DAC_VALUE_MAX;
		} else {
			return (uint16_t)modulated_dac_value;
		}
	} else {
		return dacValue;
	}
}
