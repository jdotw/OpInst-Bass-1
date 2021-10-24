/*
 * pca9555.h
 *
 *  Created on: 16 Apr 2021
 *      Author: jwilson
 */

#ifndef INC_PCA9555_H_
#define INC_PCA9555_H_

#include "main.h"
#include <stdbool.h>

bool pca9555_set_port_output(uint8_t bus, uint8_t channel, uint8_t unit,
                             uint8_t outputs[2]);
bool pca9555_read_pin_state(uint8_t bus, uint8_t channel, uint8_t unit,
                            uint16_t *stateptr);
void pca9555_init(void);

#endif /* INC_PCA9555_H_ */
