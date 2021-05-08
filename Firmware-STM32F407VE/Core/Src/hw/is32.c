/*
 * is32led.c
 *
 *  Created on: Apr 13, 2021
 *      Author: jwilson
 */

#include <stdio.h>
#include "main.h"
#include "is32.h"
#include "i2c.h"

#define DEFAULT_IS32_ADDR 0x34

void is32_init() {
	HAL_StatusTypeDef res;

	// I2C Left 0
	res = i2c_mux_select(I2C_LEFT, I2C_LEFT_MUX, 0);
	if (res != HAL_OK) {
		Error_Handler();
	}

	// LEFT0:00
	is32_enable(I2C_LEFT, 0);

	// LEFT0:11
	is32_enable(I2C_LEFT, 3);

	// I2C Left 1
	res = i2c_mux_select(I2C_LEFT, I2C_LEFT_MUX, 1);
	if (res != HAL_OK) {
		Error_Handler();
	}

	// LEFT1:00
	is32_enable(I2C_LEFT, 0);

	// LEFT1:11
	is32_enable(I2C_LEFT, 3);

	// I2C Left 3
	res = i2c_mux_select(I2C_LEFT, I2C_LEFT_MUX, 3);
	if (res != HAL_OK) {
		Error_Handler();
	}

	// LEFT3:01
	is32_enable(I2C_LEFT, 1);

	// I2C Right 1
	res = i2c_mux_select(I2C_RIGHT, I2C_RIGHT_MUX, 1);
	if (res != HAL_OK) {
		Error_Handler();
	}

	// RIGHT1:10
	is32_enable(I2C_RIGHT, 2);

	// I2C Right 2
	res = i2c_mux_select(I2C_RIGHT, I2C_RIGHT_MUX, 2);
	if (res != HAL_OK) {
		Error_Handler();
	}

	// RIGHT2:00
	is32_enable(I2C_RIGHT, 0);

}

HAL_StatusTypeDef is32_enable(uint8_t bus, uint8_t unit) {
	HAL_StatusTypeDef ret;
	uint8_t data[2] = { 0x00, 0x00 };

	data[0] =  0x7F;
	data[1] = 0x00;
	ret = HAL_I2C_Master_Transmit(i2c_bus[bus], (DEFAULT_IS32_ADDR+unit) << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}
	data[0] = 0x00;
	data[1] = 0b00000001;
	ret = HAL_I2C_Master_Transmit(i2c_bus[bus], (DEFAULT_IS32_ADDR+unit) << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}
	data[0] = 0x6E;
	data[1] = 0xFF;
	ret = HAL_I2C_Master_Transmit(i2c_bus[bus], (DEFAULT_IS32_ADDR+unit) << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}

	return HAL_OK;
}

HAL_StatusTypeDef is32_turn_on_led_rgb(uint8_t bus, uint8_t unit, uint8_t led, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness) {
	uint8_t data[2] = {  0x00, 0x00 };
	HAL_StatusTypeDef ret;

	// PWM
	data[0] =  0x01 + (led * 6);
	data[1] = red;
	ret = HAL_I2C_Master_Transmit(i2c_bus[bus], (DEFAULT_IS32_ADDR+unit) << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}
	data[0] =  0x03 + (led * 6);
	data[1] = green;
	ret = HAL_I2C_Master_Transmit(i2c_bus[bus], (DEFAULT_IS32_ADDR+unit) << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}
	data[0] =  0x05 + (led * 6);
	data[1] = blue;
	ret = HAL_I2C_Master_Transmit(i2c_bus[bus], (DEFAULT_IS32_ADDR+unit) << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}

	// Scaling (current)
	data[0] = 0x4A + (led * 3);
	data[1] = brightness;
	ret = HAL_I2C_Master_Transmit(i2c_bus[bus], (DEFAULT_IS32_ADDR+unit) << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}
	data[0] = 0x4B + (led * 3);
	data[1] = brightness;
	ret = HAL_I2C_Master_Transmit(i2c_bus[bus], (DEFAULT_IS32_ADDR+unit) << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}
	data[0] = 0x4C + (led * 3);
	data[1] = brightness;
	ret = HAL_I2C_Master_Transmit(i2c_bus[bus], (DEFAULT_IS32_ADDR+unit) << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}

	// Write the registers
	data[0] = 0x49;
	data[1] = 0x00;
	ret = HAL_I2C_Master_Transmit(i2c_bus[bus], (DEFAULT_IS32_ADDR+unit) << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}

	return HAL_OK;
}

HAL_StatusTypeDef is32_turn_on_led_single(uint8_t bus, uint8_t unit, uint8_t led, uint8_t pwm, uint8_t brightness) {
	uint8_t data[2] = {  0x00, 0x00 };
	HAL_StatusTypeDef ret;

	// PWM
	data[0] = 0x01 + (led * 2);
	data[1] = pwm;
	ret = HAL_I2C_Master_Transmit(i2c_bus[bus], (DEFAULT_IS32_ADDR+unit) << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}

	// Scaling (current)
	data[0] = 0x4A + led;
	data[1] = brightness;
	ret = HAL_I2C_Master_Transmit(i2c_bus[bus], (DEFAULT_IS32_ADDR+unit) << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}

	// Write the registers
	data[0] = 0x49;
	data[1] = 0x00;
	ret = HAL_I2C_Master_Transmit(i2c_bus[bus], (DEFAULT_IS32_ADDR+unit) << 1, data, 2, HAL_MAX_DELAY);
	if (ret != HAL_OK) {
		return ret;
	}

	return HAL_OK;
}


//#define PATTERN_LENGTH 12
//
//void is32_flux_cap(uint8_t bus, uint8_t mux, uint8_t channel, uint8_t unit) {
//	uint8_t red, green, blue;
//
//	HAL_StatusTypeDef res = i2c_mux_select(bus, mux, channel);
//	if (res != HAL_OK) {
//		Error_Handler();
//	}
//
////	uint8_t brightness[PATTERN_LENGTH] = { 10, 10, 15, 20, 30, 45, 65, 90, 130, 90, 65, 45, 30, 20, 10, 10, 10, 10, 10, 5  };
//	uint8_t brightness[PATTERN_LENGTH] = { 10, 10, 15, 20, 30, 45, 65, 90, 130, 90, 65, 45, };
//	uint8_t i = 0;
//
//	red = 0xFF;
//	green = 0x22;
//	blue = 0xAA;
//
//	while(1) {
//		uint8_t offset;
//		for (offset=0; offset < PATTERN_LENGTH; offset++) {
//			for (i=0; i < 10; i++) {
//				HAL_StatusTypeDef result = is32_turn_on_led_rgb(bus, unit, i, red, green, blue, brightness[(i+offset)%PATTERN_LENGTH]);
//				if (result != HAL_OK) {
//					printf("Failed...");
//				}
//			}
//			HAL_Delay(48);
//		}
//	}
//}
//
