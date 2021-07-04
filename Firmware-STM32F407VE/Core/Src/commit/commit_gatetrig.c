/*
 * commit_gatetrig.c
 *
 *  Created on: 1 May 2021
 *      Author: jwilson
 */

#include "note.h"
#include "main.h"
#include "i2c.h"
#include "pca9555.h"
#include "commit.h"

#define TRIGGER_LENGTH 5

void commit_gatetrig(void) {
	bool res;
	uint8_t outputs[2] = { 0, 0 };

	// Configure Gates
	outputs[0] |= commit_note_value.note_on << 3;
	outputs[0] |= commit_note_value.note_on << 5;
	outputs[0] |= commit_note_value.note_on << 7;
	outputs[1] |= commit_note_value.note_on << 1;
	outputs[1] |= commit_note_value.note_on << 3;
	outputs[1] |= commit_note_value.note_on << 5;

	// Configure Triggers
	uint32_t tick = HAL_GetTick();
	if (tick == 0) tick++; // Guarantee non-zero
	if (note_trig.ping_trigger) {
		// We need to ping the trigger
		bool trig_state = false;
		if (note_trig.triggered_at == 0) {
			// This is the start of a ping
			note_trig.triggered_at = tick;
			trig_state = true;
		} else if (tick < (note_trig.triggered_at + TRIGGER_LENGTH)) {
			// Hold trigger high
			trig_state = true;
		} else {
			// Let trig go
			note_trig.ping_trigger = false;
			note_trig.triggered_at = 0;
		}

		outputs[0] |= trig_state << 2;
		outputs[0] |= trig_state << 4;
		outputs[0] |= trig_state << 6;
		outputs[1] |= trig_state << 0;
		outputs[1] |= trig_state << 2;
		outputs[1] |= trig_state << 4;
	}

	res = pca9555_set_port_output(I2C_RIGHT, 2, 0, outputs);
	if (!res) Error_Handler();
}
