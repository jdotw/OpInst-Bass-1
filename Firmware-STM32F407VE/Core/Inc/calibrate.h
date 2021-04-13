/*
 * tuning.h
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#ifndef INC_CALIBRATE_H_
#define INC_CALIBRATE_H_

#include "stm32f4xx_hal.h"

// calibrate_osc.c
void calibrate_osc();
void calibrate_osc_timercallback(TIM_HandleTypeDef *htim, uint32_t channel);

#endif /* INC_CALIBRATE_H_ */
