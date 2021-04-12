/*
 * midi_cc.c
 *
 *  Created on: 3 Sep 2020
 *      Author: jwilson
 */


#include "midi.h"
#include "midi_internal.h"
#include "main.h"
#include "stdbool.h"

void handle_midi_cc_msg(uint8_t status_msg, uint8_t *data) {
	uint8_t cc_num = data[0];
	uint8_t cc_value = data[1];
	midi_state *midi = _midi_state_ptr();
	switch (cc_num) {
	case 1:
		// mod-wheel
		midi->mod_wheel_amount = cc_value;
		break;
	}
}
