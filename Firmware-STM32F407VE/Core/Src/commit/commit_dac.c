/*
 * commit_dac.c
 *
 *  Created on: 8 May 2021
 *      Author: jwilson
 */

#include "commit.h"
#include "ctrl.h"
#include "dac7678.h"
#include "i2c.h"
#include "main.h"
#include "note.h"
#include "osc.h"
#include <math.h>
#include <stdbool.h>

#define VCA_LIN_LOG_OFFSET 2500
#define ENV_AMT_LIN_LOG_OFFSET 0

uint16_t _vca_lin_to_log(uint16_t input) {
  // Converts a linear scale to the logarithmic scale
  // That is needed to provide linear control through a VCA
  // This value has been tuned/tweaked for audio-path VCAs
  // I worked this out in Numbers
  // *shrug*
  if (input == 0)
    return 0;
  else
    return ((log10(input) / log10(2)) * ((4095 - VCA_LIN_LOG_OFFSET) / 12)) +
           VCA_LIN_LOG_OFFSET;
}

uint16_t _env_amt_lin_to_log(uint16_t input) {
  // Converts a linear scale to the logarithmic scale
  // That is needed to provide linear control through a VCA
  // This value has been tuned/tweaked for env amt path VCAs
  // I worked this out in Numbers
  // *shrug*
  if (input == 0)
    return 0;
  else
    return ((log10(input) / log10(2)) *
            ((4095 - ENV_AMT_LIN_LOG_OFFSET) / 12)) +
           ENV_AMT_LIN_LOG_OFFSET;
}

#define FILT_CUTOFF_OFFSET 800.0

uint16_t _filt_cutoff_scale(uint16_t input) {
  if (input == 0)
    return 0;
  double dbl = (((double)input / 4095.0) * (4095.0 - FILT_CUTOFF_OFFSET)) +
               FILT_CUTOFF_OFFSET;
  return (uint16_t)dbl;
}

