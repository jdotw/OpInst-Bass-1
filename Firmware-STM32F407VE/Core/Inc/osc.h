/*
 * tuning.h
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#ifndef INC_OSC_H_
#define INC_OSC_H_

#include "stm32f4xx_hal.h"

// osc_calibrate.c
void osc_calibrate(uint8_t dac_bus, SPI_HandleTypeDef *dpot_bus, TIM_HandleTypeDef *timer);
void osc_calibrate_timercallback(TIM_HandleTypeDef *htim, uint32_t channel);

// osc_ctrl.c

#endif /* INC_OSC_H_ */
