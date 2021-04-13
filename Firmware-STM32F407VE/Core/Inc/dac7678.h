/*
 * dac7678.h
 *
 *  Created on: Apr 13, 2021
 *      Author: jwilson
 */

#ifndef INC_DAC7678_H_
#define INC_DAC7678_H_

#include "stm32f4xx_hal.h"

HAL_StatusTypeDef dac7678_reset(I2C_HandleTypeDef *dac_bus, uint8_t dac_addr);
HAL_StatusTypeDef dac7678_set_value(I2C_HandleTypeDef *dac_bus, uint8_t dac_addr, uint8_t dac_channel, uint16_t val);

#endif /* INC_DAC7678_H_ */
