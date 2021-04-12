/*
 * tuning_timer.c
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#include "main.h"

uint32_t IC_Value1 = 0;
uint32_t IC_Value2 = 0;
uint32_t Difference = 0;
uint32_t Frequency = 0;
uint8_t Is_First_Captured = 0;  // 0- not captured, 1- captured

#define OSC_SETTLING_DELAY 1000

uint32_t tuning_timer_get_frequency(TIM_HandleTypeDef *htim, uint32_t channel) {
	IC_Value1 = 0;
	IC_Value2 = 0;
	Difference = 0;
	Frequency = 0;
	Is_First_Captured = 0;
	HAL_TIM_IC_Start_IT(htim, channel);
	HAL_Delay(OSC_SETTLING_DELAY);
	HAL_TIM_IC_Stop_IT(htim, channel);
	return Frequency;
}

void tuning_timer_input_capture_callback(TIM_HandleTypeDef *htim, uint32_t channel)
{
	if (!Is_First_Captured) {
		IC_Value1 = HAL_TIM_ReadCapturedValue(htim, channel);  // capture the first value
		Is_First_Captured =1;  // set the first value captured as true
	} else {
		IC_Value2 = HAL_TIM_ReadCapturedValue(htim, channel);  // capture second value

		if (IC_Value2 > IC_Value1) {
			Difference = IC_Value2-IC_Value1;   // calculate the difference
		} else if (IC_Value2 < IC_Value1) {
			Difference = ((0xffff-IC_Value1)+IC_Value2) +1;
		} else {
			Error_Handler();
		}

		uint32_t pclk1 = HAL_RCC_GetPCLK1Freq();
		if (Difference > 0) {
			Frequency = pclk1/Difference;  // calculate frequency
		} else {
			Frequency = 0;
		}
		Is_First_Captured = 0;  // reset the first captured
	}
}

