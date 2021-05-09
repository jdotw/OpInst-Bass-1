/*
 * dac7678.h
 *
 *  Created on: Apr 13, 2021
 *      Author: jwilson
 */

#ifndef INC_DAC7678_H_
#define INC_DAC7678_H_

#include <stdbool.h>
#include "stm32f4xx_hal.h"

bool dac7678_reset(uint8_t bus, uint8_t channel, uint8_t dac);
bool dac7678_set_value(uint8_t bus, uint8_t channel, uint8_t dac, uint8_t dac_channel, uint16_t val);
bool dac7678_set_value_array(uint8_t bus, uint8_t channel, uint8_t dac, uint16_t val[7]);
void dac7678_init(void);

#endif /* INC_DAC7678_H_ */
