/*
 * osc_ctrl.c
 *
 *  Created on: 20 Apr 2021
 *      Author: jwilson
 */

#include "osc.h"
#include "ctrl.h"

osc_ctrl_state_t osc_ctrl_state;

void osc_ctrl_toggle_squ_func() {
	osc_ctrl_state.squ_func++;
	if (osc_ctrl_state.squ_func > OSC1_SQU_FUNC_MAX) {
		osc_ctrl_state.squ_func = 0;
	}
}

void osc_ctrl_inc_saw_level(int8_t delta) {
	ctrl_apply_delta(&osc_ctrl_state.squ_level, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_12BIT_MAX);
}

void osc_ctrl_inc_squ_level(int8_t delta) {
	ctrl_apply_delta(&osc_ctrl_state.squ_level, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_12BIT_MAX);
}

void osc_ctrl_inc_squ_pwm(int8_t delta) {
	ctrl_apply_delta(&osc_ctrl_state.squ_pwm, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_12BIT_MAX);
}

void osc_ctrl_inc_osc1_to_osc2_filt_mix(int8_t delta) {
	ctrl_apply_delta(&osc_ctrl_state.osc1_to_osc2_filt_mix, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_12BIT_MAX);
}
