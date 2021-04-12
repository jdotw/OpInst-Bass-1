/*
 * dac_velocity.c
 *
 *  Created on: 31 Aug 2020
 *      Author: jwilson
 */

#include "dac.h"
#include "midi.h"

uint16_t dac_value_for_midi_velocity(uint8_t velocity)
{
	// Velocity will be 0-127, and we want to scale that
	// over 0-4096 (12bit) range of the DAC
	//
	// Velocity of 64 (half way)  = 0000 0100 0000
	// = Voltage of 2.048v = 2048 = 1000 0000 0000
	//
	// This controls the Attack on the filter env generator
	// This VCEnv expects 0v=max -5=low, hence the velocity is inverted
	return (uint16_t)((127 - velocity) << 5);
}

