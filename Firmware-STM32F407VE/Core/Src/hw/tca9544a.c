/*
 * tca544a.c
 *
 *  Created on: Apr 13, 2021
 *      Author: jwilson
 */

#include <tca9544a.h>
#include "main.h"

HAL_StatusTypeDef tca9544a_select (I2C_HandleTypeDef *bus, uint8_t mux_addr, uint8_t channel) {
	uint8_t data = (channel & 0b11) | 0b100;
	HAL_StatusTypeDef res = HAL_I2C_Master_Transmit(bus, mux_addr << 1, &data, 1, HAL_MAX_DELAY);
	return res;
}


