/*
 * commit.c
 *
 *  Created on: Apr 25, 2021
 *      Author: jwilson
 */

#include <stdbool.h>
#include "ctrl.h"
#include "main.h"
#include "i2c.h"
#include "dac7678.h"
#include "osc.h"

void _commit_dac() {
  // Reset DACs
	HAL_StatusTypeDef res;
	uint16_t dac_val[8];

	// I2C Left 0
	res = i2c_mux_select(0, 0, 0);
	if (res != HAL_OK) Error_Handler();

	// Left0:000
	dac_val[0] = ctrl_value.osc1_filt_res;
	dac_val[1] = ctrl_value.osc1_filt_cutoff;
	uint8_t osc1_note = ctrl_value.note_number + ctrl_value.osc1_tune_coarse;
	uint16_t osc1_note_dac_val = osc_dac_value_for_note(OSC1, osc1_note);
	osc1_note_dac_val += ctrl_value.osc1_tune_fine; // TODO: Handle wrapping, maybe add it to osc1_note_dac_val?
	dac_val[2] = osc1_note_dac_val;
	dac_val[3] = ctrl_value.osc1_to_osc2;
	dac_val[4] = ctrl_value.osc1_to_osc1;
	dac_val[5] = ctrl_value.osc1_squ_pwm;
	dac_val[6] = ctrl_value.osc1_saw_lvl;
	dac_val[7] = ctrl_value.osc1_squ_lvl;
	res = dac7678_set_value_array(0, 0, dac_val);
	if (res != HAL_OK) Error_Handler();
}

void commit_30hz_timer(void) {
	_commit_dac();
}
