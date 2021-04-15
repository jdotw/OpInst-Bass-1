/*
 * i2cmux.c
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#include <main.h>
#include <i2c.h>

HAL_StatusTypeDef i2c_mux_reset(I2C_HandleTypeDef *bus, uint8_t mux_addr, EXTI_HandleTypeDef *irq) {
	return HAL_OK;
}

