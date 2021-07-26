/*
 * commit_led_osc.c
 *
 *  Created on: 7 May 2021
 *      Author: jwilson
 */

#include "main.h"
#include "commit.h"
#include "is32.h"
#include "i2c.h"
#include "ctrl.h"

/*
 * RGB Calculations
 */

static uint16_t val[3];

// utils

uint16_t _max16(uint16_t a, uint16_t b) {
  if (a > b) return a;
  else return b;
}

// osc1_saw

uint16_t* _osc1_saw_rgb() {
	val[0] = commit_ctrl.value[CTRL_OSC1_SAW_LVL];
	val[1] = 0;
	val[2] = 0;
	return val;
}

bool _osc1_saw_changed() {
  return commit_ctrl.changed[CTRL_OSC1_SAW_LVL];
}

// osc1_squ

uint16_t* _osc1_squ_rgb() {
	val[0] = 0;
	val[1] = 0;
	val[2] = commit_ctrl.value[CTRL_OSC1_SQU_LVL];
	return val;

}

bool _osc1_squ_changed() {
  return commit_ctrl.changed[CTRL_OSC1_SQU_LVL];
}

// osc1_mix

uint16_t* _osc1_mix_rgb() {
	val[0] = commit_ctrl.value[CTRL_OSC1_SAW_LVL];
	val[1] = 0;
	val[2] = commit_ctrl.value[CTRL_OSC1_SQU_LVL];
	return val;
}

bool _osc1_mix_changed() {
  return commit_ctrl.changed[CTRL_OSC1_SAW_LVL] || commit_ctrl.changed[CTRL_OSC1_SQU_LVL];
}

uint16_t* _osc1_filt_freq_rgb() {
  _osc1_mix_rgb();
  val[0] = (double)val[0] * ((double)(commit_ctrl.value[CTRL_OSC1_FILT_CUTOFF] / 4095.0));
  val[1] = 0;
  val[2] = (double)val[2] * ((double)(commit_ctrl.value[CTRL_OSC1_FILT_CUTOFF] / 4095.0));
  return val;
}

bool _osc1_filt_freq_changed() {
  return _osc1_mix_changed() || commit_ctrl.changed[CTRL_OSC1_FILT_CUTOFF];
}

uint16_t* _osc1_filt_reso_rgb() {
  _osc1_filt_freq_rgb();
  val[0] += (double)(CTRL_DEFAULT_MAX - val[0]) * ((double)(commit_ctrl.value[CTRL_OSC1_FILT_RES] / 4095.0)) * ((double)(commit_ctrl.value[CTRL_OSC1_FILT_CUTOFF] / 4095.0));
  val[1] += (double)(CTRL_DEFAULT_MAX - val[1]) * ((double)(commit_ctrl.value[CTRL_OSC1_FILT_RES] / 4095.0)) * ((double)(commit_ctrl.value[CTRL_OSC1_FILT_CUTOFF] / 4095.0));
  val[2] += (double)(CTRL_DEFAULT_MAX - val[2]) * ((double)(commit_ctrl.value[CTRL_OSC1_FILT_RES] / 4095.0)) * ((double)(commit_ctrl.value[CTRL_OSC1_FILT_CUTOFF] / 4095.0));
  return val;
}

bool _osc1_filt_reso_changed() {
  return _osc1_filt_freq_rgb() || commit_ctrl.changed[CTRL_OSC1_FILT_RES];
}

uint16_t* _osc1_drive_rgb() {
  _osc1_filt_reso_rgb();
  return val;
}

bool _osc1_drive_changed() {
  return _osc1_filt_reso_changed() || commit_ctrl.changed[CTRL_OSC1_DRIVE_AMT];
}


// CTRL_OSC1_TO_OSC2_MIX]_mix

uint16_t* _osc1_to_2_mix() {
	val[0] = (double)commit_ctrl.value[CTRL_OSC1_SAW_LVL] * ((double)commit_ctrl.value[CTRL_OSC1_TO_OSC2_MIX] / 4095.0);
	val[1] = 0;
	val[2] = (double)commit_ctrl.value[CTRL_OSC1_SQU_LVL] * ((double)commit_ctrl.value[CTRL_OSC1_TO_OSC2_MIX] / 4095.0);
	return val;
}

bool _osc1_to_2_mix_changed() {
  return commit_ctrl.changed[CTRL_OSC1_SAW_LVL] || commit_ctrl.changed[CTRL_OSC1_SQU_LVL] || commit_ctrl.changed[CTRL_OSC1_TO_OSC2_MIX];
}

// osc2_saw

