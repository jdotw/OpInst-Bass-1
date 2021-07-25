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

ctrl_t ctrl;
ctrl_toggle_t ctrl_toggle;
bool ctrl_enabled = true;

void ctrl_value_init() {
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

void ctrl_apply_delta(enc_enum_t enc, int8_t delta) {
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
	switch (enc) {

	/* OSC 1 */
	case ENC_OSC1_SAW:
	  if (seq_state.selected_step != UINT8_MAX) {
	    if (!ctrl_changed_ptr->osc1_saw_lvl_changed) {
	      // Establish initial value for the step based on global state
	      ctrl_value_ptr->osc1_saw_lvl = ctrl.value[CTRL_OSC1_SAW_LVL];
	    }
	  }
		_ctrl_apply_delta(&ctrl_value_ptr->osc1_saw_lvl, delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl_changed_ptr->osc1_saw_lvl_changed = true;
		break;
	case ENC_OSC1_SQU:
		switch(ctrl_toggle.osc1_squ_func) {
		case ENC_OSC_SQU_LEVEL:
			_ctrl_apply_delta(&ctrl.value[CTRL_OSC1_SQU_LVL], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_OSC1_SQU_LVL] = true;
			break;
		case ENC_OSC_SQU_PWM:
			_ctrl_apply_delta(&ctrl.value[CTRL_OSC1_SQU_PWM], delta, CTRL_SCALE_TWO_TURNS, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_OSC1_SQU_PWM] = true;
			break;
		default:
			break;
		}
		break;
	case ENC_OSC1_TO_OSC2:
		_ctrl_apply_delta(&ctrl.value[CTRL_OSC1_TO_OSC2], delta, CTRL_SCALE_HALT_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_OSC1_TO_OSC2] = true;
		break;

	/* OSC 1 TUNING */
	case ENC_OSC1_TUNE:
		switch(ctrl_toggle.osc1_tune_func) {
		case ENC_OSC_TUNE_COARSE:
			_ctrl_apply_delta((uint16_t*)&ctrl.value[CTRL_OSC1_TUNE_COARSE], delta, 100, -12, 12);
			ctrl.changed[CTRL_OSC1_TUNE_COARSE] = true;
			break;
		case ENC_OSC_TUNE_FINE:
			_ctrl_apply_delta((uint16_t*)&ctrl.value[CTRL_OSC1_TUNE_FINE], delta, 100, INT16_MIN, INT16_MAX);
			ctrl.changed[CTRL_OSC1_TUNE_FINE] = true;
			break;
		default:
			break;
		}
		break;

	/* OSC 1 FILTER AND DRIVE */
	case ENC_OSC1_FILT_CUTOFF:
		_ctrl_apply_delta(&ctrl.value[CTRL_OSC1_FILT_CUTOFF], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_OSC1_FILT_CUTOFF] = true;
		break;
	case ENC_OSC1_FILT_RES:
		_ctrl_apply_delta(&ctrl.value[CTRL_OSC1_FILT_RES], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_OSC1_FILT_RES] = true;
		break;
	case ENC_OSC1_DRIVE:
		_ctrl_apply_delta(&ctrl.value[CTRL_OSC1_DRIVE], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_OSC1_DRIVE] = true;
		break;

	/* OSC 2 */
	case ENC_OSC2_SAW:
		_ctrl_apply_delta(&ctrl.value[CTRL_OSC2_SAW_LVL], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_OSC2_SAW_LVL] = true;
		break;
	case ENC_OSC2_SQU:
		switch(ctrl_toggle.osc2_squ_func) {
		case ENC_OSC_SQU_LEVEL:
			_ctrl_apply_delta(&ctrl.value[CTRL_OSC2_SQU_LVL], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_OSC2_SQU_LVL] = true;
			break;
		case ENC_OSC_SQU_PWM:
			_ctrl_apply_delta(&ctrl.value[CTRL_OSC2_SQU_PWM], delta, CTRL_SCALE_TWO_TURNS, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_OSC2_SQU_PWM] = true;
			break;
		default:
			break;
		}
		break;
	case ENC_OSC2_NOISE:
		_ctrl_apply_delta(&ctrl.value[CTRL_OSC2_NOISE_LVL], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_OSC2_NOISE_LVL] = true;
		break;

	/* OSC 2 FILTER AND DRIVE */
	case ENC_OSC2_FILT_CUTOFF:
		_ctrl_apply_delta(&ctrl.value[CTRL_OSC2_FILT_CUTOFF], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_OSC2_FILT_CUTOFF] = true;
		break;
	case ENC_OSC2_FILT_RES:
		_ctrl_apply_delta(&ctrl.value[CTRL_OSC2_FILT_RES], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_OSC2_FILT_RES] = true;
		break;
	case ENC_OSC2_DRIVE:
		_ctrl_apply_delta(&ctrl.value[CTRL_OSC2_DRIVE], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_OSC2_DRIVE] = true;
		break;

	/* SUB */
	case ENC_SUB:
		_ctrl_apply_delta(&ctrl.value[CTRL_SUB_LVL], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_SUB_LVL] = true;
		break;
	case ENC_SUB_NOISE:
		_ctrl_apply_delta(&ctrl.value[CTRL_SUB_NOISE_LVL], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_SUB_NOISE_LVL] = true;
		break;
	case ENC_SUB_TO_OSC2:
		_ctrl_apply_delta(&ctrl.value[CTRL_SUB_TO_OSC2], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_SUB_TO_OSC2] = true;
		break;

	/* SUB FILTER */
	case ENC_SUB_FILT_CUTOFF:
		_ctrl_apply_delta(&ctrl.value[CTRL_SUB_FILT_CUTOFF], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_SUB_FILT_CUTOFF] = true;
		break;
	case ENC_SUB_FILT_RES:
		_ctrl_apply_delta(&ctrl.value[CTRL_SUB_FILT_RES], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_SUB_FILT_RES] = true;
		break;

	/* OSC FILTER ADSR ENVELOPE */
	case ENC_OSC_FILT_ENV_A:
		switch(ctrl_toggle.osc_filt_env_attack_func) {
		case ENC_SELECT_ENV_1:
			_ctrl_apply_delta(&ctrl.value[CTRL_OSC_FILT_ENV1_A], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_OSC_FILT_ENV1_A] = true;
			break;
		case ENC_SELECT_ENV_2:
			_ctrl_apply_delta(&ctrl.value[CTRL_OSC_FILT_ENV2_A], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_OSC_FILT_ENV2_A] = true;
			break;
		default:
			break;
		}
		break;
	case ENC_OSC_FILT_ENV_D:
		switch(ctrl_toggle.osc_filt_env_attack_func) {
		case ENC_SELECT_ENV_1:
			_ctrl_apply_delta(&ctrl.value[CTRL_OSC_FILT_ENV1_D], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_OSC_FILT_ENV1_D] = true;
			break;
		case ENC_SELECT_ENV_2:
			_ctrl_apply_delta(&ctrl.value[CTRL_OSC_FILT_ENV2_D], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_OSC_FILT_ENV2_D] = true;
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
				_ctrl_apply_delta(&ctrl.value[CTRL_OSC_FILT_ENV1_S], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				ctrl.changed[CTRL_OSC_FILT_ENV1_S] = true;
				break;
			case ENC_ENV_AMOUNT:
				_ctrl_apply_delta(&ctrl.value[CTRL_OSC_FILT_ENV1_AMT], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				ctrl.changed[CTRL_OSC_FILT_ENV1_AMT] = true;
				break;
			default:
				break;
			}
			break;
		case ENC_SELECT_ENV_2:
			switch(ctrl_toggle.osc_filt_env_sustain_func) {
			case ENC_ENV_SUSTAIN:
				_ctrl_apply_delta(&ctrl.value[CTRL_OSC_FILT_ENV2_S], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				ctrl.changed[CTRL_OSC_FILT_ENV2_S] = true;
				break;
			case ENC_ENV_AMOUNT:
				_ctrl_apply_delta(&ctrl.value[CTRL_OSC_FILT_ENV2_AMT], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				ctrl.changed[CTRL_OSC_FILT_ENV2_AMT] = true;
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
			_ctrl_apply_delta(&ctrl.value[CTRL_OSC_FILT_ENV1_R], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_OSC_FILT_ENV1_R] = true;
			break;
		case ENC_SELECT_ENV_2:
			_ctrl_apply_delta(&ctrl.value[CTRL_OSC_FILT_ENV2_R], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_OSC_FILT_ENV2_R] = true;
			break;
		default:
			break;
		}
		break;

	/* OSC AMP ADSR ENVELOPE */
	case ENC_OSC_AMP_ENV_A:
		_ctrl_apply_delta(&ctrl.value[CTRL_OSC_AMP_ENV_A], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_OSC_AMP_ENV_A] = true;
		break;
	case ENC_OSC_AMP_ENV_D:
		_ctrl_apply_delta(&ctrl.value[CTRL_OSC_AMP_ENV_D], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_OSC_AMP_ENV_D] = true;
		break;
	case ENC_OSC_AMP_ENV_S:
		switch(ctrl_toggle.osc_amp_env_sustain_func) {
		case ENC_ENV_SUSTAIN:
			_ctrl_apply_delta(&ctrl.value[CTRL_OSC_AMP_ENV_S], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_OSC_AMP_ENV_S] = true;
			break;
		case ENC_ENV_AMOUNT:
			_ctrl_apply_delta(&ctrl.value[CTRL_OSC_AMP_ENV_AMT], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_OSC_AMP_ENV_AMT] = true;
			break;
		default:
			break;
		}
		break;
	case ENC_OSC_AMP_ENV_R:
		_ctrl_apply_delta(&ctrl.value[CTRL_OSC_AMP_ENV_R], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_OSC_AMP_ENV_R] = true;
		break;


	/* SUB FILTER ADSR ENVELOPE */
	case ENC_SUB_FILT_ENV_A:
		switch(ctrl_toggle.sub_filt_env_attack_func) {
		case ENC_SELECT_ENV_1:
			_ctrl_apply_delta(&ctrl.value[CTRL_SUB_FILT_ENV1_A], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_SUB_FILT_ENV1_A] = true;
			break;
		case ENC_SELECT_ENV_2:
			_ctrl_apply_delta(&ctrl.value[CTRL_SUB_FILT_ENV2_A], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_SUB_FILT_ENV2_A] = true;
			break;
		default:
			break;
		}
		break;
	case ENC_SUB_FILT_ENV_D:
		switch(ctrl_toggle.sub_filt_env_attack_func) {
		case ENC_SELECT_ENV_1:
			_ctrl_apply_delta(&ctrl.value[CTRL_SUB_FILT_ENV1_D], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_SUB_FILT_ENV1_D] = true;
			break;
		case ENC_SELECT_ENV_2:
			_ctrl_apply_delta(&ctrl.value[CTRL_SUB_FILT_ENV2_D], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_SUB_FILT_ENV2_D] = true;
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
				_ctrl_apply_delta(&ctrl.value[CTRL_SUB_FILT_ENV1_S], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				ctrl.changed[CTRL_SUB_FILT_ENV1_S] = true;
				break;
			case ENC_ENV_AMOUNT:
				_ctrl_apply_delta(&ctrl.value[CTRL_SUB_FILT_ENV1_AMT], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				ctrl.changed[CTRL_SUB_FILT_ENV1_AMT] = true;
				break;
			default:
				break;
			}
			break;
		case ENC_SELECT_ENV_2:
			switch(ctrl_toggle.sub_filt_env_sustain_func) {
			case ENC_ENV_SUSTAIN:
				_ctrl_apply_delta(&ctrl.value[CTRL_SUB_FILT_ENV2_S], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				ctrl.changed[CTRL_SUB_FILT_ENV2_S] = true;
				break;
			case ENC_ENV_AMOUNT:
				_ctrl_apply_delta(&ctrl.value[CTRL_SUB_FILT_ENV2_AMT], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
				ctrl.changed[CTRL_SUB_FILT_ENV2_AMT] = true;
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
			_ctrl_apply_delta(&ctrl.value[CTRL_SUB_FILT_ENV1_R], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_SUB_FILT_ENV1_R] = true;
			break;
		case ENC_SELECT_ENV_2:
			_ctrl_apply_delta(&ctrl.value[CTRL_SUB_FILT_ENV2_R], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_SUB_FILT_ENV2_R] = true;
			break;
		default:
			break;
		}
		break;

	/* SUB AMP ADSR ENVELOPE */
	case ENC_SUB_AMP_ENV_A:
		_ctrl_apply_delta(&ctrl.value[CTRL_SUB_AMP_ENV_A], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_SUB_AMP_ENV_A] = true;
		break;
	case ENC_SUB_AMP_ENV_D:
		_ctrl_apply_delta(&ctrl.value[CTRL_SUB_AMP_ENV_D], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_SUB_AMP_ENV_D] = true;
		break;
	case ENC_SUB_AMP_ENV_S:
		switch(ctrl_toggle.sub_amp_env_sustain_func) {
		case ENC_ENV_SUSTAIN:
			_ctrl_apply_delta(&ctrl.value[CTRL_SUB_AMP_ENV_S], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_SUB_AMP_ENV_S] = true;
			break;
		case ENC_ENV_AMOUNT:
			_ctrl_apply_delta(&ctrl.value[CTRL_SUB_AMP_ENV_AMT], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
			ctrl.changed[CTRL_SUB_AMP_ENV_AMT] = true;
			break;
		default:
			break;
		}
		break;
	case ENC_SUB_AMP_ENV_R:
		_ctrl_apply_delta(&ctrl.value[CTRL_SUB_AMP_ENV_R], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_SUB_AMP_ENV_R] = true;
		break;

	/* FX */
	case ENC_FX_WETDRY:
		_ctrl_apply_delta(&ctrl.value[CTRL_FX_WETDRY], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_FX_WETDRY] = true;
		break;
	case ENC_FX_VAL1:
		_ctrl_apply_delta(&ctrl.value[CTRL_FX_VAL1], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, UINT8_MAX);
		ctrl.changed[CTRL_FX_VAL1] = true;
		break;
	case ENC_FX_VAL2:
		_ctrl_apply_delta(&ctrl.value[CTRL_FX_VAL2], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, UINT8_MAX);
		ctrl.changed[CTRL_FX_VAL2] = true;
		break;
	case ENC_FX_VAL3:
		_ctrl_apply_delta(&ctrl.value[CTRL_FX_VAL3], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, UINT8_MAX);
		ctrl.changed[CTRL_FX_VAL3] = true;
		break;
	case ENC_FX_VAL4:
		_ctrl_apply_delta(&ctrl.value[CTRL_FX_FEEDBACK], delta, CTRL_SCALE_WHOLE_TURN, CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
		ctrl.changed[CTRL_FX_FEEDBACK] = true;
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
