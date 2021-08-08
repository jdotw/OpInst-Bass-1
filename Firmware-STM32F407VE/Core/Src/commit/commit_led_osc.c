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

lab_t _primitive_lab(double r, double g, double b, ctrl_enum_t ctrl) {
  rgb_t rgb = { .r = r, .g = g, .b = b };
  lab_t lab = _rgb_to_oklab(rgb);
  lab.L *= _ctrl_double(ctrl);
  lab.v = commit_ctrl.value[ctrl];
  return lab;
}

lab_t _filt_freq_lab(lab_t in, ctrl_enum_t cutoff_enum) {
  hsv hsv = _oklab_to_hsv(in);
  double cutoff = _ctrl_double(cutoff_enum);
  hsv.s *= cutoff;
  hsv.v *= (cutoff * cutoff);
  lab_t lab = _hsv_to_oklab(hsv);
  lab.v = in.v;
  return lab;
}


lab_t _filt_reso_lab(lab_t in, ctrl_enum_t res_enum) {
  hsv hsv = _oklab_to_hsv(in);
  double res = _ctrl_double(res_enum);
  hsv.h += 90.0 * res;
  if (hsv.h >= 360.0) hsv.h -= 360.0;
  hsv.s += (MAX_HSV_V - hsv.s) * res;
  hsv.v += ((1.0 - hsv.v) * 0.75) * res;
  lab_t lab = _hsv_to_oklab(hsv);
  lab.v = in.v;
  return lab;
}

lab_t _drive_lab(lab_t in, ctrl_enum_t drive_enum) {
  hsv hsv = _oklab_to_hsv(in);
  double drive = _ctrl_double(drive_enum);
  hsv.h -= 120.0 * drive;
  if (hsv.h >= 360.0) hsv.h -= 360.0;
  if (hsv.h < 0.0) hsv.h += 360.0;
  hsv.s += (MAX_HSV_V - hsv.s) * drive;
  hsv.v += ((1.0 - hsv.v) * 0.75) * drive;
  lab_t lab = _hsv_to_oklab(hsv);
  lab.v = in.v;
  return lab;
}

// osc1_saw

lab_t _osc1_saw_lab() {
  return _primitive_lab(1.0, 1.0, 0.0, CTRL_OSC1_SAW_LVL);
}

bool _osc1_saw_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_OSC1_SAW_LVL];
}

// osc1_squ

lab_t _osc1_squ_lab() {
  return _primitive_lab(0.0, 1.0, 1.0, CTRL_OSC1_SQU_LVL);
}

bool _osc1_squ_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_OSC1_SQU_LVL];
}

// osc1_mix

lab_t _osc1_mix_lab() {
  lab_t saw = _osc1_saw_lab();
  lab_t squ = _osc1_squ_lab();
  lab_t mix = _interpolate_lab(saw, squ);
  return mix;
}

bool _osc1_mix_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_OSC1_SAW_LVL] || commit_ctrl.changed[CTRL_OSC1_SQU_LVL];
}


lab_t _osc1_filt_freq_lab() {
  return _filt_freq_lab(_osc1_mix_lab(), CTRL_OSC1_FILT_CUTOFF);
}

bool _osc1_filt_freq_changed() {
  return ALWAYS_UPDATE || _osc1_mix_changed() || commit_ctrl.changed[CTRL_OSC1_FILT_CUTOFF];
}



lab_t _osc1_filt_reso_lab() {
  return _filt_reso_lab(_osc1_filt_freq_lab(), CTRL_OSC1_FILT_RES);
}

bool _osc1_filt_reso_changed() {
  return ALWAYS_UPDATE || _osc1_filt_freq_changed() || commit_ctrl.changed[CTRL_OSC1_FILT_RES];
}


lab_t _osc1_drive_lab() {
  return _drive_lab(_osc1_filt_reso_lab(), CTRL_OSC1_DRIVE_AMT);
}

bool _osc1_drive_changed() {
  return ALWAYS_UPDATE || _osc1_filt_reso_changed() || commit_ctrl.changed[CTRL_OSC1_DRIVE_AMT];
}

// CTRL_OSC1_TO_OSC2_MIX

lab_t _osc1_to_2_mix_lab() {
  lab_t out = _osc1_mix_lab();
  out.L *= _ctrl_double(CTRL_OSC1_TO_OSC2_MIX);
  out.v = commit_ctrl.value[CTRL_OSC1_TO_OSC2_MIX];
  return out;
}

bool _osc1_to_2_mix_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_OSC1_SAW_LVL] || commit_ctrl.changed[CTRL_OSC1_SQU_LVL] || commit_ctrl.changed[CTRL_OSC1_TO_OSC2_MIX];
}

// osc2_saw

