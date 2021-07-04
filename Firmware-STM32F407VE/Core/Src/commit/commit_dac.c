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

	if (commit_ctrl_changed.osc1_filt_res_changed)
	  dac7678_set_value(I2C_LEFT, 0, 0, 0, commit_ctrl_value.osc1_filt_res);
	if (commit_ctrl_changed.osc1_filt_cutoff_changed)
	  dac7678_set_value(I2C_LEFT, 0, 0, 1, commit_ctrl_value.osc1_filt_cutoff);

	if (commit_note_changed.note_number
	    || commit_ctrl_changed.osc1_tune_coarse_changed
	    || commit_ctrl_changed.osc1_tune_fine_changed) {
	  uint8_t osc1_note = commit_note_value.note_number + commit_ctrl_value.osc1_tune_coarse;
	  uint16_t osc1_note_dac_val = osc_dac_value_for_note(OSC1, osc1_note);
	  osc1_note_dac_val += commit_ctrl_value.osc1_tune_fine; // TODO: Handle wrapping, maybe add it to osc1_note_dac_val?
	  dac7678_set_value(I2C_LEFT, 0, 0, 2, osc1_note_dac_val);
	}

  if (commit_ctrl_changed.osc1_to_osc2_changed)
    dac7678_set_value(I2C_LEFT, 0, 0, 3, _dac_lin_to_log(commit_ctrl_value.osc1_to_osc1));
  if (commit_ctrl_changed.osc1_to_osc2_changed)
    dac7678_set_value(I2C_LEFT, 0, 0, 4, _dac_lin_to_log(commit_ctrl_value.osc1_to_osc2));
  if (commit_ctrl_changed.osc1_squ_pwm_changed)
    dac7678_set_value(I2C_LEFT, 0, 0, 5, commit_ctrl_value.osc1_squ_pwm);
  if (commit_ctrl_changed.osc1_saw_lvl_changed)
    dac7678_set_value(I2C_LEFT, 0, 0, 6, _dac_lin_to_log(commit_ctrl_value.osc1_saw_lvl));
  if (commit_ctrl_changed.osc1_squ_lvl_changed)
    dac7678_set_value(I2C_LEFT, 0, 0, 7, _dac_lin_to_log(commit_ctrl_value.osc1_squ_lvl));


//	dac_val[0] = ctrl_value.osc1_filt_res;
//	dac_val[1] = ctrl_value.osc1_filt_cutoff;
//	uint8_t osc1_note = note_value.note_number + ctrl_value.osc1_tune_coarse;
//	uint16_t osc1_note_dac_val = osc_dac_value_for_note(OSC1, osc1_note);
//	osc1_note_dac_val += ctrl_value.osc1_tune_fine; // TODO: Handle wrapping, maybe add it to osc1_note_dac_val?
//	dac_val[2] = osc1_note_dac_val;
//	dac_val[3] = _dac_lin_to_log(ctrl_value.osc1_to_osc1);
//	dac_val[4] = _dac_lin_to_log(ctrl_value.osc1_to_osc2);
//	dac_val[5] = ctrl_value.osc1_squ_pwm;
//	dac_val[6] = _dac_lin_to_log(ctrl_value.osc1_saw_lvl);
//	dac_val[7] = _dac_lin_to_log(ctrl_value.osc1_squ_lvl);
//	res = dac7678_set_value_array(I2C_LEFT, 0, 0, dac_val);
//	if (!res) Error_Handler();

	// Left0:010

  if (commit_ctrl_changed.osc2_noise_lvl_changed)
    dac7678_set_value(I2C_LEFT, 0, 2, 0, _dac_lin_to_log(commit_ctrl_value.osc2_noise_lvl));
  if (commit_ctrl_changed.sub_noise_lvl_changed)
    dac7678_set_value(I2C_LEFT, 0, 2, 1, _dac_lin_to_log(commit_ctrl_value.sub_noise_lvl));
  if (commit_ctrl_changed.sub_lvl_changed)
    dac7678_set_value(I2C_LEFT, 0, 2, 2, _dac_lin_to_log(commit_ctrl_value.sub_lvl));
  if (commit_ctrl_changed.sub_to_osc2_changed)
    dac7678_set_value(I2C_LEFT, 0, 2, 3, _dac_lin_to_log(commit_ctrl_value.sub_to_osc2));
  if (commit_note_changed.note_number)
    dac7678_set_value(I2C_LEFT, 0, 2, 4, osc_dac_value_for_note(OSC2, commit_note_value.note_number));
  if (commit_ctrl_changed.osc2_squ_pwm_changed)
    dac7678_set_value(I2C_LEFT, 0, 2, 5, commit_ctrl_value.osc2_squ_pwm);
  if (commit_ctrl_changed.osc2_squ_lvl_changed)
    dac7678_set_value(I2C_LEFT, 0, 2, 6, _dac_lin_to_log(commit_ctrl_value.osc2_squ_lvl));
  if (commit_ctrl_changed.osc2_saw_lvl_changed)
    dac7678_set_value(I2C_LEFT, 0, 2, 7, _dac_lin_to_log(commit_ctrl_value.osc2_saw_lvl));


