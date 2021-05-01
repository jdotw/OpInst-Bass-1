/*
 * commit_led.c
 *
 *  Created on: 30 Apr 2021
 *      Author: jwilson
 */

#include <stdbool.h>
#include "ctrl.h"
#include "main.h"
#include "i2c.h"
#include "dac7678.h"
#include "osc.h"
#include "rotpic.h"
#include <math.h>

void commit_rotic_led() {

	uint8_t led = 0x0;
	HAL_StatusTypeDef res;

	// I2C Left 0
	res = i2c_mux_select(I2C_LEFT, I2C_LEFT_MUX, 0);
	if (res != HAL_OK) {
		Error_Handler();
	}

	// LEFT0:000
	led = 0x00;
	led |= (ctrl_toggle.osc1_squ_func == CTRL_OSC_SQU_LEVEL) << ROTPIC_LED1_BIT_SHIFT;
	led |= (ctrl_toggle.osc1_squ_func == CTRL_OSC_SQU_PWM) << ROTPIC_LED2_BIT_SHIFT;
	res = rotpic_led_set_state(I2C_LEFT, 0b000, led);
	if (res != HAL_OK) Error_Handler();

	// LEFT0:001
	led = 0x00;
	led |= (ctrl_toggle.osc_filt_env_attack_func == CTRL_SELECT_ENV_1) << ROTPIC_LED1_BIT_SHIFT;
	led |= (ctrl_toggle.osc_filt_env_attack_func == CTRL_SELECT_ENV_2) << ROTPIC_LED2_BIT_SHIFT;
	led |= (ctrl_toggle.osc_filt_env_sustain_func == CTRL_ENV_SUSTAIN) << ROTPIC_LED4_BIT_SHIFT;
	led |= (ctrl_toggle.osc_filt_env_sustain_func == CTRL_ENV_AMOUNT) << ROTPIC_LED3_BIT_SHIFT;
	res = rotpic_led_set_state(I2C_LEFT, 0b001, led);
	if (res != HAL_OK) Error_Handler();

	// LEFT0:010
	// No LEDs

	// LEFT0:011
	led = 0x00;
	led |= (ctrl_toggle.osc2_squ_func == CTRL_OSC_SQU_LEVEL) << ROTPIC_LED1_BIT_SHIFT;
	led |= (ctrl_toggle.osc2_squ_func == CTRL_OSC_SQU_PWM) << ROTPIC_LED2_BIT_SHIFT;
	res = rotpic_led_set_state(I2C_LEFT, 0b011, led);
	if (res != HAL_OK) Error_Handler();

	// LEFT0:100
	// No LEDs

	// I2C Left 2
	res = i2c_mux_select(I2C_LEFT, I2C_LEFT_MUX, 2);
	if (res != HAL_OK) {
		Error_Handler();
	}

	// LEFT2:000
	led = 0x00;
	led |= (ctrl_toggle.sub_filt_env_attack_func == CTRL_SELECT_ENV_1) << ROTPIC_LED1_BIT_SHIFT;
	led |= (ctrl_toggle.sub_filt_env_attack_func == CTRL_SELECT_ENV_2) << ROTPIC_LED2_BIT_SHIFT;
	led |= (ctrl_toggle.sub_filt_env_sustain_func == CTRL_ENV_SUSTAIN) << ROTPIC_LED3_BIT_SHIFT;
	led |= (ctrl_toggle.sub_filt_env_sustain_func == CTRL_ENV_AMOUNT) << ROTPIC_LED4_BIT_SHIFT;
	res = rotpic_led_set_state(I2C_LEFT, 0b000, led);
	if (res != HAL_OK) Error_Handler();

	// LEFT2:001
	// No LEDs

	// I2C Left 3
	res = i2c_mux_select(I2C_LEFT, I2C_LEFT_MUX, 3);
	if (res != HAL_OK) {
		Error_Handler();
	}

	// LEFT3:000
	// No LEDs

	// I2C Right 0
	res = i2c_mux_select(I2C_RIGHT, I2C_RIGHT_MUX, 0);
	if (res != HAL_OK) {
		Error_Handler();
	}

	// RIGHT0:000
	led = 0x00;
	led |= (ctrl_toggle.osc_amp_env_sustain_func == CTRL_ENV_SUSTAIN) << ROTPIC_LED1_BIT_SHIFT;
	led |= (ctrl_toggle.osc_amp_env_sustain_func == CTRL_ENV_AMOUNT) << ROTPIC_LED2_BIT_SHIFT;
	res = rotpic_led_set_state(I2C_LEFT, 0b000, led);
	if (res != HAL_OK) Error_Handler();


	// I2C Right 1
	res = i2c_mux_select(I2C_RIGHT, I2C_RIGHT_MUX, 1);
	if (res != HAL_OK) {
		Error_Handler();
	}

	// RIGHT1:000
	// No LEDs

	// RIGHT1:001
	led = 0x00;
	led |= (ctrl_toggle.sub_amp_env_sustain_func == CTRL_ENV_SUSTAIN) << ROTPIC_LED1_BIT_SHIFT;
	led |= (ctrl_toggle.sub_amp_env_sustain_func == CTRL_ENV_AMOUNT) << ROTPIC_LED2_BIT_SHIFT;
	res = rotpic_led_set_state(I2C_RIGHT, 0b001, led);
	if (res != HAL_OK) Error_Handler();

}
