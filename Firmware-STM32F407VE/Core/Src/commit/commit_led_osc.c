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
#include <math.h>

// Defaults

#define DEFAULT_HSV_V 1.0
#define MAX_HSV_V 1.0

//#define SAW_HSV_H 0.0
//#define SQU_HSV_H 240.0
#define SAW_HSV_H 60.0
#define SQU_HSV_H 180.0
#define NOISE_HSV_H 297.0

#define ALWAYS_UPDATE 1

/*
 * RGB Calculations
 */

static uint16_t val[3]; // TODO: Get rid of this

// utils

uint16_t _max16(uint16_t a, uint16_t b) {
  if (a > b) return a;
  else return b;
}

double _ctrl_double(ctrl_enum_t in) {
  return (double)(commit_ctrl.value[in] / 4095.0);
}

double _ctrl_double_inverse(ctrl_enum_t in) {
  return (double)((4095.0-commit_ctrl.value[in]) / 4095.0);
}


// osc1_saw

hsv _osc1_saw_hsv() {
  hsv hsv;
  hsv.h = SAW_HSV_H;
  hsv.s = 1.0;
  hsv.v = DEFAULT_HSV_V * _ctrl_double(CTRL_OSC1_SAW_LVL);
  return hsv;
}

bool _osc1_saw_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_OSC1_SAW_LVL];
}

// osc1_squ

hsv _osc1_squ_hsv() {
  hsv hsv;
  hsv.h = SQU_HSV_H;
  hsv.s = 1.0;
  hsv.v = DEFAULT_HSV_V * _ctrl_double(CTRL_OSC1_SQU_LVL);
	return hsv;
}

bool _osc1_squ_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_OSC1_SQU_LVL];
}

// osc1_mix

hsv _osc1_mix_hsv() {
  hsv saw_hsv = _osc1_saw_hsv();
  hsv squ_hsv = _osc1_squ_hsv();
  hsv mix = _interpolate_hsv(saw_hsv, squ_hsv);
  return mix;
}

bool _osc1_mix_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_OSC1_SAW_LVL] || commit_ctrl.changed[CTRL_OSC1_SQU_LVL];
}

hsv _filt_freq_hsv(hsv in, ctrl_enum_t cutoff_enum) {
  double cutoff = _ctrl_double(cutoff_enum);
  in.s = cutoff;
  in.v *= (cutoff * cutoff);
  return in;
}

hsv _osc1_filt_freq_hsv() {
  return _filt_freq_hsv(_osc1_mix_hsv(), CTRL_OSC1_FILT_CUTOFF);
}

bool _osc1_filt_freq_changed() {
  return ALWAYS_UPDATE || _osc1_mix_changed() || commit_ctrl.changed[CTRL_OSC1_FILT_CUTOFF];
}

hsv _filt_reso_hsv(hsv in, ctrl_enum_t res_enum) {
  hsv out = in;
  double res = _ctrl_double(res_enum);
  out.h += 90.0 * res;
  if (out.h >= 360.0) out.h -= 360.0;
  out.s += (MAX_HSV_V - out.s) * res;
  out.v += ((1.0 - out.v) * 0.75) * res;
  return out;
}

hsv _osc1_filt_reso_hsv() {
  return _filt_reso_hsv(_osc1_filt_freq_hsv(), CTRL_OSC1_FILT_RES);
}

bool _osc1_filt_reso_changed() {
  return ALWAYS_UPDATE || _osc1_filt_freq_changed() || commit_ctrl.changed[CTRL_OSC1_FILT_RES];
}

hsv _drive_hsv(hsv in, ctrl_enum_t drive_enum) {
  hsv out = in;
  double drive = _ctrl_double(drive_enum);
  out.h -= 120.0 * drive;
  if (out.h >= 360.0) out.h -= 360.0;
  if (out.h < 0.0) out.h += 360.0;
  out.s += (MAX_HSV_V - out.s) * drive;
  out.v += ((1.0 - out.v) * 0.75) * drive;
  return out;
}

hsv _osc1_drive_hsv() {
  return _drive_hsv(_osc1_filt_reso_hsv(), CTRL_OSC1_DRIVE_AMT);
}

bool _osc1_drive_changed() {
  return ALWAYS_UPDATE || _osc1_filt_reso_changed() || commit_ctrl.changed[CTRL_OSC1_DRIVE_AMT];
}

// CTRL_OSC1_TO_OSC2_MIX

