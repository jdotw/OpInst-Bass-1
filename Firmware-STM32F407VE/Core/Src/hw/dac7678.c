/*
 * dac7678.c
 *
 *  Created on: Apr 13, 2021
 *      Author: jwilson
 */

#include "dac7678.h"
#include "i2c.h"

#define DEFAULT_DAC7678_ADDRESS 0x48

HAL_StatusTypeDef dac7678_reset(uint8_t bus, uint8_t dac) {
	// Turn on internal vref
	uint8_t dac_addr = DEFAULT_DAC7678_ADDRESS + dac;
	uint8_t data[3] = { 0b10000000, 0x00, 0b00010000 };
	HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(i2c_bus[bus], dac_addr << 1, data, 3, HAL_MAX_DELAY);
	return result;
}

HAL_StatusTypeDef dac7678_set_value(uint8_t bus, uint8_t dac, uint8_t dac_channel, uint16_t val) {
	uint8_t dac_addr = DEFAULT_DAC7678_ADDRESS + dac;
	uint8_t data[3] = { 0b00110000 | (dac_channel & 0b1111), (val >> 4), ((val & 0b1111) << 4) };
	HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(i2c_bus[bus], dac_addr << 1, data, 3, HAL_MAX_DELAY);
	return result;
}

