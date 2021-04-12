/*
 * dpot.c
 *
 *  Created on: 20 Aug 2020
 *      Author: jwilson
 */

#include "main.h"
#include "dpot.h"

#define MIDI_NOTE_C2 36
#define MIDI_NOTE_C3 48

#define SCALE_ERROR_MARGIN 0 // == 1Hz

#define DAC_SETTLING_DELAY 50
#define TIMER_SAMPLING_PERIOD 10

void _set_dpot_value(I2C_HandleTypeDef *dpot_bus, uint8_t dpot_addr, uint8_t value) {
	// Set the value
	uint8_t buf[2];
	buf[0] = 0b00000000; 	// Control byte: Write to wiper 1
	buf[1] = value; 		// Data byte: Wiper value
	HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(dpot_bus, dpot_addr, buf, 2, 1000);
	if (ret == HAL_OK)
	{
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
	}
	else
	{
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
	}
}


uint32_t _dpot_value_to_scale(I2C_HandleTypeDef *dpot_bus, uint8_t dpot_addr, I2C_HandleTypeDef *dac_bus, uint8_t dac_addr, uint8_t dac_channel, TIM_HandleTypeDef *timer, uint32_t timer_channel, uint8_t near_dpot_value, uint8_t min_dpot_value, uint8_t max_dpot_value) {

	// Set the dpot to the suggested value
	_set_dpot_value(dpot_bus, dpot_addr, near_dpot_value);
	HAL_Delay(1000); // Wait for dpot to settle

	uint16_t note1_dac_value = scaled_dac_value_for_midi_note(MIDI_NOTE_C2);
	uint16_t note2_dac_value = scaled_dac_value_for_midi_note(MIDI_NOTE_C3);

	// Measure the frequency of the two notes
	uint32_t reference_frequency = frequency_for_dac_value(dac_bus, dac_addr, dac_channel, timer, timer_channel, note1_dac_value);
	uint32_t observed_frequency = frequency_for_dac_value(dac_bus, dac_addr, dac_channel, timer, timer_channel, note2_dac_value);
	uint32_t expected_frequency = reference_frequency * 2;

	// Check the result against expected
	if (observed_frequency > (expected_frequency - SCALE_ERROR_MARGIN)
			&& observed_frequency < (expected_frequency + SCALE_ERROR_MARGIN)) {
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
		return _dpot_value_to_scale(dpot_bus, dpot_addr, dac_bus, dac_addr, dac_channel, timer, timer_channel, near_dpot_value, min_dpot_value, max_dpot_value)
	}


}

void dpot_calibrate_tracking(I2C_HandleTypeDef *dpot_bus, uint8_t dpot_addr, I2C_HandleTypeDef *dac_bus, uint8_t dac_addr, uint8_t dac_channel, TIM_HandleTypeDef *timer, uint32_t timer_channel) {
	uint8_t dpot_addr = 0x2e;
	uint8_t shifted_dpot_addr = dpot_addr << 1;

	// Recursively find the dpot value to scale between these two notes
	uint8_t scale_dpot_value = _dpot_value_to_scale(dpot_bus, dpot_addr, dac_bus, dac_addr, dac_channel, timer, timer_channel, 64, 0, 127);
	if (scale_dpot_value == DPOT_VALUE_MIN) {
		// Failed, minimum dpot value hit
		return DPOT_VALUE_MIN;
	} else if (tuned_dac_value == DPOT_VALUE_MAX) {
		// Failed, maximum dpot value hit
		return DPOT_VALUE_MAX;
	} else {
		// Success!
		return scale_dpot_value;
	}
}
