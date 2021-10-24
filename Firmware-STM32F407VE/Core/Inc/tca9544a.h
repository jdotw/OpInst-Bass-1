/*
 * tca544a.h
 *
 *  Created on: Apr 13, 2021
 *      Author: jwilson
 */

#ifndef INC_TCA9544A_H_
#define INC_TCA9544A_H_

#include <stdbool.h>
#include <stdint.h>

bool tca9544a_select(uint8_t bus, uint8_t mux_addr, uint8_t channel);

#endif /* INC_TCA9544A_H_ */