//	dac_val[0] = _dac_lin_to_log(ctrl_value.osc2_noise_lvl);
//	dac_val[1] = _dac_lin_to_log(ctrl_value.sub_noise_lvl);
//	dac_val[2] = _dac_lin_to_log(ctrl_value.sub_lvl);
//	dac_val[3] = _dac_lin_to_log(ctrl_value.sub_to_osc2);
//	dac_val[4] = osc_dac_value_for_note(OSC2, note_value.note_number);
//	dac_val[5] = ctrl_value.osc2_squ_pwm;
//	dac_val[6] = _dac_lin_to_log(ctrl_value.osc2_squ_lvl);
//	dac_val[7] = _dac_lin_to_log(ctrl_value.osc2_saw_lvl);
//	res = dac7678_set_value_array(I2C_LEFT, 0, 2, dac_val);
//	if (!res) Error_Handler();

	// Left0:100
  if (commit_ctrl_changed.osc_filt_env1_amt_changed)
    dac7678_set_value(I2C_LEFT, 0, 4, 0, _dac_lin_to_log(CTRL_DEFAULT_MAX - commit_ctrl_value.osc_filt_env1_amt));
  if (commit_ctrl_changed.osc_filt_env2_amt_changed)
    dac7678_set_value(I2C_LEFT, 0, 4, 1, _dac_lin_to_log(CTRL_DEFAULT_MAX - commit_ctrl_value.osc_filt_env2_amt));
  if (commit_ctrl_changed.osc_amp_env_amt_changed)
    dac7678_set_value(I2C_LEFT, 0, 4, 2, _dac_lin_to_log(CTRL_DEFAULT_MAX - commit_ctrl_value.osc_amp_env_amt));
  if (commit_ctrl_changed.sub_amp_env_amt_changed)
    dac7678_set_value(I2C_LEFT, 0, 4, 3, _dac_lin_to_log(CTRL_DEFAULT_MAX - commit_ctrl_value.sub_amp_env_amt));
  if (commit_ctrl_changed.sub_filt_env2_amt_changed)
    dac7678_set_value(I2C_LEFT, 0, 4, 4, _dac_lin_to_log(CTRL_DEFAULT_MAX - commit_ctrl_value.sub_filt_env2_amt));
  if (commit_ctrl_changed.sub_filt_env1_amt_changed)
    dac7678_set_value(I2C_LEFT, 0, 4, 5, _dac_lin_to_log(CTRL_DEFAULT_MAX - commit_ctrl_value.sub_filt_env1_amt));
  if (commit_ctrl_changed.fx_wetdry_changed)
    dac7678_set_value(I2C_LEFT, 0, 4, 6, commit_ctrl_value.fx_wetdry);
  if (commit_ctrl_changed.fx_feedback_changed)
    dac7678_set_value(I2C_LEFT, 0, 4, 7, commit_ctrl_value.fx_feedback);


