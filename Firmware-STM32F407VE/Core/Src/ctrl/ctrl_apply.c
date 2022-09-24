/*
 * ctrl.c
 *
 *  Created on: 20 Apr 2021
 *      Author: jwilson
 */

#include "ctrl.h"
#include "midi.h"
#include "oled.h"
#include "seq.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

void _ctrl_apply_delta(ctrl_enum_t ctrl_enum, int16_t delta,
                       int16_t scale_percent, uint16_t min_value,
                       uint16_t max_value) {
  ctrl_t *ctrl = ctrl_get_active();
  seq_t *seq = seq_get();
  if (seq->state.selected_step != UINT8_MAX) {
    // We have a selected step
    // The ctrl struct should be the struct
    // specific for that step -- i.e p-lock
    ctrl_t *step_ctrl = &seq->state.step_ctrl[seq->state.selected_step];
    if (!step_ctrl->changed[ctrl_enum]) {
      // Establish initial value for the step based on global state
      step_ctrl->value[ctrl_enum] = ctrl->value[ctrl_enum];
    }
    ctrl = step_ctrl;
  }

  bool did_overflow = false;
  int16_t scaled_enc_delta = (scale_percent / 100) * delta;
  if (scaled_enc_delta > 0) {
    if (scaled_enc_delta > max_value - ctrl->value[ctrl_enum]) {
      // Overflow max
      ctrl->value[ctrl_enum] = max_value;
      did_overflow = true;
    } else {
      ctrl->value[ctrl_enum] += scaled_enc_delta;
    }
  } else if (scaled_enc_delta < 0) {
    if ((scaled_enc_delta * -1) > ctrl->value[ctrl_enum]) {
      // Overflow min
      ctrl->value[ctrl_enum] = min_value;
    } else {
      ctrl->value[ctrl_enum] += scaled_enc_delta;
    }
  }
  ctrl->changed[ctrl_enum] = true;
  if (did_overflow) {
    ctrl_overflow_handler();
  }

  // Show control changed screen
  ctrl_changed_screen_push_ctrl(ctrl_enum);
  oled_set_screen(OLED_SCREEN_CTRL, 2000);
}

#define TUNING_ENC_DEBOUNCE_TICKS 10
bool tuning_enc_debounce_forward = false;
uint32_t tuning_enc_debounce = 0;

