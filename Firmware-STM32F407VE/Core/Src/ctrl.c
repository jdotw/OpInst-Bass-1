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

typedef enum {
	CTRL_OSC_SQU_LEVEL,
	CTRL_OSC_SQL_PWM,

	CTRL_OSC_SQU_MAX,
} osc_pwm_func_t;

typedef enum {
	CTRL_OSC_TUNE_COARSE,
	CTRL_OSC_TUNE_FINE,

	CTRL_OSC_TUNE_MAX,
} osc_tune_func_t;

typedef enum {
	CTRL_SELECT_ENV_1,
	CTRL_SELECT_ENV_2,

	CTRL_SELECT_ENV_MAX,
} env_attack_func_t;

typedef enum {
	CTRL_ENV_SUSTAIN,
	CTRL_ENV_AMOUNT,

	CTRL_ENV_SUSTAIN_FUNC_MAX,
} env_sustain_func_t;


typedef struct {
	uint16_t osc1_saw_lvl;
	uint16_t osc1_squ_lvl;
	uint16_t osc1_squ_pwm;
	uint16_t osc1_to_osc2;

	uint16_t osc1_tune_coarse;
	uint16_t osc1_tune_fine;
	uint16_t osc1_filt_cutoff;
	uint16_t osc1_filt_res;
	uint16_t osc1_drive;

	uint16_t osc2_saw_lvl;
	uint16_t osc2_squ_lvl;
	uint16_t osc2_squ_pwm;
	uint16_t osc2_noise_lvl;

	uint16_t osc2_filt_cutoff;
	uint16_t osc2_filt_res;
	uint16_t osc2_drive;

	uint16_t osc_filt_env1_a;
	uint16_t osc_filt_env1_d;
	uint16_t osc_filt_env1_s;
	uint16_t osc_filt_env1_r;

	uint16_t osc_filt_env2_a;
	uint16_t osc_filt_env2_d;
	uint16_t osc_filt_env2_s;
	uint16_t osc_filt_env2_r;

	uint16_t sub_lvl;
	uint16_t sub_noise_lvl;
	uint16_t sub_to_osc2;

	uint16_t sub_filt_cutoff;
	uint16_t sub_filt_res;

	uint16_t sub_filt_env1_a;
	uint16_t sub_filt_env1_d;
	uint16_t sub_filt_env1_s;
	uint16_t sub_filt_env1_r;

	uint16_t sub_filt_env2_a;
	uint16_t sub_filt_env2_d;
	uint16_t sub_filt_env2_s;
	uint16_t sub_filt_env2_r;


} ctrl_value_t;

typedef struct {
	osc_pwm_func_t osc1_squ_func;
	osc_pwm_func_t osc2_squ_func;
	osc_tune_func_t osc1_tune_func;

	env_attack_func_t osc_env_attack_func;
	env_sustain_func_t osc_env_sustain_func;

	env_attack_func_t sub_env_attack_func;
	env_sustain_func_t sub_env_sustain_func;

} ctrl_toggle_t;

ctrl_value_t ctrl_value = {
		.osc1_saw_lvl = 0,
		.osc1_squ_lvl = 0,
		.osc1_squ_pwm = 0,
		.osc1_to_osc2 = 0,

		.osc1_tune_coarse = 0,
		.osc1_tune_fine = 0,
		.osc1_filt_cutoff = 0,
		.osc1_filt_res = 0,
		.osc1_drive = 0,

		.osc2_saw_lvl = 0,
		.osc2_squ_lvl = 0,
		.osc2_noise_lvl = 0,

		.osc2_filt_cutoff = 0,
		.osc2_filt_res = 0,
		.osc2_drive = 0,

		.sub_lvl = 0,
		.sub_noise_lvl = 0,
		.sub_to_osc2 = 0,

		.sub_filt_cutoff = 0,
		.sub_filt_res = 0,

		.osc_filt_env1_a = 0,
		.osc_filt_env1_d = 0,
		.osc_filt_env1_s = 0,
		.osc_filt_env1_r = 0,

		.osc_filt_env2_a = 0,
		.osc_filt_env2_d = 0,
		.osc_filt_env2_s = 0,
		.osc_filt_env2_r = 0,

};