void commit_dac(ctrl_t *ctrl, note_t *note) {
  // Reset DACs

  /*
   * Left0:000
   */

  if (ctrl->changed[CTRL_OSC1_FILT_RES])
    dac7678_set_value(I2C_LEFT, 0, 0, 0, ctrl->value[CTRL_OSC1_FILT_RES]);
  if (ctrl->changed[CTRL_OSC1_FILT_CUTOFF])
    dac7678_set_value(I2C_LEFT, 0, 0, 1,
                      _filt_cutoff_scale(ctrl->value[CTRL_OSC1_FILT_CUTOFF]));

  if (note->value.note_number || ctrl->changed[CTRL_OSC1_TUNE_COARSE] ||
      ctrl->changed[CTRL_OSC1_TUNE_FINE]) {
    uint8_t osc1_note =
        note->value.note_number + (12 - ctrl->value[CTRL_OSC1_TUNE_COARSE]);
    uint16_t osc1_note_dac_val = osc_dac_value_for_note(OSC1, osc1_note);
    osc1_note_dac_val +=
        ((int16_t)CTRL_DEFAULT_MID -
         ctrl->value[CTRL_OSC1_TUNE_FINE]); // TODO: Handle wrapping, maybe
                                            // add it to osc1_note_dac_val?
    dac7678_set_value(I2C_LEFT, 0, 0, 2, osc1_note_dac_val);
  }

  if (ctrl->changed[CTRL_OSC1_TO_OSC2_MIX])
    dac7678_set_value(I2C_LEFT, 0, 0, 3,
                      _vca_lin_to_log(ctrl->value[CTRL_OSC1_TO_OSC1_MIX]));
  if (ctrl->changed[CTRL_OSC1_TO_OSC2_MIX])
    dac7678_set_value(I2C_LEFT, 0, 0, 4,
                      _vca_lin_to_log(ctrl->value[CTRL_OSC1_TO_OSC2_MIX]));
  if (ctrl->changed[CTRL_OSC1_SQU_PWM])
    dac7678_set_value(I2C_LEFT, 0, 0, 5, ctrl->value[CTRL_OSC1_SQU_PWM]);
  if (ctrl->changed[CTRL_OSC1_SAW_LVL])
    dac7678_set_value(I2C_LEFT, 0, 0, 6,
                      _vca_lin_to_log(ctrl->value[CTRL_OSC1_SAW_LVL]));
  if (ctrl->changed[CTRL_OSC1_SQU_LVL])
    dac7678_set_value(I2C_LEFT, 0, 0, 7,
                      _vca_lin_to_log(ctrl->value[CTRL_OSC1_SQU_LVL]));

  // Left0:010

  if (ctrl->changed[CTRL_OSC2_NOISE_LVL])
    dac7678_set_value(I2C_LEFT, 0, 2, 0,
                      _vca_lin_to_log(ctrl->value[CTRL_OSC2_NOISE_LVL]));
  if (ctrl->changed[CTRL_SUB_NOISE_LVL])
    dac7678_set_value(I2C_LEFT, 0, 2, 1,
                      _vca_lin_to_log(ctrl->value[CTRL_SUB_NOISE_LVL]));
  if (ctrl->changed[CTRL_SUB_LVL])
    dac7678_set_value(I2C_LEFT, 0, 2, 2,
                      _vca_lin_to_log(ctrl->value[CTRL_SUB_LVL]));
  if (ctrl->changed[CTRL_SUB_TO_OSC2_MIX])
    dac7678_set_value(I2C_LEFT, 0, 2, 3,
                      _vca_lin_to_log(ctrl->value[CTRL_SUB_TO_OSC2_MIX]));
  if (note->value.note_number)
    dac7678_set_value(I2C_LEFT, 0, 2, 4,
                      osc_dac_value_for_note(OSC2, note->value.note_number));
  if (ctrl->changed[CTRL_OSC2_SQU_PWM])
    dac7678_set_value(I2C_LEFT, 0, 2, 5, ctrl->value[CTRL_OSC2_SQU_PWM]);
  if (ctrl->changed[CTRL_OSC2_SQU_LVL])
    dac7678_set_value(I2C_LEFT, 0, 2, 6,
                      _vca_lin_to_log(ctrl->value[CTRL_OSC2_SQU_LVL]));
  if (ctrl->changed[CTRL_OSC2_SAW_LVL])
    dac7678_set_value(I2C_LEFT, 0, 2, 7,
                      _vca_lin_to_log(ctrl->value[CTRL_OSC2_SAW_LVL]));

  // Left0:100
  if (ctrl->changed[CTRL_OSC_FILT_ENV1_AMT])
    dac7678_set_value(I2C_LEFT, 0, 4, 0,
                      _env_amt_lin_to_log(CTRL_DEFAULT_MAX -
                                          ctrl->value[CTRL_OSC_FILT_ENV1_AMT]));
  if (ctrl->changed[CTRL_OSC_FILT_ENV2_AMT])
    dac7678_set_value(I2C_LEFT, 0, 4, 1,
                      _env_amt_lin_to_log(CTRL_DEFAULT_MAX -
                                          ctrl->value[CTRL_OSC_FILT_ENV2_AMT]));
  if (ctrl->changed[CTRL_OSC_AMP_ENV_AMT])
    dac7678_set_value(I2C_LEFT, 0, 4, 2,
                      _env_amt_lin_to_log(CTRL_DEFAULT_MAX -
                                          ctrl->value[CTRL_OSC_AMP_ENV_AMT]));
  if (ctrl->changed[CTRL_SUB_AMP_ENV_AMT])
    dac7678_set_value(I2C_LEFT, 0, 4, 3,
                      _env_amt_lin_to_log(CTRL_DEFAULT_MAX -
                                          ctrl->value[CTRL_SUB_AMP_ENV_AMT]));
  if (ctrl->changed[CTRL_SUB_FILT_ENV2_AMT])
    dac7678_set_value(I2C_LEFT, 0, 4, 4,
                      _env_amt_lin_to_log(CTRL_DEFAULT_MAX -
                                          ctrl->value[CTRL_SUB_FILT_ENV2_AMT]));
  if (ctrl->changed[CTRL_SUB_FILT_ENV1_AMT])
    dac7678_set_value(I2C_LEFT, 0, 4, 5,
                      _env_amt_lin_to_log(CTRL_DEFAULT_MAX -
                                          ctrl->value[CTRL_SUB_FILT_ENV1_AMT]));
  if (ctrl->changed[CTRL_FX_WETDRY])
    dac7678_set_value(I2C_LEFT, 0, 4, 6,
                      _vca_lin_to_log(ctrl->value[CTRL_FX_WETDRY]));
  if (ctrl->changed[CTRL_FX_FEEDBACK])
    dac7678_set_value(I2C_LEFT, 0, 4, 7,
                      _vca_lin_to_log(ctrl->value[CTRL_FX_FEEDBACK]));

  // Left2:000
  if (ctrl->changed[CTRL_OSC2_FILT_RES])
    dac7678_set_value(I2C_LEFT, 2, 0, 0, ctrl->value[CTRL_OSC2_FILT_RES]);
  if (ctrl->changed[CTRL_SUB_FILT_CUTOFF])
    dac7678_set_value(I2C_LEFT, 2, 0, 2,
                      _filt_cutoff_scale(ctrl->value[CTRL_SUB_FILT_CUTOFF]));
  if (ctrl->changed[CTRL_SUB_FILT_RES])
    dac7678_set_value(I2C_LEFT, 2, 0, 4, ctrl->value[CTRL_SUB_FILT_RES]);
  if (ctrl->changed[CTRL_OSC2_FILT_CUTOFF])
    dac7678_set_value(I2C_LEFT, 2, 0, 5,
                      _filt_cutoff_scale(ctrl->value[CTRL_OSC2_FILT_CUTOFF]));

  // Right2:000
  if (ctrl->changed[CTRL_OSC_FILT_ENV1_R])
    dac7678_set_value(I2C_RIGHT, 2, 0, 0, ctrl->value[CTRL_OSC_FILT_ENV1_R]);
  if (ctrl->changed[CTRL_OSC_FILT_ENV2_R])
    dac7678_set_value(I2C_RIGHT, 2, 0, 1, ctrl->value[CTRL_OSC_FILT_ENV2_R]);
  if (ctrl->changed[CTRL_OSC_FILT_ENV1_S])
    dac7678_set_value(I2C_RIGHT, 2, 0, 2, ctrl->value[CTRL_OSC_FILT_ENV1_S]);
  if (ctrl->changed[CTRL_OSC_FILT_ENV2_S])
    dac7678_set_value(I2C_RIGHT, 2, 0, 3, ctrl->value[CTRL_OSC_FILT_ENV2_S]);
  if (ctrl->changed[CTRL_OSC_FILT_ENV1_A])
    dac7678_set_value(I2C_RIGHT, 2, 0, 4, ctrl->value[CTRL_OSC_FILT_ENV1_A]);
  if (ctrl->changed[CTRL_OSC_FILT_ENV2_D])
    dac7678_set_value(I2C_RIGHT, 2, 0, 5, ctrl->value[CTRL_OSC_FILT_ENV2_D]);
  if (ctrl->changed[CTRL_OSC_FILT_ENV1_D])
    dac7678_set_value(I2C_RIGHT, 2, 0, 6, ctrl->value[CTRL_OSC_FILT_ENV1_D]);
  if (ctrl->changed[CTRL_OSC_FILT_ENV2_A])
    dac7678_set_value(I2C_RIGHT, 2, 0, 7, ctrl->value[CTRL_OSC_FILT_ENV2_A]);

  // Right2:010
  if (ctrl->changed[CTRL_OSC_AMP_ENV_R])
    dac7678_set_value(I2C_RIGHT, 2, 2, 0, ctrl->value[CTRL_OSC_AMP_ENV_R]);
  if (ctrl->changed[CTRL_SUB_AMP_ENV_R])
    dac7678_set_value(I2C_RIGHT, 2, 2, 1, ctrl->value[CTRL_SUB_AMP_ENV_R]);
  if (ctrl->changed[CTRL_OSC_AMP_ENV_S])
    dac7678_set_value(I2C_RIGHT, 2, 2, 2, ctrl->value[CTRL_OSC_AMP_ENV_S]);
  if (ctrl->changed[CTRL_SUB_AMP_ENV_S])
    dac7678_set_value(I2C_RIGHT, 2, 2, 3, ctrl->value[CTRL_SUB_AMP_ENV_S]);
  if (ctrl->changed[CTRL_OSC_AMP_ENV_A])
    dac7678_set_value(I2C_RIGHT, 2, 2, 4, ctrl->value[CTRL_OSC_AMP_ENV_A]);
  if (ctrl->changed[CTRL_SUB_AMP_ENV_D])
    dac7678_set_value(I2C_RIGHT, 2, 2, 5, ctrl->value[CTRL_SUB_AMP_ENV_D]);
  if (ctrl->changed[CTRL_OSC_AMP_ENV_D])
    dac7678_set_value(I2C_RIGHT, 2, 2, 6, ctrl->value[CTRL_OSC_AMP_ENV_D]);
  if (ctrl->changed[CTRL_SUB_AMP_ENV_A])
    dac7678_set_value(I2C_RIGHT, 2, 2, 7, ctrl->value[CTRL_SUB_AMP_ENV_A]);

  // Right2:100
  if (ctrl->changed[CTRL_SUB_FILT_ENV2_R])
    dac7678_set_value(I2C_RIGHT, 2, 4, 0, ctrl->value[CTRL_SUB_FILT_ENV2_R]);
  if (ctrl->changed[CTRL_SUB_FILT_ENV1_R])
    dac7678_set_value(I2C_RIGHT, 2, 4, 1, ctrl->value[CTRL_SUB_FILT_ENV1_R]);
  if (ctrl->changed[CTRL_SUB_FILT_ENV2_S])
    dac7678_set_value(I2C_RIGHT, 2, 4, 2, ctrl->value[CTRL_SUB_FILT_ENV2_S]);
  if (ctrl->changed[CTRL_SUB_FILT_ENV1_S])
    dac7678_set_value(I2C_RIGHT, 2, 4, 3, ctrl->value[CTRL_SUB_FILT_ENV1_S]);
  if (ctrl->changed[CTRL_SUB_FILT_ENV2_A])
    dac7678_set_value(I2C_RIGHT, 2, 4, 4, ctrl->value[CTRL_SUB_FILT_ENV2_A]);
  if (ctrl->changed[CTRL_SUB_FILT_ENV1_D])
    dac7678_set_value(I2C_RIGHT, 2, 4, 5, ctrl->value[CTRL_SUB_FILT_ENV1_D]);
  if (ctrl->changed[CTRL_SUB_FILT_ENV2_D])
    dac7678_set_value(I2C_RIGHT, 2, 4, 6, ctrl->value[CTRL_SUB_FILT_ENV2_D]);
  if (ctrl->changed[CTRL_SUB_FILT_ENV1_A])
    dac7678_set_value(I2C_RIGHT, 2, 4, 7, ctrl->value[CTRL_SUB_FILT_ENV1_A]);
}
