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

static const ctrl_toggle_t _init_ctrl_toggle = {
		0
};

ctrl_t ctrl;
ctrl_toggle_t ctrl_toggle;
bool ctrl_enabled = true;

void ctrl_value_init() {
  ctrl.value[CTRL_OSC1_SAW_LVL] = CTRL_DEFAULT_MAX;
  ctrl.value[CTRL_OSC1_SQU_PWM] = CTRL_DEFAULT_MID;
  ctrl.value[CTRL_OSC1_TO_OSC1_MIX] = CTRL_DEFAULT_MAX;
  ctrl.value[CTRL_OSC1_TO_OSC2_MIX] = CTRL_DEFAULT_MIN;

  ctrl.value[CTRL_OSC2_SQU_LVL] = CTRL_DEFAULT_MAX;
  ctrl.value[CTRL_OSC2_SQU_PWM] = CTRL_DEFAULT_MID;

  ctrl.value[CTRL_SUB_LVL] = CTRL_DEFAULT_MAX;
  ctrl.value[CTRL_SUB_TO_OSC2_MIX] = CTRL_DEFAULT_MIN;

  ctrl.value[CTRL_OSC1_FILT_CUTOFF] = CTRL_DEFAULT_MAX;
  ctrl.value[CTRL_OSC2_FILT_CUTOFF] = CTRL_DEFAULT_MAX;
  ctrl.value[CTRL_SUB_FILT_CUTOFF] = CTRL_DEFAULT_MAX;

  ctrl.value[CTRL_FX_WETDRY] = CTRL_DEFAULT_MID;

  ctrl.value[CTRL_OSC_AMP_ENV_S] = CTRL_DEFAULT_MAX;
  ctrl.value[CTRL_OSC_AMP_ENV_AMT] = CTRL_DEFAULT_MAX;
  ctrl.value[CTRL_SUB_AMP_ENV_S] = CTRL_DEFAULT_MAX;
  ctrl.value[CTRL_SUB_AMP_ENV_AMT] = CTRL_DEFAULT_MAX;
}

void ctrl_changed_init() {
  memset(&ctrl.changed, 1, sizeof(ctrl.changed));
}

void ctrl_changed_reset() {
  memset(&ctrl.changed, 0, sizeof(ctrl.changed));
}

void ctrl_toggle_init() {
	ctrl_toggle = _init_ctrl_toggle;
}

void ctrl_overflow_handler() {
	// TODO: Buzz the haptic
}

void _ctrl_apply_delta(ctrl_enum_t ctrl_enum, int16_t delta, int16_t scale_percent, uint16_t min_value, uint16_t max_value) {
  ctrl_t *ctrl_ptr = &ctrl;
  if (seq_state.selected_step != UINT8_MAX) {
    // We have a selected step
    // The ctrl struct should be the struct
    // specific for that step -- i.e p-lock
    ctrl_ptr = &seq_state.step_ctrl[seq_state.selected_step];
  }
  if (seq_state.selected_step != UINT8_MAX) {
    if (!ctrl_ptr->changed[ctrl_enum]) {
      // Establish initial value for the step based on global state
      ctrl_ptr->value[ctrl_enum] = ctrl.value[ctrl_enum];
    }
  }

  bool did_overflow = false;
	int16_t scaled_enc_delta = (scale_percent / 100) * delta;
	if (scaled_enc_delta > 0) {
		if (scaled_enc_delta > max_value - ctrl_ptr->value[ctrl_enum]) {
			// Overflow max
			ctrl_ptr->value[ctrl_enum] = max_value;
			did_overflow = true;
		} else {
			ctrl_ptr->value[ctrl_enum] += scaled_enc_delta;
		}
	} else if (scaled_enc_delta < 0) {
		if ((scaled_enc_delta * -1) > ctrl_ptr->value[ctrl_enum]) {
			// Overflow min
			ctrl_ptr->value[ctrl_enum] = min_value;
		} else {
		  ctrl_ptr->value[ctrl_enum] += scaled_enc_delta;
		}
	}
	ctrl_ptr->changed[ctrl_enum] = true;
	if (did_overflow) {
		ctrl_overflow_handler();
	}
}

