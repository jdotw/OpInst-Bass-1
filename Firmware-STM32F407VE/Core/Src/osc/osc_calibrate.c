/*
 * tuning.c
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#include <osc.h>
#include <stdio.h>
#include <stdbool.h>

#include "main.h"
#include "dac7678.h"

#define MIDI_NOTE_A2 45
#define MIDI_NOTE_A3 57
#define MIDI_NOTE_A4 69
#define MIDI_NOTE_A5 81
#define MIDI_NOTE_C2 36
#define MIDI_NOTE_C3 48

/*
 * Frequency Counter
 */

uint32_t IC_Value1 = 0;
uint32_t IC_Value2 = 0;
uint32_t Difference = 0;
uint32_t Frequency = 0;
uint8_t Is_First_Captured = 0;  // 0- not captured, 1- captured

#define OSC_SETTLING_DELAY 1000

uint32_t _get_frequency(TIM_HandleTypeDef *htim, uint32_t channel) {
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

void osc_calibrate_timercallback(TIM_HandleTypeDef *htim, uint32_t channel)
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

/*
 * CV Calibration
 */

#define TUNING_ERROR_MARGIN 0

// Add the find-freq here
uint32_t _dac_value_for_freq(I2C_HandleTypeDef *dac_bus, uint8_t dac_addr, uint8_t dac_channel, TIM_HandleTypeDef *timer, uint32_t timer_channel, uint32_t expected_frequency, uint16_t near_dac_value, uint16_t min_dac_value, uint16_t max_dac_value) {

	// Set the DAC value
	dac7678_set_value(dac_bus, dac_addr, dac_channel, near_dac_value);

	// Listen to the frequency produced
	uint32_t observed_frequency = _get_frequency(timer, timer_channel);
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
		return _dac_value_for_freq(dac_bus, dac_addr, dac_channel, timer, timer_channel, expected_frequency, near_dac_value, min_dac_value, max_dac_value);
	}
}


/*
 * Scale Calibration
 */

#define DPOT_VALUE_MIN 0
#define DPOT_VALUE_MAX 255

void _set_dpot_value(SPI_HandleTypeDef *dpot_bus, GPIO_TypeDef* dpot_cs_port, uint16_t dpot_cs_pin, uint8_t value) {
	// Drop CS line
	HAL_GPIO_WritePin(dpot_cs_port, dpot_cs_pin, GPIO_PIN_RESET);

	// dPOT response is inverted
	// The lower the dpot value, the higher the freq
	uint8_t scaled_value = DPOT_VALUE_MAX - value;

	// Write the new value
	HAL_StatusTypeDef result = HAL_SPI_Transmit(dpot_bus, &scaled_value, 1, HAL_MAX_DELAY);
	if (result != HAL_OK) {
		Error_Handler();
	}

	// Raise CS Line
	HAL_GPIO_WritePin(dpot_cs_port, dpot_cs_pin, GPIO_PIN_SET);
}

#define MIDI_NOTE_OUT_OF_RANGE 0
#define MIDI_NOTE_MINIMUM 24 // C1
#define MIDI_NOTE_MAXIMUM 72 // C5

uint16_t _scaled_dac_value_for_midi_note(uint8_t note)
{
	// This instruments range is from C1= (1v) up to C4 (4v)
	if (note < MIDI_NOTE_MINIMUM || note > MIDI_NOTE_MAXIMUM)
	{
		return MIDI_NOTE_OUT_OF_RANGE;
	}

	// 5v vref version
	// Scale up to 1v/oct across a 5v/4096bit range
	// Each midi note is 1v / 12steps = 83.3333mv per step
	// Scaling 5000mv across 12bits (4096) means each bit is 0.8192mv
	// Therefore scaled dac value is steps * (83.3333mv per step * 0.8192mv per bit scale)
	uint16_t dacValue = (note - MIDI_NOTE_MINIMUM) * 68.2663936;

	return dacValue;
}

#define DAC_VALUE_MAX 0xFFF
#define DAC_VALUE_MIN 0x0

typedef struct {
	bool success;
	uint8_t error;
	uint8_t dpot_val;
	int32_t dac_offset;
} _calibration_result;

#define CAL_ERROR_DPOT_MAX 1
#define CAL_ERROR_DPOT_MIN 2
#define CAL_ERROR_DAC_MAX 3
#define CAL_ERROR_DAC_MIN 4

