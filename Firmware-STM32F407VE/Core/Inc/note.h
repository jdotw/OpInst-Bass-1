/*
 * note.h
 *
 *  Created on: 1 May 2021
 *      Author: jwilson
 */

#ifndef INC_NOTE_H_
#define INC_NOTE_H_

#include <stdbool.h>
#include "stm32f4xx_hal.h"

typedef struct {
	uint8_t note_number;
	bool note_on;
} note_value_t;

typedef struct {
	bool ping_trigger;
	uint32_t triggered_at;
} note_trig_t;

extern note_value_t note_value;
extern note_trig_t note_trig;

void note_set_on(uint8_t note_number);
void note_set_off(void);

#endif /* INC_NOTE_H_ */