void ctrl_apply_delta(enc_enum_t enc, int8_t delta) {
	if (!ctrl_enabled || delta == 0) return;


	switch (enc) {
	/* OSC 1 */
	case ENC_OSC1_SAW:
		_ctrl_apply_delta(CTRL_OSC1_SAW_LVL, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case ENC_OSC1_SQU:
		switch(ctrl_toggle.osc1_squ_func) {
		case ENC_OSC_SQU_LEVEL:
			_ctrl_apply_delta(CTRL_OSC1_SQU_LVL, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		case ENC_OSC_SQU_PWM:
			_ctrl_apply_delta(CTRL_OSC1_SQU_PWM, delta, CTRL_SCALE_TWO_TURNS, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		default:
			break;
		}
		break;
	case ENC_OSC1_TO_OSC2:
		_ctrl_apply_delta(CTRL_OSC1_TO_OSC2_MIX, delta, CTRL_SCALE_HALT_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;

	/* OSC 1 TUNING */
	case ENC_OSC1_TUNE:
		switch(ctrl_toggle.osc1_tune_func) {
		case ENC_OSC_TUNE_COARSE:
			_ctrl_apply_delta(CTRL_OSC1_TUNE_COARSE, delta, 100, -12, 12);
			break;
		case ENC_OSC_TUNE_FINE:
			_ctrl_apply_delta(CTRL_OSC1_TUNE_FINE, delta, 100, INT16_MIN, INT16_MAX);
			break;
		default:
			break;
		}
		break;

	/* OSC 1 FILTER AND DRIVE */
	case ENC_OSC1_FILT_CUTOFF:
		_ctrl_apply_delta(CTRL_OSC1_FILT_CUTOFF, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case ENC_OSC1_FILT_RES:
		_ctrl_apply_delta(CTRL_OSC1_FILT_RES, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case ENC_OSC1_DRIVE:
		_ctrl_apply_delta(CTRL_OSC1_DRIVE_AMT, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;

	/* OSC 2 */
	case ENC_OSC2_SAW:
		_ctrl_apply_delta(CTRL_OSC2_SAW_LVL, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case ENC_OSC2_SQU:
		switch(ctrl_toggle.osc2_squ_func) {
		case ENC_OSC_SQU_LEVEL:
			_ctrl_apply_delta(CTRL_OSC2_SQU_LVL, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		case ENC_OSC_SQU_PWM:
			_ctrl_apply_delta(CTRL_OSC2_SQU_PWM, delta, CTRL_SCALE_TWO_TURNS, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		default:
			break;
		}
		break;
	case ENC_OSC2_NOISE:
		_ctrl_apply_delta(CTRL_OSC2_NOISE_LVL, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;

	/* OSC 2 FILTER AND DRIVE */
	case ENC_OSC2_FILT_CUTOFF:
		_ctrl_apply_delta(CTRL_OSC2_FILT_CUTOFF, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case ENC_OSC2_FILT_RES:
		_ctrl_apply_delta(CTRL_OSC2_FILT_RES, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case ENC_OSC2_DRIVE:
		_ctrl_apply_delta(CTRL_OSC2_DRIVE_AMT, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;

	/* SUB */
	case ENC_SUB:
		_ctrl_apply_delta(CTRL_SUB_LVL, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case ENC_SUB_NOISE:
		_ctrl_apply_delta(CTRL_SUB_NOISE_LVL, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case ENC_SUB_TO_OSC2:
		_ctrl_apply_delta(CTRL_SUB_TO_OSC2_MIX, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;

	/* SUB FILTER */
	case ENC_SUB_FILT_CUTOFF:
		_ctrl_apply_delta(CTRL_SUB_FILT_CUTOFF, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case ENC_SUB_FILT_RES:
		_ctrl_apply_delta(CTRL_SUB_FILT_RES, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;

	/* OSC FILTER ADSR ENVELOPE */
	case ENC_OSC_FILT_ENV_A:
		switch(ctrl_toggle.osc_filt_env_attack_func) {
		case ENC_SELECT_ENV_1:
			_ctrl_apply_delta(CTRL_OSC_FILT_ENV1_A, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		case ENC_SELECT_ENV_2:
			_ctrl_apply_delta(CTRL_OSC_FILT_ENV2_A, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		default:
			break;
		}
		break;
	case ENC_OSC_FILT_ENV_D:
		switch(ctrl_toggle.osc_filt_env_attack_func) {
		case ENC_SELECT_ENV_1:
			_ctrl_apply_delta(CTRL_OSC_FILT_ENV1_D, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		case ENC_SELECT_ENV_2:
			_ctrl_apply_delta(CTRL_OSC_FILT_ENV2_D, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		default:
			break;
		}
		break;
	case ENC_OSC_FILT_ENV_S:
		switch(ctrl_toggle.osc_filt_env_attack_func) {
		case ENC_SELECT_ENV_1:
			switch(ctrl_toggle.osc_filt_env_sustain_func) {
			case ENC_ENV_SUSTAIN:
				_ctrl_apply_delta(CTRL_OSC_FILT_ENV1_S, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				break;
			case ENC_ENV_AMOUNT:
				_ctrl_apply_delta(CTRL_OSC_FILT_ENV1_AMT, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				break;
			default:
				break;
			}
			break;
		case ENC_SELECT_ENV_2:
			switch(ctrl_toggle.osc_filt_env_sustain_func) {
			case ENC_ENV_SUSTAIN:
				_ctrl_apply_delta(CTRL_OSC_FILT_ENV2_S, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				break;
			case ENC_ENV_AMOUNT:
				_ctrl_apply_delta(CTRL_OSC_FILT_ENV2_AMT, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case ENC_OSC_FILT_ENV_R:
		switch(ctrl_toggle.osc_filt_env_attack_func) {
		case ENC_SELECT_ENV_1:
			_ctrl_apply_delta(CTRL_OSC_FILT_ENV1_R, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		case ENC_SELECT_ENV_2:
			_ctrl_apply_delta(CTRL_OSC_FILT_ENV2_R, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		default:
			break;
		}
		break;

	/* OSC AMP ADSR ENVELOPE */
	case ENC_OSC_AMP_ENV_A:
		_ctrl_apply_delta(CTRL_OSC_AMP_ENV_A, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case ENC_OSC_AMP_ENV_D:
		_ctrl_apply_delta(CTRL_OSC_AMP_ENV_D, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case ENC_OSC_AMP_ENV_S:
		switch(ctrl_toggle.osc_amp_env_sustain_func) {
		case ENC_ENV_SUSTAIN:
			_ctrl_apply_delta(CTRL_OSC_AMP_ENV_S, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		case ENC_ENV_AMOUNT:
			_ctrl_apply_delta(CTRL_OSC_AMP_ENV_AMT, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		default:
			break;
		}
		break;
	case ENC_OSC_AMP_ENV_R:
		_ctrl_apply_delta(CTRL_OSC_AMP_ENV_R, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;


	/* SUB FILTER ADSR ENVELOPE */
	case ENC_SUB_FILT_ENV_A:
		switch(ctrl_toggle.sub_filt_env_attack_func) {
		case ENC_SELECT_ENV_1:
			_ctrl_apply_delta(CTRL_SUB_FILT_ENV1_A, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		case ENC_SELECT_ENV_2:
			_ctrl_apply_delta(CTRL_SUB_FILT_ENV2_A, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		default:
			break;
		}
		break;
	case ENC_SUB_FILT_ENV_D:
		switch(ctrl_toggle.sub_filt_env_attack_func) {
		case ENC_SELECT_ENV_1:
			_ctrl_apply_delta(CTRL_SUB_FILT_ENV1_D, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		case ENC_SELECT_ENV_2:
			_ctrl_apply_delta(CTRL_SUB_FILT_ENV2_D, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		default:
			break;
		}
		break;
	case ENC_SUB_FILT_ENV_S:
		switch(ctrl_toggle.sub_filt_env_attack_func) {
		case ENC_SELECT_ENV_1:
			switch(ctrl_toggle.sub_filt_env_sustain_func) {
			case ENC_ENV_SUSTAIN:
				_ctrl_apply_delta(CTRL_SUB_FILT_ENV1_S, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				break;
			case ENC_ENV_AMOUNT:
				_ctrl_apply_delta(CTRL_SUB_FILT_ENV1_AMT, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				break;
			default:
				break;
			}
			break;
		case ENC_SELECT_ENV_2:
			switch(ctrl_toggle.sub_filt_env_sustain_func) {
			case ENC_ENV_SUSTAIN:
				_ctrl_apply_delta(CTRL_SUB_FILT_ENV2_S, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				break;
			case ENC_ENV_AMOUNT:
				_ctrl_apply_delta(CTRL_SUB_FILT_ENV2_AMT, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case ENC_SUB_FILT_ENV_R:
		switch(ctrl_toggle.sub_filt_env_attack_func) {
		case ENC_SELECT_ENV_1:
			_ctrl_apply_delta(CTRL_SUB_FILT_ENV1_R, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		case ENC_SELECT_ENV_2:
			_ctrl_apply_delta(CTRL_SUB_FILT_ENV2_R, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		default:
			break;
		}
		break;

	/* SUB AMP ADSR ENVELOPE */
	case ENC_SUB_AMP_ENV_A:
		_ctrl_apply_delta(CTRL_SUB_AMP_ENV_A, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case ENC_SUB_AMP_ENV_D:
		_ctrl_apply_delta(CTRL_SUB_AMP_ENV_D, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case ENC_SUB_AMP_ENV_S:
		switch(ctrl_toggle.sub_amp_env_sustain_func) {
		case ENC_ENV_SUSTAIN:
			_ctrl_apply_delta(CTRL_SUB_AMP_ENV_S, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		case ENC_ENV_AMOUNT:
			_ctrl_apply_delta(CTRL_SUB_AMP_ENV_AMT, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			break;
		default:
			break;
		}
		break;
	case ENC_SUB_AMP_ENV_R:
		_ctrl_apply_delta(CTRL_SUB_AMP_ENV_R, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;

	/* FX */
	case ENC_FX_WETDRY:
		_ctrl_apply_delta(CTRL_FX_WETDRY, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	case ENC_FX_VAL1:
		_ctrl_apply_delta(CTRL_FX_VAL1, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, UINT8_MAX);
		break;
	case ENC_FX_VAL2:
		_ctrl_apply_delta(CTRL_FX_VAL2, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, UINT8_MAX);
		break;
	case ENC_FX_VAL3:
		_ctrl_apply_delta(CTRL_FX_VAL3, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, UINT8_MAX);
		break;
	case ENC_FX_VAL4:
		_ctrl_apply_delta(CTRL_FX_FEEDBACK, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		break;
	}

}

void ctrl_apply_toggle(enc_enum_t ctrl, bool changed, bool state) {
	if (ctrl_enabled && state && changed) {
		switch(ctrl) {
		case ENC_OSC1_SQU:
			ctrl_toggle.osc1_squ_func++;
			if (ctrl_toggle.osc1_squ_func == ENC_OSC_SQU_MAX) {
				ctrl_toggle.osc1_squ_func = 0;
			}
			break;
		case ENC_OSC2_SQU:
			ctrl_toggle.osc2_squ_func++;
			if (ctrl_toggle.osc2_squ_func == ENC_OSC_SQU_MAX) {
				ctrl_toggle.osc2_squ_func = 0;
			}
			break;
		case ENC_OSC1_TUNE:
			ctrl_toggle.osc1_tune_func++;
			if (ctrl_toggle.osc1_tune_func == ENC_OSC_TUNE_MAX) {
				ctrl_toggle.osc1_tune_func = 0;
			}
			break;
		case ENC_OSC_FILT_ENV_A:
			ctrl_toggle.osc_filt_env_attack_func++;
			if (ctrl_toggle.osc_filt_env_attack_func == ENC_SELECT_ENV_MAX) {
				ctrl_toggle.osc_filt_env_attack_func = 0;
			}
			break;
		case ENC_OSC_FILT_ENV_S:
			ctrl_toggle.osc_filt_env_sustain_func++;
			if (ctrl_toggle.osc_filt_env_sustain_func == ENC_ENV_SUSTAIN_FUNC_MAX) {
				ctrl_toggle.osc_filt_env_sustain_func = 0;
			}
			break;
		case ENC_OSC_AMP_ENV_S:
			ctrl_toggle.osc_amp_env_sustain_func++;
			if (ctrl_toggle.osc_amp_env_sustain_func == ENC_ENV_SUSTAIN_FUNC_MAX) {
				ctrl_toggle.osc_amp_env_sustain_func = 0;
			}
			break;
		case ENC_SUB_FILT_ENV_A:
			ctrl_toggle.sub_filt_env_attack_func++;
			if (ctrl_toggle.sub_filt_env_attack_func == ENC_SELECT_ENV_MAX) {
				ctrl_toggle.sub_filt_env_attack_func = 0;
			}
			break;
		case ENC_SUB_FILT_ENV_S:
			ctrl_toggle.sub_filt_env_sustain_func++;
			if (ctrl_toggle.sub_filt_env_sustain_func == ENC_ENV_SUSTAIN_FUNC_MAX) {
				ctrl_toggle.sub_filt_env_sustain_func = 0;
			}
			break;
		case ENC_SUB_AMP_ENV_S:
			ctrl_toggle.sub_amp_env_sustain_func++;
			if (ctrl_toggle.sub_amp_env_sustain_func == ENC_ENV_SUSTAIN_FUNC_MAX) {
				ctrl_toggle.sub_amp_env_sustain_func = 0;
			}
			break;
		default:
			break;
		}
	}
}