uint16_t* _osc1_to_2_mix() {
	val[0] = (double)commit_ctrl.value[CTRL_OSC1_SAW_LVL] * ((double)commit_ctrl.value[CTRL_OSC1_TO_OSC2_MIX] / 4095.0);
	val[1] = 0;
	val[2] = (double)commit_ctrl.value[CTRL_OSC1_SQU_LVL] * ((double)commit_ctrl.value[CTRL_OSC1_TO_OSC2_MIX] / 4095.0);
	return val;
}

hsv _osc1_to_2_mix_hsv() {
  hsv out = _osc1_mix_hsv();
  out.v *= _ctrl_double(CTRL_OSC1_TO_OSC2_MIX);
  return out;
}

bool _osc1_to_2_mix_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_OSC1_SAW_LVL] || commit_ctrl.changed[CTRL_OSC1_SQU_LVL] || commit_ctrl.changed[CTRL_OSC1_TO_OSC2_MIX];
}

// osc2_saw

hsv _osc2_saw_hsv() {
  hsv hsv;
  hsv.h = SAW_HSV_H;
  hsv.s = 1.0;
  hsv.v = DEFAULT_HSV_V * _ctrl_double(CTRL_OSC2_SAW_LVL);
  return hsv;
}


bool _osc2_saw_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_OSC2_SAW_LVL];
}

// osc2_squ

hsv _osc2_squ_hsv() {
  hsv hsv;
  hsv.h = SQU_HSV_H;
  hsv.s = 1.0;
  hsv.v = DEFAULT_HSV_V * _ctrl_double(CTRL_OSC2_SQU_LVL);
  return hsv;
}


bool _osc2_squ_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_OSC2_SQU_LVL];
}

// osc2_noise

hsv _osc2_noise_hsv() {
  hsv hsv;
  hsv.h = NOISE_HSV_H;
  hsv.s = 1.0;
  hsv.v = DEFAULT_HSV_V * _ctrl_double(CTRL_OSC2_NOISE_LVL);
  return hsv;
}

bool _osc2_noise_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_OSC2_NOISE_LVL];
}

// osc2_mix

hsv _osc2_mix_hsv() {
  hsv saw_hsv = _osc2_saw_hsv();
  hsv squ_hsv = _osc2_squ_hsv();
  hsv noise_hsv = _osc2_noise_hsv();
  hsv mix = _interpolate_hsv(_interpolate_hsv(saw_hsv, squ_hsv), noise_hsv);
  return mix;
}


bool _osc2_mix_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_OSC2_SAW_LVL] || commit_ctrl.changed[CTRL_OSC2_NOISE_LVL] || commit_ctrl.changed[CTRL_OSC2_SQU_LVL];
}

// osc2_prefilt

hsv _sub_to_osc2_mix_hsv(void);

hsv _osc2_prefilt_hsv() {
  return _interpolate_hsv(_interpolate_hsv(_osc2_mix_hsv(), _osc1_to_2_mix_hsv()), _sub_to_osc2_mix_hsv());
}

bool _osc2_prefilt_changed() {
  return ALWAYS_UPDATE
      || commit_ctrl.changed[CTRL_OSC2_SAW_LVL]
      || commit_ctrl.changed[CTRL_OSC1_TO_OSC2_MIX]
      || commit_ctrl.changed[CTRL_OSC2_NOISE_LVL]
      || commit_ctrl.changed[CTRL_SUB_TO_OSC2_MIX]
      || commit_ctrl.changed[CTRL_OSC2_SQU_LVL]
      || commit_ctrl.changed[CTRL_OSC1_TO_OSC2_MIX];
}

// osc2_filt_freq

hsv _osc2_filt_freq_hsv() {
  return _filt_freq_hsv(_osc2_prefilt_hsv(), CTRL_OSC2_FILT_CUTOFF);
}

bool _osc2_filt_freq_changed() {
  return ALWAYS_UPDATE || _osc2_prefilt_changed() || commit_ctrl.changed[CTRL_OSC2_FILT_CUTOFF];
}

// osc2_filt_reso

hsv _osc2_filt_reso_hsv() {
  return _filt_reso_hsv(_osc2_filt_freq_hsv(), CTRL_OSC2_FILT_RES);
}

bool _osc2_filt_reso_changed() {
  return ALWAYS_UPDATE || _osc2_filt_freq_changed() || commit_ctrl.changed[CTRL_OSC2_FILT_RES];
}

