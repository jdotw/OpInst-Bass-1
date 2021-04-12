/*
 * tuning.c
 *
 *  Created on: 18 Aug 2020
 *      Author: jwilson
 */

#include "main.h"
#include "dac.h"

uint32_t IC_Value1 = 0;
uint32_t IC_Value2 = 0;
uint32_t Difference = 0;
uint32_t Frequency = 0;
uint8_t Is_First_Captured = 0;  // 0- not captured, 1- captured

void dac_timer_input_capture_callback(TIM_HandleTypeDef *htim, uint32_t channel)
{
	if (Is_First_Captured==0)  // is the first value captured ?
	{
		IC_Value1 = HAL_TIM_ReadCapturedValue(htim, channel);  // capture the first value
		Is_First_Captured =1;  // set the first value captured as true
	}

	else if (Is_First_Captured)  // if the first is captured
	{
		IC_Value2 = HAL_TIM_ReadCapturedValue(htim, channel);  // capture second value

		if (IC_Value2 > IC_Value1)
		{
			Difference = IC_Value2-IC_Value1;   // calculate the difference
		}

		else if (IC_Value2 < IC_Value1)
		{
			Difference = ((0xffff-IC_Value1)+IC_Value2) +1;
		}

		else
		{
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

uint32_t frequency_for_dac_value(I2C_HandleTypeDef *dac_bus, uint8_t dac_addr, uint8_t dac_channel, TIM_HandleTypeDef *timer, uint32_t timer_channel, uint16_t dac_value) {
	// Set the DAC to the suggested value
	set_mcp4728_dac_value_channel(dac_bus, dac_addr, dac_channel, dac_value);
	HAL_Delay(1000); // Wait for osc to settle

	// Listen to the oscillator
	IC_Value1 = 0;
	IC_Value2 = 0;
	Difference = 0;
	Frequency = 0;
	Is_First_Captured = 0;
	HAL_TIM_IC_Start_IT(timer, timer_channel);
	HAL_Delay(DAC_SETTLING_DELAY);
	HAL_TIM_IC_Stop_IT(timer, timer_channel);

	return Frequency * 10;
}

#define MIDI_NOTE_A4 69
#define MIDI_NOTE_C2 36
#define MIDI_NOTE_C3 48

#define TUNING_ERROR_MARGIN 0 // == 1Hz
#define TUNING_DAC_VALUE_STEP 100

#define DAC_SETTLING_DELAY 50
#define TIMER_SAMPLING_PERIOD 10

uint32_t _dac_value_to_tune(I2C_HandleTypeDef *dac_bus, uint8_t dac_addr, uint8_t dac_channel, TIM_HandleTypeDef *timer, uint32_t timer_channel, uint32_t expected_frequency, uint16_t near_dac_value, uint16_t min_dac_value, uint16_t max_dac_value) {

	// Check the result against expected
	uint32_t observed_frequency = frequency_for_dac_value(dac_bus, dac_addr, dac_channel, timer, timer_channel, near_dac_value);
	if (observed_frequency > (expected_frequency - TUNING_ERROR_MARGIN)
			&& observed_frequency < (expected_frequency + TUNING_ERROR_MARGIN)) {
		// Got us a tuned oscillator!
		return near_dac_value;
	} else {
		if (observed_frequency > expected_frequency) {
			// The observed frequency is higher than expected
			// We need to try a LOWER dac value
			if (near_dac_value <= (min_dac_value + 1)) {
				// We can't go any lower, return the min_dac_value
				// We compare to min_dac_value + 1 to avoid rounding error
				return min_dac_value;
			} else {
				// We now know our maximum (the dac value we just used)
				max_dac_value = near_dac_value;

				// Our best guess is that the dac value is
				// half way between what we just used the and minimum
				near_dac_value -= (near_dac_value - min_dac_value) / 2;
			}
		} else if (observed_frequency < expected_frequency) {
			// The observed frequency is lower than expected
			// We need to try a HIGHER dac value
			if (near_dac_value >= (max_dac_value-1)) {
				// We can't go any higher, return the max_dac_value
				// We compare to max_dac_value-1 to avoid rounding error
				return max_dac_value;
			} else {
				// We now know our minimum (the value we just tried)
				min_dac_value = near_dac_value;

				// Our best guess is that the dac value is
				// half way between what we just used and the maximum
				near_dac_value += (max_dac_value - near_dac_value) / 2;
			}
		}

		// Recursively try the new dac against the new minimum or maximum
		return _dac_value_to_tune(dac_bus, dac_addr, dac_channel, timer, timer_channel, expected_frequency, near_dac_value, min_dac_value, max_dac_value);
	}
}

int16_t dac_tunning_offset(I2C_HandleTypeDef *dac_bus, uint8_t dac_addr, uint8_t dac_channel, TIM_HandleTypeDef *timer, uint32_t timer_channel) {
	// Uses the DAC at the specified address to determine the
	// offset voltage to use to tune the oscillator using
	// the specified timer as the frequency counter
	//
	// Does not return until the tuning sequence is done

	// Determine the presumed dac_value for A4 / 440Hz
	uint16_t initial_dac_value = scaled_dac_value_for_midi_note(MIDI_NOTE_C2);
	uint32_t expected_frequency = 67406;

	// Attempt to tune oscillator 1
	uint16_t tuned_dac_value = _dac_value_to_tune(dac_bus, dac_addr, dac_channel, timer, timer_channel, expected_frequency, initial_dac_value, DAC_VALUE_MIN, DAC_VALUE_MAX);
	if (tuned_dac_value == DAC_VALUE_MIN) {
		// Failed, minimum DAC value hit
		return DAC_VALUE_MIN;
	} else if (tuned_dac_value == DAC_VALUE_MAX) {
		// Failed, maximum DAC value hit
		return DAC_VALUE_MAX;
	} else {
		// Success!
		int16_t tuning_offset = tuned_dac_value - initial_dac_value;
		return tuning_offset;
	}

}
