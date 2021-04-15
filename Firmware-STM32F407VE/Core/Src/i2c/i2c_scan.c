/*
 * i2c_scan.c
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#include <stdio.h>

#include "main.h"
#include "i2c.h"
#include "tca9544a.h"

char* _i2c_scan_mux_label(uint8_t mux_address) {
	switch (mux_address) {
	case LEFT_I2C_MUX_ADDR:
		return "LEFT";
	case RIGHT_I2C_MUX_ADDR:
	return "RIGHT";
	default:
		return "?";
	}
}

void i2c_scan_bus(I2C_HandleTypeDef *bus, uint8_t mux_address, uint8_t mux_channel) {
	tca9544a_select(bus, mux_address, mux_channel);
	for (uint8_t i = 0; i < 128; i++) {
		HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(bus, i << 1, NULL, 0, 1000000);
		if (result == HAL_OK) {
			printf("mux %s channel %i: %i\n", _i2c_scan_mux_label(mux_address) , mux_channel, i);
		}
	}

}
