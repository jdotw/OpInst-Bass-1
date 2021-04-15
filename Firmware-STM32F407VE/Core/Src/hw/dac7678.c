/*
 * dac7678.c
 *
 *  Created on: Apr 13, 2021
 *      Author: jwilson
 */

#include "dac7678.h"

HAL_StatusTypeDef dac7678_reset(I2C_HandleTypeDef *dac_bus, uint8_t dac_addr) {
	// Turn on internal vref
	uint8_t data[3] = { 0b10000000, 0x00, 0b00010000 };
	HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(dac_bus, dac_addr << 1, data, 3, HAL_MAX_DELAY);
	return result;
}

HAL_StatusTypeDef dac7678_set_value(I2C_HandleTypeDef *dac_bus, uint8_t dac_addr, uint8_t dac_channel, uint16_t val) {
	uint8_t data[3] = { 0b00110000 | (dac_channel & 0b1111), (val >> 4), ((val & 0b1111) << 4) };
	HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(dac_bus, dac_addr << 1, data, 3, HAL_MAX_DELAY);
	return result;
}

