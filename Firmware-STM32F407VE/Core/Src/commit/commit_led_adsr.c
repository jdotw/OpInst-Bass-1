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

void commit_led_adsr() {
	HAL_StatusTypeDef res;
	uint8_t brightness = 0x80;

	// I2C Left 3
	res = i2c_mux_select(I2C_LEFT, I2C_LEFT_MUX, 3);
	if (res != HAL_OK) {
		Error_Handler();
	}

	/* Osc 1 ADSR LEDs
	 *
	 * A: LEFT3:001
	 *
	 * [ 4][ 2] [1]
	 * [ 5][ 3]
	 * [ 6]
	 */

	uint16_t val = ctrl_value.osc_filt_env1_a;

	uint8_t led4_pwm = 0;
	uint8_t led2_pwm = 0;
	uint8_t led1_pwm = 0;

	uint8_t led5_pwm = 0;
	uint8_t led3_pwm = 0;

	uint8_t led6_pwm = 0xFF;	// Always on

	if (val <= 1365) {
		// 1st
		// led4 -> led2
		uint8_t delta = (val * 3) >> 4;
		led4_pwm = 0xFF - delta;
		led5_pwm = 0xFF;
		led2_pwm = delta;
	}
	else if (val <= (1365 * 2)) {
		// 2nd
		// led5 -> led3
		uint8_t delta = ((val - 1365) * 3) >> 4;
		led5_pwm = 0xFF - delta;
		led2_pwm = 0xFF;
		led3_pwm = delta;
	} else {
		// 3rd
		// led2 -> led1
		uint8_t delta = ((val - (1365 * 2)) * 3) >> 4;
		led2_pwm = 0xFF - delta;
		led3_pwm = 0xFF;
		led1_pwm = delta;
	}

	res = is32_turn_on_led_single(0, 1, 5, led6_pwm, brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 4, led5_pwm, brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 3, led4_pwm, brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 2, led3_pwm, brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 1, led2_pwm, brightness);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = is32_turn_on_led_single(0, 1, 0, led1_pwm, brightness);
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
