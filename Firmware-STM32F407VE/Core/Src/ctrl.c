/*
 * ctrl.c
 *
 *  Created on: 20 Apr 2021
 *      Author: jwilson
 */

#include <stdint.h>
#include <stdbool.h>
#include "ctrl.h"

#define CTRL_DEFAULT_SCALE 100
#define CTRL_DEFAULT_MIN 0
#define CTRL_DEFAULT_MAX 0xFFF // 12-bit

typedef enum osc_pwm_func {
	CTRL_OSC_SQU_LEVEL,
	CTRL_OSC_SQL_PWM,

	CTRL_OSC_SQU_MAX,
} osc_pwm_func_t;

typedef struct {
	uint16_t osc1_saw_lvl;
	uint16_t osc1_squ_lvl;
	uint16_t osc1_squ_pwm;
	uint16_t osc1_to_osc2;
} ctrl_value_t;

typedef struct {
	osc_pwm_func_t osc1_squ_func;
} ctrl_toggle_t;

ctrl_value_t ctrl_value = {
		.osc1_saw_lvl = 0,
		.osc1_squ_lvl = 0,
		.osc1_squ_pwm = 0,
};

ctrl_toggle_t ctrl_toggle = {
		.osc1_squ_func = 0,
};

void ctrl_overflow_handler() {
	// TODO: Buzz the haptic
}

void _ctrl_apply_delta(uint16_t *ctrl_ptr, int8_t delta, int16_t scale_percent, uint16_t min_value, uint16_t max_value) {
	bool did_overflow = false;
	int16_t scaled_enc_delta = (scale_percent / 100) * delta;
	if (scaled_enc_delta > 0) {
		if (scaled_enc_delta > max_value - *ctrl_ptr) {
			// Overflow max
			*ctrl_ptr = max_value;
			did_overflow = true;
		} else {
			*ctrl_ptr += scaled_enc_delta;
		}
	} else if (scaled_enc_delta < 0) {
		if ((scaled_enc_delta * -1) > *ctrl_ptr) {
			// Overflow min
			*ctrl_ptr = min_value;
		} else {
			*ctrl_ptr += scaled_enc_delta;
		}
	}
	if (did_overflow) {
		ctrl_overflow_handler();
	}
}

void ctrl_apply_delta(ctrl_enum_t ctrl, int8_t delta) {
	if (delta == 0) return;
	switch (ctrl) {
	case CTRL_OSC1_SAW:
		_ctrl_apply_delta(&ctrl_value.osc1_saw_lvl, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case CTRL_OSC1_SQU:
		switch(ctrl_toggle.osc1_squ_func) {
		case CTRL_OSC_SQU_LEVEL:
			_ctrl_apply_delta(&ctrl_value.osc1_squ_lvl, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		case CTRL_OSC_SQL_PWM:
			_ctrl_apply_delta(&ctrl_value.osc1_squ_pwm, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		default:
			break;
		}
		break;
	case CTRL_OSC1_TO_OSC2:
		_ctrl_apply_delta(&ctrl_value.osc1_to_osc2, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	}
}

void ctrl_apply_toggle(ctrl_enum_t ctrl, bool changed, bool state) {
	if (state && changed) {
		switch(ctrl) {
		case CTRL_OSC1_SQU:
			ctrl_toggle.osc1_squ_func++;
			if (ctrl_toggle.osc1_squ_func == CTRL_OSC_SQU_MAX) {
				ctrl_toggle.osc1_squ_func = 0;
			}
			break;
		default:
			break;
		}
	}
}