lab_t _osc2_saw_lab() {
  return _primitive_lab(1.0, 1.0, 0.0, CTRL_OSC2_SAW_LVL);
}


bool _osc2_saw_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_OSC2_SAW_LVL];
}

// osc2_squ

lab_t _osc2_squ_lab() {
  return _primitive_lab(0.0, 1.0, 1.0, CTRL_OSC2_SQU_LVL);
}


bool _osc2_squ_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_OSC2_SQU_LVL];
}

// osc2_noise

lab_t _osc2_noise_lab() {
  return _primitive_lab(1.0, 0.0, 1.0, CTRL_OSC2_NOISE_LVL);
}

bool _osc2_noise_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_OSC2_NOISE_LVL];
}

// osc2_mix

lab_t _osc2_mix_lab() {
  lab_t saw_lab = _osc2_saw_lab();
  lab_t squ_lab = _osc2_squ_lab();
  lab_t noise_lab = _osc2_noise_lab();
  lab_t mix = _interpolate_lab(_interpolate_lab(saw_lab, squ_lab), noise_lab);
  return mix;
}


bool _osc2_mix_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_OSC2_SAW_LVL] || commit_ctrl.changed[CTRL_OSC2_NOISE_LVL] || commit_ctrl.changed[CTRL_OSC2_SQU_LVL];
}

// osc2_prefilt

lab_t _sub_to_osc2_mix_lab(void);

lab_t _osc2_prefilt_lab() {
  lab_t osc1 = _osc1_to_2_mix_lab();
  lab_t osc2 = _osc2_mix_lab();
  lab_t sub = _sub_to_osc2_mix_lab();

  lab_t osc1_and_2 = _interpolate_lab(osc2, osc1);
  lab_t osc_and_sub = _interpolate_lab(osc1_and_2, sub);

  return osc_and_sub;
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

lab_t _osc2_filt_freq_lab() {
  return _filt_freq_lab(_osc2_prefilt_lab(), CTRL_OSC2_FILT_CUTOFF);
}

bool _osc2_filt_freq_changed() {
  return ALWAYS_UPDATE || _osc2_prefilt_changed() || commit_ctrl.changed[CTRL_OSC2_FILT_CUTOFF];
}

// osc2_filt_reso

lab_t _osc2_filt_reso_lab() {
  return _filt_reso_lab(_osc2_filt_freq_lab(), CTRL_OSC2_FILT_RES);
}

bool _osc2_filt_reso_changed() {
  return ALWAYS_UPDATE || _osc2_filt_freq_changed() || commit_ctrl.changed[CTRL_OSC2_FILT_RES];
}

// osc2_drive

lab_t _osc2_drive_lab() {
  return _drive_lab(_osc2_filt_reso_lab(), CTRL_OSC2_DRIVE_AMT);
}

bool _osc2_drive_changed() {
  return ALWAYS_UPDATE || _osc2_filt_reso_changed() || commit_ctrl.changed[CTRL_OSC2_DRIVE_AMT];
}

// osc_amp

lab_t _osc_amp_out_lab() {
  lab_t osc1 = _osc1_drive_lab();
  lab_t osc2 = _osc2_drive_lab();
  return _interpolate_lab(osc1, osc2);
}

bool _osc_amp_out_changed() {
  return ALWAYS_UPDATE || _osc1_drive_changed() || _osc2_drive_changed();
}

// sub_squ

lab_t _sub_squ_lab() {
  return _primitive_lab(0.0, 1.0, 1.0, CTRL_SUB_LVL);
}

bool _sub_squ_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_SUB_LVL];
}

// sub_noise

lab_t _sub_noise_lab() {
  return _primitive_lab(1.0, 0.0, 1.0, CTRL_SUB_NOISE_LVL);
}

bool _sub_noise_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_SUB_NOISE_LVL];
}

// sub_mix

lab_t _sub_mix_lab() {
  lab_t squ = _sub_squ_lab();
  lab_t noise = _sub_noise_lab();
  lab_t out = _interpolate_lab(squ, noise);
  return out;
}

bool _sub_mix_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_SUB_NOISE_LVL] || commit_ctrl.changed[CTRL_SUB_LVL];
}

// CTRL_SUB_TO_OSC2_MIX]_mix

lab_t _sub_to_osc2_mix_lab() {
  lab_t out = _sub_mix_lab();
  out.L *= _ctrl_double(CTRL_SUB_TO_OSC2_MIX);
  out.v = commit_ctrl.value[CTRL_SUB_TO_OSC2_MIX];
  return out;
}

bool _sub_to_osc2_mix_changed() {
  return ALWAYS_UPDATE
      || commit_ctrl.changed[CTRL_SUB_NOISE_LVL]
      || commit_ctrl.changed[CTRL_SUB_TO_OSC2_MIX]
      || commit_ctrl.changed[CTRL_SUB_LVL];
}

