/*
 * i2c.h
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

#define I2C_LEFT 0
#define I2C_RIGHT 1

#define I2C_CHANNEL_DIRECT 0xFF // Skip setting the channel before TX/RX

/* i2c.c */
extern I2C_HandleTypeDef *i2c_bus[2];
bool i2c_tx(uint8_t bus, uint8_t channel, uint8_t address, uint8_t *data,
            uint8_t len);
bool i2c_rx(uint8_t bus, uint8_t channel, uint8_t address, uint8_t *data,
            uint8_t len);

/* i2c_scan.c */
void i2c_scan_bus(uint8_t bus, uint8_t channel);

/* i2c_isr.c */
void i2c_isr(uint8_t bus);

#endif /* INC_I2C_H_ */