uint16_t* _osc2_saw_rgb() {
	val[0] = commit_ctrl.value[CTRL_OSC2_SAW_LVL];
	val[1] = 0;
	val[2] = 0;
	return val;
}

bool _osc2_saw_changed() {
  return commit_ctrl.changed[CTRL_OSC2_SAW_LVL];
}

// osc2_squ

uint16_t* _osc2_squ_rgb() {
	val[0] = 0;
	val[1] = 0;
	val[2] = commit_ctrl.value[CTRL_OSC2_SQU_LVL];
	return val;
}

bool _osc2_squ_changed() {
  return commit_ctrl.changed[CTRL_OSC2_SQU_LVL];
}

// osc2_noise

uint16_t* _osc2_noise_rgb() {
	val[0] = 0;
	val[1] = commit_ctrl.value[CTRL_OSC2_NOISE_LVL];
	val[2] = 0;
	return val;
}

bool _osc2_noise_changed() {
  return commit_ctrl.changed[CTRL_OSC2_NOISE_LVL];
}

// osc2_mix

uint16_t* _osc2_mix() {
	/* Osc2-Only Mix
	 * LEFT0:11
	 * 5, 6, 7, 8
	 */

	val[0] = commit_ctrl.value[CTRL_OSC2_SAW_LVL];
	val[1] = commit_ctrl.value[CTRL_OSC2_NOISE_LVL];
	val[2] = commit_ctrl.value[CTRL_OSC2_SQU_LVL];
	return val;
}

bool _osc2_mix_changed() {
  return commit_ctrl.changed[CTRL_OSC2_SAW_LVL] || commit_ctrl.changed[CTRL_OSC2_NOISE_LVL] || commit_ctrl.changed[CTRL_OSC2_SQU_LVL];
}

// osc2_prefilt

uint16_t* _osc2_prefilt_rgb() {
	val[0] = commit_ctrl.value[CTRL_OSC2_SAW_LVL] | (uint16_t)((double)commit_ctrl.value[CTRL_OSC1_SAW_LVL] * ((double)commit_ctrl.value[CTRL_OSC1_TO_OSC2_MIX] / 4095.0));
	val[1] = commit_ctrl.value[CTRL_OSC2_NOISE_LVL] | (uint16_t)((double)commit_ctrl.value[CTRL_SUB_NOISE_LVL] * ((double)commit_ctrl.value[CTRL_SUB_TO_OSC2_MIX] / 4095.0));
	val[2] = commit_ctrl.value[CTRL_OSC2_SQU_LVL] | (uint16_t)((double)commit_ctrl.value[CTRL_OSC1_SQU_LVL] * ((double)commit_ctrl.value[CTRL_OSC1_TO_OSC2_MIX] / 4095.0)) | (uint16_t)((double)commit_ctrl.value[CTRL_SUB_LVL] * ((double)commit_ctrl.value[CTRL_SUB_TO_OSC2_MIX] / 4095.0));
	return val;
}

bool _osc2_prefilt_changed() {
  return commit_ctrl.changed[CTRL_OSC2_SAW_LVL]
      || commit_ctrl.changed[CTRL_OSC1_TO_OSC2_MIX]
      || commit_ctrl.changed[CTRL_OSC2_NOISE_LVL]
      || commit_ctrl.changed[CTRL_SUB_TO_OSC2_MIX]
      || commit_ctrl.changed[CTRL_OSC2_SQU_LVL]
      || commit_ctrl.changed[CTRL_OSC1_TO_OSC2_MIX];
}

// osc2_filt_freq

uint16_t* _osc2_filt_freq_rgb() {
  _osc2_prefilt_rgb();
  val[0] = (double)val[0] * ((double)(commit_ctrl.value[CTRL_OSC2_FILT_CUTOFF] / 4095.0));
  val[1] = (double)val[1] * ((double)(commit_ctrl.value[CTRL_OSC2_FILT_CUTOFF] / 4095.0));
  val[2] = (double)val[2] * ((double)(commit_ctrl.value[CTRL_OSC2_FILT_CUTOFF] / 4095.0));
	return val;
}

bool _osc2_filt_freq_changed() {
  return _osc2_prefilt_changed() || commit_ctrl.changed[CTRL_OSC2_FILT_CUTOFF];
}

// osc2_filt_reso

