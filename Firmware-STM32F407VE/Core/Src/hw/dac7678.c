/*
 * dac7678.c
 *
 *  Created on: Apr 13, 2021
 *      Author: jwilson
 */

#include "dac7678.h"
#include "i2c.h"
#include "main.h"
#include <stdbool.h>

#define DEFAULT_DAC7678_ADDRESS 0x48
#define DAC_CHANNEL_ALL 15
#define DAC_CHANNEL_DEFAULT_VALUE 0

bool dac7678_reset(uint8_t bus, uint8_t channel, uint8_t dac) {
  // Turn on internal vref
  uint8_t dac_addr = DEFAULT_DAC7678_ADDRESS + dac;
  uint8_t data[3] = {0b10000000, 0x00, 0b00010000};
  bool res = i2c_tx(bus, channel, dac_addr, data, 3);
  if (!res)
    Error_Handler();
  res = dac7678_set_value(bus, channel, dac, DAC_CHANNEL_ALL,
                          DAC_CHANNEL_DEFAULT_VALUE);
  if (!res)
    Error_Handler();
  return true;
}

bool dac7678_set_value(uint8_t bus, uint8_t channel, uint8_t dac,
                       uint8_t dac_channel, uint16_t val) {
  uint8_t dac_addr = DEFAULT_DAC7678_ADDRESS + dac;
  uint8_t data[3] = {0b00110000 | (dac_channel & 0b1111), (val >> 4),
                     ((val & 0b1111) << 4)};
  bool res = i2c_tx(bus, channel, dac_addr, data, 3);
  return res;
}

bool dac7678_set_value_array(uint8_t bus, uint8_t channel, uint8_t dac,
                             uint16_t val[7]) {
  for (uint8_t i = 0; i < 8; i++) {
    bool res = dac7678_set_value(bus, channel, dac, i, val[i]);
    if (!res)
      return res;
  }
  return true;
}

void dac7678_init(void) {
  // Reset DACs
  HAL_StatusTypeDef res;

  res = dac7678_reset(I2C_LEFT, 0, 0); // Left 0:000
  if (!res)
    Error_Handler();

  res = dac7678_reset(I2C_LEFT, 0, 2); // Left 0:010
  if (!res)
    Error_Handler();

  res = dac7678_reset(I2C_LEFT, 0, 4); // Left 0:100
  if (!res)
    Error_Handler();

  res = dac7678_reset(I2C_LEFT, 2, 0); // Left 2:000
  if (!res)
    Error_Handler();

  res = dac7678_reset(I2C_RIGHT, 2, 0); // Right 2:000
  if (!res)
    Error_Handler();

  res = dac7678_reset(I2C_RIGHT, 2, 2); // Right 2:010
  if (!res)
    Error_Handler();

  res = dac7678_reset(I2C_RIGHT, 2, 4); // Right 2:100
  if (!res)
    Error_Handler();
}
