/*
 * i2c_isr.c
 *
 *  Created on: Apr 19, 2021
 *      Author: jwilson
 */

#include <stdio.h>

#include "i2c.h"
#include "main.h"
#include "rotpic.h"
#include "seq.h"

#define I2C_MUX_INT_3_SET 0b10000000
#define I2C_MUX_INT_2_SET 0b01000000
#define I2C_MUX_INT_1_SET 0b00100000
#define I2C_MUX_INT_0_SET 0b00010000
#define I2C_MUX_INT_MASK 0b11110000
#define I2C_MUX_INT_ERROR 0b00001000 // Data sheet says this bit is unused

#define I2C_CHANNEL_DIRECT 0xFF // Skip setting the channel before TX/RX

uint8_t _i2c_mux_get_int_status(uint8_t bus) {
  uint8_t rx;
  bool res = i2c_rx(bus, I2C_CHANNEL_DIRECT, i2c_bus[bus].mux_addr, &rx, 1,
                    NULL, NULL);
  if (!res) {
    Error_Handler(); // FATAL -- We must be able to get our status!
  }

  return (rx & I2C_MUX_INT_MASK); // Only return the INT status bits
}

void i2c_isr(uint8_t bus) {
  // Called when any of the I2C interrupt lines go active-low
  // From here we work out which line it was, then query the
  // mux on that bus to determine which channel has interrupted
  uint8_t int_status = _i2c_mux_get_int_status(bus);
  if (int_status & I2C_MUX_INT_ERROR) {
    Error_Handler();
  }
  if (int_status & I2C_MUX_INT_0_SET) {
    // Channel 0 Interrupt
    rotpic_poll_all(bus, 0);
  }
  if (int_status & I2C_MUX_INT_1_SET) {
    // Channel 1 Interrupt
    rotpic_poll_all(bus, 1);
  }
  if (int_status & I2C_MUX_INT_2_SET) {
    // Channel 2 Interrupt
    rotpic_poll_all(bus, 2);
    seq_poll_gpio(bus, 2);
  }
  if (int_status & I2C_MUX_INT_3_SET) {
    // Channel 3 Interrupt
    rotpic_poll_all(bus, 3);
  }
}
