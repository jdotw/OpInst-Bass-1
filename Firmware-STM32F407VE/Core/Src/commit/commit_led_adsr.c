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

#define DEFAULT_BRIGHTNESS 0x15
#define HALF_BRIGHTNESS 0x04

/*
 * Osc Filter Envelope
 */

void _commit_led_adsr_osc_filt_env_a() {
	/* Osc Filter ADSR Attack LEDs
	 * LEFT3:001
	 */

  if (!commit_ctrl_changed.osc_filt_env1_a_changed && !commit_ctrl_changed.osc_filt_env2_a_changed) {
    return;
  }

	uint8_t brightness = 0x80;
	uint16_t a_val = 0;
	switch (ctrl_toggle.osc_filt_env_attack_func) {
	case ENC_SELECT_ENV_1:
		a_val = commit_ctrl_value.osc_filt_env1_a;
		break;
	case ENC_SELECT_ENV_2:
		a_val = commit_ctrl_value.osc_filt_env2_a;
		break;
	default:
		break;
	}
	switch (ctrl_toggle.osc_filt_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		brightness = DEFAULT_BRIGHTNESS;
		break;
	case ENC_ENV_AMOUNT:
		brightness = HALF_BRIGHTNESS;
		break;
	default:
		break;
	}

	/* [0,2=3][1,2=1][2,2=0]
	 * [0,1=4][1,1=2]
	 * [0,0=5]
	 */

	_adsr_led_set_grid_curve(a_val);
	uint8_t pwm_seq[6] = {
			grid[2][2], grid[1][2], grid[1][1], grid[0][2], grid[0][1], grid[0][0]
	};
	uint8_t scale_seq[6] = {
			brightness, brightness, brightness, brightness, brightness, brightness
	};

	bool res;
	res = is32_set_sequence_pwm(I2C_LEFT, 3, 1, 0, pwm_seq, 6);
	if (!res) Error_Handler();
	res = is32_set_sequence_scale(I2C_LEFT, 3, 1, 0, scale_seq, 6);
	if (!res) Error_Handler();
	res = is32_write_registers(I2C_LEFT, 3, 1);
	if (!res) Error_Handler();
}

void _commit_led_adsr_osc_filt_env_d() {
	/* Osc Filter ADSR Decay LEDs
	 * LEFT3:001
	 */

  if (!commit_ctrl_changed.osc_filt_env1_d_changed && !commit_ctrl_changed.osc_filt_env2_d_changed) {
    return;
  }

	uint16_t d_val = 0;
	switch (ctrl_toggle.osc_filt_env_attack_func) {
	case ENC_SELECT_ENV_1:
		d_val = commit_ctrl_value.osc_filt_env1_d;
		break;
	case ENC_SELECT_ENV_2:
		d_val = commit_ctrl_value.osc_filt_env2_d;
		break;
	default:
		break;
	}

	uint8_t brightness = 0x80;
	switch (ctrl_toggle.osc_filt_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		brightness = DEFAULT_BRIGHTNESS;
		break;
	case ENC_ENV_AMOUNT:
		brightness = HALF_BRIGHTNESS;
		break;
	default:
		break;
	}

	/* [0,2][1,2][2,2]
	 * [0,1][1,1]
	 * [0,0]
	 *
	 * transposes to:
	 *
	 * [0,0=27]
	 * [0,1=28][1,1=25]
	 * [0,2=29][1,2=26][2,2=24]
	 */

	_adsr_led_set_grid_curve(d_val);
	uint8_t pwm_seq[6] = { grid[2][2], grid[1][1], grid[1][2], grid[0][0], grid[0][1], grid[0][2] };
	uint8_t scale_seq[6] = { brightness, brightness, brightness, brightness, brightness, brightness };

	bool res;
	res = is32_set_sequence_pwm(I2C_LEFT, 3, 1, 24, pwm_seq, 6);
	if (!res) Error_Handler();
	res = is32_set_sequence_scale(I2C_LEFT, 3, 1, 24, scale_seq, 6);
	if (!res) Error_Handler();
	res = is32_write_registers(I2C_LEFT, 3, 1);
	if (!res) Error_Handler();
}


