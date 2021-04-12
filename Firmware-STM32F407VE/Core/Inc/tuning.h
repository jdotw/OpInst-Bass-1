/*
 * tuning.h
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#ifndef INC_TUNING_H_
#define INC_TUNING_H_

#include "stm32f4xx_hal.h"

uint32_t tuning_timer_get_frequency(TIM_HandleTypeDef *htim, uint32_t channel);
void tuning_timer_input_capture_callback(TIM_HandleTypeDef *htim, uint32_t channel);

#endif /* INC_TUNING_H_ */
