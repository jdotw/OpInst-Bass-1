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

uint8_t _max(uint16_t a, uint16_t b) {
	if (a > b) return a;
	else return b;
}

uint8_t _min(uint16_t a, uint16_t b) {
	if (a < b) return a;
	else return b;
}


uint8_t _12_to_8(uint16_t a) {
	return a >> 4;
}

#define DEFAULT_SCALE 0x15
#define DEFAULT_SCALE_R 0x17
#define DEFAULT_SCALE_G 0x07
#define DEFAULT_SCALE_B 0x16

/*
 * Utilities
 */

void _set_pwm_seq(uint16_t *rgb, uint8_t *pwm_seq, uint8_t len) {
	for (uint8_t i=0; i < len; i++) {
		pwm_seq[i] = _12_to_8(rgb[i%3]);
	}
}

void _set_scale_seq(uint8_t *pwm_seq, uint8_t *scale_seq, uint8_t len) {
	for (uint8_t i=0; i < len; i++) {
		switch(i%3) {
		case 0:
			scale_seq[i] = DEFAULT_SCALE_R;
			break;
		case 1:
			scale_seq[i] = DEFAULT_SCALE_G;
			break;
		case 2:
			scale_seq[i] = DEFAULT_SCALE_B;
			break;
		}
	}
}

/*
 * RGB Calculations
 */

static uint16_t val[3];

uint16_t* _osc1_saw_rgb() {
	val[0] = ctrl_value.osc1_saw_lvl;
	val[1] = 0;
	val[2] = 0;
	return val;
}

uint16_t* _osc1_squ_rgb() {
	val[0] = 0;
	val[1] = 0;
	val[2] = ctrl_value.osc1_squ_lvl;
	return val;
}

uint16_t* _osc1_mix_rgb() {
	val[0] = ctrl_value.osc1_saw_lvl;
	val[1] = 0;
	val[2] = ctrl_value.osc1_squ_lvl;
	return val;
}

uint16_t* _osc1_to_2_mix() {
	val[0] = (double)ctrl_value.osc1_saw_lvl * ((double)ctrl_value.osc1_to_osc2 / 4095.0);
	val[1] = 0;
	val[2] = (double)ctrl_value.osc1_squ_lvl * ((double)ctrl_value.osc1_to_osc2 / 4095.0);
	return val;
}

uint16_t* _osc2_saw_rgb() {
	val[0] = ctrl_value.osc2_saw_lvl;
	val[1] = 0;
	val[2] = 0;
	return val;
}

uint16_t* _osc2_squ_rgb() {
	val[0] = 0;
	val[1] = 0;
	val[2] = ctrl_value.osc2_squ_lvl;
	return val;
}

uint16_t* _osc2_noise_rgb() {
	val[0] = 0;
	val[1] = ctrl_value.osc2_noise_lvl;
	val[2] = 0;
	return val;
}

uint16_t* _osc2_mix() {
	/* Osc2-Only Mix
	 * LEFT0:11
	 * 5, 6, 7, 8
	 */

	val[0] = ctrl_value.osc2_saw_lvl;
	val[1] = ctrl_value.osc2_noise_lvl;
	val[2] = ctrl_value.osc2_squ_lvl;
	return val;
}

uint16_t* _osc2_prefilt_rgb() {
	val[0] = ctrl_value.osc2_saw_lvl | (uint16_t)((double)ctrl_value.osc1_saw_lvl * ((double)ctrl_value.osc1_to_osc2 / 4095.0));
	val[1] = ctrl_value.osc2_noise_lvl | (uint16_t)((double)ctrl_value.sub_noise_lvl * ((double)ctrl_value.sub_to_osc2 / 4095.0));
	val[2] = ctrl_value.osc2_squ_lvl | (uint16_t)((double)ctrl_value.osc1_squ_lvl * ((double)ctrl_value.osc1_to_osc2 / 4095.0)) | (uint16_t)((double)ctrl_value.sub_lvl * ((double)ctrl_value.sub_to_osc2 / 4095.0));
	return val;
}

