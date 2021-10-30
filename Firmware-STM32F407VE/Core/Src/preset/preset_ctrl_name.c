/*
 * preset.c
 *
 *  Created on: 6 Oct. 2021
 *      Author: jwilson
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "ctrl.h"
#include "main.h"
#include "preset.h"

/*
 * Mapping
 */

const char *_preset_ctrl_name_func(uint8_t i) {
  switch (i) {
  case CTRL_OSC1_SAW_LVL:
    return "osc1_saw_lvl";
  case CTRL_OSC1_SQU_LVL:
    return "osc1_squ_lvl";
  case CTRL_OSC1_SQU_PWM:
    return "osc1_squ_pwm";
  case CTRL_OSC1_TO_OSC1_MIX:
    return "osc1_to_osc1_mix";
  case CTRL_OSC1_TO_OSC2_MIX:
    return "osc1_to_osc2_mix";

  case CTRL_OSC1_TUNE_COARSE:
    return "osc1_tune_coarse";
  case CTRL_OSC1_TUNE_FINE:
    return "osc1_tune_fine";

  case CTRL_OSC1_FILT_CUTOFF:
    return "osc1_filt_cutoff";
  case CTRL_OSC1_FILT_RES:
    return "osc1_filt_res";
  case CTRL_OSC1_DRIVE_AMT:
    return "osc1_drive_amt";

  case CTRL_OSC2_SAW_LVL:
    return "osc2_saw_lvl";
  case CTRL_OSC2_SQU_LVL:
    return "osc2_squ_lvl";
  case CTRL_OSC2_SQU_PWM:
    return "osc2_squ_pwm";
  case CTRL_OSC2_NOISE_LVL:
    return "osc2_noise_lvl";

  case CTRL_OSC2_FILT_CUTOFF:
    return "osc2_filt_cutoff";
  case CTRL_OSC2_FILT_RES:
    return "osc2_filt_res";
  case CTRL_OSC2_DRIVE_AMT:
    return "osc2_drive_amt";

  case CTRL_SUB_LVL:
    return "sub_lvl";
  case CTRL_SUB_NOISE_LVL:
    return "sub_noise_lvl";
  case CTRL_SUB_TO_OSC2_MIX:
    return "sub_to_osc2_mix";

  case CTRL_SUB_FILT_CUTOFF:
    return "sub_filt_cutoff";
  case CTRL_SUB_FILT_RES:
    return "sub_filt_res";

  case CTRL_OSC_FILT_ENV1_A:
    return "osc_filt_env1_a";
  case CTRL_OSC_FILT_ENV1_D:
    return "osc_filt_env1_d";
  case CTRL_OSC_FILT_ENV1_S:
    return "osc_filt_env1_s";
  case CTRL_OSC_FILT_ENV1_R:
    return "osc_filt_env1_r";
  case CTRL_OSC_FILT_ENV1_AMT:
    return "osc_filt_env1_amt";

  case CTRL_OSC_FILT_ENV2_A:
    return "osc_filt_env2_a";
  case CTRL_OSC_FILT_ENV2_D:
    return "osc_filt_env2_d";
  case CTRL_OSC_FILT_ENV2_S:
    return "osc_filt_env2_s";
  case CTRL_OSC_FILT_ENV2_R:
    return "osc_filt_env2_r";
  case CTRL_OSC_FILT_ENV2_AMT:
    return "osc_filt_env2_amt";

  case CTRL_SUB_FILT_ENV1_A:
    return "sub_filt_env1_a";
  case CTRL_SUB_FILT_ENV1_D:
    return "sub_filt_env1_d";
  case CTRL_SUB_FILT_ENV1_S:
    return "sub_filt_env1_s";
  case CTRL_SUB_FILT_ENV1_R:
    return "sub_filt_env1_r";
  case CTRL_SUB_FILT_ENV1_AMT:
    return "sub_filt_env1_a_amt";

  case CTRL_SUB_FILT_ENV2_A:
    return "sub_filt_env2_a";
  case CTRL_SUB_FILT_ENV2_D:
    return "sub_filt_env2_d";
  case CTRL_SUB_FILT_ENV2_S:
    return "sub_filt_env2_s";
  case CTRL_SUB_FILT_ENV2_R:
    return "sub_filt_env2_r";
  case CTRL_SUB_FILT_ENV2_AMT:
    return "sub_filt_env2_amt";

  case CTRL_OSC_AMP_ENV_A:
    return "osc_amp_env_a";
  case CTRL_OSC_AMP_ENV_D:
    return "osc_amp_env_d";
  case CTRL_OSC_AMP_ENV_S:
    return "osc_amp_env_s";
  case CTRL_OSC_AMP_ENV_R:
    return "osc_amp_env_r";
  case CTRL_OSC_AMP_ENV_AMT:
    return "osc_amp_env_amt";

  case CTRL_SUB_AMP_ENV_A:
    return "sub_amp_env_a";
  case CTRL_SUB_AMP_ENV_D:
    return "sub_amp_env_d";
  case CTRL_SUB_AMP_ENV_S:
    return "sub_amp_env_s";
  case CTRL_SUB_AMP_ENV_R:
    return "sub_amp_env_r";
  case CTRL_SUB_AMP_ENV_AMT:
    return "sub_amp_env_amt";

  case CTRL_FX_WETDRY:
    return "fx_wetdry";
  case CTRL_FX_VAL1:
    return "fx_val1";
  case CTRL_FX_VAL2:
    return "fx_val2";
  case CTRL_FX_VAL3:
    return "fx_val3";
  case CTRL_FX_FEEDBACK:
    return "fx_feedback";

  default:
    Error_Handler();
    return NULL;
  }
}

ctrl_enum_t _preset_ctrl_enum(char *name) {
  for (ctrl_enum_t i = 0; i < CTRL_ENUM_MAX; i++) {
    const char *candidate = _preset_ctrl_name_func(i);
    if (strcmp(candidate, name) == 0) {
      return i;
    }
  }
  Error_Handler();
  return CTRL_ENUM_MAX;
}