//	dac_val[0] = _dac_lin_to_log(CTRL_DEFAULT_MAX - ctrl_value.osc_filt_env1_amt);
//	dac_val[1] = _dac_lin_to_log(CTRL_DEFAULT_MAX - ctrl_value.osc_filt_env2_amt);
//	dac_val[2] = _dac_lin_to_log(CTRL_DEFAULT_MAX - ctrl_value.osc_amp_env_amt);
//	dac_val[3] = _dac_lin_to_log(CTRL_DEFAULT_MAX - ctrl_value.sub_amp_env_amt);
//	dac_val[4] = _dac_lin_to_log(CTRL_DEFAULT_MAX - ctrl_value.sub_filt_env2_amt);
//	dac_val[5] = _dac_lin_to_log(CTRL_DEFAULT_MAX - ctrl_value.sub_filt_env1_amt);
//	dac_val[6] = _dac_lin_to_log(ctrl_value.fx_wetdry);
//	dac_val[7] = _dac_lin_to_log(ctrl_value.fx_feedback);
//	res = dac7678_set_value_array(I2C_LEFT, 0, 4, dac_val);
//	if (!res) Error_Handler();

	// Left2:000
  if (commit_ctrl_changed.osc2_filt_res_changed)
    dac7678_set_value(I2C_LEFT, 2, 0, 0, commit_ctrl_value.osc2_filt_res);
  if (commit_ctrl_changed.sub_filt_cutoff_changed)
     dac7678_set_value(I2C_LEFT, 2, 0, 2, commit_ctrl_value.sub_filt_cutoff);
  if (commit_ctrl_changed.sub_filt_res_changed)
     dac7678_set_value(I2C_LEFT, 2, 0, 4, commit_ctrl_value.sub_filt_res);
  if (commit_ctrl_changed.osc2_filt_cutoff_changed)
     dac7678_set_value(I2C_LEFT, 2, 0, 5, commit_ctrl_value.osc2_filt_cutoff);

 //	dac_val[0] = ctrl_value.osc2_filt_res;
//	dac_val[1] = 0; // Not connected
//	dac_val[2] = ctrl_value.sub_filt_cutoff;
//	dac_val[3] = 0; // Not connected
//	dac_val[4] = ctrl_value.sub_filt_res;
//	dac_val[5] = ctrl_value.osc2_filt_cutoff;
//	dac_val[6] = 0; // Not connected
//	dac_val[7] = 0; // Not connected
//	res = dac7678_set_value_array(I2C_LEFT, 2, 0, dac_val);
//	if (!res) Error_Handler();

	// Right2:000
  if (commit_ctrl_changed.osc_filt_env1_r_changed)
    dac7678_set_value(I2C_RIGHT, 2, 0, 0, commit_ctrl_value.osc_filt_env1_r);
  if (commit_ctrl_changed.osc_filt_env2_r_changed)
    dac7678_set_value(I2C_RIGHT, 2, 0, 1, commit_ctrl_value.osc_filt_env2_r);
  if (commit_ctrl_changed.osc_filt_env1_s_changed)
    dac7678_set_value(I2C_RIGHT, 2, 0, 2, commit_ctrl_value.osc_filt_env1_s);
  if (commit_ctrl_changed.osc_filt_env2_s_changed)
    dac7678_set_value(I2C_RIGHT, 2, 0, 3, commit_ctrl_value.osc_filt_env2_s);
  if (commit_ctrl_changed.osc_filt_env1_a_changed)
    dac7678_set_value(I2C_RIGHT, 2, 0, 4, commit_ctrl_value.osc_filt_env1_a);
  if (commit_ctrl_changed.osc_filt_env2_d_changed)
    dac7678_set_value(I2C_RIGHT, 2, 0, 5, commit_ctrl_value.osc_filt_env2_d);
  if (commit_ctrl_changed.osc_filt_env1_d_changed)
    dac7678_set_value(I2C_RIGHT, 2, 0, 6, commit_ctrl_value.osc_filt_env1_d);
  if (commit_ctrl_changed.osc_filt_env2_a_changed)
    dac7678_set_value(I2C_RIGHT, 2, 0, 7, commit_ctrl_value.osc_filt_env2_a);