// sub_filt

lab_t _sub_filt_freq_lab() {
  return _filt_freq_lab(_sub_mix_lab(), CTRL_SUB_FILT_CUTOFF);
}

bool _sub_filt_freq_changed() {
  return ALWAYS_UPDATE || _sub_mix_changed() || commit_ctrl.changed[CTRL_SUB_FILT_CUTOFF];
}

lab_t _sub_filt_reso_lab() {
  return _filt_reso_lab(_sub_filt_freq_lab(), CTRL_SUB_FILT_RES);
}

bool _sub_filt_reso_changed() {
  return ALWAYS_UPDATE || _sub_filt_freq_changed() || commit_ctrl.changed[CTRL_SUB_FILT_RES];
}

lab_t _sub_filt_out_lab() {
  return _sub_filt_reso_lab();
}

bool _sub_filt_out_changed() {
  return ALWAYS_UPDATE || commit_ctrl.changed[CTRL_SUB_NOISE_LVL] || commit_ctrl.changed[CTRL_SUB_LVL];
}

lab_t _sub_amp_out_lab() {
  return _sub_filt_out_lab();
}

bool _sub_amp_out_changed() {
  return ALWAYS_UPDATE || _sub_filt_reso_changed();
}

// fx_dry

lab_t _fx_dry_lab() {
  lab_t osc = _osc_amp_out_lab();
  lab_t sub = _sub_amp_out_lab();
  lab_t out = _interpolate_lab(osc, sub);
  out.L *= _ctrl_double_inverse(CTRL_FX_WETDRY);
  out.v = 4095 - commit_ctrl.value[CTRL_FX_WETDRY];
  return out;
}

bool _fx_dry_changed() {
  return ALWAYS_UPDATE || _osc_amp_out_changed() || _sub_amp_out_changed() || commit_ctrl.changed[CTRL_FX_WETDRY];
}

lab_t _fx_wet_lab() {
  lab_t out = _interpolate_lab(_osc_amp_out_lab(), _sub_amp_out_lab());
  out.L *= _ctrl_double(CTRL_FX_WETDRY);
  out.v = commit_ctrl.value[CTRL_FX_WETDRY];
  return out;
}

bool _fx_wet_changed() {
  return ALWAYS_UPDATE || _osc_amp_out_changed() || _sub_amp_out_changed() || commit_ctrl.changed[CTRL_FX_WETDRY];
}

