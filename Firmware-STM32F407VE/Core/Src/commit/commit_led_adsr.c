/*
 * commit_led_adsr.c
 *
 *  Created on: 3 May 2021
 *      Author: jwilson
 */

#include "main.h"
#include "commit.h"
#include "is32.h"
#include "i2c.h"
#include "ctrl.h"

#define MASK_2_MSB 0b110000000000
#define MASK_10_LSB  0b1111111111

#define THIRD_1    0b010000000000
#define THIRD_2    0b100000000000
#define THIRD_3    0b110000000000

#define MAX_PWM 0x80

static uint8_t grid[3][3];

void _adsr_led_set_grid_curve(uint16_t val) {
	/* Produces graph like this:
	 *
	 * [0,2][1,2][2,2]
	 * [0,1][1,1]
	 * [0,0]
	 *
	 */

	grid[0][0] = 0;
	grid[0][1] = 0;
	grid[0][2] = 0;
	grid[1][0] = 0;
	grid[1][1] = 0;
	grid[1][2] = 0;
	grid[2][0] = 0;
	grid[2][1] = 0;
	grid[2][2] = 0;

	grid[0][0] = 0xFF;	// Always on

	if (val <= 1365) {
		// 0% - 33%
		// [0,2] -> [1,2]
		uint8_t delta = (val * 3) >> 4;
		grid[0][2] = 0xFF - delta;
		grid[1][2] = delta;
		grid[0][1] = 0xFF;
	}
	else if (val <= (1365 * 2)) {
		// 33% - 66%
		// [0,1] -> [1,1]
		uint8_t delta = ((val - 1365) * 3) >> 4;
		grid[0][1] = 0xFF - delta;
		grid[1][1] = delta;
		grid[1][2] = 0xFF;
	} else {
		// 66% - 100%
		// [2,1] -> [2,2]
		uint8_t delta = ((val - (1365 * 2)) * 3) >> 4;
		grid[1][2] = 0xFF - delta;
		grid[2][2] = delta;
		grid[1][1] = 0xFF;
	}
}

void _adsr_led_set_grid_stack(uint16_t val) {
	/* Produces graph like this:
	 *
	 * [0,2][1,2]
	 * [0,1][1,1]
	 * [0,0][1,0]
	 *
	 */

	grid[0][0] = 0;
	grid[0][1] = 0;
	grid[0][2] = 0;
	grid[1][0] = 0;
	grid[1][1] = 0;
	grid[1][2] = 0;
	grid[2][0] = 0;
	grid[2][1] = 0;
	grid[2][2] = 0;

	if (val <= 1365) {
		// 0% - 33%
		// [0,2] -> [1,2]
		uint8_t delta = (val * 3) >> 4;
		grid[0][0] = delta;
		grid[1][0] = delta;
	}
	else if (val <= (1365 * 2)) {
		// 33% - 66%
		// [0,1] -> [1,1]
		uint8_t delta = ((val - 1365) * 3) >> 4;
		grid[0][0] = 0xFF;
		grid[1][0] = 0xFF;
		grid[0][1] = delta;
		grid[1][1] = delta;
	} else {
		// 66% - 100%
		// [2,1] -> [2,2]
		uint8_t delta = ((val - (1365 * 2)) * 3) >> 4;
		grid[0][0] = 0xFF;
		grid[1][0] = 0xFF;
		grid[0][1] = 0xFF;
		grid[1][1] = 0xFF;
		grid[0][2] = delta;
		grid[1][2] = delta;
	}
}

void _adsr_led_set_grid_bar(uint16_t val) {
	/* Produces graph like this:
	 *
	 * [0,2][1,2]
	 * [0,1][1,1]
	 * [0,0][1,0]
	 *
	 */

	grid[0][0] = 0;
	grid[0][1] = 0;
	grid[0][2] = 0;
	grid[1][0] = 0;
	grid[1][1] = 0;
	grid[1][2] = 0;
	grid[2][0] = 0;
	grid[2][1] = 0;
	grid[2][2] = 0;

	if (val <= 2048) {
		// 0% - 50%
		// [0,0][1,0] -> [0,1][1,1]
		uint8_t delta = (val * 2) >> 4;
		grid[0][0] = 0xFF - delta;
		grid[1][0] = 0xFF - delta;
		grid[0][1] = delta;
		grid[1][1] = delta;
	}
	else {
		// 50-100%
		// [0,1][1,1] -> [0,2][1,2]
		uint8_t delta = ((val - 2048) * 2) >> 4;
		grid[0][1] = 0xFF - delta;
		grid[1][1] = 0xFF - delta;
		grid[0][2] = delta;
		grid[1][2] = delta;
	}
}