//	dac_val[0] = ctrl_value.osc_filt_env1_r;
//	dac_val[1] = ctrl_value.osc_filt_env2_r;
//	dac_val[2] = ctrl_value.osc_filt_env1_s;
//	dac_val[3] = ctrl_value.osc_filt_env2_s;
//	dac_val[4] = ctrl_value.osc_filt_env1_a;
//	dac_val[5] = ctrl_value.osc_filt_env2_d;
//	dac_val[6] = ctrl_value.osc_filt_env1_d;
//	dac_val[7] = ctrl_value.osc_filt_env2_a;
//	res = dac7678_set_value_array(I2C_RIGHT, 2, 0, dac_val);
//	if (!res) Error_Handler();

	// Right2:010
  if (commit_ctrl_changed.osc_amp_env_r_changed)
    dac7678_set_value(I2C_RIGHT, 2, 2, 0, commit_ctrl_value.osc_amp_env_r);
  if (commit_ctrl_changed.sub_amp_env_r_changed)
    dac7678_set_value(I2C_RIGHT, 2, 2, 1, commit_ctrl_value.sub_amp_env_r);
  if (commit_ctrl_changed.osc_amp_env_s_changed)
    dac7678_set_value(I2C_RIGHT, 2, 2, 2, commit_ctrl_value.osc_amp_env_s);
  if (commit_ctrl_changed.sub_amp_env_s_changed)
    dac7678_set_value(I2C_RIGHT, 2, 2, 3, commit_ctrl_value.sub_amp_env_s);
  if (commit_ctrl_changed.osc_amp_env_a_changed)
    dac7678_set_value(I2C_RIGHT, 2, 2, 4, commit_ctrl_value.osc_amp_env_a);
  if (commit_ctrl_changed.sub_amp_env_d_changed)
    dac7678_set_value(I2C_RIGHT, 2, 2, 5, commit_ctrl_value.sub_amp_env_d);
  if (commit_ctrl_changed.osc_amp_env_d_changed)
    dac7678_set_value(I2C_RIGHT, 2, 2, 6, commit_ctrl_value.osc_amp_env_d);
  if (commit_ctrl_changed.sub_amp_env_a_changed)
    dac7678_set_value(I2C_RIGHT, 2, 2, 7, commit_ctrl_value.sub_amp_env_a);

//	dac_val[0] = ctrl_value.osc_amp_env_r;
//	dac_val[1] = ctrl_value.sub_amp_env_r;
//	dac_val[2] = ctrl_value.osc_amp_env_s;
//	dac_val[3] = ctrl_value.sub_amp_env_s;
//	dac_val[4] = ctrl_value.osc_amp_env_a;
//	dac_val[5] = ctrl_value.sub_amp_env_d;
//	dac_val[6] = ctrl_value.osc_amp_env_d;
//	dac_val[7] = ctrl_value.sub_amp_env_a;
//	res = dac7678_set_value_array(I2C_RIGHT, 2, 2, dac_val);
//	if (!res) Error_Handler();

	// Right2:100
  if (commit_ctrl_changed.sub_filt_env2_r_changed)
    dac7678_set_value(I2C_RIGHT, 2, 4, 0, commit_ctrl_value.sub_filt_env2_r);
  if (commit_ctrl_changed.sub_filt_env1_r_changed)
    dac7678_set_value(I2C_RIGHT, 2, 4, 1, commit_ctrl_value.sub_filt_env1_r);
  if (commit_ctrl_changed.sub_filt_env2_s_changed)
    dac7678_set_value(I2C_RIGHT, 2, 4, 2, commit_ctrl_value.sub_filt_env2_s);
  if (commit_ctrl_changed.sub_filt_env1_s_changed)
    dac7678_set_value(I2C_RIGHT, 2, 4, 3, commit_ctrl_value.sub_filt_env1_s);
  if (commit_ctrl_changed.sub_filt_env2_a_changed)
    dac7678_set_value(I2C_RIGHT, 2, 4, 4, commit_ctrl_value.sub_filt_env2_a);
  if (commit_ctrl_changed.sub_filt_env1_d_changed)
    dac7678_set_value(I2C_RIGHT, 2, 4, 5, commit_ctrl_value.sub_filt_env1_d);
  if (commit_ctrl_changed.sub_filt_env2_d_changed)
    dac7678_set_value(I2C_RIGHT, 2, 4, 6, commit_ctrl_value.sub_filt_env2_d);
  if (commit_ctrl_changed.sub_filt_env1_a_changed)
    dac7678_set_value(I2C_RIGHT, 2, 4, 7, commit_ctrl_value.sub_filt_env1_a);


//	dac_val[0] = ctrl_value.sub_filt_env2_r;
//	dac_val[1] = ctrl_value.sub_filt_env1_r;
//	dac_val[2] = ctrl_value.sub_filt_env2_s;
//	dac_val[3] = ctrl_value.sub_filt_env1_s;
//	dac_val[4] = ctrl_value.sub_filt_env2_a;
//	dac_val[5] = ctrl_value.sub_filt_env1_d;
//	dac_val[6] = ctrl_value.sub_filt_env2_d;
//	dac_val[7] = ctrl_value.sub_filt_env1_a;
//	res = dac7678_set_value_array(I2C_RIGHT, 2, 4, dac_val);
//	if (!res) Error_Handler();

}