uint16_t* _osc2_filt_freq_rgb() {
	val[0] = ctrl_value.osc2_saw_lvl | (uint16_t)((double)ctrl_value.osc1_saw_lvl * ((double)ctrl_value.osc1_to_osc2 / 4095.0));
	val[1] = ctrl_value.osc2_noise_lvl | (uint16_t)((double)ctrl_value.sub_noise_lvl * ((double)ctrl_value.sub_to_osc2 / 4095.0));
	val[2] = ctrl_value.osc2_squ_lvl | (uint16_t)((double)ctrl_value.osc1_squ_lvl * ((double)ctrl_value.osc1_to_osc2 / 4095.0)) | (uint16_t)((double)ctrl_value.sub_lvl * ((double)ctrl_value.sub_to_osc2 / 4095.0));
	return val;
}

uint16_t* _osc2_filt_reso_rgb() {
	val[0] = ctrl_value.osc2_saw_lvl | (uint16_t)((double)ctrl_value.osc1_saw_lvl * ((double)ctrl_value.osc1_to_osc2 / 4095.0));
	val[1] = ctrl_value.osc2_noise_lvl | (uint16_t)((double)ctrl_value.sub_noise_lvl * ((double)ctrl_value.sub_to_osc2 / 4095.0));
	val[2] = ctrl_value.osc2_squ_lvl | (uint16_t)((double)ctrl_value.osc1_squ_lvl * ((double)ctrl_value.osc1_to_osc2 / 4095.0)) | (uint16_t)((double)ctrl_value.sub_lvl * ((double)ctrl_value.sub_to_osc2 / 4095.0));
	return val;
}

uint16_t* _osc2_drive_rgb() {
	val[0] = ctrl_value.osc2_saw_lvl | (uint16_t)((double)ctrl_value.osc1_saw_lvl * ((double)ctrl_value.osc1_to_osc2 / 4095.0));
	val[1] = ctrl_value.osc2_noise_lvl | (uint16_t)((double)ctrl_value.sub_noise_lvl * ((double)ctrl_value.sub_to_osc2 / 4095.0));
	val[2] = ctrl_value.osc2_squ_lvl | (uint16_t)((double)ctrl_value.osc1_squ_lvl * ((double)ctrl_value.osc1_to_osc2 / 4095.0)) | (uint16_t)((double)ctrl_value.sub_lvl * ((double)ctrl_value.sub_to_osc2 / 4095.0));
	return val;
}

uint16_t* _osc_amp_out_rgb() {
	val[0] = ctrl_value.osc1_saw_lvl | ctrl_value.osc2_saw_lvl | (uint16_t)((double)ctrl_value.osc1_saw_lvl * ((double)ctrl_value.osc1_to_osc2 / 4095.0));
	val[1] = ctrl_value.osc2_noise_lvl | (uint16_t)((double)ctrl_value.sub_noise_lvl * ((double)ctrl_value.sub_to_osc2 / 4095.0));
	val[2] = ctrl_value.osc1_squ_lvl | ctrl_value.osc2_squ_lvl | (uint16_t)((double)ctrl_value.osc1_squ_lvl * ((double)ctrl_value.osc1_to_osc2 / 4095.0)) | (uint16_t)((double)ctrl_value.sub_lvl * ((double)ctrl_value.sub_to_osc2 / 4095.0));
	return val;
}

uint16_t* _sub_squ_rgb() {
	val[0] = 0;
	val[1] = 0;
	val[2] = ctrl_value.sub_lvl;
	return val;
}

uint16_t* _sub_noise_rgb() {
	val[0] = 0;
	val[1] = ctrl_value.sub_noise_lvl;
	val[2] = 0;
	return val;
}

uint16_t* _sub_mix_rgb() {
	val[0] = 0;
	val[1] = ctrl_value.sub_noise_lvl;
	val[2] = ctrl_value.sub_lvl;
	return val;
}

uint16_t* _sub_to_osc2_mix_rgb() {
	val[0] = 0;
	val[1] = ((double)ctrl_value.sub_noise_lvl * ((double)ctrl_value.sub_to_osc2 / 4095.0));
	val[2] = ((double)ctrl_value.sub_lvl * ((double)ctrl_value.sub_to_osc2 / 4095.0));
	return val;
}

