/*
 * tca544a.c
 *
 *  Created on: Apr 13, 2021
 *      Author: jwilson
 */

#include "i2c.h"
#include "main.h"
#include <tca9544a.h>

bool tca9544a_select(uint8_t bus, uint8_t mux_addr, uint8_t channel) {
  uint8_t data = (channel & 0b11) | 0b100;
  bool res = i2c_tx(bus, I2C_CHANNEL_DIRECT, mux_addr, &data, 1);
  return res;
}