uint16_t* _osc2_filt_reso_rgb() {
  _osc2_filt_freq_rgb();
  val[0] += (double)(CTRL_DEFAULT_MAX - val[0]) * ((double)(commit_ctrl.value[CTRL_OSC2_FILT_RES] / 4095.0)) * ((double)(commit_ctrl.value[CTRL_OSC2_FILT_CUTOFF] / 4095.0));
  val[1] += (double)(CTRL_DEFAULT_MAX - val[1]) * ((double)(commit_ctrl.value[CTRL_OSC2_FILT_RES] / 4095.0)) * ((double)(commit_ctrl.value[CTRL_OSC2_FILT_CUTOFF] / 4095.0));
  val[2] += (double)(CTRL_DEFAULT_MAX - val[2]) * ((double)(commit_ctrl.value[CTRL_OSC2_FILT_RES] / 4095.0)) * ((double)(commit_ctrl.value[CTRL_OSC2_FILT_CUTOFF] / 4095.0));
  return val;
}

bool _osc2_filt_reso_changed() {
  return _osc2_filt_freq_changed() || commit_ctrl.changed[CTRL_OSC2_FILT_RES];
}

// osc2_drive

uint16_t* _osc2_drive_rgb() {
  _osc2_filt_reso_rgb();
	return val;
}

bool _osc2_drive_changed() {
  return _osc2_filt_reso_changed() || commit_ctrl.changed[CTRL_OSC2_DRIVE_AMT];
}

// osc_amp

uint16_t* _osc_amp_out_rgb() {
  _osc1_drive_rgb();
  uint16_t osc1_drive[3];
  osc1_drive[0] = val[0];
  osc1_drive[1] = val[1];
  osc1_drive[2] = val[2];
  _osc2_drive_rgb();
  uint16_t osc2_drive[3];
  osc2_drive[0] = val[0];
  osc2_drive[1] = val[1];
  osc2_drive[2] = val[2];
  val[0] = osc1_drive[0] | osc2_drive[0];
  val[1] = osc1_drive[1] | osc2_drive[1];
  val[2] = osc1_drive[2] | osc2_drive[2];
	return val;
}

bool _osc_amp_out_changed() {
  return _osc1_drive_changed() || _osc2_drive_changed();
}

// sub_squ

uint16_t* _sub_squ_rgb() {
	val[0] = 0;
	val[1] = 0;
	val[2] = commit_ctrl.value[CTRL_SUB_LVL];
	return val;
}

bool _sub_squ_changed() {
  return commit_ctrl.changed[CTRL_SUB_LVL];
}

// sub_noise

uint16_t* _sub_noise_rgb() {
	val[0] = 0;
	val[1] = commit_ctrl.value[CTRL_SUB_NOISE_LVL];
	val[2] = 0;
	return val;
}

bool _sub_noise_changed() {
  return commit_ctrl.changed[CTRL_SUB_NOISE_LVL];
}

// sub_mix

uint16_t* _sub_mix_rgb() {
	val[0] = 0;
	val[1] = commit_ctrl.value[CTRL_SUB_NOISE_LVL];
	val[2] = commit_ctrl.value[CTRL_SUB_LVL];
	return val;
}

bool _sub_mix_changed() {
  return commit_ctrl.changed[CTRL_SUB_NOISE_LVL] || commit_ctrl.changed[CTRL_SUB_LVL];
}

// CTRL_SUB_TO_OSC2_MIX]_mix

uint16_t* _sub_to_osc2_mix_rgb() {
	val[0] = 0;
	val[1] = ((double)commit_ctrl.value[CTRL_SUB_NOISE_LVL] * ((double)commit_ctrl.value[CTRL_SUB_TO_OSC2_MIX] / 4095.0));
	val[2] = ((double)commit_ctrl.value[CTRL_SUB_LVL] * ((double)commit_ctrl.value[CTRL_SUB_TO_OSC2_MIX] / 4095.0));
	return val;
}

bool _sub_to_osc2_mix_changed() {
  return commit_ctrl.changed[CTRL_SUB_NOISE_LVL]
      || commit_ctrl.changed[CTRL_SUB_TO_OSC2_MIX]
      || commit_ctrl.changed[CTRL_SUB_LVL];
}

// sub_filt

uint16_t* _sub_filt_freq_rgb() {
  _sub_mix_rgb();
  val[0] = (double)val[0] * ((double)(commit_ctrl.value[CTRL_SUB_FILT_CUTOFF] / 4095.0));
  val[1] = (double)val[1] * ((double)(commit_ctrl.value[CTRL_SUB_FILT_CUTOFF] / 4095.0));
  val[2] = (double)val[2] * ((double)(commit_ctrl.value[CTRL_SUB_FILT_CUTOFF] / 4095.0));
  return val;
}

bool _sub_filt_freq_changed() {
  return _sub_mix_changed() || commit_ctrl.changed[CTRL_SUB_FILT_CUTOFF];
}

