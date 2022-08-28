/*
 * i2c_scan.c
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#include <stdio.h>

#include "i2c.h"
#include "main.h"

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

bool _i2c_scan_mux_select(uint8_t bus, uint8_t channel) {
  I2C_HandleTypeDef *hi2c = i2c_bus[bus].hi2c;
  uint8_t data = (channel & 0b11) | 0b100;
  HAL_StatusTypeDef res;
  res = HAL_I2C_Master_Transmit(hi2c, i2c_bus[bus].mux_addr << 1, &data, 1,
                                HAL_MAX_DELAY);
  if (!res)
    return false;
  return true;
}

void i2c_scan_bus(uint8_t bus, uint8_t channel) {
  _i2c_scan_mux_select(bus, channel);
  for (uint8_t i = 0; i < 128; i++) {
    HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(
        i2c_bus[bus].hi2c, i << 1, NULL, 0, HAL_MAX_DELAY);
    if (result == HAL_OK) {
      printf("mux %s channel %i: %i\n", _i2c_scan_mux_label(bus), channel, i);
    }
  }
}
