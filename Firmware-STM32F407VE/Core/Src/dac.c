/*
 * dac.c
 *
 *  Created on: 25 Apr 2021
 *      Author: jwilson
 */

#include "main.h"
#include "dac.h"
#include "i2c.h"
#include "dac7678.h"
#include <stdio.h>

void dac_init(void) {
  // Reset DACs
	HAL_StatusTypeDef res;

	// I2C Left 0
	res = i2c_mux_select(0, 0, 0);
	if (res != HAL_OK) {
		printf("Failed to select I2C Left 0\n");
		Error_Handler();
	}

	res = dac7678_reset(0, 0); // Left 0:000
	if (res != HAL_OK) {
		printf("Failed to enable internal vref on DAC7678 at Left0:000\n");
		Error_Handler();
	}
	res = dac7678_set_value(0, 0, 15, 0);
	if (res != HAL_OK) {
		printf("Failed to set all channel values to zero on DAC7678 at Left0:000\n");
		Error_Handler();
	}

  res = dac7678_reset(0, 2); // Left 0:010
	if (res != HAL_OK) {
		printf("Failed to enable internal vref on DAC7678 at Left0:010\n");
		Error_Handler();
	}
	res = dac7678_set_value(0, 2, 15, 0);
	if (res != HAL_OK) {
		printf("Failed to set all channel values to zero on DAC7678 at Left0:010\n");
		Error_Handler();
	}

	res = dac7678_reset(0, 4); // Left 0:100
	if (res != HAL_OK) {
		printf("Failed to enable internal vref on DAC7678 at Left0:100\n");
		Error_Handler();
	}
	res = dac7678_set_value(0, 4, 15, 0);
	if (res != HAL_OK) {
		printf("Failed to set all channel values to zero on DAC7678 at Left0:100\n");
		Error_Handler();
	}

	// I2C Left 2
	res = i2c_mux_select(0, 0, 2);
	if (res != HAL_OK) {
		printf("Failed to select I2C Left 2\n");
		Error_Handler();
	}
	res = dac7678_reset(0, 0); // Left 2:000
	if (res != HAL_OK) {
		printf("Failed to enable internal vref on DAC7678 at Left2:000\n");
		Error_Handler();
	}
	res = dac7678_set_value(0, 0, 15, 0);
	if (res != HAL_OK) {
		printf("Failed to set all channel values to zero on DAC7678 at Left2:000\n");
		Error_Handler();
	}

	// I2C Right 2
	res = i2c_mux_select(1, 1, 2);
	if (res != HAL_OK) {
		printf("Failed to select I2C Right 2\n");
		Error_Handler();
	}

	res = dac7678_reset(1, 0); // Right 2:000
	if (res != HAL_OK) {
		printf("Failed to enable internal vref on DAC7678 at Right2:000\n");
		Error_Handler();
	}
	res = dac7678_set_value(1, 0, 15, 0);
	if (res != HAL_OK) {
		printf("Failed to set all channel values to zero on DAC7678 at Right2:000\n");
		Error_Handler();
	}

	res = dac7678_reset(1, 2); // Right 2:010
	if (res != HAL_OK) {
		printf("Failed to enable internal vref on DAC7678 at Right2:010\n");
		Error_Handler();
	}
	res = dac7678_set_value(1, 2, 15, 0);
	if (res != HAL_OK) {
		printf("Failed to set all channel values to zero on DAC7678 at Right2:010\n");
		Error_Handler();
	}

	res = dac7678_reset(1, 4); // Right 2:100
	if (res != HAL_OK) {
		printf("Failed to enable internal vref on DAC7678 at Right2:100\n");
		Error_Handler();
	}
	res = dac7678_set_value(1, 4, 15, 0);
	if (res != HAL_OK) {
		printf("Failed to set all channel values to zero on DAC7678 at Right2:100\n");
		Error_Handler();
	}

}
