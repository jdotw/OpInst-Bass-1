/*
 * ctrl.h
 *
 *  Created on: 20 Apr 2021
 *      Author: jwilson
 */

#ifndef INC_CTRL_H_
#define INC_CTRL_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum {
	CTRL_OSC1_SAW,
	CTRL_OSC1_SQU,
	CTRL_OSC1_TO_OSC2,
} ctrl_enum_t;

void ctrl_apply_delta(ctrl_enum_t ctrl, int8_t delta);
void ctrl_apply_toggle(ctrl_enum_t ctrl, bool changed, bool state);

#endif /* INC_CTRL_H_ */
