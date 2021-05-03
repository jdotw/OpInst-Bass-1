/*
 * commit.c
 *
 *  Created on: Apr 25, 2021
 *      Author: jwilson
 */

#include <stdbool.h>
#include "ctrl.h"
#include "main.h"
#include "i2c.h"
#include "dac7678.h"
#include "osc.h"
#include "note.h"
#include <math.h>

void commit_rotic_led(void);
void commit_led_adsr(void);
void _commit_gatetrig(void);

uint16_t _dac_lin_to_log(uint16_t input) {
	// Converts a linear scale to the logarithmic scale
	// That is needed to provide linear control through a VCA
	// I worked this out in Number
	// *shrug*

	if (input == 0) return 0;
	else return (log10(input)/log10(2))*(4095/12);
}

void _commit_dac() {
  // Reset DACs
	HAL_StatusTypeDef res;
	uint16_t dac_val[8];

	// I2C Left 0
	res = i2c_mux_select(0, 0, 0);
	if (res != HAL_OK) Error_Handler();

	// Left0:000
	dac_val[0] = ctrl_value.osc1_filt_res;
	dac_val[1] = ctrl_value.osc1_filt_cutoff;
	uint8_t osc1_note = note_value.note_number + ctrl_value.osc1_tune_coarse;
	uint16_t osc1_note_dac_val = osc_dac_value_for_note(OSC1, osc1_note);
	osc1_note_dac_val += ctrl_value.osc1_tune_fine; // TODO: Handle wrapping, maybe add it to osc1_note_dac_val?
	dac_val[2] = osc1_note_dac_val;
	dac_val[3] = _dac_lin_to_log(ctrl_value.osc1_to_osc1);
	dac_val[4] = _dac_lin_to_log(ctrl_value.osc1_to_osc2);
	dac_val[5] = ctrl_value.osc1_squ_pwm;
	dac_val[6] = _dac_lin_to_log(ctrl_value.osc1_saw_lvl);
	dac_val[7] = _dac_lin_to_log(ctrl_value.osc1_squ_lvl);
	res = dac7678_set_value_array(I2C_LEFT, 0, dac_val);
	if (res != HAL_OK) Error_Handler();

	// Left0:010
	dac_val[0] = _dac_lin_to_log(ctrl_value.osc2_noise_lvl);
	dac_val[1] = _dac_lin_to_log(ctrl_value.sub_noise_lvl);
	dac_val[2] = _dac_lin_to_log(ctrl_value.sub_lvl);
	dac_val[3] = _dac_lin_to_log(ctrl_value.sub_to_osc2);
	dac_val[4] = osc_dac_value_for_note(OSC2, note_value.note_number);
	dac_val[5] = ctrl_value.osc2_squ_pwm;
	dac_val[6] = _dac_lin_to_log(ctrl_value.osc2_squ_lvl);
	dac_val[7] = _dac_lin_to_log(ctrl_value.osc2_saw_lvl);
	res = dac7678_set_value_array(I2C_LEFT, 2, dac_val);
	if (res != HAL_OK) Error_Handler();

	// Left0:100
	dac_val[0] = _dac_lin_to_log(CTRL_DEFAULT_MAX - ctrl_value.osc_filt_env1_amt);
	dac_val[1] = _dac_lin_to_log(CTRL_DEFAULT_MAX - ctrl_value.osc_filt_env2_amt);
	dac_val[2] = _dac_lin_to_log(CTRL_DEFAULT_MAX - ctrl_value.osc_amp_env_amt);
	dac_val[3] = _dac_lin_to_log(CTRL_DEFAULT_MAX - ctrl_value.sub_amp_env_amt);
	dac_val[4] = _dac_lin_to_log(CTRL_DEFAULT_MAX - ctrl_value.sub_filt_env2_amt);
	dac_val[5] = _dac_lin_to_log(CTRL_DEFAULT_MAX - ctrl_value.sub_filt_env1_amt);
	dac_val[6] = _dac_lin_to_log(ctrl_value.fx_wetdry);
	dac_val[7] = _dac_lin_to_log(ctrl_value.fx_feedback);
	res = dac7678_set_value_array(I2C_LEFT, 4, dac_val);
	if (res != HAL_OK) Error_Handler();


	// I2C Left 2
	res = i2c_mux_select(0, 0, 2);
	if (res != HAL_OK) Error_Handler();

	// Left2:000
	dac_val[0] = ctrl_value.osc2_filt_res;
	dac_val[1] = 0; // Not connected
	dac_val[2] = ctrl_value.sub_filt_cutoff;
	dac_val[3] = 0; // Not connected
	dac_val[4] = ctrl_value.sub_filt_res;
	dac_val[5] = ctrl_value.osc2_filt_cutoff;
	dac_val[6] = 0; // Not connected
	dac_val[7] = 0; // Not connected
	res = dac7678_set_value_array(I2C_LEFT, 0, dac_val);
	if (res != HAL_OK) Error_Handler();

	// I2C Right 2
	res = i2c_mux_select(I2C_RIGHT, I2C_RIGHT_MUX, 2);
	if (res != HAL_OK) Error_Handler();

	// Right2:000
	dac_val[0] = ctrl_value.osc_filt_env1_r;
	dac_val[1] = ctrl_value.osc_filt_env2_r;
	dac_val[2] = ctrl_value.osc_filt_env1_s;
	dac_val[3] = ctrl_value.osc_filt_env2_s;
	dac_val[4] = ctrl_value.osc_filt_env1_a;
	dac_val[5] = ctrl_value.osc_filt_env2_d;
	dac_val[6] = ctrl_value.osc_filt_env1_d;
	dac_val[7] = ctrl_value.osc_filt_env2_a;
	res = dac7678_set_value_array(I2C_RIGHT, 0, dac_val);
	if (res != HAL_OK) Error_Handler();

	// Right2:010
	dac_val[0] = ctrl_value.osc_amp_env_r;
	dac_val[1] = ctrl_value.sub_amp_env_r;
	dac_val[2] = ctrl_value.osc_amp_env_s;
	dac_val[3] = ctrl_value.sub_amp_env_s;
	dac_val[4] = ctrl_value.osc_amp_env_a;
	dac_val[5] = ctrl_value.sub_amp_env_d;
	dac_val[6] = ctrl_value.osc_amp_env_d;
	dac_val[7] = ctrl_value.sub_amp_env_a;
	res = dac7678_set_value_array(I2C_RIGHT, 2, dac_val);
	if (res != HAL_OK) Error_Handler();

	// Right2:100
	dac_val[0] = ctrl_value.sub_filt_env2_r;
	dac_val[1] = ctrl_value.sub_filt_env1_r;
	dac_val[2] = ctrl_value.sub_filt_env2_s;
	dac_val[3] = ctrl_value.sub_filt_env1_s;
	dac_val[4] = ctrl_value.sub_filt_env2_a;
	dac_val[5] = ctrl_value.sub_filt_env1_d;
	dac_val[6] = ctrl_value.sub_filt_env2_d;
	dac_val[7] = ctrl_value.sub_filt_env1_a;
	res = dac7678_set_value_array(I2C_RIGHT, 4, dac_val);
	if (res != HAL_OK) Error_Handler();

}

void commit_30hz_timer(void) {
	_commit_dac();
	commit_rotic_led();
	commit_led_adsr();
	_commit_gatetrig();
}
