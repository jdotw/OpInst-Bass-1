/*
 * i2cmux.c
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#include <stdbool.h>
#include <main.h>
#include <i2c.h>
#include "tca9544a.h"

#define I2C_MUX_INVALID 0xFF

uint8_t i2c_mux_addr[2] = { 0x70, 0x71 };
uint8_t i2c_mux_selected[2] = { 0xFF, 0xFF };
uint8_t i2c_mux_pushed[2] = { 0xFF, 0xFF };
bool i2c_mux_popped[2] = { true, true };

HAL_StatusTypeDef i2c_mux_reset(I2C_HandleTypeDef *bus, uint8_t mux_addr, EXTI_HandleTypeDef *irq) {
	return HAL_OK;
}

HAL_StatusTypeDef i2c_mux_select(uint8_t bus, uint8_t mux, uint8_t channel) {
	if (i2c_mux_pushed[bus] == channel) return HAL_OK;
	else return tca9544a_select(i2c_bus[bus], i2c_mux_addr[mux], channel);
}

uint8_t i2c_mux_get_int_status(I2C_HandleTypeDef *bus, uint8_t mux_addr) {
	uint8_t rx;
	HAL_StatusTypeDef res = HAL_I2C_Master_Receive(bus, mux_addr << 1, &rx, 1, HAL_MAX_DELAY);
	if (res != HAL_OK) {
		Error_Handler();	// FATAL -- We must be able to get our status!
	}

	return (rx & I2C_MUX_INT_MASK);	// Only return the INT status bits
}

void i2c_mux_push(uint8_t bus) {
	if (!i2c_mux_popped[bus]) {
		// ERROR: We're pushing on top of a push
		// We only support a single-stack here
		Error_Handler();
	}
	i2c_mux_pushed[bus] = i2c_mux_selected[bus];
}

void i2c_mux_pop(uint8_t bus) {
	if (i2c_mux_pushed[bus] == I2C_MUX_INVALID) {
		// Popped without a push!
		Error_Handler();
	}
	HAL_StatusTypeDef res = i2c_mux_select(bus, bus, i2c_mux_pushed[bus]);
	if (res != HAL_OK) Error_Handler();
	i2c_mux_pushed[bus] = I2C_MUX_INVALID;
	i2c_mux_popped[bus] = true;
}