void _commit_led_adsr_osc_filt_env_s() {
	/* Osc Filter ADSR Sustain LEDs
	 * LEFT3:001
	 */

  if (!commit_ctrl_changed.osc_filt_env1_s_changed
      && !commit_ctrl_changed.osc_filt_env1_amt_changed
      && !commit_ctrl_changed.osc_filt_env2_s_changed
      && !commit_ctrl_changed.osc_filt_env2_amt_changed) {
    return;
  }

	uint16_t s_val = 0;
	switch (ctrl_toggle.osc_filt_env_attack_func) {
	case ENC_SELECT_ENV_1:
		switch(ctrl_toggle.osc_filt_env_sustain_func) {
		case ENC_ENV_SUSTAIN:
			s_val = commit_ctrl_value.osc_filt_env1_s;
			break;
		case ENC_ENV_AMOUNT:
			s_val = commit_ctrl_value.osc_filt_env1_amt;
			break;
		default:
			break;
		}
		break;
	case ENC_SELECT_ENV_2:
		switch(ctrl_toggle.osc_filt_env_sustain_func) {
		case ENC_ENV_SUSTAIN:
			s_val = commit_ctrl_value.osc_filt_env2_s;
			break;
		case ENC_ENV_AMOUNT:
			s_val = commit_ctrl_value.osc_filt_env2_amt;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	/* [0,2=14=23][1,2=17=18]
	 * [0,1=13=22][1,1=16=19]
	 * [0,0=12=21][1,0=15=20]
	 */

	switch (ctrl_toggle.osc_filt_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		_adsr_led_set_grid_bar(s_val);
		break;
	case ENC_ENV_AMOUNT:
		_adsr_led_set_grid_stack(s_val);
		break;
	default:
		break;
	}

	// Sustain graph is always DEFAULT_BRIGHTNESS
	uint8_t brightness = DEFAULT_BRIGHTNESS;
	uint8_t pwm_seq[6] = { grid[1][2], grid[1][1], grid[1][0], grid[0][0], grid[0][1], grid[0][2] };
	uint8_t scale_seq[6] = { brightness, brightness, brightness, brightness, brightness, brightness };

	bool res;
	res = is32_set_sequence_pwm(I2C_LEFT, 3, 1, 18, pwm_seq, 6);
	if (!res) Error_Handler();
	res = is32_set_sequence_scale(I2C_LEFT, 3, 1, 18, scale_seq, 6);
	if (!res) Error_Handler();
	res = is32_write_registers(I2C_LEFT, 3, 1);
	if (!res) Error_Handler();
}

void _commit_led_adsr_osc_filt_env_r() {
	/* Osc Filter ADSR Release LEDs
	 * LEFT3:001
	 */

  if (!commit_ctrl_changed.osc_filt_env1_r_changed && !commit_ctrl_changed.osc_filt_env2_r_changed) {
    return;
  }

	uint16_t r_val = 0;
	switch (ctrl_toggle.osc_filt_env_attack_func) {
	case ENC_SELECT_ENV_1:
		r_val = commit_ctrl_value.osc_filt_env1_r;
		break;
	case ENC_SELECT_ENV_2:
		r_val = commit_ctrl_value.osc_filt_env2_r;
		break;
	default:
		break;
	}

	uint8_t brightness = 0x80;
	switch (ctrl_toggle.osc_filt_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		brightness = DEFAULT_BRIGHTNESS;
		break;
	case ENC_ENV_AMOUNT:
		brightness = HALF_BRIGHTNESS;
		break;
	default:
		break;
	}

	/* [0,2][1,2][2,2]
	 * [0,1][1,1]
	 * [0,0]
	 *
	 * transposes to:
	 *
	 *
	 * [0,0=20=17]
	 * [0,1=19=16][1,1=22=13]
	 * [0,2=18=15][1,2=21=14][2,2=23=12]
	 */

	_adsr_led_set_grid_curve(r_val);

	uint8_t pwm_seq[6] = { grid[2][2], grid[1][1], grid[1][2], grid[0][2], grid[0][1], grid[0][0] };
	uint8_t scale_seq[6] = { brightness, brightness, brightness, brightness, brightness, brightness };

	bool res;
	res = is32_set_sequence_pwm(I2C_LEFT, 3, 1, 12, pwm_seq, 6);
	if (!res) Error_Handler();
	res = is32_set_sequence_scale(I2C_LEFT, 3, 1, 12, scale_seq, 6);
	if (!res) Error_Handler();
	res = is32_write_registers(I2C_LEFT, 3, 1);
	if (!res) Error_Handler();
}

/*
 * Sub Filter ADSR Envelope
 */

void _commit_led_adsr_sub_filt_env_a() {
	/* Sub Filter ADSR Attack LEDs
	 * LEFT1:11
	 */

  if (!commit_ctrl_changed.sub_filt_env1_a_changed && !commit_ctrl_changed.sub_filt_env2_a_changed) {
    return;
  }

	uint16_t a_val = 0;
	switch (ctrl_toggle.sub_filt_env_attack_func) {
	case ENC_SELECT_ENV_1:
		a_val = commit_ctrl_value.sub_filt_env1_a;
		break;
	case ENC_SELECT_ENV_2:
		a_val = commit_ctrl_value.sub_filt_env2_a;
		break;
	default:
		break;
	}

	uint8_t brightness = 0x80;
	switch (ctrl_toggle.sub_filt_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		brightness = DEFAULT_BRIGHTNESS;
		break;
	case ENC_ENV_AMOUNT:
		brightness = HALF_BRIGHTNESS;
		break;
	default:
		break;
	}

	/* [0,2=2][1,2=4][2,2=5]
	 * [0,1=1][1,1=3]
	 * [0,0=0]
	 */

	_adsr_led_set_grid_curve(a_val);

	uint8_t pwm_seq[6] = { grid[0][0], grid[0][1], grid[0][2], grid[1][1], grid[1][2], grid[2][2] };
	uint8_t scale_seq[6] = { brightness, brightness, brightness, brightness, brightness, brightness };

	bool res;
	res = is32_set_sequence_pwm(I2C_LEFT, 1, 3, 0, pwm_seq, 6);
	if (!res) Error_Handler();
	res = is32_set_sequence_scale(I2C_LEFT, 1, 3, 0, scale_seq, 6);
	if (!res) Error_Handler();
	res = is32_write_registers(I2C_LEFT, 1, 3);
	if (!res) Error_Handler();
}

void _commit_led_adsr_sub_filt_env_d() {
	/* Sub Filter Decay ADSR LEDs
	 * LEFT1:11
	 */

  if (!commit_ctrl_changed.sub_filt_env1_d_changed && !commit_ctrl_changed.sub_filt_env2_d_changed) {
    return;
  }

	uint16_t d_val = 0;
	switch (ctrl_toggle.sub_filt_env_attack_func) {
	case ENC_SELECT_ENV_1:
		d_val = commit_ctrl_value.sub_filt_env1_d;
		break;
	case ENC_SELECT_ENV_2:
		d_val = commit_ctrl_value.sub_filt_env2_d;
		break;
	default:
		break;
	}

	uint8_t brightness = 0x80;
	switch (ctrl_toggle.sub_filt_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		brightness = DEFAULT_BRIGHTNESS;
		break;
	case ENC_ENV_AMOUNT:
		brightness = HALF_BRIGHTNESS;
		break;
	default:
		break;
	}

	/* [0,2][1,2][2,2]
	 * [0,1][1,1]
	 * [0,0]
	 *
	 * transposes to:
	 *
	 * [0,0=08]
	 * [0,1=07][1,1=10]
	 * [0,2=06][1,2=09][2,2=11]
	 */

	_adsr_led_set_grid_curve(d_val);

	uint8_t pwm_seq[6] = { grid[0][2], grid[0][1], grid[0][0], grid[1][2], grid[1][1], grid[2][2] };
	uint8_t scale_seq[6] = { brightness, brightness, brightness, brightness, brightness, brightness };

	bool res;
	res = is32_set_sequence_pwm(I2C_LEFT, 1, 3, 6, pwm_seq, 6);
	if (!res) Error_Handler();
	res = is32_set_sequence_scale(I2C_LEFT, 1, 3, 6, scale_seq, 6);
	if (!res) Error_Handler();
	res = is32_write_registers(I2C_LEFT, 1, 3);
	if (!res) Error_Handler();
}

void _commit_led_adsr_sub_filt_env_s() {
	/* Sub Filter ADSR Sustain LEDs
	 * LEFT1:11
	 */

  if (!commit_ctrl_changed.sub_filt_env1_s_changed
      && !commit_ctrl_changed.sub_filt_env1_amt_changed
      && !commit_ctrl_changed.sub_filt_env2_s_changed
      && !commit_ctrl_changed.sub_filt_env2_amt_changed) {
    return;
  }

	uint16_t s_val = 0;
	switch (ctrl_toggle.sub_filt_env_attack_func) {
	case ENC_SELECT_ENV_1:
		switch(ctrl_toggle.sub_filt_env_sustain_func) {
		case ENC_ENV_SUSTAIN:
			s_val = commit_ctrl_value.sub_filt_env1_s;
			break;
		case ENC_ENV_AMOUNT:
			s_val = commit_ctrl_value.sub_filt_env1_amt;
			break;
		default:
			break;
		}
		break;
	case ENC_SELECT_ENV_2:
		switch(ctrl_toggle.sub_filt_env_sustain_func) {
		case ENC_ENV_SUSTAIN:
			s_val = commit_ctrl_value.sub_filt_env2_s;
			break;
		case ENC_ENV_AMOUNT:
			s_val = commit_ctrl_value.sub_filt_env2_amt;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	/* [0,2=14=14][1,2=17=17]
	 * [0,1=13=13][1,1=16=16]
	 * [0,0=12=12][1,0=15=15]
	 */

	switch (ctrl_toggle.sub_filt_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		_adsr_led_set_grid_bar(s_val);
		break;
	case ENC_ENV_AMOUNT:
		_adsr_led_set_grid_stack(s_val);
		break;
	default:
		break;
	}

	uint8_t pwm_seq[6] = { grid[0][0], grid[0][1], grid[0][2], grid[1][0], grid[1][1], grid[1][2] };
	uint8_t scale_seq[6] = { DEFAULT_BRIGHTNESS, DEFAULT_BRIGHTNESS, DEFAULT_BRIGHTNESS, DEFAULT_BRIGHTNESS, DEFAULT_BRIGHTNESS, DEFAULT_BRIGHTNESS };

	bool res;
	res = is32_set_sequence_pwm(I2C_LEFT, 1, 3, 12, pwm_seq, 6);
	if (!res) Error_Handler();
	res = is32_set_sequence_scale(I2C_LEFT, 1, 3, 12, scale_seq, 6);
	if (!res) Error_Handler();
	res = is32_write_registers(I2C_LEFT, 1, 3);
	if (!res) Error_Handler();
}

void _commit_led_adsr_sub_filt_env_r() {
	/* Sub Filter ADSR Release LEDs
	 * LEFT1:11
	 */

  if (!commit_ctrl_changed.sub_filt_env1_r_changed && !commit_ctrl_changed.sub_filt_env2_r_changed) {
    return;
  }

	uint16_t r_val = 0;
	switch (ctrl_toggle.sub_filt_env_attack_func) {
	case ENC_SELECT_ENV_1:
		r_val = commit_ctrl_value.sub_filt_env1_r;
		break;
	case ENC_SELECT_ENV_2:
		r_val = commit_ctrl_value.sub_filt_env2_r;
		break;
	default:
		break;
	}

	uint8_t brightness = 0x80;
	switch (ctrl_toggle.sub_filt_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		brightness = DEFAULT_BRIGHTNESS;
		break;
	case ENC_ENV_AMOUNT:
		brightness = HALF_BRIGHTNESS;
		break;
	default:
		break;
	}

	/* [0,2][1,2][2,2]
	 * [0,1][1,1]
	 * [0,0]
	 *
	 * transposes to:
	 *
	 *
	 * [0,0=20]
	 * [0,1=19][1,1=22]
	 * [0,2=18][1,2=21][2,2=23]
	 */

	_adsr_led_set_grid_curve(r_val);

	uint8_t pwm_seq[6] = { grid[0][2], grid[0][1], grid[0][0], grid[1][2], grid[1][1], grid[2][2] };
	uint8_t scale_seq[6] = { brightness, brightness, brightness, brightness, brightness, brightness };

	bool res;
	res = is32_set_sequence_pwm(I2C_LEFT, 1, 3, 18, pwm_seq, 6);
	if (!res) Error_Handler();
	res = is32_set_sequence_scale(I2C_LEFT, 1, 3, 18, scale_seq, 6);
	if (!res) Error_Handler();
	res = is32_write_registers(I2C_LEFT, 1, 3);
	if (!res) Error_Handler();
}


/*
 * Osc Amp ADSR Envelope
 */

void _commit_led_adsr_osc_amp_env_a() {
	/* Osc Amp ADSR Attack LEDs
	 * RIGHT2:00
	 */

  if (!commit_ctrl_changed.osc_amp_env_a_changed) {
    return;
  }

	uint16_t a_val = commit_ctrl_value.osc_amp_env_a;
	uint8_t brightness = 0x80;
	switch (ctrl_toggle.osc_amp_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		brightness = DEFAULT_BRIGHTNESS;
		break;
	case ENC_ENV_AMOUNT:
		brightness = HALF_BRIGHTNESS;
		break;
	default:
		break;
	}

	/* [0,2=2=20][1,2=4=22][2,2=5=23]
	 * [0,1=1=19][1,1=3=21]
	 * [0,0=0=18]
	 */

	_adsr_led_set_grid_curve(a_val);

	uint8_t pwm_seq[6] = { grid[0][0], grid[0][1], grid[0][2], grid[1][1], grid[1][2], grid[2][2] };
	uint8_t scale_seq[6] = { brightness, brightness, brightness, brightness, brightness, brightness };

	bool res;
	res = is32_set_sequence_pwm(I2C_RIGHT, 2, 0, 18, pwm_seq, 6);
	if (!res) Error_Handler();
	res = is32_set_sequence_scale(I2C_RIGHT, 2, 0, 18, scale_seq, 6);
	if (!res) Error_Handler();
	res = is32_write_registers(I2C_RIGHT, 2, 0);
	if (!res) Error_Handler();
}

void _commit_led_adsr_osc_amp_env_d() {
	/* Osc Amp ADSR Decay LEDs
	 * RIGHT2:00
	 */

  if (!commit_ctrl_changed.osc_amp_env_d_changed) {
    return;
  }

	uint16_t d_val = commit_ctrl_value.osc_amp_env_d;
	uint8_t brightness = 0x80;
	switch (ctrl_toggle.osc_amp_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		brightness = DEFAULT_BRIGHTNESS;
		break;
	case ENC_ENV_AMOUNT:
		brightness = HALF_BRIGHTNESS;
		break;
	default:
		break;
	}

	/* [0,2][1,2][2,2]
	 * [0,1][1,1]
	 * [0,0]
	 *
	 * transposes to:
	 *
	 * [0,0=08=12]
	 * [0,1=07=14][1,1=10=13]
	 * [0,2=06=15][1,2=09=16][2,2=11=17]
	 */

	_adsr_led_set_grid_curve(d_val);

	uint8_t pwm_seq[6] = { grid[0][0], grid[1][1], grid[0][1], grid[0][2], grid[1][2], grid[2][2] };
	uint8_t scale_seq[6] = { brightness, brightness, brightness, brightness, brightness, brightness };

	bool res;
	res = is32_set_sequence_pwm(I2C_RIGHT, 2, 0, 12, pwm_seq, 6);
	if (!res) Error_Handler();
	res = is32_set_sequence_scale(I2C_RIGHT, 2, 0, 12, scale_seq, 6);
	if (!res) Error_Handler();
	res = is32_write_registers(I2C_RIGHT, 2, 0);
	if (!res) Error_Handler();
}

void _commit_led_adsr_osc_amp_env_s() {
	/* Osc Amp ADSR Sustain LEDs
	 *
	 * RIGHT2:00
	 */

  if (!commit_ctrl_changed.osc_amp_env_s_changed && !commit_ctrl_changed.osc_amp_env_amt_changed) {
    return;
  }

	uint8_t brightness = DEFAULT_BRIGHTNESS;
	uint16_t s_val;
	switch(ctrl_toggle.osc_amp_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		s_val = commit_ctrl_value.osc_amp_env_s;
		break;
	case ENC_ENV_AMOUNT:
		s_val = commit_ctrl_value.osc_amp_env_amt;
		break;
	default:
		s_val = 0;
	}

	/* [0,2=14=11][1,2=17=08]
	 * [0,1=13=10][1,1=16=07]
	 * [0,0=12=09][1,0=15=06]
	 */

	switch (ctrl_toggle.osc_amp_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		_adsr_led_set_grid_bar(s_val);
		break;
	case ENC_ENV_AMOUNT:
		_adsr_led_set_grid_stack(s_val);
		break;
	default:
		break;
	}

	uint8_t pwm_seq[6] = { grid[1][0], grid[1][1], grid[1][2], grid[0][0], grid[0][1], grid[0][2] };
	uint8_t scale_seq[6] = { brightness, brightness, brightness, brightness, brightness, brightness };

	bool res;
	res = is32_set_sequence_pwm(I2C_RIGHT, 2, 0, 6, pwm_seq, 6);
	if (!res) Error_Handler();
	res = is32_set_sequence_scale(I2C_RIGHT, 2, 0, 6, scale_seq, 6);
	if (!res) Error_Handler();
	res = is32_write_registers(I2C_RIGHT, 2, 0);
	if (!res) Error_Handler();

}

void _commit_led_adsr_osc_amp_env_r() {
	/* Osc Amp ADSR Release LEDs
	 * RIGHT2:00
	 */

  if (!commit_ctrl_changed.osc_amp_env_r_changed) {
    return;
  }

	uint16_t r_val = commit_ctrl_value.osc_amp_env_r;
	uint8_t brightness = 0x80;
	switch (ctrl_toggle.osc_amp_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		brightness = DEFAULT_BRIGHTNESS;
		break;
	case ENC_ENV_AMOUNT:
		brightness = HALF_BRIGHTNESS;
		break;
	default:
		break;
	}

	/* [0,2][1,2][2,2]
	 * [0,1][1,1]
	 * [0,0]
	 *
	 * transposes to:
	 *
	 * [0,0=20=2]
	 * [0,1=19=1][1,1=22=4]
	 * [0,2=18=0][1,2=21=3][2,2=23=5]
	 */

	_adsr_led_set_grid_curve(r_val);

	uint8_t pwm_seq[6] = { grid[0][2], grid[0][1], grid[0][0], grid[1][2], grid[1][1], grid[2][2] };
	uint8_t scale_seq[6] = { brightness, brightness, brightness, brightness, brightness, brightness };

	bool res;
	res = is32_set_sequence_pwm(I2C_RIGHT, 2, 0, 0, pwm_seq, 6);
	if (!res) Error_Handler();
	res = is32_set_sequence_scale(I2C_RIGHT, 2, 0, 0, scale_seq, 6);
	if (!res) Error_Handler();
	res = is32_write_registers(I2C_RIGHT, 2, 0);
	if (!res) Error_Handler();
}

/*
 * Sub Amp ADSR Envelope
 */

void _commit_led_adsr_sub_amp_env_a() {
	/* Sub Amp ADSR Attack LEDs
	 * RIGHT1:10
	 */

  if (!commit_ctrl_changed.sub_amp_env_a_changed) {
    return;
  }

	uint16_t a_val = commit_ctrl_value.sub_amp_env_a;
	uint8_t brightness = 0x80;
	switch (ctrl_toggle.sub_amp_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		brightness = DEFAULT_BRIGHTNESS;
		break;
	case ENC_ENV_AMOUNT:
		brightness = HALF_BRIGHTNESS;
		break;
	default:
		break;
	}

	/* [0,2=2=08][1,2=4=10][2,2=5=11]
	 * [0,1=1=07][1,1=3=09]
	 * [0,0=0=06]
	 */

	_adsr_led_set_grid_curve(a_val);

	uint8_t pwm_seq[6] = { grid[0][0], grid[0][1], grid[0][2], grid[1][1], grid[1][2], grid[2][2] };
	uint8_t scale_seq[6] = { brightness, brightness, brightness, brightness, brightness, brightness };

	bool res;
	res = is32_set_sequence_pwm(I2C_RIGHT, 1, 2, 6, pwm_seq, 6);
	if (!res) Error_Handler();
	res = is32_set_sequence_scale(I2C_RIGHT, 1, 2, 6, scale_seq, 6);
	if (!res) Error_Handler();
	res = is32_write_registers(I2C_RIGHT, 1, 2);
	if (!res) Error_Handler();

}

void _commit_led_adsr_sub_amp_env_d() {
	/* Sub Amp ADSR Decay LEDs
	 * RIGHT1:10
	 */

  if (!commit_ctrl_changed.sub_amp_env_d_changed) {
    return;
  }

	uint16_t d_val = commit_ctrl_value.sub_amp_env_d;
	uint8_t brightness = 0x80;
	switch (ctrl_toggle.sub_amp_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		brightness = DEFAULT_BRIGHTNESS;
		break;
	case ENC_ENV_AMOUNT:
		brightness = HALF_BRIGHTNESS;
		break;
	default:
		break;
	}

	/* [0,2][1,2][2,2]
	 * [0,1][1,1]
	 * [0,0]
	 *
	 * transposes to:
	 *
	 * [0,0=08=02]
	 * [0,1=07=01][1,1=10=04]
	 * [0,2=06=00][1,2=09=03][2,2=11=05]
	 */

	_adsr_led_set_grid_curve(d_val);

	uint8_t pwm_seq[6] = { grid[0][2], grid[0][1], grid[0][0], grid[1][2], grid[1][1], grid[2][2] };
	uint8_t scale_seq[6] = { brightness, brightness, brightness, brightness, brightness, brightness };

	bool res;
	res = is32_set_sequence_pwm(I2C_RIGHT, 1, 2, 0, pwm_seq, 6);
	if (!res) Error_Handler();
	res = is32_set_sequence_scale(I2C_RIGHT, 1, 2, 0, scale_seq, 6);
	if (!res) Error_Handler();
	res = is32_write_registers(I2C_RIGHT, 1, 2);
	if (!res) Error_Handler();
}

void _commit_led_adsr_sub_amp_env_s() {
	/* Sub Amp ADSR Sustain LEDs
	 * RIGHT1:10
	 */

  if (!commit_ctrl_changed.sub_amp_env_s_changed && !commit_ctrl_changed.sub_amp_env_amt_changed) {
    return;
  }

	uint8_t brightness = DEFAULT_BRIGHTNESS;
	uint16_t s_val = 0;
	switch(ctrl_toggle.sub_amp_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		s_val = commit_ctrl_value.sub_amp_env_s;
		break;
	case ENC_ENV_AMOUNT:
		s_val = commit_ctrl_value.sub_amp_env_amt;
		break;
	default:
		break;
	}

	/* [0,2=14=14][1,2=17=17]
	 * [0,1=13=13][1,1=16=16]
	 * [0,0=12=12][1,0=15=15]
	 */

	switch (ctrl_toggle.sub_amp_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		_adsr_led_set_grid_bar(s_val);
		break;
	case ENC_ENV_AMOUNT:
		_adsr_led_set_grid_stack(s_val);
		break;
	default:
		break;
	}

	uint8_t pwm_seq[6] = { grid[0][0], grid[0][1], grid[0][2], grid[1][0], grid[1][1], grid[1][2] };
	uint8_t scale_seq[6] = { brightness, brightness, brightness, brightness, brightness, brightness };

	bool res;
	res = is32_set_sequence_pwm(I2C_RIGHT, 1, 2, 12, pwm_seq, 6);
	if (!res) Error_Handler();
	res = is32_set_sequence_scale(I2C_RIGHT, 1, 2, 12, scale_seq, 6);
	if (!res) Error_Handler();
	res = is32_write_registers(I2C_RIGHT, 1, 2);
	if (!res) Error_Handler();
}

void _commit_led_adsr_sub_amp_env_r() {
	/* Sub Amp ADSR Release LEDs
	 * A: RIGHT1:10
	 */

  if (!commit_ctrl_changed.sub_amp_env_r_changed) {
    return;
  }

	uint16_t r_val = commit_ctrl_value.sub_amp_env_r;
	uint8_t brightness = 0x80;
	switch (ctrl_toggle.sub_amp_env_sustain_func) {
	case ENC_ENV_SUSTAIN:
		brightness = DEFAULT_BRIGHTNESS;
		break;
	case ENC_ENV_AMOUNT:
		brightness = HALF_BRIGHTNESS;
		break;
	default:
		break;
	}


	/* [0,2][1,2][2,2]
	 * [0,1][1,1]
	 * [0,0]
	 *
	 * transposes to:
	 *
	 * [0,0=20=20]
	 * [0,1=19=19][1,1=22=22]
	 * [0,2=18=18][1,2=21=21][2,2=23=23]
	 */

	_adsr_led_set_grid_curve(r_val);

	uint8_t pwm_seq[6] = { grid[0][2], grid[0][1], grid[0][0], grid[1][2], grid[1][1], grid[2][2] };
	uint8_t scale_seq[6] = { brightness, brightness, brightness, brightness, brightness, brightness };

	bool res;
	res = is32_set_sequence_pwm(I2C_RIGHT, 1, 2, 18, pwm_seq, 6);
	if (!res) Error_Handler();
	res = is32_set_sequence_scale(I2C_RIGHT, 1, 2, 18, scale_seq, 6);
	if (!res) Error_Handler();
	res = is32_write_registers(I2C_RIGHT, 1, 2);
	if (!res) Error_Handler();
}

/*
 * ADSR LED Graphs
 */

void commit_led_adsr(commit_cycle_t cycle) {
	switch(cycle) {
	case COMMIT_LED_ADSR_OSC_FILT_A:
		_commit_led_adsr_osc_filt_env_a();
		break;
	case COMMIT_LED_ADSR_OSC_FILT_D:
		_commit_led_adsr_osc_filt_env_d();
		break;
	case COMMIT_LED_ADSR_OSC_FILT_S:
		_commit_led_adsr_osc_filt_env_s();
		break;
	case COMMIT_LED_ADSR_OSC_FILT_R:
		_commit_led_adsr_osc_filt_env_r();
		break;
	case COMMIT_LED_ADSR_SUB_FILT_A:
		_commit_led_adsr_sub_filt_env_a();
		break;
	case COMMIT_LED_ADSR_SUB_FILT_D:
		_commit_led_adsr_sub_filt_env_d();
		break;
	case COMMIT_LED_ADSR_SUB_FILT_S:
		_commit_led_adsr_sub_filt_env_s();
		break;
	case COMMIT_LED_ADSR_SUB_FILT_R:
		_commit_led_adsr_sub_filt_env_r();
		break;
	case COMMIT_LED_ADSR_OSC_AMP_A:
		_commit_led_adsr_osc_amp_env_a();
		break;
	case COMMIT_LED_ADSR_OSC_AMP_D:
		_commit_led_adsr_osc_amp_env_d();
		break;
	case COMMIT_LED_ADSR_OSC_AMP_S:
		_commit_led_adsr_osc_amp_env_s();
		break;
	case COMMIT_LED_ADSR_OSC_AMP_R:
		_commit_led_adsr_osc_amp_env_r();
		break;
	case COMMIT_LED_ADSR_SUB_AMP_A:
		_commit_led_adsr_sub_amp_env_a();
		break;
	case COMMIT_LED_ADSR_SUB_AMP_D:
		_commit_led_adsr_sub_amp_env_d();
		break;
	case COMMIT_LED_ADSR_SUB_AMP_S:
		_commit_led_adsr_sub_amp_env_s();
		break;
	case COMMIT_LED_ADSR_SUB_AMP_R:
		_commit_led_adsr_sub_amp_env_r();
		break;
	default:
		break;
	}
}
