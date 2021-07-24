/*
 * ctrl.c
 *
 *  Created on: 20 Apr 2021
 *      Author: jwilson
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "ctrl.h"
#include "midi.h"
#include "seq.h"

static const ctrl_value_t _init_ctrl_value = {
		.osc1_saw_lvl = CTRL_DEFAULT_MAX,
		.osc1_squ_pwm = CTRL_DEFAULT_MID,
		.osc1_to_osc1 = CTRL_DEFAULT_MAX,
		.osc1_to_osc2 = CTRL_DEFAULT_MIN,

		.osc2_squ_lvl = CTRL_DEFAULT_MAX,
		.osc2_squ_pwm = CTRL_DEFAULT_MID,

		.sub_lvl = CTRL_DEFAULT_MAX,
		.sub_to_osc2 = CTRL_DEFAULT_MIN,

		.osc1_filt_cutoff = CTRL_DEFAULT_MAX,
		.osc2_filt_cutoff = CTRL_DEFAULT_MAX,
		.sub_filt_cutoff = CTRL_DEFAULT_MAX,

		.fx_wetdry = CTRL_DEFAULT_MID,

		.osc_amp_env_s = CTRL_DEFAULT_MAX,
		.osc_amp_env_amt = CTRL_DEFAULT_MAX,
		.sub_amp_env_s = CTRL_DEFAULT_MAX,
		.sub_amp_env_amt = CTRL_DEFAULT_MAX,

		0.
};

static const ctrl_toggle_t _init_ctrl_toggle = {
		0
};

ctrl_value_t ctrl_value;
ctrl_changed_t ctrl_changed;
ctrl_toggle_t ctrl_toggle;
bool ctrl_enabled = true;

void ctrl_value_init() {
	ctrl_value = _init_ctrl_value;
}

void ctrl_changed_init() {
  memset(&ctrl_changed, 1, sizeof(ctrl_changed_t));
}

void ctrl_changed_reset() {
  ctrl_changed = (const ctrl_changed_t){0};
}

void ctrl_toggle_init() {
	ctrl_toggle = _init_ctrl_toggle;
}

void ctrl_overflow_handler() {
	// TODO: Buzz the haptic
}

void _ctrl_apply_delta(uint16_t *ctrl_ptr, int16_t delta, int16_t scale_percent, uint16_t min_value, uint16_t max_value) {
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
	if (!ctrl_enabled || delta == 0) return;

	ctrl_value_t *ctrl_value_ptr = &ctrl_value;
	ctrl_changed_t *ctrl_changed_ptr = &ctrl_changed;
	if (seq_state.selected_step != UINT8_MAX) {
	  // We have a selected step
	  // The ctrl struct should be the struct
	  // specific for that step -- i.e p-lock
    ctrl_value_ptr = &seq_state.step_ctrl_value[seq_state.selected_step];
    ctrl_changed_ptr = &seq_state.step_ctrl_changed[seq_state.selected_step];
	}
	switch (ctrl) {

	/* OSC 1 */
	case CTRL_OSC1_SAW:
	  if (seq_state.selected_step != UINT8_MAX) {
	    if (!ctrl_changed_ptr->osc1_saw_lvl_changed) {
	      ctrl_value_ptr->osc1_saw_lvl = ctrl_value.osc1_saw_lvl;
	    }
	  }
		_ctrl_apply_delta(&ctrl_value_ptr->osc1_saw_lvl, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed_ptr->osc1_saw_lvl_changed = true;
		break;
	case CTRL_OSC1_SQU:
		switch(ctrl_toggle.osc1_squ_func) {
		case CTRL_OSC_SQU_LEVEL:
			_ctrl_apply_delta(&ctrl_value.osc1_squ_lvl, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.osc1_squ_lvl_changed = true;
			break;
		case CTRL_OSC_SQU_PWM:
			_ctrl_apply_delta(&ctrl_value.osc1_squ_pwm, delta, CTRL_SCALE_TWO_TURNS, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.osc1_squ_pwm_changed = true;
			break;
		default:
			break;
		}
		break;
	case CTRL_OSC1_TO_OSC2:
		_ctrl_apply_delta(&ctrl_value.osc1_to_osc2, delta, CTRL_SCALE_HALT_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.osc1_to_osc2_changed = true;
		break;

	/* OSC 1 TUNING */
	case CTRL_OSC1_TUNE:
		switch(ctrl_toggle.osc1_tune_func) {
		case CTRL_OSC_TUNE_COARSE:
			_ctrl_apply_delta((uint16_t*)&ctrl_value.osc1_tune_coarse, delta, 100, -12, 12);
			ctrl_changed.osc1_tune_coarse_changed = true;
			break;
		case CTRL_OSC_TUNE_FINE:
			_ctrl_apply_delta((uint16_t*)&ctrl_value.osc1_tune_fine, delta, 100, INT16_MIN, INT16_MAX);
			ctrl_changed.osc1_tune_fine_changed = true;
			break;
		default:
			break;
		}
		break;

	/* OSC 1 FILTER AND DRIVE */
	case CTRL_OSC1_FILT_CUTOFF:
		_ctrl_apply_delta(&ctrl_value.osc1_filt_cutoff, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.osc1_filt_cutoff_changed = true;
		break;
	case CTRL_OSC1_FILT_RES:
		_ctrl_apply_delta(&ctrl_value.osc1_filt_res, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.osc1_filt_res_changed = true;
		break;
	case CTRL_OSC1_DRIVE:
		_ctrl_apply_delta(&ctrl_value.osc1_drive, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.osc1_drive_changed = true;
		break;

	/* OSC 2 */
	case CTRL_OSC2_SAW:
		_ctrl_apply_delta(&ctrl_value.osc2_saw_lvl, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.osc2_saw_lvl_changed = true;
		break;
	case CTRL_OSC2_SQU:
		switch(ctrl_toggle.osc2_squ_func) {
		case CTRL_OSC_SQU_LEVEL:
			_ctrl_apply_delta(&ctrl_value.osc2_squ_lvl, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.osc2_squ_lvl_changed = true;
			break;
		case CTRL_OSC_SQU_PWM:
			_ctrl_apply_delta(&ctrl_value.osc2_squ_pwm, delta, CTRL_SCALE_TWO_TURNS, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.osc2_squ_pwm_changed = true;
			break;
		default:
			break;
		}
		break;
	case CTRL_OSC2_NOISE:
		_ctrl_apply_delta(&ctrl_value.osc2_noise_lvl, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.osc2_noise_lvl_changed = true;
		break;

	/* OSC 2 FILTER AND DRIVE */
	case CTRL_OSC2_FILT_CUTOFF:
		_ctrl_apply_delta(&ctrl_value.osc2_filt_cutoff, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.osc2_filt_cutoff_changed = true;
		break;
	case CTRL_OSC2_FILT_RES:
		_ctrl_apply_delta(&ctrl_value.osc2_filt_res, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.osc2_filt_res_changed = true;
		break;
	case CTRL_OSC2_DRIVE:
		_ctrl_apply_delta(&ctrl_value.osc2_drive, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.osc2_drive_changed = true;
		break;

	/* SUB */
	case CTRL_SUB:
		_ctrl_apply_delta(&ctrl_value.sub_lvl, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.sub_lvl_changed = true;
		break;
	case CTRL_SUB_NOISE:
		_ctrl_apply_delta(&ctrl_value.sub_noise_lvl, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.sub_noise_lvl_changed = true;
		break;
	case CTRL_SUB_TO_OSC2:
		_ctrl_apply_delta(&ctrl_value.sub_to_osc2, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.sub_to_osc2_changed = true;
		break;

	/* SUB FILTER */
	case CTRL_SUB_FILT_CUTOFF:
		_ctrl_apply_delta(&ctrl_value.sub_filt_cutoff, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.sub_filt_cutoff_changed = true;
		break;
	case CTRL_SUB_FILT_RES:
		_ctrl_apply_delta(&ctrl_value.sub_filt_res, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.sub_filt_res_changed = true;
		break;

	/* OSC FILTER ADSR ENVELOPE */
	case CTRL_OSC_FILT_ENV_A:
		switch(ctrl_toggle.osc_filt_env_attack_func) {
		case CTRL_SELECT_ENV_1:
			_ctrl_apply_delta(&ctrl_value.osc_filt_env1_a, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.osc_filt_env1_a_changed = true;
			break;
		case CTRL_SELECT_ENV_2:
			_ctrl_apply_delta(&ctrl_value.osc_filt_env2_a, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.osc_filt_env2_a_changed = true;
			break;
		default:
			break;
		}
		break;
	case CTRL_OSC_FILT_ENV_D:
		switch(ctrl_toggle.osc_filt_env_attack_func) {
		case CTRL_SELECT_ENV_1:
			_ctrl_apply_delta(&ctrl_value.osc_filt_env1_d, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.osc_filt_env1_d_changed = true;
			break;
		case CTRL_SELECT_ENV_2:
			_ctrl_apply_delta(&ctrl_value.osc_filt_env2_d, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.osc_filt_env2_d_changed = true;
			break;
		default:
			break;
		}
		break;
	case CTRL_OSC_FILT_ENV_S:
		switch(ctrl_toggle.osc_filt_env_attack_func) {
		case CTRL_SELECT_ENV_1:
			switch(ctrl_toggle.osc_filt_env_sustain_func) {
			case CTRL_ENV_SUSTAIN:
				_ctrl_apply_delta(&ctrl_value.osc_filt_env1_s, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				ctrl_changed.osc_filt_env1_s_changed = true;
				break;
			case CTRL_ENV_AMOUNT:
				_ctrl_apply_delta(&ctrl_value.osc_filt_env1_amt, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				ctrl_changed.osc_filt_env1_amt_changed = true;
				break;
			default:
				break;
			}
			break;
		case CTRL_SELECT_ENV_2:
			switch(ctrl_toggle.osc_filt_env_sustain_func) {
			case CTRL_ENV_SUSTAIN:
				_ctrl_apply_delta(&ctrl_value.osc_filt_env2_s, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				ctrl_changed.osc_filt_env2_s_changed = true;
				break;
			case CTRL_ENV_AMOUNT:
				_ctrl_apply_delta(&ctrl_value.osc_filt_env2_amt, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				ctrl_changed.osc_filt_env2_amt_changed = true;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case CTRL_OSC_FILT_ENV_R:
		switch(ctrl_toggle.osc_filt_env_attack_func) {
		case CTRL_SELECT_ENV_1:
			_ctrl_apply_delta(&ctrl_value.osc_filt_env1_r, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.osc_filt_env1_r_changed = true;
			break;
		case CTRL_SELECT_ENV_2:
			_ctrl_apply_delta(&ctrl_value.osc_filt_env2_r, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.osc_filt_env2_r_changed = true;
			break;
		default:
			break;
		}
		break;

	/* OSC AMP ADSR ENVELOPE */
	case CTRL_OSC_AMP_ENV_A:
		_ctrl_apply_delta(&ctrl_value.osc_amp_env_a, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.osc_amp_env_a_changed = true;
		break;
	case CTRL_OSC_AMP_ENV_D:
		_ctrl_apply_delta(&ctrl_value.osc_amp_env_d, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.osc_amp_env_d_changed = true;
		break;
	case CTRL_OSC_AMP_ENV_S:
		switch(ctrl_toggle.osc_amp_env_sustain_func) {
		case CTRL_ENV_SUSTAIN:
			_ctrl_apply_delta(&ctrl_value.osc_amp_env_s, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.osc_amp_env_s_changed = true;
			break;
		case CTRL_ENV_AMOUNT:
			_ctrl_apply_delta(&ctrl_value.osc_amp_env_amt, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.osc_amp_env_amt_changed = true;
			break;
		default:
			break;
		}
		break;
	case CTRL_OSC_AMP_ENV_R:
		_ctrl_apply_delta(&ctrl_value.osc_amp_env_r, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.osc_amp_env_r_changed = true;
		break;


	/* SUB FILTER ADSR ENVELOPE */
	case CTRL_SUB_FILT_ENV_A:
		switch(ctrl_toggle.sub_filt_env_attack_func) {
		case CTRL_SELECT_ENV_1:
			_ctrl_apply_delta(&ctrl_value.sub_filt_env1_a, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.sub_filt_env1_a_changed = true;
			break;
		case CTRL_SELECT_ENV_2:
			_ctrl_apply_delta(&ctrl_value.sub_filt_env2_a, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.sub_filt_env2_a_changed = true;
			break;
		default:
			break;
		}
		break;
	case CTRL_SUB_FILT_ENV_D:
		switch(ctrl_toggle.sub_filt_env_attack_func) {
		case CTRL_SELECT_ENV_1:
			_ctrl_apply_delta(&ctrl_value.sub_filt_env1_d, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.sub_filt_env1_d_changed = true;
			break;
		case CTRL_SELECT_ENV_2:
			_ctrl_apply_delta(&ctrl_value.sub_filt_env2_d, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.sub_filt_env2_d_changed = true;
			break;
		default:
			break;
		}
		break;
	case CTRL_SUB_FILT_ENV_S:
		switch(ctrl_toggle.sub_filt_env_attack_func) {
		case CTRL_SELECT_ENV_1:
			switch(ctrl_toggle.sub_filt_env_sustain_func) {
			case CTRL_ENV_SUSTAIN:
				_ctrl_apply_delta(&ctrl_value.sub_filt_env1_s, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				ctrl_changed.sub_filt_env1_s_changed = true;
				break;
			case CTRL_ENV_AMOUNT:
				_ctrl_apply_delta(&ctrl_value.sub_filt_env1_amt, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				ctrl_changed.sub_filt_env1_amt_changed = true;
				break;
			default:
				break;
			}
			break;
		case CTRL_SELECT_ENV_2:
			switch(ctrl_toggle.sub_filt_env_sustain_func) {
			case CTRL_ENV_SUSTAIN:
				_ctrl_apply_delta(&ctrl_value.sub_filt_env2_s, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				ctrl_changed.sub_filt_env2_s_changed = true;
				break;
			case CTRL_ENV_AMOUNT:
				_ctrl_apply_delta(&ctrl_value.sub_filt_env2_amt, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				ctrl_changed.sub_filt_env2_amt_changed = true;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case CTRL_SUB_FILT_ENV_R:
		switch(ctrl_toggle.sub_filt_env_attack_func) {
		case CTRL_SELECT_ENV_1:
			_ctrl_apply_delta(&ctrl_value.sub_filt_env1_r, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.sub_filt_env1_r_changed = true;
			break;
		case CTRL_SELECT_ENV_2:
			_ctrl_apply_delta(&ctrl_value.sub_filt_env2_r, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.sub_filt_env2_r_changed = true;
			break;
		default:
			break;
		}
		break;

	/* SUB AMP ADSR ENVELOPE */
	case CTRL_SUB_AMP_ENV_A:
		_ctrl_apply_delta(&ctrl_value.sub_amp_env_a, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.sub_amp_env_a_changed = true;
		break;
	case CTRL_SUB_AMP_ENV_D:
		_ctrl_apply_delta(&ctrl_value.sub_amp_env_d, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.sub_amp_env_d_changed = true;
		break;
	case CTRL_SUB_AMP_ENV_S:
		switch(ctrl_toggle.sub_amp_env_sustain_func) {
		case CTRL_ENV_SUSTAIN:
			_ctrl_apply_delta(&ctrl_value.sub_amp_env_s, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.sub_amp_env_s_changed = true;
			break;
		case CTRL_ENV_AMOUNT:
			_ctrl_apply_delta(&ctrl_value.sub_amp_env_amt, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl_changed.sub_amp_env_amt_changed = true;
			break;
		default:
			break;
		}
		break;
	case CTRL_SUB_AMP_ENV_R:
		_ctrl_apply_delta(&ctrl_value.sub_amp_env_r, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.sub_amp_env_r_changed = true;
		break;

	/* FX */
	case CTRL_FX_WETDRY:
		_ctrl_apply_delta(&ctrl_value.fx_wetdry, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.fx_wetdry_changed = true;
		break;
	case CTRL_FX_VAL1:
		_ctrl_apply_delta(&ctrl_value.fx_val1, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, UINT8_MAX);
		ctrl_changed.fx_val1_changed = true;
		break;
	case CTRL_FX_VAL2:
		_ctrl_apply_delta(&ctrl_value.fx_val2, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, UINT8_MAX);
		ctrl_changed.fx_val2_changed = true;
		break;
	case CTRL_FX_VAL3:
		_ctrl_apply_delta(&ctrl_value.fx_val3, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, UINT8_MAX);
		ctrl_changed.fx_val3_changed = true;
		break;
	case CTRL_FX_VAL4:
		_ctrl_apply_delta(&ctrl_value.fx_feedback, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed.fx_feedback_changed = true;
		break;
	}

}

void ctrl_apply_toggle(ctrl_enum_t ctrl, bool changed, bool state) {
	if (ctrl_enabled && state && changed) {
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
			ctrl_toggle.osc_filt_env_attack_func++;
			if (ctrl_toggle.osc_filt_env_attack_func == CTRL_SELECT_ENV_MAX) {
				ctrl_toggle.osc_filt_env_attack_func = 0;
			}
			break;
		case CTRL_OSC_FILT_ENV_S:
			ctrl_toggle.osc_filt_env_sustain_func++;
			if (ctrl_toggle.osc_filt_env_sustain_func == CTRL_ENV_SUSTAIN_FUNC_MAX) {
				ctrl_toggle.osc_filt_env_sustain_func = 0;
			}
			break;
		case CTRL_OSC_AMP_ENV_S:
			ctrl_toggle.osc_amp_env_sustain_func++;
			if (ctrl_toggle.osc_amp_env_sustain_func == CTRL_ENV_SUSTAIN_FUNC_MAX) {
				ctrl_toggle.osc_amp_env_sustain_func = 0;
			}
			break;
		case CTRL_SUB_FILT_ENV_A:
			ctrl_toggle.sub_filt_env_attack_func++;
			if (ctrl_toggle.sub_filt_env_attack_func == CTRL_SELECT_ENV_MAX) {
				ctrl_toggle.sub_filt_env_attack_func = 0;
			}
			break;
		case CTRL_SUB_FILT_ENV_S:
			ctrl_toggle.sub_filt_env_sustain_func++;
			if (ctrl_toggle.sub_filt_env_sustain_func == CTRL_ENV_SUSTAIN_FUNC_MAX) {
				ctrl_toggle.sub_filt_env_sustain_func = 0;
			}
			break;
		case CTRL_SUB_AMP_ENV_S:
			ctrl_toggle.sub_amp_env_sustain_func++;
			if (ctrl_toggle.sub_amp_env_sustain_func == CTRL_ENV_SUSTAIN_FUNC_MAX) {
				ctrl_toggle.sub_amp_env_sustain_func = 0;
			}
			break;
		default:
			break;
		}
	}
}
