/*
 * rotpic_led.c
 *
 *  Created on: 30 Apr 2021
 *      Author: jwilson
 */


#include <stdio.h>
#include <stdbool.h>

#include "main.h"
#include "rotpic.h"
#include "tca9544a.h"
#include "i2c.h"
#include "osc.h"
#include "ctrl.h"

HAL_StatusTypeDef rotpic_led_set_state(uint8_t bus, uint8_t pic, uint8_t led_state) {
	// led_state is a single-byte bit array:
	// 0 = LED 1 On=1 Off=0
	// 1 = LED 2 On=1 Off=0
	// 2 = LED 3 On=1 Off=0
	// 3 = LED 4 On=1 Off=0
	// 4 = LED 5 On=1 Off=0
	// 5 = LED 6 On=1 Off=0
	// 6 = Unused
	// 7 = Unused
	//
	// NOTE: Assumed mux channel is alredy select

	HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(i2c_bus[bus], (DEFAULT_ROTPIC_ADDR + pic) << 1, &led_state, 1, HAL_MAX_DELAY);
	if (result != HAL_OK) return result;

	return result;
}
