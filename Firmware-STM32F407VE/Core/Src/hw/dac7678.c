/*
 * dac7678.c
 *
 *  Created on: Apr 13, 2021
 *      Author: jwilson
 */

#include "main.h"
#include "dac7678.h"
#include "i2c.h"

#define DEFAULT_DAC7678_ADDRESS 0x48
#define DAC_CHANNEL_ALL 15
#define DAC_CHANNEL_DEFAULT_VALUE 0

HAL_StatusTypeDef dac7678_reset(uint8_t bus, uint8_t dac) {
	// Turn on internal vref
	uint8_t dac_addr = DEFAULT_DAC7678_ADDRESS + dac;
	uint8_t data[3] = { 0b10000000, 0x00, 0b00010000 };
	HAL_StatusTypeDef res = HAL_I2C_Master_Transmit(i2c_bus[bus], dac_addr << 1, data, 3, HAL_MAX_DELAY);
	if (res != HAL_OK) Error_Handler();
	res = dac7678_set_value(bus, dac, DAC_CHANNEL_ALL, DAC_CHANNEL_DEFAULT_VALUE);
	if (res != HAL_OK) Error_Handler();
	return res;
}

HAL_StatusTypeDef dac7678_set_value(uint8_t bus, uint8_t dac, uint8_t dac_channel, uint16_t val) {
	uint8_t dac_addr = DEFAULT_DAC7678_ADDRESS + dac;
	uint8_t data[3] = { 0b00110000 | (dac_channel & 0b1111), (val >> 4), ((val & 0b1111) << 4) };
	HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(i2c_bus[bus], dac_addr << 1, data, 3, HAL_MAX_DELAY);
	return result;
}

HAL_StatusTypeDef dac7678_set_value_array(uint8_t bus, uint8_t dac, uint16_t val[7]) {
	for (uint8_t i = 0; i < 8; i++) {
		HAL_StatusTypeDef res = dac7678_set_value(bus, dac, i, val[i]);
		if (res != HAL_OK) return res;
	}
	return HAL_OK;
}


void dac7678_init(void) {
  // Reset DACs
	HAL_StatusTypeDef res;

	// I2C Left 0
	res = i2c_mux_select(0, 0, 0);
	if (res != HAL_OK) Error_Handler();

	res = dac7678_reset(0, 0); // Left 0:000
	if (res != HAL_OK) Error_Handler();

  res = dac7678_reset(0, 2); // Left 0:010
	if (res != HAL_OK) Error_Handler();
	res = dac7678_set_value(0, 2, 15, 0);
	if (res != HAL_OK) Error_Handler();

	res = dac7678_reset(0, 4); // Left 0:100
	if (res != HAL_OK) Error_Handler();
	res = dac7678_set_value(0, 4, 15, 0);
	if (res != HAL_OK) Error_Handler();

	// I2C Left 2
	res = i2c_mux_select(0, 0, 2);
	if (res != HAL_OK) {
		Error_Handler();
	}
	res = dac7678_reset(0, 0); // Left 2:000
	if (res != HAL_OK) {
		Error_Handler();
	}
	res = dac7678_set_value(0, 0, 15, 0);
	if (res != HAL_OK) {
		Error_Handler();
	}

	// I2C Right 2
	res = i2c_mux_select(1, 1, 2);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = dac7678_reset(1, 0); // Right 2:000
	if (res != HAL_OK) {
		Error_Handler();
	}
	res = dac7678_set_value(1, 0, 15, 0);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = dac7678_reset(1, 2); // Right 2:010
	if (res != HAL_OK) {
		Error_Handler();
	}
	res = dac7678_set_value(1, 2, 15, 0);
	if (res != HAL_OK) {
		Error_Handler();
	}

	res = dac7678_reset(1, 4); // Right 2:100
	if (res != HAL_OK) {
		Error_Handler();
	}
	res = dac7678_set_value(1, 4, 15, 0);
	if (res != HAL_OK) {
		Error_Handler();
	}

}