#define DEFAULT_BRIGHTNESS 0x30
#define HALF_BRIGHTNESS 0x05

void _commit_led_adsr_osc_filt_env() {
	HAL_StatusTypeDef res;
	uint8_t brightness = 0x80;

	uint16_t a_val, d_val, s_val, r_val;
	switch (ctrl_toggle.osc_filt_env_attack_func) {
	case CTRL_SELECT_ENV_1:
		a_val = ctrl_value.osc_filt_env1_a;
		d_val = ctrl_value.osc_filt_env1_d;
		switch(ctrl_toggle.osc_filt_env_sustain_func) {
		case CTRL_ENV_SUSTAIN:
			s_val = ctrl_value.osc_filt_env1_s;
			break;
		case CTRL_ENV_AMOUNT:
			s_val = ctrl_value.osc_filt_env1_amt;
			break;
		default:
			s_val = 0;
		}
		r_val = ctrl_value.osc_filt_env1_r;
		break;
	case CTRL_SELECT_ENV_2:
		a_val = ctrl_value.osc_filt_env2_a;
		d_val = ctrl_value.osc_filt_env2_d;
		switch(ctrl_toggle.osc_filt_env_sustain_func) {
		case CTRL_ENV_SUSTAIN:
			s_val = ctrl_value.osc_filt_env2_s;
			break;
		case CTRL_ENV_AMOUNT:
			s_val = ctrl_value.osc_filt_env2_amt;
			break;
		default:
			s_val = 0;
		}
		r_val = ctrl_value.osc_filt_env2_r;
		break;
	default:
		a_val = 0;
		d_val = 0;
		s_val = 0;
		r_val = 0;
		break;
	}

	switch (ctrl_toggle.osc_filt_env_sustain_func) {
	case CTRL_ENV_SUSTAIN:
		brightness = DEFAULT_BRIGHTNESS;
		break;
	case CTRL_ENV_AMOUNT:
		brightness = HALF_BRIGHTNESS;
		break;
	default:
		break;
	}


	// I2C Left 3
	res = i2c_mux_select(I2C_LEFT, I2C_LEFT_MUX, 3);
	if (res != HAL_OK) {
		Error_Handler();
	}

	/* Osc 1 ADSR LEDs
	 *
	 * A: LEFT3:001
	 *
	 * [0,2=3][1,2=1][2,2=0]
	 * [0,1=4][1,1=2]
	 * [0,0=5]
	 */

	_adsr_led_set_grid_curve(a_val);

	res = is32_turn_on_led_single(0, 1, 5, grid[0][0], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 4, grid[0][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 3, grid[0][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 2, grid[1][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 1, grid[1][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 0, grid[2][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	/* D: G1Cx
	 *
	 * [0,2][1,2][2,2]
	 * [0,1][1,1]
	 * [0,0]
	 *
	 * transposes to:
	 *
	 * [0,0=27]
	 * [0,1=28][1,1=25]
	 * [0,2=29][1,2=26][2,2=24]
	 *
	 */

	_adsr_led_set_grid_curve(d_val);

	res = is32_turn_on_led_single(0, 1, 24, grid[2][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 25, grid[1][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 26, grid[1][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 27, grid[0][0], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 28, grid[0][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 29, grid[0][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}


	/*
	 *
	 * S: G1Cx
	 *
	 * [0,2=14=23][1,2=17=18]
	 * [0,1=13=22][1,1=16=19]
	 * [0,0=12=21][1,0=15=20]
	 *
	 */

	switch (ctrl_toggle.osc_filt_env_sustain_func) {
	case CTRL_ENV_SUSTAIN:
		_adsr_led_set_grid_bar(s_val);
		break;
	case CTRL_ENV_AMOUNT:
		_adsr_led_set_grid_stack(s_val);
		break;
	default:
		break;
	}

	// Sustain graph is always DEFAULT_BRIGHTNESS

	res = is32_turn_on_led_single(0, 1, 23, grid[0][2], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 22, grid[0][1], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 21, grid[0][0], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 20, grid[1][0], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 19, grid[1][1], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 18, grid[1][2], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}


	/*
	 * R: G1Cx
	 *
	 * [0,2][1,2][2,2]
	 * [0,1][1,1]
	 * [0,0]
	 *
	 * transposes to:
	 *
	 *
	 * [0,0=20=17]
	 * [0,1=19=16][1,1=22=13]
	 * [0,2=18=15][1,2=21=14][2,2=23=12]
	 *
	 */

	_adsr_led_set_grid_curve(r_val);

	res = is32_turn_on_led_single(0, 1, 12, grid[2][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 13, grid[1][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 14, grid[1][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 17, grid[0][0], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 16, grid[0][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 15, grid[0][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

}

void _commit_led_adsr_sub_filt_env() {
	HAL_StatusTypeDef res;
	uint8_t brightness = 0x80;

	uint16_t a_val, d_val, s_val, r_val;
	switch (ctrl_toggle.sub_filt_env_attack_func) {
	case CTRL_SELECT_ENV_1:
		a_val = ctrl_value.sub_filt_env1_a;
		d_val = ctrl_value.sub_filt_env1_d;
		switch(ctrl_toggle.sub_filt_env_sustain_func) {
		case CTRL_ENV_SUSTAIN:
			s_val = ctrl_value.sub_filt_env1_s;
			break;
		case CTRL_ENV_AMOUNT:
			s_val = ctrl_value.sub_filt_env1_amt;
			break;
		default:
			s_val = 0;
		}
		r_val = ctrl_value.sub_filt_env1_r;
		break;
	case CTRL_SELECT_ENV_2:
		a_val = ctrl_value.sub_filt_env2_a;
		d_val = ctrl_value.sub_filt_env2_d;
		switch(ctrl_toggle.sub_filt_env_sustain_func) {
		case CTRL_ENV_SUSTAIN:
			s_val = ctrl_value.sub_filt_env2_s;
			break;
		case CTRL_ENV_AMOUNT:
			s_val = ctrl_value.sub_filt_env2_amt;
			break;
		default:
			s_val = 0;
		}
		r_val = ctrl_value.sub_filt_env2_r;
		break;
	default:
		a_val = 0;
		d_val = 0;
		s_val = 0;
		r_val = 0;
		break;
	}

	switch (ctrl_toggle.sub_filt_env_sustain_func) {
	case CTRL_ENV_SUSTAIN:
		brightness = DEFAULT_BRIGHTNESS;
		break;
	case CTRL_ENV_AMOUNT:
		brightness = HALF_BRIGHTNESS;
		break;
	default:
		break;
	}


	// I2C Left 1
	res = i2c_mux_select(I2C_LEFT, I2C_LEFT_MUX, 1);
	if (res != HAL_OK) {
		Error_Handler();
	}

	/* Osc 1 ADSR LEDs
	 *
	 * A: LEFT1:11
	 *
	 * [0,2=2][1,2=4][2,2=5]
	 * [0,1=1][1,1=3]
	 * [0,0=0]
	 */

	_adsr_led_set_grid_curve(a_val);

	res = is32_turn_on_led_single(0, 3, 0, grid[0][0], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 1, grid[0][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 2, grid[0][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 3, grid[1][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 4, grid[1][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 5, grid[2][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	/* D: G1Cx
	 *
	 * [0,2][1,2][2,2]
	 * [0,1][1,1]
	 * [0,0]
	 *
	 * transposes to:
	 *
	 * [0,0=08]
	 * [0,1=07][1,1=10]
	 * [0,2=06][1,2=09][2,2=11]
	 *
	 */

	_adsr_led_set_grid_curve(d_val);

	res = is32_turn_on_led_single(0, 3, 11, grid[2][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 10, grid[1][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 9, grid[1][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 8, grid[0][0], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 7, grid[0][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 6, grid[0][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}


	/*
	 *
	 * S: G1Cx
	 *
	 * [0,2=14=14][1,2=17=17]
	 * [0,1=13=13][1,1=16=16]
	 * [0,0=12=12][1,0=15=15]
	 *
	 */

	switch (ctrl_toggle.sub_filt_env_sustain_func) {
	case CTRL_ENV_SUSTAIN:
		_adsr_led_set_grid_bar(s_val);
		break;
	case CTRL_ENV_AMOUNT:
		_adsr_led_set_grid_stack(s_val);
		break;
	default:
		break;
	}

	// Sustain graph is always DEFAULT_BRIGHTNESS

	res = is32_turn_on_led_single(0, 3, 14, grid[0][2], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 13, grid[0][1], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 12, grid[0][0], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 15, grid[1][0], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 16, grid[1][1], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 17, grid[1][2], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}


	/*
	 * R: G1Cx
	 *
	 * [0,2][1,2][2,2]
	 * [0,1][1,1]
	 * [0,0]
	 *
	 * transposes to:
	 *
	 *
	 * [0,0=20]
	 * [0,1=19][1,1=22]
	 * [0,2=18][1,2=21][2,2=23]
	 *
	 */

	_adsr_led_set_grid_curve(r_val);

	res = is32_turn_on_led_single(0, 3, 23, grid[2][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 22, grid[1][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 21, grid[1][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 20, grid[0][0], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 19, grid[0][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 3, 18, grid[0][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

}

void _commit_led_adsr_osc_amp_env() {
	HAL_StatusTypeDef res;
	uint8_t brightness = 0x80;

	uint16_t a_val, d_val, s_val, r_val;
	a_val = ctrl_value.osc_amp_env_a;
	d_val = ctrl_value.osc_amp_env_d;
	switch(ctrl_toggle.osc_amp_env_sustain_func) {
	case CTRL_ENV_SUSTAIN:
		s_val = ctrl_value.osc_amp_env_s;
		break;
	case CTRL_ENV_AMOUNT:
		s_val = ctrl_value.osc_amp_env_amt;
		break;
	default:
		s_val = 0;
	}
	r_val = ctrl_value.osc_amp_env_r;

	switch (ctrl_toggle.osc_amp_env_sustain_func) {
	case CTRL_ENV_SUSTAIN:
		brightness = DEFAULT_BRIGHTNESS;
		break;
	case CTRL_ENV_AMOUNT:
		brightness = HALF_BRIGHTNESS;
		break;
	default:
		break;
	}


	// I2C Right 2
	res = i2c_mux_select(I2C_RIGHT, I2C_RIGHT_MUX, 2);
	if (res != HAL_OK) {
		Error_Handler();
	}

	/* Osc 1 ADSR LEDs
	 *
	 * A: RIGHT2:00
	 *
	 * [0,2=2=20][1,2=4=22][2,2=5=23]
	 * [0,1=1=19][1,1=3=21]
	 * [0,0=0=18]
	 */

	_adsr_led_set_grid_curve(a_val);

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 18, grid[0][0], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 19, grid[0][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 20, grid[0][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 21, grid[1][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 22, grid[1][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 23, grid[2][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	/* D: G1Cx
	 *
	 * [0,2][1,2][2,2]
	 * [0,1][1,1]
	 * [0,0]
	 *
	 * transposes to:
	 *
	 * [0,0=08=12]
	 * [0,1=07=14][1,1=10=13]
	 * [0,2=06=15][1,2=09=16][2,2=11=17]
	 *
	 */

	_adsr_led_set_grid_curve(d_val);

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 17, grid[2][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 13, grid[1][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 16, grid[1][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 12, grid[0][0], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 14, grid[0][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 15, grid[0][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}


	/*
	 *
	 * S: G1Cx
	 *
	 * [0,2=14=11][1,2=17=08]
	 * [0,1=13=10][1,1=16=07]
	 * [0,0=12=09][1,0=15=06]
	 *
	 */

	switch (ctrl_toggle.osc_amp_env_sustain_func) {
	case CTRL_ENV_SUSTAIN:
		_adsr_led_set_grid_bar(s_val);
		break;
	case CTRL_ENV_AMOUNT:
		_adsr_led_set_grid_stack(s_val);
		break;
	default:
		break;
	}

	// Sustain graph is always DEFAULT_BRIGHTNESS

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 11, grid[0][2], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 10, grid[0][1], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 9, grid[0][0], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 6, grid[1][0], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 7, grid[1][1], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 8, grid[1][2], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}


	/*
	 * R: G1Cx
	 *
	 * [0,2][1,2][2,2]
	 * [0,1][1,1]
	 * [0,0]
	 *
	 * transposes to:
	 *
	 *
	 * [0,0=20=2]
	 * [0,1=19=1][1,1=22=4]
	 * [0,2=18=0][1,2=21=3][2,2=23=5]
	 *
	 */

	_adsr_led_set_grid_curve(r_val);

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 5, grid[2][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 4, grid[1][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 3, grid[1][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 2, grid[0][0], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 1, grid[0][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 0, 0, grid[0][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

}

void _commit_led_adsr_sub_amp_env() {
	HAL_StatusTypeDef res;
	uint8_t brightness = 0x80;

	uint16_t a_val, d_val, s_val, r_val;
	a_val = ctrl_value.sub_amp_env_a;
	d_val = ctrl_value.sub_amp_env_d;
	switch(ctrl_toggle.sub_amp_env_sustain_func) {
	case CTRL_ENV_SUSTAIN:
		s_val = ctrl_value.sub_amp_env_s;
		break;
	case CTRL_ENV_AMOUNT:
		s_val = ctrl_value.sub_amp_env_amt;
		break;
	default:
		s_val = 0;
	}
	r_val = ctrl_value.sub_amp_env_r;

	switch (ctrl_toggle.sub_amp_env_sustain_func) {
	case CTRL_ENV_SUSTAIN:
		brightness = DEFAULT_BRIGHTNESS;
		break;
	case CTRL_ENV_AMOUNT:
		brightness = HALF_BRIGHTNESS;
		break;
	default:
		break;
	}


	// I2C Right 1
	res = i2c_mux_select(I2C_RIGHT, I2C_RIGHT_MUX, 1);
	if (res != HAL_OK) {
		Error_Handler();
	}

	/* Osc 1 ADSR LEDs
	 *
	 * A: RIGHT1:10
	 *
	 * [0,2=2=08][1,2=4=10][2,2=5=11]
	 * [0,1=1=07][1,1=3=09]
	 * [0,0=0=06]
	 */

	_adsr_led_set_grid_curve(a_val);

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 6, grid[0][0], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 7, grid[0][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 8, grid[0][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 9, grid[1][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 10, grid[1][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 11, grid[2][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	/* D: G1Cx
	 *
	 * [0,2][1,2][2,2]
	 * [0,1][1,1]
	 * [0,0]
	 *
	 * transposes to:
	 *
	 * [0,0=08=02]
	 * [0,1=07=01][1,1=10=04]
	 * [0,2=06=00][1,2=09=03][2,2=11=05]
	 *
	 */

	_adsr_led_set_grid_curve(d_val);

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 5, grid[2][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 4, grid[1][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 3, grid[1][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 2, grid[0][0], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 1, grid[0][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 0, grid[0][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}


	/*
	 *
	 * S: G1Cx
	 *
	 * [0,2=14=14][1,2=17=17]
	 * [0,1=13=13][1,1=16=16]
	 * [0,0=12=12][1,0=15=15]
	 *
	 */

	switch (ctrl_toggle.sub_amp_env_sustain_func) {
	case CTRL_ENV_SUSTAIN:
		_adsr_led_set_grid_bar(s_val);
		break;
	case CTRL_ENV_AMOUNT:
		_adsr_led_set_grid_stack(s_val);
		break;
	default:
		break;
	}

	// Sustain graph is always DEFAULT_BRIGHTNESS

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 14, grid[0][2], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 13, grid[0][1], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 12, grid[0][0], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 15, grid[1][0], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 16, grid[1][1], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 17, grid[1][2], DEFAULT_BRIGHTNESS);
	if (res != HAL_OK) {
		Error_Handler();
	}


	/*
	 * R: G1Cx
	 *
	 * [0,2][1,2][2,2]
	 * [0,1][1,1]
	 * [0,0]
	 *
	 * transposes to:
	 *
	 *
	 * [0,0=20=20]
	 * [0,1=19=19][1,1=22=22]
	 * [0,2=18=18][1,2=21=21][2,2=23=23]
	 *
	 */

	_adsr_led_set_grid_curve(r_val);

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 23, grid[2][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 22, grid[1][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 21, grid[1][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 20, grid[0][0], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 19, grid[0][1], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(I2C_RIGHT, 2, 18, grid[0][2], brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

}


void commit_led_adsr() {
	_commit_led_adsr_osc_filt_env();
	_commit_led_adsr_sub_filt_env();
	_commit_led_adsr_osc_amp_env();
	_commit_led_adsr_sub_amp_env();
}