uint16_t* _sub_filt_out_rgb() {
	val[0] = 0;
	val[1] = ctrl_value.sub_noise_lvl;
	val[2] = ctrl_value.sub_lvl;
	return val;
}

uint16_t* _sub_amp_out_rgb() {
	val[0] = 0;
	val[1] = ctrl_value.sub_noise_lvl;
	val[2] = ctrl_value.sub_lvl;
	return val;
}

void _rgb_copy(uint16_t *dst, uint16_t *src) {
	dst[0] = src[0];
	dst[1] = src[1];
	dst[2] = src[2];
}

uint16_t* _fx_dry_rgb() {
	uint16_t osc_amp_out_rgb[3];
	_rgb_copy(osc_amp_out_rgb, _osc_amp_out_rgb());
	uint16_t sub_amp_out_rgb[3];
	_rgb_copy(sub_amp_out_rgb, _sub_amp_out_rgb());
	val[0] = (osc_amp_out_rgb[0] | sub_amp_out_rgb[0]) * ((double)(4095-ctrl_value.fx_wetdry) / 4095);
	val[1] = (osc_amp_out_rgb[1] | sub_amp_out_rgb[1]) * ((double)(4095-ctrl_value.fx_wetdry) / 4095);
	val[2] = (osc_amp_out_rgb[2] | sub_amp_out_rgb[2]) * ((double)(4095-ctrl_value.fx_wetdry) / 4095);
	return val;
}

uint16_t* _fx_wet_rgb() {
	uint16_t osc_amp_out_rgb[3];
	_rgb_copy(osc_amp_out_rgb, _osc_amp_out_rgb());
	uint16_t sub_amp_out_rgb[3];
	_rgb_copy(sub_amp_out_rgb, _sub_amp_out_rgb());
	val[0] = (osc_amp_out_rgb[0] | sub_amp_out_rgb[0]) * ((double)ctrl_value.fx_wetdry / 4095);
	val[1] = (osc_amp_out_rgb[1] | sub_amp_out_rgb[1]) * ((double)ctrl_value.fx_wetdry / 4095);
	val[2] = (osc_amp_out_rgb[2] | sub_amp_out_rgb[2]) * ((double)ctrl_value.fx_wetdry / 4095);
	return val;
}

uint16_t* _fx_feedback_rgb() {
	uint16_t fx_wet_rgb[3];
	_rgb_copy(fx_wet_rgb, _fx_wet_rgb());
	val[0] = (fx_wet_rgb[0]) * ((double)ctrl_value.fx_feedback / 4095);
	val[1] = (fx_wet_rgb[1]) * ((double)ctrl_value.fx_feedback / 4095);
	val[2] = (fx_wet_rgb[2]) * ((double)ctrl_value.fx_feedback / 4095);
	return val;
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

	_set_pwm_seq(_osc1_mix_rgb(), pwm_seq, 7*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0, 12, pwm_seq, 7*3);
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 7*3);
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0, 12, scale_seq, 7*3);
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0);
	if (!res) Error_Handler();
}

