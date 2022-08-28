/*
 * i2c.c
 *
 *  Created on: Apr 19, 2021
 *      Author: jwilson
 */

#include "i2c.h"
#include "main.h"
#include "tca9544a.h"
#include <stdbool.h>

I2C_HandleTypeDef *i2c_bus[2];

#define LEFT_I2C_MUX_ADDR 0x70
#define RIGHT_I2C_MUX_ADDR 0x71
uint8_t i2c_mux_addr[2] = {0x70, 0x71};

#define I2C_MUX_INVALID 0xFF
uint8_t i2c_mux_selected[2] = {0xFF, 0xFF};

void _i2c_wait_for_ready(uint8_t bus) {
  // Wait for bus to become available again
  I2C_HandleTypeDef *hi2c = i2c_bus[bus];
  while (hi2c->State != HAL_I2C_STATE_READY) {
    // Do nothing...
  }
}

bool _i2c_mux_select(uint8_t bus, uint8_t channel) {
  if (i2c_mux_selected[bus] == channel)
    return true;
  else {
    bool res = tca9544a_select(bus, i2c_mux_addr[bus], channel);
    if (!res)
      return false;
    else
      i2c_mux_selected[bus] = channel;
    return true;
  }
}

void _i2c_wait_and_select_channel(uint8_t bus, uint8_t channel) {
  // Wait for bus to become available
  _i2c_wait_for_ready(bus);

  // Select channel
  // NOTE: Will return HAL_OK if already on channel
  if (channel != I2C_CHANNEL_DIRECT) {
    bool did_select = _i2c_mux_select(bus, channel);
    if (!did_select)
      Error_Handler();
  }
}

bool i2c_tx(uint8_t bus, uint8_t channel, uint8_t address, uint8_t *data,
            uint8_t len) {
  // Transmits using Interrupt/DMA and waits
  // until the bus becomes ready again.

  HAL_StatusTypeDef res;
  I2C_HandleTypeDef *hi2c = i2c_bus[bus];

  // Wait for ready and select channel
  _i2c_wait_and_select_channel(bus, channel);

  // Transmit via DMA
  res = HAL_I2C_Master_Transmit_DMA(hi2c, address << 1, data, len);
  if (res != HAL_OK)
    return false;

  // Wait for bus to become available again
  _i2c_wait_for_ready(bus);

  return true;
}

bool i2c_rx(uint8_t bus, uint8_t channel, uint8_t address, uint8_t *data,
            uint8_t len) {
  // Receives using Interrupt/DMA and waits
  // until the bus becomes ready again.

  HAL_StatusTypeDef res;
  I2C_HandleTypeDef *hi2c = i2c_bus[bus];

  // Wait for ready and select channel
  _i2c_wait_and_select_channel(bus, channel);

  // Transmit via DMA
  res = HAL_I2C_Master_Receive_DMA(hi2c, address << 1, data, len);
  if (res != HAL_OK)
    return false;

  // Wait for bus to become available again
  _i2c_wait_for_ready(bus);

  return true;
}
