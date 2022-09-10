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

bool dac7678_set_value_sync(uint8_t bus, uint8_t channel, uint8_t dac,
                            uint8_t dac_channel, uint16_t val) {
  uint8_t dac_addr = DEFAULT_DAC7678_ADDRESS + dac;
  uint8_t data[3] = {0b00110000 | (dac_channel & 0b1111), (val >> 4),
                     ((val & 0b1111) << 4)};
  bool res = i2c_tx_sync(bus, channel, dac_addr, data, 3);
  return res;
}

bool dac7678_set_value(uint8_t bus, uint8_t channel, uint8_t dac,
                       uint8_t dac_channel, uint16_t val,
                       i2c_callback_t callback, void *userdata) {
  uint8_t dac_addr = DEFAULT_DAC7678_ADDRESS + dac;
  static uint8_t data[I2C_BUS_MAX][3];
  data[bus][0] = 0b00110000 | (dac_channel & 0b1111);
  data[bus][1] = val >> 4;
  data[bus][2] = (val & 0b1111) << 4;
  bool res = i2c_tx(bus, channel, dac_addr, data[bus], 3, callback, userdata);
  return res;
}

bool dac7678_reset(uint8_t bus, uint8_t channel, uint8_t dac) {
  // Turn on internal vref
  uint8_t dac_addr = DEFAULT_DAC7678_ADDRESS + dac;
  uint8_t vref_data[3] = {0b10000000, 0x00, 0b00010000};
  bool res = i2c_tx_sync(bus, channel, dac_addr, vref_data, 3);
  if (!res)
    Error_Handler();

  uint8_t channel_data[3] = {0b00110000 | (DAC_CHANNEL_ALL & 0b1111),
                             (DAC_CHANNEL_DEFAULT_VALUE >> 4),
                             ((DAC_CHANNEL_DEFAULT_VALUE & 0b1111) << 4)};
  res = i2c_tx_sync(bus, channel, dac_addr, channel_data, 3);
  if (!res)
    Error_Handler();
  return res;
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

  // UNUSED: The osc and sub filter env amount DAC channels
  //         are not used (due to switching to programatic envelopes)
  //         but the DAC channel must be initialized to CTRL_DEFAULT_MAX
  //         to effectively disable the analog envelope generators
  res = dac7678_set_value_sync(I2C_LEFT, 0, 4, 0, 4095);
  if (!res)
    Error_Handler();
  res = dac7678_set_value_sync(I2C_LEFT, 0, 4, 1, 4095);
  if (!res)
    Error_Handler();
  res = dac7678_set_value_sync(I2C_LEFT, 0, 4, 4, 4095);
  if (!res)
    Error_Handler();
  res = dac7678_set_value_sync(I2C_LEFT, 0, 4, 5, 4095);
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
