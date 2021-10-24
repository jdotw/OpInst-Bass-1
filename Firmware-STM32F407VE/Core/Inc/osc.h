/*
 * tuning.h
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#ifndef INC_OSC_H_
#define INC_OSC_H_

#include "stm32f4xx_hal.h"

// osc.c
typedef enum { OSC1 = 0, OSC2 } osc_t;
extern uint16_t osc_dac_offset[2];
uint16_t osc_dac_value_for_note(osc_t osc, uint8_t note_number);

// osc_calibrate.c
void osc_calibrate(SPI_HandleTypeDef *dpot_bus, TIM_HandleTypeDef *timer);
void osc_calibrate_timercallback(TIM_HandleTypeDef *htim, uint32_t channel);

// osc_ctrl.c

#endif /* INC_OSC_H_ */