uint16_t* _sub_filt_reso_rgb() {
  _sub_filt_freq_rgb();
  val[0] += (double)(CTRL_DEFAULT_MAX - val[0]) * ((double)(commit_ctrl.value[CTRL_SUB_FILT_RES] / 4095.0)) * ((double)(commit_ctrl.value[CTRL_SUB_FILT_CUTOFF] / 4095.0));
  val[1] += (double)(CTRL_DEFAULT_MAX - val[1]) * ((double)(commit_ctrl.value[CTRL_SUB_FILT_RES] / 4095.0)) * ((double)(commit_ctrl.value[CTRL_SUB_FILT_CUTOFF] / 4095.0));
  val[2] += (double)(CTRL_DEFAULT_MAX - val[2]) * ((double)(commit_ctrl.value[CTRL_SUB_FILT_RES] / 4095.0)) * ((double)(commit_ctrl.value[CTRL_SUB_FILT_CUTOFF] / 4095.0));
  return val;
}

bool _sub_filt_reso_changed() {
  return _sub_filt_freq_changed() || commit_ctrl.changed[CTRL_SUB_FILT_RES];
}

uint16_t* _sub_filt_out_rgb() {
	val[0] = 0;
	val[1] = commit_ctrl.value[CTRL_SUB_NOISE_LVL];
	val[2] = commit_ctrl.value[CTRL_SUB_LVL];
	return val;
}

bool _sub_filt_out_changed() {
  return commit_ctrl.changed[CTRL_SUB_NOISE_LVL] || commit_ctrl.changed[CTRL_SUB_LVL];
}

uint16_t* _sub_amp_out_rgb() {
  _sub_filt_reso_rgb();
	return val;
}

bool _sub_amp_out_changed() {
  return _sub_filt_reso_changed();
}

// fx_dry

uint16_t* _fx_dry_rgb() {
	uint16_t osc_amp_out_rgb[3];
	_rgb_copy(osc_amp_out_rgb, _osc_amp_out_rgb());
	uint16_t sub_amp_out_rgb[3];
	_rgb_copy(sub_amp_out_rgb, _sub_amp_out_rgb());
	val[0] = (osc_amp_out_rgb[0] | sub_amp_out_rgb[0]) * ((double)(4095-commit_ctrl.value[CTRL_FX_WETDRY]) / 4095);
	val[1] = (osc_amp_out_rgb[1] | sub_amp_out_rgb[1]) * ((double)(4095-commit_ctrl.value[CTRL_FX_WETDRY]) / 4095);
	val[2] = (osc_amp_out_rgb[2] | sub_amp_out_rgb[2]) * ((double)(4095-commit_ctrl.value[CTRL_FX_WETDRY]) / 4095);
	return val;
}

bool _fx_dry_changed() {
  return _osc_amp_out_changed() || _sub_amp_out_changed() || commit_ctrl.changed[CTRL_FX_WETDRY];
}

uint16_t* _fx_wet_rgb() {
	uint16_t osc_amp_out_rgb[3];
	_rgb_copy(osc_amp_out_rgb, _osc_amp_out_rgb());
	uint16_t sub_amp_out_rgb[3];
	_rgb_copy(sub_amp_out_rgb, _sub_amp_out_rgb());
	val[0] = (osc_amp_out_rgb[0] | sub_amp_out_rgb[0]) * ((double)commit_ctrl.value[CTRL_FX_WETDRY] / 4095);
	val[1] = (osc_amp_out_rgb[1] | sub_amp_out_rgb[1]) * ((double)commit_ctrl.value[CTRL_FX_WETDRY] / 4095);
	val[2] = (osc_amp_out_rgb[2] | sub_amp_out_rgb[2]) * ((double)commit_ctrl.value[CTRL_FX_WETDRY] / 4095);
	return val;
}

bool _fx_wet_changed() {
  return _osc_amp_out_changed() || _sub_amp_out_changed() || commit_ctrl.changed[CTRL_FX_WETDRY];
}

uint16_t* _fx_feedback_rgb() {
	uint16_t fx_wet_rgb[3];
	_rgb_copy(fx_wet_rgb, _fx_wet_rgb());
	val[0] = (fx_wet_rgb[0]) * ((double)commit_ctrl.value[CTRL_FX_FEEDBACK] / 4095);
	val[1] = (fx_wet_rgb[1]) * ((double)commit_ctrl.value[CTRL_FX_FEEDBACK] / 4095);
	val[2] = (fx_wet_rgb[2]) * ((double)commit_ctrl.value[CTRL_FX_FEEDBACK] / 4095);
	return val;
}

bool _fx_feedback_changed() {
  return _fx_wet_changed() || commit_ctrl.changed[CTRL_FX_FEEDBACK];
}

/*
 * Commit Functions
 */

