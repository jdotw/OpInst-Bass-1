/*
 * i2c.c
 *
 *  Created on: Apr 19, 2021
 *      Author: jwilson
 */

#include <stdbool.h>
#include "main.h"
#include "i2c.h"

I2C_HandleTypeDef *i2c_bus[2];

bool i2c_tx(uint8_t bus, uint8_t channel, uint8_t address, uint8_t *data, uint8_t len) {
	// Transmits using Interrupt/DMA and waits
	// until the bus becomes ready again.

	HAL_StatusTypeDef res;
	I2C_HandleTypeDef *hi2c = i2c_bus[bus];

	// Wait for bus to become available again
	while(hi2c->State != HAL_I2C_STATE_READY) {
		// Do nothing...
	}

	// Select channel
	// NOTE: Will return HAL_OK if already on channel
	if (channel != I2C_CHANNEL_DIRECT) {
		bool did_select = i2c_mux_select(bus, channel);
		if (!did_select) Error_Handler();
	}

	// Transmit via DMA
	res = HAL_I2C_Master_Transmit_DMA(hi2c, address << 1, data, len);
	if (res != HAL_OK) return false;

	// Wait for bus to become available again
	while(hi2c->State != HAL_I2C_STATE_READY) {
		// Do nothing...
	}

	return true;
}
