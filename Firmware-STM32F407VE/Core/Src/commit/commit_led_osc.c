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
	if (a > b) return a >> 4;
	else return b >> 4;
}

uint8_t _12_to_8(uint16_t a) {
	return a >> 4;
}

void _commit_led_osc1() {
	bool res;

	/* LEFT0:00 */

	/* Osc1 Saw */
	res = is32_set_rgb(I2C_LEFT, 0, 0, 0, 0xFF, 0x00, 0x00, _12_to_8(ctrl_value.osc1_saw_lvl));
	if (!res) Error_Handler();
	res = is32_set_rgb(I2C_LEFT, 0, 0, 1, 0xFF, 0x00, 0x00, _12_to_8(ctrl_value.osc1_saw_lvl));
	if (!res) Error_Handler();

	/* Osc1 Sq */
	res = is32_set_rgb(I2C_LEFT, 0, 0, 2, 0x00, 0x00, 0xFF, _12_to_8(ctrl_value.osc1_squ_lvl));
	if (!res) Error_Handler();
	res = is32_set_rgb(I2C_LEFT, 0, 0, 3, 0x00, 0x00, 0xFF, _12_to_8(ctrl_value.osc1_squ_lvl));
	if (!res) Error_Handler();

	/* Osc1 Mix */
	res = is32_set_rgb(I2C_LEFT, 0, 0, 4, _12_to_8(ctrl_value.osc1_saw_lvl), 0x00, _12_to_8(ctrl_value.osc1_squ_lvl), _max(ctrl_value.osc1_saw_lvl, ctrl_value.osc1_squ_lvl));
	if (!res) Error_Handler();

	/* LEFT0:11 */

	/* Osc2 Saw */
	res = is32_set_rgb(I2C_LEFT, 0, 3, 0, 0xFF, 0x00, 0x00, _12_to_8(ctrl_value.osc2_saw_lvl));
	if (!res) Error_Handler();
	res = is32_set_rgb(I2C_LEFT, 0, 3, 1, 0xFF, 0x00, 0x00, _12_to_8(ctrl_value.osc2_saw_lvl));
	if (!res) Error_Handler();

	/* Osc2 Sq */
	res = is32_set_rgb(I2C_LEFT, 0, 3, 2, 0x00, 0x00, 0xFF, _12_to_8(ctrl_value.osc2_squ_lvl));
	if (!res) Error_Handler();
	res = is32_set_rgb(I2C_LEFT, 0, 3, 3, 0x00, 0x00, 0xFF, _12_to_8(ctrl_value.osc2_squ_lvl));
	if (!res) Error_Handler();

	/* Osc2 Noise */
	res = is32_set_rgb(I2C_LEFT, 0, 3, 2, 0x00, 0x00, 0xFF, _12_to_8(ctrl_value.osc2_noise_lvl));
	if (!res) Error_Handler();
	res = is32_set_rgb(I2C_LEFT, 0, 3, 3, 0x00, 0x00, 0xFF, _12_to_8(ctrl_value.osc2_noise_lvl));
	if (!res) Error_Handler();

	/* Osc2 Mix */
	res = is32_set_rgb(I2C_LEFT, 0, 3, 4, _12_to_8(ctrl_value.osc1_saw_lvl), 0x00, _12_to_8(ctrl_value.osc1_squ_lvl), _max(ctrl_value.osc1_saw_lvl, ctrl_value.osc1_squ_lvl));
	if (!res) Error_Handler();

	/* LEFT1:00 */

	/* Sub */
	res = is32_set_rgb(I2C_LEFT, 1, 0, 0, 0xFF, 0x00, 0x00, _12_to_8(ctrl_value.sub_lvl));
	if (!res) Error_Handler();
	res = is32_set_rgb(I2C_LEFT, 1, 0, 1, 0xFF, 0x00, 0x00, _12_to_8(ctrl_value.sub_lvl));
	if (!res) Error_Handler();

	/* Noise */
	res = is32_set_rgb(I2C_LEFT, 1, 0, 2, 0x00, 0x00, 0xFF, _12_to_8(ctrl_value.sub_noise_lvl));
	if (!res) Error_Handler();
	res = is32_set_rgb(I2C_LEFT, 1, 0, 3, 0x00, 0x00, 0xFF, _12_to_8(ctrl_value.sub_noise_lvl));
	if (!res) Error_Handler();

	/* Sub Noise Mix */
	res = is32_set_rgb(I2C_LEFT, 1, 0, 4, _12_to_8(ctrl_value.sub_lvl), 0x00, _12_to_8(ctrl_value.sub_noise_lvl), _max(ctrl_value.sub_lvl, ctrl_value.sub_noise_lvl));
	if (!res) Error_Handler();

}

void commit_led_osc() {
	_commit_led_osc1();
}
