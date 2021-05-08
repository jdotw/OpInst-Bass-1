/*
 * i2c.c
 *
 *  Created on: Apr 19, 2021
 *      Author: jwilson
 */

#include <stdbool.h>
#include "i2c.h"

I2C_HandleTypeDef *i2c_bus[2];

bool i2c_tx(uint8_t bus, uint8_t channel, uint8_t address, uint8_t *data, uint8_t len) {
	// Transmits using Interrupt/DMA and waits
	// until the bus becomes ready again.


}