// osc2_drive

hsv _osc2_drive_hsv() {
  return _drive_hsv(_osc2_filt_reso_hsv(), CTRL_OSC2_DRIVE_AMT);
}

bool _osc2_drive_changed() {
  return ALWAYS_UPDATE || _osc2_filt_reso_changed() || commit_ctrl.changed[CTRL_OSC2_DRIVE_AMT];
}

// osc_amp

hsv _osc_amp_out_hsv() {
  return _interpolate_hsv(_osc1_drive_hsv(), _osc2_drive_hsv());
}

bool _osc_amp_out_changed() {
  return ALWAYS_UPDATE || _osc1_drive_changed() || _osc2_drive_changed();
}

// sub_squ

hsv _sub_squ_hsv() {
  hsv hsv;
  hsv.h = SQU_HSV_H;
  hsv.s = 1.0;
  hsv.v = DEFAULT_HSV_V * _ctrl_double(CTRL_SUB_LVL);
  return hsv;
}

bool _sub_squ_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_SUB_LVL];
}

// sub_noise

hsv _sub_noise_hsv() {
  hsv hsv;
  hsv.h = NOISE_HSV_H;
  hsv.s = 1.0;
  hsv.v = DEFAULT_HSV_V * _ctrl_double(CTRL_SUB_NOISE_LVL);
  return hsv;
}


bool _sub_noise_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_SUB_NOISE_LVL];
}

// sub_mix

hsv _sub_mix_hsv() {
  return _interpolate_hsv(_sub_squ_hsv(), _sub_noise_hsv());
}

bool _sub_mix_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_SUB_NOISE_LVL] || commit_ctrl.changed[CTRL_SUB_LVL];
}

// CTRL_SUB_TO_OSC2_MIX]_mix

hsv _sub_to_osc2_mix_hsv() {
  hsv out = _sub_mix_hsv();
  out.v *= _ctrl_double(CTRL_SUB_TO_OSC2_MIX);
  return out;
}

bool _sub_to_osc2_mix_changed() {
  return ALWAYS_UPDATE
      || commit_ctrl.changed[CTRL_SUB_NOISE_LVL]
      || commit_ctrl.changed[CTRL_SUB_TO_OSC2_MIX]
      || commit_ctrl.changed[CTRL_SUB_LVL];
}

// sub_filt

hsv _sub_filt_freq_hsv() {
  return _filt_freq_hsv(_sub_mix_hsv(), CTRL_SUB_FILT_CUTOFF);
}

bool _sub_filt_freq_changed() {
  return ALWAYS_UPDATE || _sub_mix_changed() || commit_ctrl.changed[CTRL_SUB_FILT_CUTOFF];
}

hsv _sub_filt_reso_hsv() {
  return _filt_reso_hsv(_sub_filt_freq_hsv(), CTRL_SUB_FILT_RES);
}

bool _sub_filt_reso_changed() {
  return ALWAYS_UPDATE || _sub_filt_freq_changed() || commit_ctrl.changed[CTRL_SUB_FILT_RES];
}

hsv _sub_filt_out_hsv() {
  return _sub_filt_reso_hsv();
}

bool _sub_filt_out_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_SUB_NOISE_LVL] || commit_ctrl.changed[CTRL_SUB_LVL];
}

hsv _sub_amp_out_hsv() {
  return _sub_filt_out_hsv();
}

bool _sub_amp_out_changed() {
  return ALWAYS_UPDATE || _sub_filt_reso_changed();
}

// fx_dry

hsv _fx_dry_hsv() {
  hsv out = _interpolate_hsv(_osc_amp_out_hsv(), _sub_amp_out_hsv());
  out.v *= _ctrl_double_inverse(CTRL_FX_WETDRY);
  return out;
}

bool _fx_dry_changed() {
  return ALWAYS_UPDATE || _osc_amp_out_changed() || _sub_amp_out_changed() || commit_ctrl.changed[CTRL_FX_WETDRY];
}

hsv _fx_wet_hsv() {
  hsv out = _interpolate_hsv(_osc_amp_out_hsv(), _sub_amp_out_hsv());
  out.v *= _ctrl_double(CTRL_FX_WETDRY);
  return out;
}

bool _fx_wet_changed() {
  return ALWAYS_UPDATE || _osc_amp_out_changed() || _sub_amp_out_changed() || commit_ctrl.changed[CTRL_FX_WETDRY];
}