void _commit_led_osc1_saw() {
  bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc1 Saw
	 * LEFT0:00
	 * 0, 1
	 */

	if (!_osc1_saw_changed()) return;

	_set_pwm_seq(_osc1_saw_rgb(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0, 0, pwm_seq, 2*3);
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 6);
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0, 0, scale_seq, 2*3);
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0);
	if (!res) Error_Handler();
}

void _commit_led_osc1_squ() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc1 Squ
	 * LEFT0:00
	 * 2, 3
	 */

	if (!_osc1_squ_changed()) return;

	_set_pwm_seq(_osc1_squ_rgb(), pwm_seq, 2*3);

	// Work around Red and Blue pins being transposed
	pwm_seq[0] = pwm_seq[2];
	pwm_seq[2] = 0;
	pwm_seq[3] = pwm_seq[5];
	pwm_seq[5] = 0;

	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0, 2*3, pwm_seq, 2*3);
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 2*3);
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0, 2*3, scale_seq, 2*3);
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0);
	if (!res) Error_Handler();
}

void _commit_led_osc1_mix() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc1 Mix
	 * LEFT0:00
	 * 4, 5, 6, 7, 8, 9, 10
	 */

	if (!_osc1_mix_changed()) return;

	_set_pwm_seq(_osc1_mix_rgb(), pwm_seq, 7*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0, 12, pwm_seq, 7*3);
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 7*3);
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0, 12, scale_seq, 7*3);
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0);
	if (!res) Error_Handler();
}

void _commit_led_osc1_filt_freq() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc1 Filter
	 * LEFT0:10
	 * Freq: 3, 4
	 */

	if (!_osc1_filt_freq_changed()) return;

	_set_pwm_seq(_osc1_filt_freq_rgb(), pwm_seq, 4*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 2, (3*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 4*3);
	res = is32_set_sequence_scale(I2C_LEFT, 0, 2, (3*3), scale_seq, (2*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 2);
	if (!res) Error_Handler();
}

void _commit_led_osc1_filt_reso() {
  bool res;
  uint8_t pwm_seq[36];
  uint8_t scale_seq[36];

  /* Osc1 Filter
   * LEFT0:10
   * Reso: 5, 6
   */

  if (!_osc1_filt_reso_changed()) return;

  _set_pwm_seq(_osc1_filt_reso_rgb(), pwm_seq, 4*3);
  res = is32_set_sequence_pwm(I2C_LEFT, 0, 2, (5*3), pwm_seq, (2*3));
  if (!res) Error_Handler();

  _set_scale_seq(pwm_seq, scale_seq, 4*3);
  res = is32_set_sequence_scale(I2C_LEFT, 0, 2, (5*3), scale_seq, (2*3));
  if (!res) Error_Handler();

  res = is32_write_registers(I2C_LEFT, 0, 2);
  if (!res) Error_Handler();
}


void _commit_led_osc1_drive() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc1 Drive
	 * RIGHT1:00
	 * 0, 1, 3, 2 <--- Note 3 before 2
	 *
	 */

	if (!_osc1_drive_changed()) return;

	_set_pwm_seq(_osc1_drive_rgb(), pwm_seq, 4*3);
	res = is32_set_sequence_pwm(I2C_RIGHT, 1, 0, (0*3), pwm_seq, (4*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 4*3);
	res = is32_set_sequence_scale(I2C_RIGHT, 1, 0, (0*3), scale_seq, (4*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_RIGHT, 1, 0);
	if (!res) Error_Handler();
}

void _commit_led_osc1_to_osc2() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc1toOsc2 Mix Level
	 * LEFT0:10
	 * 0, 1
	 */

	if (!_osc1_to_2_mix_changed()) return;

	_set_pwm_seq(_osc1_to_2_mix(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b10, (0*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 2*3);
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0b10, (0*3), scale_seq, (2*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0b10);
	if (!res) Error_Handler();
}

void _commit_led_osc2_saw() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];


	/* Osc2 Saw
	 * LEFT0:11
	 * 1, 2
	 */

	if (!_osc2_saw_changed()) return;

	_set_pwm_seq(_osc2_saw_rgb(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b11, (1*3), pwm_seq, 2*3);
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 2*3);
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0b11, (1*3), scale_seq, 6);
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0b11);
	if (!res) Error_Handler();
}

void _commit_led_osc2_squ() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc2 Squ
	 * LEFT0:11
	 * 0
	 */

	if (!_osc2_squ_changed()) return;

	_set_pwm_seq(_osc2_squ_rgb(), pwm_seq, 1*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b11, (0*3), pwm_seq, 1*3);
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 1*3);
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0b11, (0*3), scale_seq, 1*3);
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0b11);
	if (!res) Error_Handler();
}

