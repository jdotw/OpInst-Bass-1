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

void commit_led_adsr() {
	HAL_StatusTypeDef res;

	/* Osc 1 ADSR LEDs
	 *
	 * A: LEFT3:001
	 *
	 * [ 6][ 4] [5]
	 * [ 5][ 3]
	 * [ 4]
	 */

	// I2C Left 3
	res = i2c_mux_select(I2C_LEFT, I2C_LEFT_MUX, 3);
	if (res != HAL_OK) {
		Error_Handler();
	}

	uint8_t brightness = 0x80;
	uint8_t pwm = 0x80;

	pwm = ctrl_value.osc_filt_env1_a & 0xFF0 ? 0xF : (ctrl_value.osc_filt_env1_a & 0x00F);
	res = is32_turn_on_led_single(0, 1, 5, pwm, brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	pwm = ctrl_value.osc_filt_env1_a & 0xF00 ? 0xF : ((ctrl_value.osc_filt_env1_a & 0x0F0) >> 4);
	res = is32_turn_on_led_single(0, 1, 4, pwm, brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	pwm = (ctrl_value.osc_filt_env1_a & 0xF00) >> 8;
	res = is32_turn_on_led_single(0, 1, 3, pwm, brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}


	/*
	 *
	 * D: G1Cx
	 *
	 * [ 8]
	 * [ 7][10]
	 * [ 6][ 9][11]
	 *
	 * S: G1Cx
	 *
	 * [14][17]
	 * [13][16]
	 * [12][15]
	 *
	 * R: G1Cx
	 *
	 * [20]
	 * [19][22]
	 * [18][21][23]
	 *
	 */


}
