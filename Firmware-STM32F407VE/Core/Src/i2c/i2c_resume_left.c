/*
 * i2c.c
 *
 *  Created on: Apr 19, 2021
 *      Author: jwilson
 */

#include "i2c.h"
#include "main.h"
#include <stdbool.h>

void _i2c_resume_left_bus(uint8_t bus) {
  I2C_HandleTypeDef *hi2c = i2c_bus[bus].hi2c;

  HAL_GPIO_ReadPin(x, GPIO_PIN_5);
}
