/*
 * is32.h
 *
 *  Created on: Apr 13, 2021
 *      Author: jwilson
 */

#ifndef INC_IS32_H_
#define INC_IS32_H_

#include "stm32f4xx_hal.h"

HAL_StatusTypeDef is32_enable(I2C_HandleTypeDef *bus, uint8_t address);
HAL_StatusTypeDef is32_turn_on_led(I2C_HandleTypeDef *bus, uint8_t address, uint8_t led, uint8_t red, uint8_t green, uint8_t blue, uint8_t brightness);
void is32_flux_cap(I2C_HandleTypeDef *bus, uint8_t address);


#endif /* INC_IS32_H_ */
