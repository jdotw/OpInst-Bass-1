/*
 * is32led.c
 *
 *  Created on: Apr 13, 2021
 *      Author: jwilson
 */

#include "is32.h"
#include "main.h"

HAL_StatusTypeDef is32_enable(I2C_HandleTypeDef *bus, uint8_t address) {
	uint8_t data[2] = { 0x7F, 0x00 };
	HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(bus, address << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}
	data[0] = 0x00;
	data[1] = 0b00000001;
	ret = HAL_I2C_Master_Transmit(bus, address << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}
	data[0] = 0x6E;
	data[1] = 0xFF;
	ret = HAL_I2C_Master_Transmit(bus, address << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}

	return HAL_OK;
}

HAL_StatusTypeDef is32_turn_on_led(I2C_HandleTypeDef *bus, uint8_t address, uint8_t led, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness) {
	uint8_t data[2] = {  0x00, 0x00 };
	HAL_StatusTypeDef ret;

	// PWM
	data[0] =  0x01 + (led * 6);
	data[1] = red;
	ret = HAL_I2C_Master_Transmit(bus, address << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}
	data[0] =  0x03 + (led * 6);
	data[1] = green;
	ret = HAL_I2C_Master_Transmit(bus, address << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}
	data[0] =  0x05 + (led * 6);
	data[1] = blue;
	ret = HAL_I2C_Master_Transmit(bus, address << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}

	// Scaling (current)
	data[0] = 0x4A + (led * 3);
	data[1] = brightness;
	ret = HAL_I2C_Master_Transmit(bus, address << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}
	data[0] = 0x4B + (led * 3);
	data[1] = brightness;
	ret = HAL_I2C_Master_Transmit(bus, address << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}
	data[0] = 0x4C + (led * 3);
	data[1] = brightness;
	ret = HAL_I2C_Master_Transmit(bus, address << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}

	// Write the registers
	data[0] = 0x49;
	data[1] = 0x00;
	ret = HAL_I2C_Master_Transmit(bus, address << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}

	return HAL_OK;
}

#define PATTERN_LENGTH 12

void is32_flux_cap(I2C_HandleTypeDef *bus, uint8_t address) {
	uint8_t red, green, blue;

	uint8_t brightness[PATTERN_LENGTH] = { 10, 10, 15, 20, 30, 45, 65, 90, 130, 90, 65, 45, 30, 20, 10, 10, 10, 10, 10, 5  };
	uint8_t i = 0;

	red = 0xFF;
	green = 0x22;
	blue = 0xAA;

	while(1) {
		uint8_t offset;
		for (offset=0; offset < PATTERN_LENGTH; offset++) {
			for (i=0; i < 10; i++) {
				is32_turn_on_led(bus, address, i, red, green, blue, brightness[(i+offset)%PATTERN_LENGTH]);
			}
			HAL_Delay(32);
		}
	}
}