ctrl_toggle_t ctrl_toggle = {
		.osc1_squ_func = 0,
		.osc2_squ_func = 0,
		.osc1_tune_func = 0,

		.osc_env_attack_func = 0,
		.osc_env_sustain_func = 0,
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

	/* OSC 1 */
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

	/* OSC 1 TUNING */
	case CTRL_OSC1_TUNE:
		switch(ctrl_toggle.osc1_tune_func) {
		case CTRL_OSC_TUNE_COARSE:
			_ctrl_apply_delta(&ctrl_value.osc1_tune_coarse, delta, CTRL_DEFAULT_SCALE, -12, 12);
			break;
		case CTRL_OSC_TUNE_FINE:
			_ctrl_apply_delta(&ctrl_value.osc1_tune_fine, delta, CTRL_DEFAULT_SCALE, INT16_MIN, INT16_MAX);
			break;
		default:
			break;
		}
		break;

	/* OSC 1 FILTER AND DRIVE */
	case CTRL_OSC1_FILT_CUTOFF:
		_ctrl_apply_delta(&ctrl_value.osc1_filt_cutoff, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case CTRL_OSC1_FILT_RES:
		_ctrl_apply_delta(&ctrl_value.osc1_filt_res, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case CTRL_OSC1_DRIVE:
		_ctrl_apply_delta(&ctrl_value.osc1_drive, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;

	/* OSC 2 */
	case CTRL_OSC2_SAW:
		_ctrl_apply_delta(&ctrl_value.osc2_saw_lvl, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case CTRL_OSC2_SQU:
		switch(ctrl_toggle.osc2_squ_func) {
		case CTRL_OSC_SQU_LEVEL:
			_ctrl_apply_delta(&ctrl_value.osc2_squ_lvl, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		case CTRL_OSC_SQL_PWM:
			_ctrl_apply_delta(&ctrl_value.osc2_squ_pwm, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		default:
			break;
		}
		break;
	case CTRL_OSC2_NOISE:
		_ctrl_apply_delta(&ctrl_value.osc2_noise_lvl, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;

	/* OSC 2 FILTER AND DRIVE */
	case CTRL_OSC2_FILT_CUTOFF:
		_ctrl_apply_delta(&ctrl_value.osc2_filt_cutoff, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case CTRL_OSC2_FILT_RES:
		_ctrl_apply_delta(&ctrl_value.osc2_filt_res, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case CTRL_OSC2_DRIVE:
		_ctrl_apply_delta(&ctrl_value.osc2_drive, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;

	/* SUB */
	case CTRL_SUB:
		_ctrl_apply_delta(&ctrl_value.sub_lvl, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case CTRL_SUB_NOISE:
		_ctrl_apply_delta(&ctrl_value.sub_noise_lvl, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case CTRL_SUB_TO_OSC2:
		_ctrl_apply_delta(&ctrl_value.sub_to_osc2, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;

	/* SUB FILTER */
	case CTRL_SUB_FILT_CUTOFF:
		_ctrl_apply_delta(&ctrl_value.sub_filt_cutoff, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case CTRL_SUB_FILT_RES:
		_ctrl_apply_delta(&ctrl_value.sub_filt_res, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;

	/* OSC FILTER ADSR ENVELOPE */
	case CTRL_OSC_FILT_ENV_A:
		switch(ctrl_toggle.osc_env_attack_func) {
		case CTRL_SELECT_ENV_1:
			_ctrl_apply_delta(&ctrl_value.osc_filt_env1_a, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		case CTRL_SELECT_ENV_2:
			_ctrl_apply_delta(&ctrl_value.osc_filt_env2_a, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		default:
			break;
		}
		break;
	case CTRL_OSC_FILT_ENV_D:
		switch(ctrl_toggle.osc_env_attack_func) {
		case CTRL_SELECT_ENV_1:
			_ctrl_apply_delta(&ctrl_value.osc_filt_env1_d, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		case CTRL_SELECT_ENV_2:
			_ctrl_apply_delta(&ctrl_value.osc_filt_env2_d, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		default:
			break;
		}
		break;
	case CTRL_OSC_FILT_ENV_S:
		switch(ctrl_toggle.osc_env_attack_func) {
		case CTRL_SELECT_ENV_1:
			_ctrl_apply_delta(&ctrl_value.osc_filt_env1_s, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		case CTRL_SELECT_ENV_2:
			_ctrl_apply_delta(&ctrl_value.osc_filt_env2_s, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		default:
			break;
		}
		break;
	case CTRL_OSC_FILT_ENV_R:
		switch(ctrl_toggle.osc_env_attack_func) {
		case CTRL_SELECT_ENV_1:
			_ctrl_apply_delta(&ctrl_value.osc_filt_env1_r, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		case CTRL_SELECT_ENV_2:
			_ctrl_apply_delta(&ctrl_value.osc_filt_env2_r, delta, CTRL_DEFAULT_SCALE, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		default:
			break;
		}
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
		case CTRL_OSC2_SQU:
			ctrl_toggle.osc2_squ_func++;
			if (ctrl_toggle.osc2_squ_func == CTRL_OSC_SQU_MAX) {
				ctrl_toggle.osc2_squ_func = 0;
			}
			break;
		case CTRL_OSC1_TUNE:
			ctrl_toggle.osc1_tune_func++;
			if (ctrl_toggle.osc1_tune_func == CTRL_OSC_TUNE_MAX) {
				ctrl_toggle.osc1_tune_func = 0;
			}
			break;
		case CTRL_OSC_FILT_ENV_A:
			ctrl_toggle.osc_env_attack_func++;
			if (ctrl_toggle.osc_env_attack_func == CTRL_SELECT_ENV_MAX) {
				ctrl_toggle.osc_env_attack_func = 0;
			}
			break;
		case CTRL_OSC_FILT_ENV_S:
			ctrl_toggle.osc_env_sustain_func++;
			if (ctrl_toggle.osc_env_sustain_func == CTRL_ENV_SUSTAIN_FUNC_MAX) {
				ctrl_toggle.osc_env_sustain_func = 0;
			}
			break;
		default:
			break;
		}
	}
}