void _commit_led_osc1_filt() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc1 Filter
	 * LEFT0:10
	 * Freq: 3, 4
	 * Reso: 5, 6
	 */

	_set_pwm_seq(_osc1_mix_rgb(), pwm_seq, 4*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 2, (3*3), pwm_seq, (4*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 4*3);
	res = is32_set_sequence_scale(I2C_LEFT, 0, 2, (3*3), scale_seq, (4*3));
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

	_set_pwm_seq(_osc1_mix_rgb(), pwm_seq, 4*3);
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

	_set_pwm_seq(_osc2_prefilt_rgb(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b11, (9*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 2*3);
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0b11, (9*3), scale_seq, (2*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0b11);
	if (!res) Error_Handler();
}

void _commit_led_osc2_filt() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* Osc2 Filt
	 * LEFT3:000
	 * Freq: 0, 1
	 * Reso: 2, 3
	 */

	_set_pwm_seq(_osc2_filt_freq_rgb(), pwm_seq, 4*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 3, 0b00, (0*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 2*3);
	res = is32_set_sequence_scale(I2C_LEFT, 3, 0b00, (0*3), scale_seq, (2*3));
	if (!res) Error_Handler();

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

	_set_pwm_seq(_sub_mix_rgb(), pwm_seq, 7*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 1, 0b00, (4 * 3), pwm_seq, (7 * 3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 7*3);
	res = is32_set_sequence_scale(I2C_LEFT, 1, 0b00, (4 * 3), scale_seq, (7 * 3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 1, 0b00);
	if (!res) Error_Handler();
}

void _commit_led_sub_to_osc2() {
	bool res;
	uint8_t pwm_seq[36];
	uint8_t scale_seq[36];

	/* SubToOsc2 Mix Level
	 * LEFT3:00
	 * 7, 8
	 */

	_set_pwm_seq(_sub_to_osc2_mix_rgb(), pwm_seq, 2*3);
	res = is32_set_sequence_pwm(I2C_LEFT, 3, 0b00, (7*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

	_set_scale_seq(pwm_seq, scale_seq, 2*3);
	res = is32_set_sequence_scale(I2C_LEFT, 3, 0b00, (7*3), scale_seq, (2*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 3, 0b00);
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
	case COMMIT_LED_OSC1_FILT:
		_commit_led_osc1_filt();
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
	case COMMIT_LED_OSC2_FILT:
		_commit_led_osc2_filt();
		break;
	case COMMIT_LED_OSC2_DRIVE:
		_commit_led_osc2_drive();
		break;

	case COMMIT_LED_OSC_AMP_OUT:
		_commit_led_osc_amp_out();
		break;
	case COMMIT_LED_SUB_AMP_OUT:
		break;
		_commit_led_sub_amp_out();

	case COMMIT_LED_SUB_SQU:
		_commit_led_sub_squ();
		break;
	case COMMIT_LED_SUB_NOISE:
		_commit_led_sub_noise();
		break;
	case COMMIT_LED_SUB_MIX:
		_commit_led_sub_mix();
	case COMMIT_LED_SUB_TO_OSC2:
		break;
		_commit_led_sub_to_osc2();
	case COMMIT_LED_SUB_FILT_OUT:
		_commit_led_sub_filt_out();
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

//
//	_commit_led_osc1_saw();
//	ticks_after = HAL_GetTick();
//	ticks_cost = ticks_after - ticks_before;
//
//	ticks_before = HAL_GetTick();
//	_commit_led_osc1_squ();
//	ticks_after = HAL_GetTick();
//	ticks_cost = ticks_after - ticks_before;
//
//	ticks_before = HAL_GetTick();
//	_commit_led_osc1_mix();
//	ticks_after = HAL_GetTick();
//	ticks_cost = ticks_after - ticks_before;
//
//	ticks_before = HAL_GetTick();
//	_commit_led_osc1_filt();
//	ticks_after = HAL_GetTick();
//	ticks_cost = ticks_after - ticks_before;
//
//	ticks_before = HAL_GetTick();
//	_commit_led_osc1_drive();
//	ticks_after = HAL_GetTick();
//	ticks_cost = ticks_after - ticks_before;
//
//	ticks_before = HAL_GetTick();
//	_commit_led_osc1_to_osc2();
//	ticks_after = HAL_GetTick();
//	ticks_cost = ticks_after - ticks_before;
//
//	ticks_before = HAL_GetTick();
//	_commit_led_osc2();
//	ticks_after = HAL_GetTick();
//	ticks_cost = ticks_after - ticks_before;
//
//	ticks_before = HAL_GetTick();
//	_commit_led_amp();
//	ticks_after = HAL_GetTick();
//	ticks_cost = ticks_after - ticks_before;
//
//	ticks_before = HAL_GetTick();
//	_commit_led_sub();
//	ticks_after = HAL_GetTick();
//	ticks_cost = ticks_after - ticks_before;
//
//	ticks_before = HAL_GetTick();
//	_commit_led_fx();
//	ticks_after = HAL_GetTick();
//	ticks_cost = ticks_after - ticks_before;
}
