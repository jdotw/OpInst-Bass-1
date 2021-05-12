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

#define LEFT_I2C_MUX_ADDR 0x70
#define RIGHT_I2C_MUX_ADDR 0x71
uint8_t i2c_mux_addr[2] = { 0x70, 0x71 };

#define I2C_MUX_INVALID 0xFF
uint8_t i2c_mux_selected[2] = { 0xFF, 0xFF };
uint8_t i2c_mux_pushed[2] = { 0xFF, 0xFF };

bool i2c_mux_select(uint8_t bus, uint8_t channel) {
	if (i2c_mux_selected[bus] == channel) return true;
	else {
		bool res = tca9544a_select(bus, i2c_mux_addr[bus], channel);
		if (!res) return false;
		else i2c_mux_selected[bus] = channel;
		return true;
	}
}

uint8_t i2c_mux_get_int_status(uint8_t bus) {
	uint8_t rx;
	bool res = i2c_rx(bus, I2C_CHANNEL_DIRECT, i2c_mux_addr[bus], &rx, 1);
	if (!res) {
		Error_Handler();	// FATAL -- We must be able to get our status!
	}

	return (rx & I2C_MUX_INT_MASK);	// Only return the INT status bits
}

void i2c_mux_push(uint8_t bus) {
	if (i2c_mux_pushed[bus] != I2C_MUX_INVALID) {
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
	bool res = i2c_mux_select(bus, i2c_mux_pushed[bus]);
	if (!res) Error_Handler();
	i2c_mux_pushed[bus] = I2C_MUX_INVALID;
}
