/*
 * i2c.c
 *
 *  Created on: Apr 19, 2021
 *      Author: jwilson
 */

#include "i2c.h"
#include "main.h"
#include <stdbool.h>

void _i2c_resume_left_bus(uint8_t bus, i2c_callback_t callback, void *userdata);

void _i2c_resume_callback(uint8_t bus, void *userdata) {
  if (!i2c_bus[bus].interrupted) {
    i2c_resume(bus);
  }
}

void i2c_resume(uint8_t bus) {

  if (i2c_bus[bus].hi2c->State != HAL_I2C_STATE_READY ||
      i2c_bus[bus].state != I2C_STATE_IDLE || i2c_bus[bus].interrupted) {
    // Not ready to resume
    // This is a failure of the state machine
    Error_Handler();
  }

  switch (bus) {
  case I2C_LEFT:
    _i2c_resume_left_bus(bus, _i2c_resume_callback, NULL);
    break;
  case I2C_RIGHT:
    // _i2c_resume_right_bus(bus);
    break;
  default:
    Error_Handler();
  }
}