hsv _fx_feedback_hsv() {
  hsv out = _fx_wet_hsv();
  out.v *= _ctrl_double(CTRL_FX_FEEDBACK);
  return out;
}

bool _fx_feedback_changed() {
  return ALWAYS_UPDATE || _fx_wet_changed() || commit_ctrl.changed[CTRL_FX_FEEDBACK];
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

	_set_pwm_seq_hsv(_osc1_saw_hsv(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0, 0, pwm_seq, 2*3);
	if (!res) Error_Handler();

	_set_scale_seq_animated(pwm_seq, scale_seq, 6, 0, false);
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

	_set_pwm_seq_hsv(_osc1_squ_hsv(), pwm_seq, 2*3);

	// Work around Red and Blue pins being transposed
	pwm_seq[0] = pwm_seq[2];
	pwm_seq[2] = 0;
	pwm_seq[3] = pwm_seq[5];
	pwm_seq[5] = 0;

	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0, 2*3, pwm_seq, 2*3);
	if (!res) Error_Handler();

	_set_scale_seq_animated(pwm_seq, scale_seq, 2*3, 0, false);
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

	_set_pwm_seq_hsv(_osc1_mix_hsv(), pwm_seq, 7*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0, 12, pwm_seq, 7*3);
	if (!res) Error_Handler();

	_set_scale_seq_animated(pwm_seq, scale_seq, 5*3, 2, false);
  _set_scale_seq_animated(pwm_seq + (5*3), scale_seq + (5*3), 2*3, 4, false);
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

	_set_pwm_seq_hsv(_osc1_filt_freq_hsv(), pwm_seq, 4*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 2, (3*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

	_set_scale_seq_animated(pwm_seq, scale_seq, 4*3, 7, false);
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

  _set_pwm_seq_hsv(_osc1_filt_reso_hsv(), pwm_seq, 4*3);
  res = is32_set_sequence_pwm(I2C_LEFT, 0, 2, (5*3), pwm_seq, (2*3));
  if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 4*3, 9, false);
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

	_set_pwm_seq_hsv(_osc1_drive_hsv(), pwm_seq, 4*3);
	res = is32_set_sequence_pwm(I2C_RIGHT, 1, 0, (0*3), pwm_seq, (4*3));
	if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 2*3, 11, false);
  _set_scale_seq_animated(pwm_seq + (2*3), scale_seq + (2*3), 2*3, 13, true);
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

	_set_pwm_seq_hsv(_osc1_to_2_mix_hsv(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b10, (0*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

	_set_scale_seq_animated(pwm_seq, scale_seq, 2*3, 6, false);
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0b10, (0*3), scale_seq, (2*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0b10);
	if (!res) Error_Handler();
}

#define OSC2_PATTERN_OFFSET -8

void _commit_led_osc2_saw() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];


	/* Osc2 Saw
	 * LEFT0:11
	 * 1, 2
	 */

	if (!_osc2_saw_changed()) return;

	_set_pwm_seq_hsv(_osc2_saw_hsv(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b11, (1*3), pwm_seq, 2*3);
	if (!res) Error_Handler();

	_set_scale_seq_animated(pwm_seq, scale_seq, 2*3, 0+OSC2_PATTERN_OFFSET, false);
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

	_set_pwm_seq_hsv(_osc2_squ_hsv(), pwm_seq, 1*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b11, (0*3), pwm_seq, 1*3);
	if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 1*3, 1+OSC2_PATTERN_OFFSET, false);
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

	_set_pwm_seq_hsv(_osc2_noise_hsv(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b11, (3*3), pwm_seq, 2*3);
	if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 2*3, 0+OSC2_PATTERN_OFFSET, false);
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

	_set_pwm_seq_hsv(_osc2_mix_hsv(), pwm_seq, 4*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b11, (5*3), pwm_seq, (4*3));
	if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 4*3, 2+OSC2_PATTERN_OFFSET, false);
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

	_set_pwm_seq_hsv(_osc2_prefilt_hsv(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b11, (9*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 2*3, 6+OSC2_PATTERN_OFFSET, false);
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

	_set_pwm_seq_hsv(_osc2_filt_freq_hsv(), pwm_seq, 4*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 3, 0b00, (0*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 2*3, 8+OSC2_PATTERN_OFFSET, false);
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

  _set_pwm_seq_hsv(_osc2_filt_reso_hsv(), pwm_seq, 4*3);
  res = is32_set_sequence_pwm(I2C_LEFT, 3, 0b00, (2*3), pwm_seq, (2*3));
  if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 2*3, 10+OSC2_PATTERN_OFFSET, false);
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

	_set_pwm_seq_hsv(_osc2_drive_hsv(), pwm_seq, 8*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 3, 0b10, (3*3), pwm_seq, (8*3));
	if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 8*3, 12+OSC2_PATTERN_OFFSET, false);
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

	_set_pwm_seq_hsv(_osc_amp_out_hsv(), pwm_seq, 5*3);
	res = is32_set_sequence_pwm(I2C_RIGHT, 1, 0, (4*3), pwm_seq, (5*3));
	if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 5*3, 20+OSC2_PATTERN_OFFSET, false);
	res = is32_set_sequence_scale(I2C_RIGHT, 1, 0, (4*3), scale_seq, (5*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_RIGHT, 1, 0);
	if (!res) Error_Handler();
}

#define SUB_PATTERN_OFFSET 0

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

	_set_pwm_seq_hsv(_sub_amp_out_hsv(), pwm_seq, 5*3);
	res = is32_set_sequence_pwm(I2C_RIGHT, 1, 0b01, (7*3), pwm_seq, (5*3));
	if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 5*3, 16+SUB_PATTERN_OFFSET, false);
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

	_set_pwm_seq_hsv(_sub_squ_hsv(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 1, 0b00, (0*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

	_set_scale_seq_animated(pwm_seq, scale_seq, 2*3, 0+SUB_PATTERN_OFFSET, false);
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

	_set_pwm_seq_hsv(_sub_noise_hsv(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 1, 0b00, (2*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 2*3, 0+SUB_PATTERN_OFFSET, false);
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

	_set_pwm_seq_hsv(_sub_mix_hsv(), pwm_seq, 7*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 1, 0b00, (4 * 3), pwm_seq, (7 * 3));
	if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 5*3, 2+SUB_PATTERN_OFFSET, false);
  _set_scale_seq_animated(pwm_seq+(5*3), scale_seq+(5*3), 2*3, 4+SUB_PATTERN_OFFSET, false);
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

	_set_pwm_seq_hsv(_sub_to_osc2_mix_hsv(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 3, 0b00, (7*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 2*3, 6+SUB_PATTERN_OFFSET, false);
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

	_set_pwm_seq_hsv(_sub_filt_freq_hsv(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 1, 0b10, (0 * 3), pwm_seq, (2 * 3));
	if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 2*3, 7+SUB_PATTERN_OFFSET, false);
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

  _set_pwm_seq_hsv(_sub_filt_reso_hsv(), pwm_seq, 7*3);
  res = is32_set_sequence_pwm(I2C_LEFT, 1, 0b10, (2 * 3), pwm_seq, (7 * 3));
  if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 7*3, 9+SUB_PATTERN_OFFSET, false);
  res = is32_set_sequence_scale(I2C_LEFT, 1, 0b10, (2 * 3), scale_seq, (7 * 3));
  if (!res) Error_Handler();

  res = is32_write_registers(I2C_LEFT, 1, 0b10);
  if (!res) Error_Handler();
}

#define FX_PATTERN_OFFSET 2

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

	_set_pwm_seq_hsv(_fx_dry_hsv(), pwm_seq, 11*3);
	res = is32_set_sequence_pwm(I2C_RIGHT, 2, 0b01, (0*3), pwm_seq, (11*3));
	if (!res) Error_Handler();

	_set_scale_seq_animated(pwm_seq, scale_seq, 11*3, 0+FX_PATTERN_OFFSET, false);
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

	_set_pwm_seq_hsv(_fx_wet_hsv(), pwm_seq, 7*3);
	res = is32_set_sequence_pwm(I2C_RIGHT, 1, 0b01, (0*3), pwm_seq, (7*3));
	if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 7*3, 0+FX_PATTERN_OFFSET, false);
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

	_set_pwm_seq_hsv(_fx_feedback_hsv(), pwm_seq, 8*3);
	res = is32_set_sequence_pwm(I2C_RIGHT, 2, 0b10, (0*3), pwm_seq, (8*3));
	if (!res) Error_Handler();

  _set_scale_seq_animated(pwm_seq, scale_seq, 8*3, 5+FX_PATTERN_OFFSET, true);
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
