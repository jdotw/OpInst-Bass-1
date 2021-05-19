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

#define DEFAULT_SCALE 0x15

void _commit_led_osc1() {
	bool res;

	/* Osc1 Saw
	 * LEFT0:00
	 * 0, 1
	 */

	uint8_t pwm_seq[24];
	pwm_seq[0] = _12_to_8(ctrl_value.osc1_saw_lvl);
	pwm_seq[1] = 0;
	pwm_seq[2] = 0;
	pwm_seq[3] = _12_to_8(ctrl_value.osc1_saw_lvl);
	pwm_seq[4] = 0;
	pwm_seq[5] = 0;
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0, 0, pwm_seq, 6);
	if (!res) Error_Handler();

	uint8_t scale_seq[24];
	scale_seq[0] = DEFAULT_SCALE;
	scale_seq[1] = DEFAULT_SCALE;
	scale_seq[2] = DEFAULT_SCALE;
	scale_seq[3] = DEFAULT_SCALE;
	scale_seq[4] = DEFAULT_SCALE;
	scale_seq[5] = DEFAULT_SCALE;
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0, 0, scale_seq, 6);
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0);
	if (!res) Error_Handler();

	/* Osc1 Squ
	 * LEFT0:00
	 * 2, 3
	 */

	pwm_seq[0] = _12_to_8(ctrl_value.osc1_squ_lvl);
	pwm_seq[1] = 0;
	pwm_seq[2] = 0;
	pwm_seq[3] = _12_to_8(ctrl_value.osc1_squ_lvl);;
	pwm_seq[4] = 0;
	pwm_seq[5] = 0;
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0, 6, pwm_seq, 6);
	if (!res) Error_Handler();

	scale_seq[0] = DEFAULT_SCALE;
	scale_seq[1] = DEFAULT_SCALE;
	scale_seq[2] = DEFAULT_SCALE;
	scale_seq[3] = DEFAULT_SCALE;
	scale_seq[4] = DEFAULT_SCALE;
	scale_seq[5] = DEFAULT_SCALE;
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0, 6, scale_seq, 6);
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0);
	if (!res) Error_Handler();

	/* Osc1 Mix
	 * LEFT0:00
	 * 4, 5, 6, 7, 8, 9, 10
	 */

	pwm_seq[0] = _12_to_8(ctrl_value.osc1_saw_lvl);
	pwm_seq[1] = 0;
	pwm_seq[2] = _12_to_8(ctrl_value.osc1_squ_lvl);
	pwm_seq[3] = _12_to_8(ctrl_value.osc1_saw_lvl);
	pwm_seq[4] = 0;
	pwm_seq[5] = _12_to_8(ctrl_value.osc1_squ_lvl);
	pwm_seq[6] = _12_to_8(ctrl_value.osc1_saw_lvl);
	pwm_seq[7] = 0;
	pwm_seq[8] = _12_to_8(ctrl_value.osc1_squ_lvl);
	pwm_seq[9] = _12_to_8(ctrl_value.osc1_saw_lvl);
	pwm_seq[10] = 0;
	pwm_seq[11] = _12_to_8(ctrl_value.osc1_squ_lvl);
	pwm_seq[12] = _12_to_8(ctrl_value.osc1_saw_lvl);
	pwm_seq[13] = 0;
	pwm_seq[14] = _12_to_8(ctrl_value.osc1_squ_lvl);
	pwm_seq[15] = _12_to_8(ctrl_value.osc1_saw_lvl);
	pwm_seq[16] = 0;
	pwm_seq[17] = _12_to_8(ctrl_value.osc1_squ_lvl);
	pwm_seq[18] = _12_to_8(ctrl_value.osc1_saw_lvl);
	pwm_seq[19] = 0;
	pwm_seq[20] = _12_to_8(ctrl_value.osc1_squ_lvl);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0, 12, pwm_seq, 21);
	if (!res) Error_Handler();

	scale_seq[0] = DEFAULT_SCALE;
	scale_seq[1] = DEFAULT_SCALE;
	scale_seq[2] = DEFAULT_SCALE;
	scale_seq[3] = DEFAULT_SCALE;
	scale_seq[4] = DEFAULT_SCALE;
	scale_seq[5] = DEFAULT_SCALE;
	scale_seq[6] = DEFAULT_SCALE;
	scale_seq[7] = DEFAULT_SCALE;
	scale_seq[8] = DEFAULT_SCALE;
	scale_seq[9] = DEFAULT_SCALE;
	scale_seq[10] = DEFAULT_SCALE;
	scale_seq[11] = DEFAULT_SCALE;
	scale_seq[12] = DEFAULT_SCALE;
	scale_seq[13] = DEFAULT_SCALE;
	scale_seq[14] = DEFAULT_SCALE;
	scale_seq[15] = DEFAULT_SCALE;
	scale_seq[16] = DEFAULT_SCALE;
	scale_seq[17] = DEFAULT_SCALE;
	scale_seq[18] = DEFAULT_SCALE;
	scale_seq[19] = DEFAULT_SCALE;
	scale_seq[20] = DEFAULT_SCALE;
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0, 12, scale_seq, 21);
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0);
	if (!res) Error_Handler();

	/* Osc1toOsc2 Mix Level
	 * LEFT0:10
	 * 0, 1
	 */

	pwm_seq[0] = _12_to_8(((double)ctrl_value.osc1_saw_lvl * ((double)ctrl_value.osc1_to_osc2 / 4095.0)));
	pwm_seq[1] = 0;
	pwm_seq[2] = _12_to_8(((double)ctrl_value.osc1_squ_lvl * ((double)ctrl_value.osc1_to_osc2 / 4095.0)));
	pwm_seq[3] = _12_to_8(((double)ctrl_value.osc1_saw_lvl * ((double)ctrl_value.osc1_to_osc2 / 4095.0)));
	pwm_seq[4] = 0;
	pwm_seq[5] = _12_to_8(((double)ctrl_value.osc1_squ_lvl * ((double)ctrl_value.osc1_to_osc2 / 4095.0)));
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b10, (0*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

	scale_seq[0] = DEFAULT_SCALE;
	scale_seq[1] = DEFAULT_SCALE;
	scale_seq[2] = DEFAULT_SCALE;
	scale_seq[3] = DEFAULT_SCALE;
	scale_seq[4] = DEFAULT_SCALE;
	scale_seq[5] = DEFAULT_SCALE;
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0b10, (0*3), scale_seq, (2*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0b10);
	if (!res) Error_Handler();


	/* Osc2 Saw
	 * LEFT0:11
	 * 1, 2
	 */

	pwm_seq[0] = _12_to_8(ctrl_value.osc2_saw_lvl);
	pwm_seq[1] = 0;
	pwm_seq[2] = 0;
	pwm_seq[3] = _12_to_8(ctrl_value.osc2_saw_lvl);
	pwm_seq[4] = 0;
	pwm_seq[5] = 0;
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b11, (1*3), pwm_seq, 6);
	if (!res) Error_Handler();

	scale_seq[0] = DEFAULT_SCALE;
	scale_seq[1] = DEFAULT_SCALE;
	scale_seq[2] = DEFAULT_SCALE;
	scale_seq[3] = DEFAULT_SCALE;
	scale_seq[4] = DEFAULT_SCALE;
	scale_seq[5] = DEFAULT_SCALE;
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0b11, (1*3), scale_seq, 6);
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0b11);
	if (!res) Error_Handler();

	/* Osc2 Squ
	 * LEFT0:11
	 * 0
	 */

	pwm_seq[0] = 0;
	pwm_seq[1] = 0;
	pwm_seq[2] = _12_to_8(ctrl_value.osc2_squ_lvl);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b11, (0*3), pwm_seq, 3);
	if (!res) Error_Handler();

	scale_seq[0] = DEFAULT_SCALE;
	scale_seq[1] = DEFAULT_SCALE;
	scale_seq[2] = DEFAULT_SCALE;
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0b11, (0*3), scale_seq, 3);
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0b11);
	if (!res) Error_Handler();

	/* Osc2 Noise
	 * LEFT0:11
	 * 3, 4
	 */

	pwm_seq[0] = 0;
	pwm_seq[1] = _12_to_8(ctrl_value.osc2_noise_lvl);
	pwm_seq[2] = 0;
	pwm_seq[3] = 0;
	pwm_seq[4] = _12_to_8(ctrl_value.osc2_noise_lvl);
	pwm_seq[5] = 0;
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b11, (3*3), pwm_seq, 6);
	if (!res) Error_Handler();

	scale_seq[0] = DEFAULT_SCALE;
	scale_seq[1] = DEFAULT_SCALE;
	scale_seq[2] = DEFAULT_SCALE;
	scale_seq[3] = DEFAULT_SCALE;
	scale_seq[4] = DEFAULT_SCALE;
	scale_seq[5] = DEFAULT_SCALE;
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0b11, (3*3), scale_seq, 6);
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0b11);
	if (!res) Error_Handler();

	/* Osc2-Only Mix
	 * LEFT0:00
	 * 5, 6, 7, 8
	 */

	pwm_seq[0] = _12_to_8(ctrl_value.osc2_saw_lvl);
	pwm_seq[1] = _12_to_8(ctrl_value.osc2_noise_lvl);
	pwm_seq[2] = _12_to_8(ctrl_value.osc2_squ_lvl);
	pwm_seq[3] = _12_to_8(ctrl_value.osc2_saw_lvl);
	pwm_seq[4] = _12_to_8(ctrl_value.osc2_noise_lvl);
	pwm_seq[5] = _12_to_8(ctrl_value.osc2_squ_lvl);
	pwm_seq[6] = _12_to_8(ctrl_value.osc2_saw_lvl);
	pwm_seq[7] = _12_to_8(ctrl_value.osc2_noise_lvl);
	pwm_seq[8] = _12_to_8(ctrl_value.osc2_squ_lvl);
	pwm_seq[9] = _12_to_8(ctrl_value.osc2_saw_lvl);
	pwm_seq[10] = _12_to_8(ctrl_value.osc2_noise_lvl);
	pwm_seq[11] = _12_to_8(ctrl_value.osc2_squ_lvl);
	res = is32_set_sequence_pwm(I2C_LEFT, 0, 0b11, (5*3), pwm_seq, (4*3));
	if (!res) Error_Handler();

	scale_seq[0] = DEFAULT_SCALE;
	scale_seq[1] = DEFAULT_SCALE;
	scale_seq[2] = DEFAULT_SCALE;
	scale_seq[3] = DEFAULT_SCALE;
	scale_seq[4] = DEFAULT_SCALE;
	scale_seq[5] = DEFAULT_SCALE;
	scale_seq[6] = DEFAULT_SCALE;
	scale_seq[7] = DEFAULT_SCALE;
	scale_seq[8] = DEFAULT_SCALE;
	scale_seq[9] = DEFAULT_SCALE;
	scale_seq[10] = DEFAULT_SCALE;
	scale_seq[11] = DEFAULT_SCALE;
	res = is32_set_sequence_scale(I2C_LEFT, 0, 0b11, (5*3), scale_seq, (4*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 0, 0b11);
	if (!res) Error_Handler();

	/* Sub Squ
	 * LEFT1:00
	 * 0, 1
	 */

	pwm_seq[0] = 0;
	pwm_seq[1] = 0;
	pwm_seq[2] = _12_to_8(ctrl_value.sub_lvl);
	pwm_seq[3] = 0;
	pwm_seq[4] = 0;
	pwm_seq[5] = _12_to_8(ctrl_value.sub_lvl);
	res = is32_set_sequence_pwm(I2C_LEFT, 1, 0b00, (0*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

	scale_seq[0] = DEFAULT_SCALE;
	scale_seq[1] = DEFAULT_SCALE;
	scale_seq[2] = DEFAULT_SCALE;
	scale_seq[3] = DEFAULT_SCALE;
	scale_seq[4] = DEFAULT_SCALE;
	scale_seq[5] = DEFAULT_SCALE;
	res = is32_set_sequence_scale(I2C_LEFT, 1, 0b00, (0*3), scale_seq, (2*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 1, 0b00);
	if (!res) Error_Handler();

	/* Sub Noise
	 * LEFT1:00
	 * 2, 3
	 */

	pwm_seq[0] = 0;
	pwm_seq[1] = _12_to_8(ctrl_value.sub_noise_lvl);
	pwm_seq[2] = 0;
	pwm_seq[3] = 0;
	pwm_seq[4] = _12_to_8(ctrl_value.sub_noise_lvl);
	pwm_seq[5] = 0;
	res = is32_set_sequence_pwm(I2C_LEFT, 1, 0b00, (2*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

	scale_seq[0] = DEFAULT_SCALE;
	scale_seq[1] = DEFAULT_SCALE;
	scale_seq[2] = DEFAULT_SCALE;
	scale_seq[3] = DEFAULT_SCALE;
	scale_seq[4] = DEFAULT_SCALE;
	scale_seq[5] = DEFAULT_SCALE;
	res = is32_set_sequence_scale(I2C_LEFT, 1, 0b00, (2*3), scale_seq, (2*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 1, 0b00);
	if (!res) Error_Handler();

	/* Sub Mix
	 * LEFT1:00
	 * 4, 5, 6, 7, 8, 9, 10
	 */

	pwm_seq[0] = 0;
	pwm_seq[1] = _12_to_8(ctrl_value.sub_noise_lvl);
	pwm_seq[2] = _12_to_8(ctrl_value.sub_lvl);
	pwm_seq[3] = 0;
	pwm_seq[4] = _12_to_8(ctrl_value.sub_noise_lvl);
	pwm_seq[5] = _12_to_8(ctrl_value.sub_lvl);
	pwm_seq[6] = 0;
	pwm_seq[7] = _12_to_8(ctrl_value.sub_noise_lvl);
	pwm_seq[8] = _12_to_8(ctrl_value.sub_lvl);
	pwm_seq[9] = 0;
	pwm_seq[10] = _12_to_8(ctrl_value.sub_noise_lvl);
	pwm_seq[11] = _12_to_8(ctrl_value.sub_lvl);
	pwm_seq[12] = 0;
	pwm_seq[13] = _12_to_8(ctrl_value.sub_noise_lvl);
	pwm_seq[14] = _12_to_8(ctrl_value.sub_lvl);
	pwm_seq[15] = 0;
	pwm_seq[16] = _12_to_8(ctrl_value.sub_noise_lvl);
	pwm_seq[17] = _12_to_8(ctrl_value.sub_lvl);
	pwm_seq[18] = 0;
	pwm_seq[19] = _12_to_8(ctrl_value.sub_noise_lvl);
	pwm_seq[20] = _12_to_8(ctrl_value.sub_lvl);
	res = is32_set_sequence_pwm(I2C_LEFT, 1, 0b00, (4 * 3), pwm_seq, (7 * 3));
	if (!res) Error_Handler();

	scale_seq[0] = DEFAULT_SCALE;
	scale_seq[1] = DEFAULT_SCALE;
	scale_seq[2] = DEFAULT_SCALE;
	scale_seq[3] = DEFAULT_SCALE;
	scale_seq[4] = DEFAULT_SCALE;
	scale_seq[5] = DEFAULT_SCALE;
	scale_seq[6] = DEFAULT_SCALE;
	scale_seq[7] = DEFAULT_SCALE;
	scale_seq[8] = DEFAULT_SCALE;
	scale_seq[9] = DEFAULT_SCALE;
	scale_seq[10] = DEFAULT_SCALE;
	scale_seq[11] = DEFAULT_SCALE;
	scale_seq[12] = DEFAULT_SCALE;
	scale_seq[13] = DEFAULT_SCALE;
	scale_seq[14] = DEFAULT_SCALE;
	scale_seq[15] = DEFAULT_SCALE;
	scale_seq[16] = DEFAULT_SCALE;
	scale_seq[17] = DEFAULT_SCALE;
	scale_seq[18] = DEFAULT_SCALE;
	scale_seq[19] = DEFAULT_SCALE;
	scale_seq[20] = DEFAULT_SCALE;
	res = is32_set_sequence_scale(I2C_LEFT, 1, 0b00, (4 * 3), scale_seq, (7 * 3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 1, 0b00);
	if (!res) Error_Handler();

	/* SubToOsc2 Mix Level
	 * LEFT3:00
	 * 7, 8
	 */

	pwm_seq[0] = 0;
	pwm_seq[1] = _12_to_8(((double)ctrl_value.sub_noise_lvl * ((double)ctrl_value.sub_to_osc2 / 4095.0)));
	pwm_seq[2] = _12_to_8(((double)ctrl_value.sub_lvl * ((double)ctrl_value.sub_to_osc2 / 4095.0)));
	pwm_seq[3] = 0;
	pwm_seq[4] = _12_to_8(((double)ctrl_value.sub_noise_lvl * ((double)ctrl_value.sub_to_osc2 / 4095.0)));
	pwm_seq[5] = _12_to_8(((double)ctrl_value.sub_lvl * ((double)ctrl_value.sub_to_osc2 / 4095.0)));
	res = is32_set_sequence_pwm(I2C_LEFT, 3, 0b00, (7*3), pwm_seq, (2*3));
	if (!res) Error_Handler();

	scale_seq[0] = DEFAULT_SCALE;
	scale_seq[1] = DEFAULT_SCALE;
	scale_seq[2] = DEFAULT_SCALE;
	scale_seq[3] = DEFAULT_SCALE;
	scale_seq[4] = DEFAULT_SCALE;
	scale_seq[5] = DEFAULT_SCALE;
	res = is32_set_sequence_scale(I2C_LEFT, 3, 0b00, (7*3), scale_seq, (2*3));
	if (!res) Error_Handler();

	res = is32_write_registers(I2C_LEFT, 3, 0b00);
	if (!res) Error_Handler();



//	res = is32_set_rgb(I2C_LEFT, 0, 0, 0, 0xFF, 0x00, 0x00, _12_to_8(ctrl_value.osc1_saw_lvl));
//	if (!res) Error_Handler();
//	res = is32_set_rgb(I2C_LEFT, 0, 0, 1, 0xFF, 0x00, 0x00, _12_to_8(ctrl_value.osc1_saw_lvl));
//	if (!res) Error_Handler();
//
//	/* Osc1 Sq */
//	res = is32_set_rgb(I2C_LEFT, 0, 0, 2, 0x00, 0x00, 0xFF, _12_to_8(ctrl_value.osc1_squ_lvl));
//	if (!res) Error_Handler();
//	res = is32_set_rgb(I2C_LEFT, 0, 0, 3, 0x00, 0x00, 0xFF, _12_to_8(ctrl_value.osc1_squ_lvl));
//	if (!res) Error_Handler();
//
//	/* Osc1 Mix */
//	res = is32_set_rgb(I2C_LEFT, 0, 0, 4, _12_to_8(ctrl_value.osc1_saw_lvl), 0x00, _12_to_8(ctrl_value.osc1_squ_lvl), _max(ctrl_value.osc1_saw_lvl, ctrl_value.osc1_squ_lvl));
//	if (!res) Error_Handler();
//
//	/* LEFT0:11 */
//
//	/* Osc2 Saw */
//	res = is32_set_rgb(I2C_LEFT, 0, 3, 0, 0xFF, 0x00, 0x00, _12_to_8(ctrl_value.osc2_saw_lvl));
//	if (!res) Error_Handler();
//	res = is32_set_rgb(I2C_LEFT, 0, 3, 1, 0xFF, 0x00, 0x00, _12_to_8(ctrl_value.osc2_saw_lvl));
//	if (!res) Error_Handler();
//
//	/* Osc2 Sq */
//	res = is32_set_rgb(I2C_LEFT, 0, 3, 2, 0x00, 0x00, 0xFF, _12_to_8(ctrl_value.osc2_squ_lvl));
//	if (!res) Error_Handler();
//	res = is32_set_rgb(I2C_LEFT, 0, 3, 3, 0x00, 0x00, 0xFF, _12_to_8(ctrl_value.osc2_squ_lvl));
//	if (!res) Error_Handler();
//
//	/* Osc2 Noise */
//	res = is32_set_rgb(I2C_LEFT, 0, 3, 2, 0x00, 0x00, 0xFF, _12_to_8(ctrl_value.osc2_noise_lvl));
//	if (!res) Error_Handler();
//	res = is32_set_rgb(I2C_LEFT, 0, 3, 3, 0x00, 0x00, 0xFF, _12_to_8(ctrl_value.osc2_noise_lvl));
//	if (!res) Error_Handler();
//
//	/* Osc2 Mix */
//	res = is32_set_rgb(I2C_LEFT, 0, 3, 4, _12_to_8(ctrl_value.osc1_saw_lvl), 0x00, _12_to_8(ctrl_value.osc1_squ_lvl), _max(ctrl_value.osc1_saw_lvl, ctrl_value.osc1_squ_lvl));
//	if (!res) Error_Handler();
//
//	/* LEFT1:00 */
//
//	/* Sub */
//	res = is32_set_rgb(I2C_LEFT, 1, 0, 0, 0xFF, 0x00, 0x00, _12_to_8(ctrl_value.sub_lvl));
//	if (!res) Error_Handler();
//	res = is32_set_rgb(I2C_LEFT, 1, 0, 1, 0xFF, 0x00, 0x00, _12_to_8(ctrl_value.sub_lvl));
//	if (!res) Error_Handler();
//
//	/* Noise */
//	res = is32_set_rgb(I2C_LEFT, 1, 0, 2, 0x00, 0x00, 0xFF, _12_to_8(ctrl_value.sub_noise_lvl));
//	if (!res) Error_Handler();
//	res = is32_set_rgb(I2C_LEFT, 1, 0, 3, 0x00, 0x00, 0xFF, _12_to_8(ctrl_value.sub_noise_lvl));
//	if (!res) Error_Handler();
//
//	/* Sub Noise Mix */
//	res = is32_set_rgb(I2C_LEFT, 1, 0, 4, _12_to_8(ctrl_value.sub_lvl), 0x00, _12_to_8(ctrl_value.sub_noise_lvl), _max(ctrl_value.sub_lvl, ctrl_value.sub_noise_lvl));
//	if (!res) Error_Handler();

}

void commit_led_osc() {
	_commit_led_osc1();
}
