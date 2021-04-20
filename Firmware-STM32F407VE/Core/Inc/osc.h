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
void osc_calibrate();
void osc_calibrate_timercallback(TIM_HandleTypeDef *htim, uint32_t channel);

// osc_ctrl.c
#define OSC1_SQU_FUNC_LEVEL 0
#define OSC1_SQU_FUNC_PWM 1
#define OSC1_SQU_FUNC_MAX 1
typedef struct {
	uint16_t saw_level;

	uint16_t squ_level;
	uint16_t squ_pwm;
	uint8_t squ_func;

	uint16_t osc1_to_osc2_filt_mix;

} osc_ctrl_state_t;
extern osc_ctrl_state_t osc_ctrl_state;
void osc_ctrl_toggle_squ_func();
void osc_ctrl_inc_saw_level(int8_t delta);
void osc_ctrl_inc_squ_level(int8_t delta);
void osc_ctrl_inc_squ_pwm(int8_t delta);

#endif /* INC_OSC_H_ */