void ctrl_apply_delta(enc_enum_t enc, int8_t delta) {
  ctrl_toggle_t *toggle = ctrl_get_active_toggle();
  if (!ctrl_get_enabled() || delta == 0)
    return;

  bool is_forward = delta > 0 ? true : false;

  switch (enc) {
  /* OSC 1 */
  case ENC_OSC1_SAW:
    _ctrl_apply_delta(CTRL_OSC1_SAW_LVL, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;
  case ENC_OSC1_SQU:
    switch (toggle->osc1_squ_func) {
    case ENC_OSC_SQU_LEVEL:
      _ctrl_apply_delta(CTRL_OSC1_SQU_LVL, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    case ENC_OSC_SQU_PWM:
      _ctrl_apply_delta(CTRL_OSC1_SQU_PWM, delta, CTRL_SCALE_TWO_TURNS,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    default:
      break;
    }
    break;
  case ENC_OSC1_TO_OSC2:
    _ctrl_apply_delta(CTRL_OSC1_TO_OSC2_MIX, delta, CTRL_SCALE_HALT_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;

  /* OSC 1 TUNING */
  case ENC_OSC1_TUNE:
    if (is_forward == tuning_enc_debounce_forward ||
        (HAL_GetTick() - tuning_enc_debounce) > TUNING_ENC_DEBOUNCE_TICKS) {
      tuning_enc_debounce_forward = is_forward;
      switch (toggle->osc1_tune_func) {
      case ENC_OSC_TUNE_COARSE:
        _ctrl_apply_delta(CTRL_OSC1_TUNE_COARSE, delta, 100, 0, 24);
        break;
      case ENC_OSC_TUNE_FINE:
        _ctrl_apply_delta(CTRL_OSC1_TUNE_FINE, delta, CTRL_SCALE_TWO_TURNS,
                          CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
        break;
      default:
        break;
      }
    }
    tuning_enc_debounce = HAL_GetTick();
    break;

  /* OSC 1 FILTER AND DRIVE */
  case ENC_OSC1_FILT_CUTOFF:
    _ctrl_apply_delta(CTRL_OSC1_FILT_CUTOFF, delta, CTRL_SCALE_FOUR_TURNS,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;
  case ENC_OSC1_FILT_RES:
    _ctrl_apply_delta(CTRL_OSC1_FILT_RES, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;
  case ENC_OSC1_DRIVE:
    _ctrl_apply_delta(CTRL_OSC1_DRIVE_AMT, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;

  /* OSC 2 */
  case ENC_OSC2_SAW:
    _ctrl_apply_delta(CTRL_OSC2_SAW_LVL, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;
  case ENC_OSC2_SQU:
    switch (toggle->osc2_squ_func) {
    case ENC_OSC_SQU_LEVEL:
      _ctrl_apply_delta(CTRL_OSC2_SQU_LVL, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    case ENC_OSC_SQU_PWM:
      _ctrl_apply_delta(CTRL_OSC2_SQU_PWM, delta, CTRL_SCALE_TWO_TURNS,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    default:
      break;
    }
    break;
  case ENC_OSC2_NOISE:
    _ctrl_apply_delta(CTRL_OSC2_NOISE_LVL, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;

  /* OSC 2 FILTER AND DRIVE */
  case ENC_OSC2_FILT_CUTOFF:
    _ctrl_apply_delta(CTRL_OSC2_FILT_CUTOFF, delta, CTRL_SCALE_FOUR_TURNS,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;
  case ENC_OSC2_FILT_RES:
    _ctrl_apply_delta(CTRL_OSC2_FILT_RES, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;
  case ENC_OSC2_DRIVE:
    _ctrl_apply_delta(CTRL_OSC2_DRIVE_AMT, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;

  /* SUB */
  case ENC_SUB:
    _ctrl_apply_delta(CTRL_SUB_LVL, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;
  case ENC_SUB_NOISE:
    _ctrl_apply_delta(CTRL_SUB_NOISE_LVL, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;
  case ENC_SUB_TO_OSC2:
    _ctrl_apply_delta(CTRL_SUB_TO_OSC2_MIX, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;

  /* SUB FILTER */
  case ENC_SUB_FILT_CUTOFF:
    _ctrl_apply_delta(CTRL_SUB_FILT_CUTOFF, delta, CTRL_SCALE_FOUR_TURNS,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;
  case ENC_SUB_FILT_RES:
    _ctrl_apply_delta(CTRL_SUB_FILT_RES, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;

  /* OSC FILTER ADSR ENVELOPE */
  case ENC_OSC_FILT_ENV_A:
    switch (toggle->osc_filt_env_attack_func) {
    case ENC_SELECT_ENV_1:
      _ctrl_apply_delta(CTRL_OSC_FILT_ENV1_A, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    case ENC_SELECT_ENV_2:
      _ctrl_apply_delta(CTRL_OSC_FILT_ENV2_A, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    default:
      break;
    }
    break;
  case ENC_OSC_FILT_ENV_D:
    switch (toggle->osc_filt_env_attack_func) {
    case ENC_SELECT_ENV_1:
      _ctrl_apply_delta(CTRL_OSC_FILT_ENV1_D, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    case ENC_SELECT_ENV_2:
      _ctrl_apply_delta(CTRL_OSC_FILT_ENV2_D, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    default:
      break;
    }
    break;
  case ENC_OSC_FILT_ENV_S:
    switch (toggle->osc_filt_env_attack_func) {
    case ENC_SELECT_ENV_1:
      switch (toggle->osc_filt_env_sustain_func) {
      case ENC_ENV_SUSTAIN:
        _ctrl_apply_delta(CTRL_OSC_FILT_ENV1_S, delta, CTRL_SCALE_WHOLE_TURN,
                          CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
        break;
      case ENC_ENV_AMOUNT:
        _ctrl_apply_delta(CTRL_OSC_FILT_ENV1_AMT, delta, CTRL_SCALE_WHOLE_TURN,
                          CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
        break;
      default:
        break;
      }
      break;
    case ENC_SELECT_ENV_2:
      switch (toggle->osc_filt_env_sustain_func) {
      case ENC_ENV_SUSTAIN:
        _ctrl_apply_delta(CTRL_OSC_FILT_ENV2_S, delta, CTRL_SCALE_WHOLE_TURN,
                          CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
        break;
      case ENC_ENV_AMOUNT:
        _ctrl_apply_delta(CTRL_OSC_FILT_ENV2_AMT, delta, CTRL_SCALE_WHOLE_TURN,
                          CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
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
    switch (toggle->osc_filt_env_attack_func) {
    case ENC_SELECT_ENV_1:
      _ctrl_apply_delta(CTRL_OSC_FILT_ENV1_R, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    case ENC_SELECT_ENV_2:
      _ctrl_apply_delta(CTRL_OSC_FILT_ENV2_R, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    default:
      break;
    }
    break;

  /* OSC AMP ADSR ENVELOPE */
  case ENC_OSC_AMP_ENV_A:
    _ctrl_apply_delta(CTRL_OSC_AMP_ENV_A, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;
  case ENC_OSC_AMP_ENV_D:
    _ctrl_apply_delta(CTRL_OSC_AMP_ENV_D, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;
  case ENC_OSC_AMP_ENV_S:
    switch (toggle->osc_amp_env_sustain_func) {
    case ENC_ENV_SUSTAIN:
      _ctrl_apply_delta(CTRL_OSC_AMP_ENV_S, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    case ENC_ENV_AMOUNT:
      _ctrl_apply_delta(CTRL_OSC_AMP_ENV_AMT, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    default:
      break;
    }
    break;
  case ENC_OSC_AMP_ENV_R:
    _ctrl_apply_delta(CTRL_OSC_AMP_ENV_R, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;

  /* SUB FILTER ADSR ENVELOPE */
  case ENC_SUB_FILT_ENV_A:
    switch (toggle->sub_filt_env_attack_func) {
    case ENC_SELECT_ENV_1:
      _ctrl_apply_delta(CTRL_SUB_FILT_ENV1_A, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    case ENC_SELECT_ENV_2:
      _ctrl_apply_delta(CTRL_SUB_FILT_ENV2_A, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    default:
      break;
    }
    break;
  case ENC_SUB_FILT_ENV_D:
    switch (toggle->sub_filt_env_attack_func) {
    case ENC_SELECT_ENV_1:
      _ctrl_apply_delta(CTRL_SUB_FILT_ENV1_D, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    case ENC_SELECT_ENV_2:
      _ctrl_apply_delta(CTRL_SUB_FILT_ENV2_D, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    default:
      break;
    }
    break;
  case ENC_SUB_FILT_ENV_S:
    switch (toggle->sub_filt_env_attack_func) {
    case ENC_SELECT_ENV_1:
      switch (toggle->sub_filt_env_sustain_func) {
      case ENC_ENV_SUSTAIN:
        _ctrl_apply_delta(CTRL_SUB_FILT_ENV1_S, delta, CTRL_SCALE_WHOLE_TURN,
                          CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
        break;
      case ENC_ENV_AMOUNT:
        _ctrl_apply_delta(CTRL_SUB_FILT_ENV1_AMT, delta, CTRL_SCALE_WHOLE_TURN,
                          CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
        break;
      default:
        break;
      }
      break;
    case ENC_SELECT_ENV_2:
      switch (toggle->sub_filt_env_sustain_func) {
      case ENC_ENV_SUSTAIN:
        _ctrl_apply_delta(CTRL_SUB_FILT_ENV2_S, delta, CTRL_SCALE_WHOLE_TURN,
                          CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
        break;
      case ENC_ENV_AMOUNT:
        _ctrl_apply_delta(CTRL_SUB_FILT_ENV2_AMT, delta, CTRL_SCALE_WHOLE_TURN,
                          CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
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
    switch (toggle->sub_filt_env_attack_func) {
    case ENC_SELECT_ENV_1:
      _ctrl_apply_delta(CTRL_SUB_FILT_ENV1_R, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    case ENC_SELECT_ENV_2:
      _ctrl_apply_delta(CTRL_SUB_FILT_ENV2_R, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    default:
      break;
    }
    break;

  /* SUB AMP ADSR ENVELOPE */
  case ENC_SUB_AMP_ENV_A:
    _ctrl_apply_delta(CTRL_SUB_AMP_ENV_A, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;
  case ENC_SUB_AMP_ENV_D:
    _ctrl_apply_delta(CTRL_SUB_AMP_ENV_D, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;
  case ENC_SUB_AMP_ENV_S:
    switch (toggle->sub_amp_env_sustain_func) {
    case ENC_ENV_SUSTAIN:
      _ctrl_apply_delta(CTRL_SUB_AMP_ENV_S, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    case ENC_ENV_AMOUNT:
      _ctrl_apply_delta(CTRL_SUB_AMP_ENV_AMT, delta, CTRL_SCALE_WHOLE_TURN,
                        CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
      break;
    default:
      break;
    }
    break;
  case ENC_SUB_AMP_ENV_R:
    _ctrl_apply_delta(CTRL_SUB_AMP_ENV_R, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;

  /* FX */
  case ENC_FX_WETDRY:
    _ctrl_apply_delta(CTRL_FX_WETDRY, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;
  case ENC_FX_VAL1:
    _ctrl_apply_delta(CTRL_FX_VAL1, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, UINT8_MAX);
    break;
  case ENC_FX_VAL2:
    _ctrl_apply_delta(CTRL_FX_VAL2, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, UINT8_MAX);
    break;
  case ENC_FX_VAL3:
    _ctrl_apply_delta(CTRL_FX_VAL3, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, UINT8_MAX);
    break;
  case ENC_FX_VAL4:
    _ctrl_apply_delta(CTRL_FX_FEEDBACK, delta, CTRL_SCALE_WHOLE_TURN,
                      CTRL_DEFAULT_MIN, CTRL_DEFAULT_MAX);
    break;
  }
}

void ctrl_apply_toggle(enc_enum_t ctrl_enum, bool changed, bool state) {
  ctrl_toggle_t *toggle = ctrl_get_active_toggle();
  if (ctrl_get_enabled() && state && changed) {
    switch (ctrl_enum) {
    case ENC_OSC1_SQU:
      toggle->osc1_squ_func++;
      if (toggle->osc1_squ_func == ENC_OSC_SQU_MAX) {
        toggle->osc1_squ_func = 0;
      }
      break;
    case ENC_OSC2_SQU:
      toggle->osc2_squ_func++;
      if (toggle->osc2_squ_func == ENC_OSC_SQU_MAX) {
        toggle->osc2_squ_func = 0;
      }
      break;
    case ENC_OSC1_TUNE:
      toggle->osc1_tune_func++;
      if (toggle->osc1_tune_func == ENC_OSC_TUNE_MAX) {
        toggle->osc1_tune_func = 0;
      }
      break;
    case ENC_OSC_FILT_ENV_A:
      toggle->osc_filt_env_attack_func++;
      if (toggle->osc_filt_env_attack_func == ENC_SELECT_ENV_MAX) {
        toggle->osc_filt_env_attack_func = 0;
      }
      break;
    case ENC_OSC_FILT_ENV_S:
      toggle->osc_filt_env_sustain_func++;
      if (toggle->osc_filt_env_sustain_func == ENC_ENV_SUSTAIN_FUNC_MAX) {
        toggle->osc_filt_env_sustain_func = 0;
      }
      break;
    case ENC_OSC_AMP_ENV_S:
      toggle->osc_amp_env_sustain_func++;
      if (toggle->osc_amp_env_sustain_func == ENC_ENV_SUSTAIN_FUNC_MAX) {
        toggle->osc_amp_env_sustain_func = 0;
      }
      break;
    case ENC_SUB_FILT_ENV_A:
      toggle->sub_filt_env_attack_func++;
      if (toggle->sub_filt_env_attack_func == ENC_SELECT_ENV_MAX) {
        toggle->sub_filt_env_attack_func = 0;
      }
      break;
    case ENC_SUB_FILT_ENV_S:
      toggle->sub_filt_env_sustain_func++;
      if (toggle->sub_filt_env_sustain_func == ENC_ENV_SUSTAIN_FUNC_MAX) {
        toggle->sub_filt_env_sustain_func = 0;
      }
      break;
    case ENC_SUB_AMP_ENV_S:
      toggle->sub_amp_env_sustain_func++;
      if (toggle->sub_amp_env_sustain_func == ENC_ENV_SUSTAIN_FUNC_MAX) {
        toggle->sub_amp_env_sustain_func = 0;
      }
      break;
    default:
      break;
    }
  }

  if (ctrl_enum == ENC_FX_WETDRY) {
    if (state && changed) {
      toggle->fx_wetdry_func = ENC_FX_WETDRY_PRESET;
    } else if (changed) {
      toggle->fx_wetdry_func = ENC_FX_WETDRY_FX;
    }
  }
}
