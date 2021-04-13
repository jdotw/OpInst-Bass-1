/*
 * i2c.h
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "stm32f4xx_hal.h"

/* i2c_mux.c */
#define I2C_MUX_DEFAULT_ADDRESS 0x70
HAL_StatusTypeDef i2c_mux_select_channel(I2C_HandleTypeDef *bus, uint8_t mux_addr, uint8_t channel);

/* i2c_scan.c */
void i2c_scan_bus(I2C_HandleTypeDef *bus);

#endif /* INC_I2C_H_ */