void _commit_led_osc2_noise() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc2 Noise
	 * LEFT0:11
	 * 3, 4
	 */

	if(!_osc2_noise_changed()) return;

	_set_pwm_seq(_osc2_noise_rgb(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b11, (3*3), pwm_seq, 2*3);
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 2*3);
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0b11, (3*3), scale_seq, 2*3);
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0b11);
	if (!res) Error_Handler();
}

void _commit_led_osc2_only() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc2-Only Mix
	 * LEFT0:11
	 * 5, 6, 7, 8
	 */

	if (!_osc2_mix_changed()) return;

	_set_pwm_seq(_osc2_mix(), pwm_seq, 4*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b11, (5*3), pwm_seq, (4*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 4*3);
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0b11, (5*3), scale_seq, (4*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0b11);
	if (!res) Error_Handler();
}

void _commit_led_osc2_prefilt() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc2-PreFilter Mix
	 * LEFT0:11
	 * 9, 10
	 */

	if (!_osc2_prefilt_changed()) return;

	_set_pwm_seq(_osc2_prefilt_rgb(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b11, (9*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 2*3);
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0b11, (9*3), scale_seq, (2*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0b11);
	if (!res) Error_Handler();
}

void _commit_led_osc2_filt_freq() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc2 Filt
	 * LEFT3:000
	 * Freq: 0, 1
	 */

	if (!_osc2_filt_freq_changed()) return;

	_set_pwm_seq(_osc2_filt_freq_rgb(), pwm_seq, 4*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 3, 0b00, (0*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 2*3);
	res = is32_set_sequence_scale(I2C_LEFT, 3, 0b00, (0*3), scale_seq, (2*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 3, 0b00);
	if (!res) Error_Handler();
}

void _commit_led_osc2_filt_reso() {
  bool res;
  uint8_t pwm_seq[36];
  uint8_t scale_seq[36];

  /* Osc2 Filt
   * LEFT3:000
   * Reso: 2, 3
   */

  if (!_osc2_filt_reso_changed()) return;

  _set_pwm_seq(_osc2_filt_reso_rgb(), pwm_seq, 4*3);
  res = is32_set_sequence_pwm(I2C_LEFT, 3, 0b00, (2*3), pwm_seq, (2*3));
  if (!res) Error_Handler();

  _set_scale_seq(pwm_seq, scale_seq, 2*3);
  res = is32_set_sequence_scale(I2C_LEFT, 3, 0b00, (2*3), scale_seq, (2*3));
  if (!res) Error_Handler();

  res = is32_write_registers(I2C_LEFT, 3, 0b00);
  if (!res) Error_Handler();
}


void _commit_led_osc2_drive() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc2 Drive
	 * LEFT3:10
	 * 3, 4, 5, 6, 7, 8, 9, 10
	 */

	if (!_osc2_drive_changed()) return;

	_set_pwm_seq(_osc2_drive_rgb(), pwm_seq, 8*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 3, 0b10, (3*3), pwm_seq, (8*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 8*3);
	res = is32_set_sequence_scale(I2C_LEFT, 3, 0b10, (3*3), scale_seq, (8*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 3, 0b10);
	if (!res) Error_Handler();
}

void _commit_led_osc_amp_out() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];


	/* Osc Amp Out
	 * RIGHT1:00
	 * 4, 5, 6, 7, 8
	 *
	 */

	if (!_osc_amp_out_changed()) return;

	_set_pwm_seq(_osc_amp_out_rgb(), pwm_seq, 5*3);
	res = is32_set_sequence_pwm(I2C_RIGHT, 1, 0, (4*3), pwm_seq, (5*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 5*3);
	res = is32_set_sequence_scale(I2C_RIGHT, 1, 0, (4*3), scale_seq, (5*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_RIGHT, 1, 0);
	if (!res) Error_Handler();
}

void _commit_led_sub_amp_out() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Sub Amp Out
	 * RIGHT1:01
	 * 7, 8, 9, 10, 11
	 *
	 */

	if (!_sub_amp_out_changed()) return;

	_set_pwm_seq(_sub_amp_out_rgb(), pwm_seq, 5*3);
	res = is32_set_sequence_pwm(I2C_RIGHT, 1, 0b01, (7*3), pwm_seq, (5*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 5*3);
	res = is32_set_sequence_scale(I2C_RIGHT, 1, 0b01, (7*3), scale_seq, (5*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_RIGHT, 1, 0b01);
	if (!res) Error_Handler();
}

void _commit_led_sub_squ() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Sub Squ
	 * LEFT1:00
	 * 0, 1
	 */

	if (!_sub_squ_changed()) return;

	_set_pwm_seq(_sub_squ_rgb(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 1, 0b00, (0*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 2*3);
	res = is32_set_sequence_scale(I2C_LEFT, 1, 0b00, (0*3), scale_seq, (2*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 1, 0b00);
	if (!res) Error_Handler();
}

void _commit_led_sub_noise() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Sub Noise
	 * LEFT1:00
	 * 2, 3
	 */

	if (!_sub_noise_changed()) return;

	_set_pwm_seq(_sub_noise_rgb(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 1, 0b00, (2*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 2*3);
	res = is32_set_sequence_scale(I2C_LEFT, 1, 0b00, (2*3), scale_seq, (2*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 1, 0b00);
	if (!res) Error_Handler();
}

void _commit_led_sub_mix() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Sub Mix
	 * LEFT1:00
	 * 4, 5, 6, 7, 8, 9, 10
	 */

	if (!_sub_mix_changed()) return;

	_set_pwm_seq(_sub_mix_rgb(), pwm_seq, 7*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 1, 0b00, (4 * 3), pwm_seq, (7 * 3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 7*3);
	res = is32_set_sequence_scale(I2C_LEFT, 1, 0b00, (4 * 3), scale_seq, (7 * 3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 1, 0b00);
	if (!res) Error_Handler();
}

void _commit_led_sub_to_osc2_mix() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* SubToOsc2 Mix Level
	 * LEFT3:00
	 * 7, 8
	 */

	if (!_sub_to_osc2_mix_changed()) return;

	_set_pwm_seq(_sub_to_osc2_mix_rgb(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 3, 0b00, (7*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 2*3);
	res = is32_set_sequence_scale(I2C_LEFT, 3, 0b00, (7*3), scale_seq, (2*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 3, 0b00);
	if (!res) Error_Handler();
}

void _commit_led_sub_filt_freq() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Sub Filter Freq Cutoff
	 * LEFT1:10
	 * 0, 1,
	 */

	if (!_sub_filt_freq_changed()) return;

	_set_pwm_seq(_sub_filt_freq_rgb(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 1, 0b10, (0 * 3), pwm_seq, (2 * 3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 1*3);
	res = is32_set_sequence_scale(I2C_LEFT, 1, 0b10, (0 * 3), scale_seq, (2 * 3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 1, 0b10);
	if (!res) Error_Handler();
}

void _commit_led_sub_filt_reso() {
  bool res;
  uint8_t pwm_seq[36];
  uint8_t scale_seq[36];

  /* Sub Filter Out
   * LEFT1:10
   * 2, 3, 4, 5, 6, 7, 8
   */

  if (!_sub_filt_reso_changed()) return;

  _set_pwm_seq(_sub_filt_reso_rgb(), pwm_seq, 7*3);
  res = is32_set_sequence_pwm(I2C_LEFT, 1, 0b10, (2 * 3), pwm_seq, (7 * 3));
  if (!res) Error_Handler();

  _set_scale_seq(pwm_seq, scale_seq, 7*3);
  res = is32_set_sequence_scale(I2C_LEFT, 1, 0b10, (2 * 3), scale_seq, (7 * 3));
  if (!res) Error_Handler();

  res = is32_write_registers(I2C_LEFT, 1, 0b10);
  if (!res) Error_Handler();
}

void _commit_led_sub_filt_out() {
  bool res;
  uint8_t pwm_seq[36];
  uint8_t scale_seq[36];

  /* Sub Filter Out
   * LEFT1:10
   * 0, 1, 2, 3, 4, 5, 6, 7, 8
   */

  if (!_sub_filt_out_changed()) return;

  _set_pwm_seq(_sub_filt_out_rgb(), pwm_seq, 9*3);
  res = is32_set_sequence_pwm(I2C_LEFT, 1, 0b10, (0 * 3), pwm_seq, (9 * 3));
  if (!res) Error_Handler();

  _set_scale_seq(pwm_seq, scale_seq, 9*3);
  res = is32_set_sequence_scale(I2C_LEFT, 1, 0b10, (0 * 3), scale_seq, (9 * 3));
  if (!res) Error_Handler();

  res = is32_write_registers(I2C_LEFT, 1, 0b10);
  if (!res) Error_Handler();
}


void _commit_led_fx_dry() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* FX Dry
	 * RIGHT2:01
	 * 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
	 *
	 */

	if (!_fx_dry_changed()) return;

	_set_pwm_seq(_fx_dry_rgb(), pwm_seq, 11*3);
	res = is32_set_sequence_pwm(I2C_RIGHT, 2, 0b01, (0*3), pwm_seq, (11*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 11*3);
	res = is32_set_sequence_scale(I2C_RIGHT, 2, 0b01, (0*3), scale_seq, (11*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_RIGHT, 2, 0b01);
	if (!res) Error_Handler();
}

void _commit_led_fx_wet() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* FX Wet
	 * RIGHT1:01
	 * 0, 1, 2, 3, 4, 5, 6
	 *
	 */

	if (!_fx_wet_changed()) return;

	_set_pwm_seq(_fx_wet_rgb(), pwm_seq, 7*3);
	res = is32_set_sequence_pwm(I2C_RIGHT, 1, 0b01, (0*3), pwm_seq, (7*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 7*3);
	res = is32_set_sequence_scale(I2C_RIGHT, 1, 0b01, (0*3), scale_seq, (7*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_RIGHT, 1, 0b01);
	if (!res) Error_Handler();
}

void _commit_led_fx_feedback() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* FX Feedback
	 * RIGHT2:10
	 * 0, 1, 2, 3, 4, 5, 7
	 *
	 */

	if (!_fx_feedback_changed()) return;

	_set_pwm_seq(_fx_feedback_rgb(), pwm_seq, 8*3);
	res = is32_set_sequence_pwm(I2C_RIGHT, 2, 0b10, (0*3), pwm_seq, (8*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 8*3);
	res = is32_set_sequence_scale(I2C_RIGHT, 2, 0b10, (0*3), scale_seq, (8*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_RIGHT, 2, 0b10);
	if (!res) Error_Handler();

}

void commit_led_osc(commit_cycle_t cycle) {
	uint32_t ticks_before = HAL_GetTick();

	switch(cycle) {
	case COMMIT_LED_OSC1_SAW:
	  _commit_led_osc1_saw();
		break;
	case COMMIT_LED_OSC1_SQU:
	  _commit_led_osc1_squ();
		break;
	case COMMIT_LED_OSC1_MIX:
		_commit_led_osc1_mix();
		break;
	case COMMIT_LED_OSC1_FILT_FREQ:
		_commit_led_osc1_filt_freq();
		break;
  case COMMIT_LED_OSC1_FILT_RESO:
    _commit_led_osc1_filt_reso();
    break;
	case COMMIT_LED_OSC1_DRIVE:
		_commit_led_osc1_drive();
		break;
	case COMMIT_LED_OSC1_TO_OSC2:
		_commit_led_osc1_to_osc2();
		break;

	case COMMIT_LED_OSC2_SAW:
		_commit_led_osc2_saw();
		break;
	case COMMIT_LED_OSC2_SQU:
		_commit_led_osc2_squ();
		break;
	case COMMIT_LED_OSC2_NOISE:
		_commit_led_osc2_noise();
		break;

	case COMMIT_LED_OSC2_ONLY:
		_commit_led_osc2_only();
		break;
	case COMMIT_LED_OSC2_PREFILT:
		_commit_led_osc2_prefilt();
		break;
	case COMMIT_LED_OSC2_FILT_FREQ:
		_commit_led_osc2_filt_freq();
		break;
  case COMMIT_LED_OSC2_FILT_RESO:
    _commit_led_osc2_filt_reso();
    break;
	case COMMIT_LED_OSC2_DRIVE:
		_commit_led_osc2_drive();
		break;

	case COMMIT_LED_OSC_AMP_OUT:
		_commit_led_osc_amp_out();
		break;
	case COMMIT_LED_SUB_AMP_OUT:
		_commit_led_sub_amp_out();
    break;

	case COMMIT_LED_SUB_SQU:
		_commit_led_sub_squ();
		break;
	case COMMIT_LED_SUB_NOISE:
		_commit_led_sub_noise();
		break;
	case COMMIT_LED_SUB_MIX:
		_commit_led_sub_mix();
    break;
	case COMMIT_LED_SUB_TO_OSC2:
		_commit_led_sub_to_osc2_mix();
    break;
	case COMMIT_LED_SUB_FILT_FREQ:
		_commit_led_sub_filt_freq();
		break;
  case COMMIT_LED_SUB_FILT_RESO:
    _commit_led_sub_filt_reso();
    break;

	case COMMIT_LED_FX_DRY:
		_commit_led_fx_dry();
		break;
	case COMMIT_LED_FX_WET:
		_commit_led_fx_wet();
		break;
	case COMMIT_LED_FX_FEEDBACK:
		_commit_led_fx_feedback();
		break;

	default:
		break;
	}

	uint32_t ticks_after = HAL_GetTick();
	uint32_t ticks_cost = ticks_after - ticks_before;

	UNUSED(ticks_cost);

}
