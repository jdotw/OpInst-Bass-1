/*
 * blink.h
 *
 *  Created on: Jul 11, 2021
 *      Author: jwilson
 */

#ifndef INC_BLINK_H_
#define INC_BLINK_H_

#include <stdbool.h>
#include "stm32f4xx_hal.h"

extern bool blink;
extern bool blink_changed;

void blink_systick();
void blink_reset(void);

#endif /* INC_BLINK_H_ */
