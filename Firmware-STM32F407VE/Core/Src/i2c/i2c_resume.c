/*
 * i2c.c
 *
 *  Created on: Apr 19, 2021
 *      Author: jwilson
 */

#include "i2c.h"
#include "main.h"
#include <stdbool.h>

void _i2c_resume_left_bus() {}

void _i2c_resume_right_bus() {}

void i2c_resume(uint8_t bus) {
  I2C_HandleTypeDef *hi2c = i2c_bus[bus].hi2c;

  if (hi2c->State != HAL_I2C_STATE_READY) {
    // Bail, not ready; and that's OK!
    return;
  }

  switch (bus) {
  case I2C_LEFT:
    _i2c_resume_left_bus(hi2c);
    break;
  case I2C_RIGHT:
    _i2c_resume_right_bus(hi2c);
    break;
  default:
    Error_Handler();
  }
}