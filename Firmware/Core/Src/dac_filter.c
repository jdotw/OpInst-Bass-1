/*
 * dac_filter.c
 *
 *  Created on: 2 Sep 2020
 *      Author: jwilson
 */

#include <stdint.h>

#include "dac.h"
#include "midi.h"

uint16_t scaled_dac_value_for_primary_filter(uint8_t mod_wheel_cc, uint8_t filter_1_cc) {
	// Takes the 0-127 control values, normalizes between the two
	// And then scales the normalized value to 0-4095 for the 12bit DAC
	return (mod_wheel_cc << 5) | (mod_wheel_cc >> 2);	// credit to biker_bea!
}

uint16_t scaled_dac_value_for_sub_filter(uint8_t mod_wheel_cc, uint8_t filter_2_cc) {
	return 0;
}

uint16_t dac_value_for_primary_filter_cutoff(midi_state midi) {
	// midi.mod_wheel_amount is a 7bit 0-127 representation of the modwheel
	return scaled_dac_value_for_primary_filter(midi.mod_wheel_amount, 0);
}
