/*
 * i2c_scan.c
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#include <stdio.h>

#include "i2c.h"
#include "main.h"
#include "tca9544a.h"

char *_i2c_scan_mux_label(uint8_t bus) {
  switch (bus) {
  case I2C_LEFT:
    return "LEFT";
  case I2C_RIGHT:
    return "RIGHT";
  default:
    return "?";
  }
}

void i2c_scan_bus(uint8_t bus, uint8_t channel) {
  i2c_mux_select(bus, channel);
  for (uint8_t i = 0; i < 128; i++) {
    HAL_StatusTypeDef result =
        HAL_I2C_Master_Transmit(i2c_bus[bus], i << 1, NULL, 0, 1000000);
    if (result == HAL_OK) {
      printf("mux %s channel %i: %i\n", _i2c_scan_mux_label(bus), channel, i);
    }
  }
}
