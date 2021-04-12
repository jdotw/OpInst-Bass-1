/*
 * i2c_scan.c
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#include <stdio.h>

#include "main.h"
#include "i2c.h"

void i2c_scan_bus(I2C_HandleTypeDef *bus) {
	uint8_t i;
	for (i = 0; i < 128; i++) {
		HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(bus, i << 1, NULL, 0, 1000000);
		if (result == HAL_OK) {
			printf("FOUND ONE: %i\n", i);
		}
	}

}
