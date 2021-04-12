/*
 * timer_callback.c
 *
 *  Created on: 18 Aug 2020
 *      Author: jwilson
 */

#include "main.h"
#include "dac.h"

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{
		// DAC Tuning channel (oscillator 1)
		dac_timer_input_capture_callback(htim, TIM_CHANNEL_3);
	}
	else if (htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		// DAC Tuning channel (oscillator 2)
		dac_timer_input_capture_callback(htim, TIM_CHANNEL_1);
	}
}

