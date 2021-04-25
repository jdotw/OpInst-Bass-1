/*
 * rotpic.h
 *
 *  Created on: 17 Apr 2021
 *      Author: jwilson
 */

#ifndef INC_ROTPIC_H_
#define INC_ROTPIC_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

typedef struct {
	bool success;
	int8_t enc1_delta;
	int8_t enc2_delta;
	int8_t enc3_delta;
	int8_t enc4_delta;
	bool sw1_state;
	bool sw1_changed;
	bool sw2_state;
	bool sw2_changed;
} rotpic_state;

void rotpic_poll_all(uint8_t bus, uint8_t mux, uint8_t channel, bool ignore_state);


#endif /* INC_ROTPIC_H_ */
