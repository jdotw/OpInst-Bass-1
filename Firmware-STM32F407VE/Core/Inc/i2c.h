/*
 * i2c.h
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include <stdbool.h>
#include "stm32f4xx_hal.h"

#define I2C_LEFT 0
#define I2C_RIGHT 1

#define I2C_CHANNEL_DIRECT 0xFF // Skip setting the channel before TX/RX

/* i2c.c */
extern I2C_HandleTypeDef *i2c_bus[2];
bool i2c_tx(uint8_t bus, uint8_t channel, uint8_t address, uint8_t *data, uint8_t len);

/* i2c_mux.c */
HAL_StatusTypeDef i2c_mux_select(uint8_t bus, uint8_t channel);
#define I2C_MUX_INT_3_SET 0b10000000
#define I2C_MUX_INT_2_SET 0b01000000
#define I2C_MUX_INT_1_SET 0b00100000
#define I2C_MUX_INT_0_SET 0b00010000
#define I2C_MUX_INT_MASK  0b11110000
#define I2C_MUX_INT_ERROR 0b00001000	// Data sheet says this bit is unused
uint8_t i2c_mux_get_int_status(uint8_t bus);

/* i2c_scan.c */
void i2c_scan_bus(uint8_t bus, uint8_t channel);

/* i2c_isr.c */
#define I2C_LEFT_BUS_ID 0
#define I2C_RIGHT_BUS_ID 1
void i2c_isr(uint8_t bus);

#endif /* INC_I2C_H_ */
