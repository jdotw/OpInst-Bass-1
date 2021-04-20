/*
 * ctrl.h
 *
 *  Created on: 20 Apr 2021
 *      Author: jwilson
 */

#ifndef INC_CTRL_H_
#define INC_CTRL_H_

#include <stdint.h>

#define CTRL_DEFAULT_SCALE 100
#define CTRL_DEFAULT_MIN 0
#define CTRL_12BIT_MAX 0xFFF

void ctrl_apply_delta(uint16_t *ctrl, int8_t enc_delta, int16_t scale_percent, uint16_t min_value, uint16_t max_value);


#endif /* INC_CTRL_H_ */
