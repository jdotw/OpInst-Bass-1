/*
 * i2c_isr.c
 *
 *  Created on: Apr 19, 2021
 *      Author: jwilson
 */

#include <stdio.h>

#include "main.h"
#include "i2c.h"
#include "rotpic.h"

void _i2c_isr_handle_channel_int(uint8_t bus, uint8_t mux, uint8_t channel) {
	rotpic_poll_all(bus, mux, channel);
}

void _i2c_isr_check_mux(uint8_t bus_id, uint8_t mux_id) {
	uint8_t int_status = i2c_mux_get_int_status(i2c_bus[bus_id], i2c_mux_addr[mux_id]);
	if (int_status & I2C_MUX_INT_ERROR) {
		return;
	}
	if (int_status & I2C_MUX_INT_0_SET) {
		// Channel 0 Interrupt
		_i2c_isr_handle_channel_int(bus_id, mux_id, 0);
	}
	if (int_status & I2C_MUX_INT_1_SET) {
		// Channel 1 Interrupt
		_i2c_isr_handle_channel_int(bus_id, mux_id, 1);
	}
	if (int_status & I2C_MUX_INT_2_SET) {
		// Channel 2 Interrupt
		_i2c_isr_handle_channel_int(bus_id, mux_id, 2);
	}
	if (int_status & I2C_MUX_INT_3_SET) {
		// Channel 3 Interrupt
		_i2c_isr_handle_channel_int(bus_id, mux_id, 3);
	}
}

void i2c_isr() {
	// Called when any of the I2C interrupt lines go active-low
	// From here we work out which line it was, then query the
	// mux on that bus to determine which channel has interrupted
	if (HAL_GPIO_ReadPin(GPIOB, I2C1_INT_Pin) == GPIO_PIN_RESET) {
		_i2c_isr_check_mux(0, 0);
	}
	if (HAL_GPIO_ReadPin(GPIOB, I2C2_INT_Pin) == GPIO_PIN_RESET) {
		_i2c_isr_check_mux(1, 1);
	}
}

