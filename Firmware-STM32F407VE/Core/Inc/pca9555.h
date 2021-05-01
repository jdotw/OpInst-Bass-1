/*
 * pca9555.h
 *
 *  Created on: 16 Apr 2021
 *      Author: jwilson
 */

#ifndef INC_PCA9555_H_
#define INC_PCA9555_H_

#include "stm32f4xx_hal.h"

#define DEFAULT_PCA9555_ADDRESS 0x20

void pca9555_init(void);
HAL_StatusTypeDef pca9555_set_port_output(uint8_t bus, uint8_t addr, uint8_t outputs[2]);

#endif /* INC_PCA9555_H_ */
