/*
 * commit_dac.c
 *
 *  Created on: 8 May 2021
 *      Author: jwilson
 */

#include <stdbool.h>
#include "ctrl.h"
#include "main.h"
#include "i2c.h"
#include "dac7678.h"
#include "osc.h"
#include "note.h"
#include "commit.h"
#include <math.h>

uint16_t _dac_lin_to_log(uint16_t input) {
	// Converts a linear scale to the logarithmic scale
	// That is needed to provide linear control through a VCA
	// I worked this out in Number
	// *shrug*

	if (input == 0) return 0;
	else return (log10(input)/log10(2))*(4095/12);
}

void commit_dac() {
  // Reset DACs

	/*
	 * Left0:000
	 */

	if (commit_ctrl.changed[CTRL_OSC1_FILT_RES])
	  dac7678_set_value(I2C_LEFT, 0, 0, 0, commit_ctrl.value[CTRL_OSC1_FILT_RES]);
	if (commit_ctrl.changed[CTRL_OSC1_FILT_CUTOFF])
	  dac7678_set_value(I2C_LEFT, 0, 0, 1, commit_ctrl.value[CTRL_OSC1_FILT_CUTOFF]);

	if (commit_note_value.note_number
	    || commit_ctrl.changed[CTRL_OSC1_TUNE_COARSE]
	    || commit_ctrl.changed[CTRL_OSC1_TUNE_FINE]) {
	  uint8_t osc1_note = commit_note_value.note_number + commit_ctrl.value[CTRL_OSC1_TUNE_COARSE];
	  uint16_t osc1_note_dac_val = osc_dac_value_for_note(OSC1, osc1_note);
	  osc1_note_dac_val += commit_ctrl.value[CTRL_OSC1_TUNE_FINE]; // TODO: Handle wrapping, maybe add it to osc1_note_dac_val?
	  dac7678_set_value(I2C_LEFT, 0, 0, 2, osc1_note_dac_val);
	}

  if (commit_ctrl.changed[CTRL_OSC1_TO_OSC2_MIX])
    dac7678_set_value(I2C_LEFT, 0, 0, 3, _dac_lin_to_log(commit_ctrl.value[CTRL_OSC1_TO_OSC1_MIX]));
  if (commit_ctrl.changed[CTRL_OSC1_TO_OSC2_MIX])
    dac7678_set_value(I2C_LEFT, 0, 0, 4, _dac_lin_to_log(commit_ctrl.value[CTRL_OSC1_TO_OSC2_MIX]));
  if (commit_ctrl.changed[CTRL_OSC1_SQU_PWM])
    dac7678_set_value(I2C_LEFT, 0, 0, 5, commit_ctrl.value[CTRL_OSC1_SQU_PWM]);
  if (commit_ctrl.changed[CTRL_OSC1_SAW_LVL])
    dac7678_set_value(I2C_LEFT, 0, 0, 6, _dac_lin_to_log(commit_ctrl.value[CTRL_OSC1_SAW_LVL]));
  if (commit_ctrl.changed[CTRL_OSC1_SQU_LVL])
    dac7678_set_value(I2C_LEFT, 0, 0, 7, _dac_lin_to_log(commit_ctrl.value[CTRL_OSC1_SQU_LVL]));


	// Left0:010

  if (commit_ctrl.changed[CTRL_OSC2_NOISE_LVL])
    dac7678_set_value(I2C_LEFT, 0, 2, 0, _dac_lin_to_log(commit_ctrl.value[CTRL_OSC2_NOISE_LVL]));
  if (commit_ctrl.changed[CTRL_SUB_NOISE_LVL])
    dac7678_set_value(I2C_LEFT, 0, 2, 1, _dac_lin_to_log(commit_ctrl.value[CTRL_SUB_NOISE_LVL]));
  if (commit_ctrl.changed[CTRL_SUB_LVL])
    dac7678_set_value(I2C_LEFT, 0, 2, 2, _dac_lin_to_log(commit_ctrl.value[CTRL_SUB_LVL]));
  if (commit_ctrl.changed[CTRL_SUB_TO_OSC2_MIX])
    dac7678_set_value(I2C_LEFT, 0, 2, 3, _dac_lin_to_log(commit_ctrl.value[CTRL_SUB_TO_OSC2_MIX]));
  if (commit_note_value.note_number)
    dac7678_set_value(I2C_LEFT, 0, 2, 4, osc_dac_value_for_note(OSC2, commit_note_value.note_number));
  if (commit_ctrl.changed[CTRL_OSC2_SQU_PWM])
    dac7678_set_value(I2C_LEFT, 0, 2, 5, commit_ctrl.value[CTRL_OSC2_SQU_PWM]);
  if (commit_ctrl.changed[CTRL_OSC2_SQU_LVL])
    dac7678_set_value(I2C_LEFT, 0, 2, 6, _dac_lin_to_log(commit_ctrl.value[CTRL_OSC2_SQU_LVL]));
  if (commit_ctrl.changed[CTRL_OSC2_SAW_LVL])
    dac7678_set_value(I2C_LEFT, 0, 2, 7, _dac_lin_to_log(commit_ctrl.value[CTRL_OSC2_SAW_LVL]));

	// Left0:100
  if (commit_ctrl.changed[CTRL_OSC_FILT_ENV1_AMT])
    dac7678_set_value(I2C_LEFT, 0, 4, 0, _dac_lin_to_log(CTRL_DEFAULT_MAX - commit_ctrl.value[CTRL_OSC_FILT_ENV1_AMT]));
  if (commit_ctrl.changed[  CTRL_OSC_FILT_ENV2_AMT])
    dac7678_set_value(I2C_LEFT, 0, 4, 1, _dac_lin_to_log(CTRL_DEFAULT_MAX - commit_ctrl.value[CTRL_OSC_FILT_ENV2_AMT]));
  if (commit_ctrl.changed[CTRL_OSC_AMP_ENV_AMT])
    dac7678_set_value(I2C_LEFT, 0, 4, 2, _dac_lin_to_log(CTRL_DEFAULT_MAX - commit_ctrl.value[CTRL_OSC_AMP_ENV_AMT]));
  if (commit_ctrl.changed[CTRL_SUB_AMP_ENV_AMT])
    dac7678_set_value(I2C_LEFT, 0, 4, 3, _dac_lin_to_log(CTRL_DEFAULT_MAX - commit_ctrl.value[CTRL_SUB_AMP_ENV_AMT]));
  if (commit_ctrl.changed[CTRL_SUB_FILT_ENV2_AMT])
    dac7678_set_value(I2C_LEFT, 0, 4, 4, _dac_lin_to_log(CTRL_DEFAULT_MAX - commit_ctrl.value[CTRL_SUB_FILT_ENV2_AMT]));
  if (commit_ctrl.changed[CTRL_SUB_FILT_ENV1_AMT])
    dac7678_set_value(I2C_LEFT, 0, 4, 5, _dac_lin_to_log(CTRL_DEFAULT_MAX - commit_ctrl.value[CTRL_SUB_FILT_ENV1_AMT]));
  if (commit_ctrl.changed[CTRL_FX_WETDRY])
    dac7678_set_value(I2C_LEFT, 0, 4, 6, commit_ctrl.value[CTRL_FX_WETDRY]);
  if (commit_ctrl.changed[CTRL_FX_FEEDBACK])
    dac7678_set_value(I2C_LEFT, 0, 4, 7, commit_ctrl.value[CTRL_FX_FEEDBACK]);

	// Left2:000
  if (commit_ctrl.changed[CTRL_OSC2_FILT_RES])
    dac7678_set_value(I2C_LEFT, 2, 0, 0, commit_ctrl.value[CTRL_OSC2_FILT_RES]);
  if (commit_ctrl.changed[CTRL_SUB_FILT_CUTOFF])
     dac7678_set_value(I2C_LEFT, 2, 0, 2, commit_ctrl.value[CTRL_SUB_FILT_CUTOFF]);
  if (commit_ctrl.changed[CTRL_SUB_FILT_RES])
     dac7678_set_value(I2C_LEFT, 2, 0, 4, commit_ctrl.value[CTRL_SUB_FILT_RES]);
  if (commit_ctrl.changed[CTRL_OSC2_FILT_CUTOFF])
     dac7678_set_value(I2C_LEFT, 2, 0, 5, commit_ctrl.value[CTRL_OSC2_FILT_CUTOFF]);

	// Right2:000
  if (commit_ctrl.changed[CTRL_OSC_FILT_ENV1_R])
    dac7678_set_value(I2C_RIGHT, 2, 0, 0, commit_ctrl.value[CTRL_OSC_FILT_ENV1_R]);
  if (commit_ctrl.changed[CTRL_OSC_FILT_ENV2_R])
    dac7678_set_value(I2C_RIGHT, 2, 0, 1, commit_ctrl.value[CTRL_OSC_FILT_ENV2_R]);
  if (commit_ctrl.changed[CTRL_OSC_FILT_ENV1_S])
    dac7678_set_value(I2C_RIGHT, 2, 0, 2, commit_ctrl.value[CTRL_OSC_FILT_ENV1_S]);
  if (commit_ctrl.changed[CTRL_OSC_FILT_ENV2_S])
    dac7678_set_value(I2C_RIGHT, 2, 0, 3, commit_ctrl.value[CTRL_OSC_FILT_ENV2_S]);
  if (commit_ctrl.changed[CTRL_OSC_FILT_ENV1_A])
    dac7678_set_value(I2C_RIGHT, 2, 0, 4, commit_ctrl.value[CTRL_OSC_FILT_ENV1_A]);
  if (commit_ctrl.changed[CTRL_OSC_FILT_ENV2_D])
    dac7678_set_value(I2C_RIGHT, 2, 0, 5, commit_ctrl.value[CTRL_OSC_FILT_ENV2_D]);
  if (commit_ctrl.changed[CTRL_OSC_FILT_ENV1_D])
    dac7678_set_value(I2C_RIGHT, 2, 0, 6, commit_ctrl.value[CTRL_OSC_FILT_ENV1_D]);
  if (commit_ctrl.changed[CTRL_OSC_FILT_ENV2_A])
    dac7678_set_value(I2C_RIGHT, 2, 0, 7, commit_ctrl.value[CTRL_OSC_FILT_ENV2_A]);

	// Right2:010
  if (commit_ctrl.changed[CTRL_OSC_AMP_ENV_R])
    dac7678_set_value(I2C_RIGHT, 2, 2, 0, commit_ctrl.value[CTRL_OSC_AMP_ENV_R]);
  if (commit_ctrl.changed[CTRL_SUB_AMP_ENV_R])
    dac7678_set_value(I2C_RIGHT, 2, 2, 1, commit_ctrl.value[CTRL_SUB_AMP_ENV_R]);
  if (commit_ctrl.changed[CTRL_OSC_AMP_ENV_S])
    dac7678_set_value(I2C_RIGHT, 2, 2, 2, commit_ctrl.value[CTRL_OSC_AMP_ENV_S]);
  if (commit_ctrl.changed[CTRL_SUB_AMP_ENV_S])
    dac7678_set_value(I2C_RIGHT, 2, 2, 3, commit_ctrl.value[CTRL_SUB_AMP_ENV_S]);
  if (commit_ctrl.changed[CTRL_OSC_AMP_ENV_A])
    dac7678_set_value(I2C_RIGHT, 2, 2, 4, commit_ctrl.value[CTRL_OSC_AMP_ENV_A]);
  if (commit_ctrl.changed[CTRL_SUB_AMP_ENV_D])
    dac7678_set_value(I2C_RIGHT, 2, 2, 5, commit_ctrl.value[CTRL_SUB_AMP_ENV_D]);
  if (commit_ctrl.changed[CTRL_OSC_AMP_ENV_D])
    dac7678_set_value(I2C_RIGHT, 2, 2, 6, commit_ctrl.value[CTRL_OSC_AMP_ENV_D]);
  if (commit_ctrl.changed[CTRL_SUB_AMP_ENV_A])
    dac7678_set_value(I2C_RIGHT, 2, 2, 7, commit_ctrl.value[CTRL_SUB_AMP_ENV_A]);

	// Right2:100
  if (commit_ctrl.changed[CTRL_SUB_FILT_ENV2_R])
    dac7678_set_value(I2C_RIGHT, 2, 4, 0, commit_ctrl.value[CTRL_SUB_FILT_ENV2_R]);
  if (commit_ctrl.changed[CTRL_SUB_FILT_ENV1_R])
    dac7678_set_value(I2C_RIGHT, 2, 4, 1, commit_ctrl.value[CTRL_SUB_FILT_ENV1_R]);
  if (commit_ctrl.changed[CTRL_SUB_FILT_ENV2_S])
    dac7678_set_value(I2C_RIGHT, 2, 4, 2, commit_ctrl.value[CTRL_SUB_FILT_ENV2_S]);
  if (commit_ctrl.changed[CTRL_SUB_FILT_ENV1_S])
    dac7678_set_value(I2C_RIGHT, 2, 4, 3, commit_ctrl.value[CTRL_SUB_FILT_ENV1_S]);
  if (commit_ctrl.changed[CTRL_SUB_FILT_ENV2_A])
    dac7678_set_value(I2C_RIGHT, 2, 4, 4, commit_ctrl.value[CTRL_SUB_FILT_ENV2_A]);
  if (commit_ctrl.changed[CTRL_SUB_FILT_ENV1_D])
    dac7678_set_value(I2C_RIGHT, 2, 4, 5, commit_ctrl.value[CTRL_SUB_FILT_ENV1_D]);
  if (commit_ctrl.changed[CTRL_SUB_FILT_ENV2_D])
    dac7678_set_value(I2C_RIGHT, 2, 4, 6, commit_ctrl.value[CTRL_SUB_FILT_ENV2_D]);
  if (commit_ctrl.changed[CTRL_SUB_FILT_ENV1_A])
    dac7678_set_value(I2C_RIGHT, 2, 4, 7, commit_ctrl.value[CTRL_SUB_FILT_ENV1_A]);

}