lab_t _fx_feedback_lab() {
  lab_t out = _fx_wet_lab();
  out.L *= _ctrl_double(CTRL_FX_FEEDBACK);
  out.v = commit_ctrl.value[CTRL_FX_FEEDBACK];
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
	uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc1 Saw
	 * LEFT0:00
	 * 0, 1
	 */

	if (!_osc1_saw_changed()) return;

	_set_pwm_seq_lab(_osc1_saw_lab(), pwm_seq, 2*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc1 Squ
	 * LEFT0:00
	 * 2, 3
	 */

	if (!_osc1_squ_changed()) return;

	_set_pwm_seq_lab(_osc1_squ_lab(), pwm_seq, 2*3);

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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc1 Mix
	 * LEFT0:00
	 * 4, 5, 6, 7, 8, 9, 10
	 */

	if (!_osc1_mix_changed()) return;

	_set_pwm_seq_lab(_osc1_mix_lab(), pwm_seq, 7*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc1 Filter
	 * LEFT0:10
	 * Freq: 3, 4
	 */

	if (!_osc1_filt_freq_changed()) return;

	_set_pwm_seq_lab(_osc1_filt_freq_lab(), pwm_seq, 4*3);
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
  uint16_t pwm_seq[36];
  uint8_t scale_seq[36];

  /* Osc1 Filter
   * LEFT0:10
   * Reso: 5, 6
   */

  if (!_osc1_filt_reso_changed()) return;

  _set_pwm_seq_lab(_osc1_filt_reso_lab(), pwm_seq, 4*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc1 Drive
	 * RIGHT1:00
	 * 0, 1, 3, 2 <--- Note 3 before 2
	 *
	 */

	if (!_osc1_drive_changed()) return;

	_set_pwm_seq_lab(_osc1_drive_lab(), pwm_seq, 4*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc1toOsc2 Mix Level
	 * LEFT0:10
	 * 0, 1
	 */

	if (!_osc1_to_2_mix_changed()) return;

	_set_pwm_seq_lab(_osc1_to_2_mix_lab(), pwm_seq, 2*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];


	/* Osc2 Saw
	 * LEFT0:11
	 * 1, 2
	 */

	if (!_osc2_saw_changed()) return;

	_set_pwm_seq_lab(_osc2_saw_lab(), pwm_seq, 2*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc2 Squ
	 * LEFT0:11
	 * 0
	 */

	if (!_osc2_squ_changed()) return;

	_set_pwm_seq_lab(_osc2_squ_lab(), pwm_seq, 1*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc2 Noise
	 * LEFT0:11
	 * 3, 4
	 */

	if(!_osc2_noise_changed()) return;

	_set_pwm_seq_lab(_osc2_noise_lab(), pwm_seq, 2*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc2-Only Mix
	 * LEFT0:11
	 * 5, 6, 7, 8
	 */

	if (!_osc2_mix_changed()) return;

	_set_pwm_seq_lab(_osc2_mix_lab(), pwm_seq, 4*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc2-PreFilter Mix
	 * LEFT0:11
	 * 9, 10
	 */

	if (!_osc2_prefilt_changed()) return;

	_set_pwm_seq_lab(_osc2_prefilt_lab(), pwm_seq, 2*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc2 Filt
	 * LEFT3:000
	 * Freq: 0, 1
	 */

	if (!_osc2_filt_freq_changed()) return;

	_set_pwm_seq_lab(_osc2_filt_freq_lab(), pwm_seq, 4*3);
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
  uint16_t pwm_seq[36];
  uint8_t scale_seq[36];

  /* Osc2 Filt
   * LEFT3:000
   * Reso: 2, 3
   */

  if (!_osc2_filt_reso_changed()) return;

  _set_pwm_seq_lab(_osc2_filt_reso_lab(), pwm_seq, 4*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc2 Drive
	 * LEFT3:10
	 * 3, 4, 5, 6, 7, 8, 9, 10
	 */

	if (!_osc2_drive_changed()) return;

	_set_pwm_seq_lab(_osc2_drive_lab(), pwm_seq, 8*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];


	/* Osc Amp Out
	 * RIGHT1:00
	 * 4, 5, 6, 7, 8
	 *
	 */

	if (!_osc_amp_out_changed()) return;

	_set_pwm_seq_lab(_osc_amp_out_lab(), pwm_seq, 5*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Sub Amp Out
	 * RIGHT1:01
	 * 7, 8, 9, 10, 11
	 *
	 */

	if (!_sub_amp_out_changed()) return;

	_set_pwm_seq_lab(_sub_amp_out_lab(), pwm_seq, 5*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Sub Squ
	 * LEFT1:00
	 * 0, 1
	 */

	if (!_sub_squ_changed()) return;

	_set_pwm_seq_lab(_sub_squ_lab(), pwm_seq, 2*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Sub Noise
	 * LEFT1:00
	 * 2, 3
	 */

	if (!_sub_noise_changed()) return;

	_set_pwm_seq_lab(_sub_noise_lab(), pwm_seq, 2*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Sub Mix
	 * LEFT1:00
	 * 4, 5, 6, 7, 8, 9, 10
	 */

	if (!_sub_mix_changed()) return;

	_set_pwm_seq_lab(_sub_mix_lab(), pwm_seq, 7*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* SubToOsc2 Mix Level
	 * LEFT3:00
	 * 7, 8
	 */

	if (!_sub_to_osc2_mix_changed()) return;

	_set_pwm_seq_lab(_sub_to_osc2_mix_lab(), pwm_seq, 2*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Sub Filter Freq Cutoff
	 * LEFT1:10
	 * 0, 1,
	 */

	if (!_sub_filt_freq_changed()) return;

	_set_pwm_seq_lab(_sub_filt_freq_lab(), pwm_seq, 2*3);
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
  uint16_t pwm_seq[36];
  uint8_t scale_seq[36];

  /* Sub Filter Out
   * LEFT1:10
   * 2, 3, 4, 5, 6, 7, 8
   */

  if (!_sub_filt_reso_changed()) return;

  _set_pwm_seq_lab(_sub_filt_reso_lab(), pwm_seq, 7*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* FX Dry
	 * RIGHT2:01
	 * 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
	 *
	 */

	if (!_fx_dry_changed()) return;

	_set_pwm_seq_lab(_fx_dry_lab(), pwm_seq, 11*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* FX Wet
	 * RIGHT1:01
	 * 0, 1, 2, 3, 4, 5, 6
	 *
	 */

	if (!_fx_wet_changed()) return;

	_set_pwm_seq_lab(_fx_wet_lab(), pwm_seq, 7*3);
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
  uint16_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* FX Feedback
	 * RIGHT2:10
	 * 0, 1, 2, 3, 4, 5, 7
	 *
	 */

	if (!_fx_feedback_changed()) return;

	_set_pwm_seq_lab(_fx_feedback_lab(), pwm_seq, 8*3);
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
