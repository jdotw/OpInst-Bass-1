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

extern ctrl_t ctrl;

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

  ctrl.value[CTRL_FX_WETDRY] = CTRL_DEFAULT_MIN;

  ctrl.value[CTRL_OSC_AMP_ENV_S] = CTRL_DEFAULT_MAX;
  ctrl.value[CTRL_OSC_AMP_ENV_AMT] = CTRL_DEFAULT_MAX;
  ctrl.value[CTRL_SUB_AMP_ENV_S] = CTRL_DEFAULT_MAX;
  ctrl.value[CTRL_SUB_AMP_ENV_AMT] = CTRL_DEFAULT_MAX;

  ctrl.value[CTRL_OSC1_TUNE_COARSE] = 12;
  ctrl.value[CTRL_OSC1_TUNE_FINE] = CTRL_DEFAULT_MID;
}

int8_t ctrl_value_get_percent(ctrl_enum_t ctrl_enum) {
  double percent = (double)ctrl.value[ctrl_enum] / 4095.0;
  return (uint16_t)(percent * 100.0);
}

void ctrl_value_set_min(ctrl_enum_t ctrl_enum) {
  ctrl.value[ctrl_enum] = CTRL_DEFAULT_MIN;
}

void ctrl_value_set_max(ctrl_enum_t ctrl_enum) {
  ctrl.value[ctrl_enum] = CTRL_DEFAULT_MAX;
}