_calibration_result _dpot_value_for_tracking(I2C_HandleTypeDef *dac_bus, uint8_t dac_addr, uint8_t dac_channel, SPI_HandleTypeDef *dpot_bus, GPIO_TypeDef* dpot_cs_port, uint16_t dpot_cs_pin, TIM_HandleTypeDef *timer, uint8_t timer_channel, uint8_t min_val, uint8_t max_val, uint8_t cur_val) {
	_calibration_result result;

	// Set dPOT to the cur_val
	_set_dpot_value(dpot_bus, dpot_cs_port, dpot_cs_pin, cur_val);

	// Set DAC value to achieve desired frequency
	uint16_t dac_value = _scaled_dac_value_for_midi_note(MIDI_NOTE_A2);
	uint32_t expected_frequency = 11000;

	// Attempt to tune oscillator
	uint16_t tuned_dac_value = _dac_value_for_freq(dac_bus, dac_addr, dac_channel, timer, timer_channel, expected_frequency, dac_value, DAC_VALUE_MIN, DAC_VALUE_MAX);
	if (tuned_dac_value == DAC_VALUE_MIN) {
		// Failed, minimum DAC value hit
		result.error = CAL_ERROR_DAC_MIN;
		return result;
	} else if (tuned_dac_value == DAC_VALUE_MAX) {
		// Failed, maximum DAC value hit
		result.error = CAL_ERROR_DAC_MAX;
		return result;
	}
	int32_t dac_tuning_offset = tuned_dac_value - dac_value;
	result.dac_offset = dac_tuning_offset;

	// Set DAC value to +1 octave
	dac_value = _scaled_dac_value_for_midi_note(MIDI_NOTE_A3);
	dac_value += dac_tuning_offset;
	expected_frequency = 22000;
	dac7678_set_value(dac_bus, dac_addr, dac_channel, dac_value);

	// Listen to achieved frequency
	uint32_t actual_freq =  _get_frequency(timer, timer_channel);
	if (actual_freq > expected_frequency) {
		// Too high, need to try a lower value
		uint8_t new_val = (cur_val - 1) - ((cur_val - min_val) / 2);
		if (new_val == min_val) {
			// Exhausted search space
			if (new_val == DPOT_VALUE_MIN) {
				// We're already at the dpot's min value, error out
				result.error = CAL_ERROR_DPOT_MIN;
			} else {
				// Search exhausted, but within range
				result.success = true;
				result.dpot_val = cur_val;
			}
			return result;
		} else {
			max_val = cur_val;
			return _dpot_value_for_tracking(dac_bus, dac_addr, dac_channel, dpot_bus, dpot_cs_port, dpot_cs_pin, timer, timer_channel, min_val, max_val, new_val);
		}
	} else if (actual_freq < expected_frequency) {
		// Too low
		uint8_t new_val = (cur_val + 1) + ((max_val - cur_val) / 2);
		if (new_val == max_val) {
			// Exhausted search space
			if (new_val == DPOT_VALUE_MAX) {
				// We're already at the dpot's max value, error out
				result.error = CAL_ERROR_DPOT_MAX;
			} else {
				result.success = true;
				result.dpot_val = cur_val;
			}
			return result;
		} else {
			min_val = cur_val;
			return _dpot_value_for_tracking(dac_bus, dac_addr, dac_channel, dpot_bus, dpot_cs_port, dpot_cs_pin, timer, timer_channel, min_val, max_val, new_val);
		}
	} else {
		// Nailed it, we're in tune!
		result.success = true;
		result.dpot_val = cur_val;
		return result;
	}
}

void _init_dpot() {
	HAL_GPIO_WritePin(OSC1SCALEDPOTCS_GPIO_Port, OSC1SCALEDPOTCS_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(OSC2SCALEDPOTCS_GPIO_Port, OSC2SCALEDPOTCS_Pin, GPIO_PIN_SET);
}

void _osc_calibrate_voice(I2C_HandleTypeDef *dac_bus, uint8_t dac_addr, uint8_t dac_channel, SPI_HandleTypeDef *dpot_bus, GPIO_TypeDef* dpot_cs_port, uint16_t dpot_cs_pin, TIM_HandleTypeDef *timer, uint8_t timer_channel) {
	// Trying to determine the correct dPOT value
	// for the scaling between 1v variance in CV
	// to produce exactly double the frequency.
	//
	// E.g. CV 3v = 440Hz
	//      CV 4v = 880Hz

	_init_dpot();

  uint32_t freq = _get_frequency(timer, timer_channel);
  printf("Osc 1 Starting Frequency: %lu\n", freq);

	_calibration_result result = _dpot_value_for_tracking(dac_bus, dac_addr, dac_channel, dpot_bus, dpot_cs_port, dpot_cs_pin, timer, timer_channel, DPOT_VALUE_MIN, DPOT_VALUE_MAX, (DPOT_VALUE_MAX - DPOT_VALUE_MIN)/2);
  printf("Osc 1 Final dpot value: %u\n", result.dpot_val);

  uint16_t dac_value = _scaled_dac_value_for_midi_note(MIDI_NOTE_A2);
  dac_value += result.dac_offset;
	dac7678_set_value(dac_bus, dac_addr, dac_channel, dac_value);
  freq = _get_frequency(timer, timer_channel);
  printf("Osc 1 A2 Frequency: %lu\n", freq);

  dac_value = _scaled_dac_value_for_midi_note(MIDI_NOTE_A3);
  dac_value += result.dac_offset;
	dac7678_set_value(dac_bus, dac_addr, dac_channel, dac_value);
  freq = _get_frequency(timer, timer_channel);
  printf("Osc 1 A3 Frequency: %lu\n", freq);

  dac_value = _scaled_dac_value_for_midi_note(MIDI_NOTE_A4);
  dac_value += result.dac_offset;
	dac7678_set_value(dac_bus, dac_addr, dac_channel, dac_value);
  freq = _get_frequency(timer, timer_channel);
  printf("Osc 1 A4 Frequency: %lu\n", freq);


}


void osc_calibrate(I2C_HandleTypeDef *dac_bus, SPI_HandleTypeDef *dpot_bus, TIM_HandleTypeDef *timer) {
	_osc_calibrate_voice(dac_bus, 0x48, 2, dpot_bus, OSC1SCALEDPOTCS_GPIO_Port, OSC1SCALEDPOTCS_Pin, timer, TIM_CHANNEL_1);
	_osc_calibrate_voice(dac_bus, 0x4a, 4, dpot_bus, OSC2SCALEDPOTCS_GPIO_Port, OSC2SCALEDPOTCS_Pin, timer, TIM_CHANNEL_2);
}
