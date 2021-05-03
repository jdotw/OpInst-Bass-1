/*
 * i2cmux.c
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#include <main.h>
#include <i2c.h>
#include "tca9544a.h"

uint8_t i2c_mux_addr[2] = { 0x70, 0x71 };

HAL_StatusTypeDef i2c_mux_reset(I2C_HandleTypeDef *bus, uint8_t mux_addr, EXTI_HandleTypeDef *irq) {
	return HAL_OK;
}

HAL_StatusTypeDef i2c_mux_select(uint8_t bus, uint8_t mux, uint8_t channel) {
	return tca9544a_select(i2c_bus[bus], i2c_mux_addr[mux], channel);
}

uint8_t i2c_mux_get_int_status(I2C_HandleTypeDef *bus, uint8_t mux_addr) {
	uint8_t rx;
	HAL_StatusTypeDef res = HAL_I2C_Master_Receive(bus, mux_addr << 1, &rx, 1, HAL_MAX_DELAY);
	if (res != HAL_OK) {
		Error_Handler();	// FATAL -- We must be able to get our status!
	}

	return (rx & I2C_MUX_INT_MASK);	// Only return the INT status bits
}
