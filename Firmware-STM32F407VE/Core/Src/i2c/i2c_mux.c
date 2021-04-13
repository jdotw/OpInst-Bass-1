/*
 * i2cmux.c
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#include <main.h>
#include <i2c.h>

void _i2c_mux_irq_callback() {
	printf("I2C MUX IRQ!!");
}

HAL_StatusTypeDef i2c_mux_reset(I2C_HandleTypeDef *bus, uint8_t mux_addr, EXTI_HandleTypeDef *irq) {
	HAL_StatusTypeDef result = HAL_EXTI_RegisterCallback(irq, HAL_EXTI_COMMON_CB_ID, &_i2c_mux_irq_callback);
	if (result != HAL_OK) {
		return result;
	}
	return HAL_OK;
}

HAL_StatusTypeDef i2c_mux_select_channel(I2C_HandleTypeDef *bus, uint8_t mux_addr, uint8_t channel) {
	uint8_t data = (channel & 0b11) | 0b100;
	HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(bus, mux_addr << 1, &data, 1, HAL_MAX_DELAY);
	if (result != HAL_OK) {
		return result;
	}
	return HAL_OK;
}
