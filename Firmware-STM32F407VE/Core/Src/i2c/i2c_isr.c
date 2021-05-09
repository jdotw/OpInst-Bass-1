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

void i2c_isr(uint8_t bus) {
	// Called when any of the I2C interrupt lines go active-low
	// From here we work out which line it was, then query the
	// mux on that bus to determine which channel has interrupted
	uint8_t int_status = i2c_mux_get_int_status(bus);
	if (int_status & I2C_MUX_INT_ERROR) {
		Error_Handler();
	}
	if (int_status & I2C_MUX_INT_0_SET) {
		// Channel 0 Interrupt
		rotpic_poll_all(bus, 0);
	}
	if (int_status & I2C_MUX_INT_1_SET) {
		// Channel 1 Interrupt
		rotpic_poll_all(bus, 1);
	}
	if (int_status & I2C_MUX_INT_2_SET) {
		// Channel 2 Interrupt
		rotpic_poll_all(bus, 2);
	}
	if (int_status & I2C_MUX_INT_3_SET) {
		// Channel 3 Interrupt
		rotpic_poll_all(bus, 3);
	}
}

