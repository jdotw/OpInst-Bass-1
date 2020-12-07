/*
 * dac.c
 *
 *  Created on: Feb 29, 2020
 *      Author: jwilson
 */

#include "dac.h"
#include "main.h"
#include "stdbool.h"
#include "midi.h"

I2C_HandleTypeDef *single_dac1_i2c = NULL;
I2C_HandleTypeDef *single_dac2_i2c = NULL;
I2C_HandleTypeDef *quad_dac1_i2c = NULL;
I2C_HandleTypeDef *quad_dac2_i2c = NULL;

TIM_HandleTypeDef *osc1_tuning_timer = NULL;
uint32_t osc1_tuning_timer_channel = 0;
TIM_HandleTypeDef *osc2_tuning_timer = NULL;
uint32_t osc2_tuning_timer_channel = 0;

int16_t osc1_tuning_offset = 0;
int16_t osc2_tuning_offset = 0;

void set_single_dac1_i2c(I2C_HandleTypeDef *value) {
	single_dac1_i2c = value;
}
void set_single_dac2_i2c(I2C_HandleTypeDef *value) {
	single_dac2_i2c = value;
}
void set_quad_dac1_i2c(I2C_HandleTypeDef *value) {
	quad_dac1_i2c = value;
}
void set_quad_dac2_i2c(I2C_HandleTypeDef *value) {
	quad_dac2_i2c = value;
}

void set_osc1_tuning_timer(TIM_HandleTypeDef *value, uint32_t channel) {
	osc1_tuning_timer = value;
	osc1_tuning_timer_channel = channel;
}
void set_osc2_tuning_timer(TIM_HandleTypeDef *value, uint32_t channel) {
	osc2_tuning_timer = value;
	osc2_tuning_timer_channel = channel;
}

void set_mcp4725_dac_value(I2C_HandleTypeDef *i2c, uint8_t dacAddr, uint16_t value) {
	uint8_t buf[3];
	buf[0] = 0x40;
	buf[1] = value / 16;
	buf[2] = (value % 16) << 4;
	uint8_t shiftedDACAddr = dacAddr << 1;

	HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(i2c, shiftedDACAddr, buf, 3, 1000);
	if (ret == HAL_OK)
	{
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
	}
	else
	{
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
	}
}

void set_mcp4728_dac_value_channel(I2C_HandleTypeDef *i2c, uint8_t dacAddr, uint8_t channel, uint16_t value) {
	uint8_t buf[3];

	buf[0] = 0b01000000 + ((channel & 0x3) << 1);
	buf[1] = 0b10010000 + ((value & 0x0F00) >> 8);
	buf[2] = value & 0x00FF;

	uint8_t shiftedDACAddr = dacAddr << 1;

	HAL_StatusTypeDef ret = HAL_I2C_Master_Transmit(i2c, shiftedDACAddr, buf, 3, 1000);
	if (ret == HAL_OK)
	{
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 0);
	}
	else
	{
		HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, 1);
	}
}

void update_dac_control_voltages(midi_state midi) {
	uint16_t osc1_note_dac_value = dac_value_for_midi_state(midi, 0, true) + osc1_tuning_offset;
	set_mcp4728_dac_value_channel(quad_dac1_i2c, QUAD_DAC_1_ADDRESS, 0, osc1_note_dac_value);

	uint16_t osc2_note_dac_value = dac_value_for_midi_state(midi, 0, false) + osc2_tuning_offset;
	set_mcp4728_dac_value_channel(quad_dac1_i2c, QUAD_DAC_1_ADDRESS, 1, osc2_note_dac_value);

	uint16_t velocityDACValue = dac_value_for_midi_velocity(midi.cur_note_velocity);
	set_mcp4728_dac_value_channel(quad_dac1_i2c, QUAD_DAC_1_ADDRESS, 2, velocityDACValue);

	uint16_t filterDACValue = dac_value_for_primary_filter_cutoff(midi);
	set_mcp4728_dac_value_channel(quad_dac1_i2c, QUAD_DAC_1_ADDRESS, 3, filterDACValue);
}

void dac_calibrate_all() {
	bool audible_tuning = true;
  HAL_GPIO_WritePin(NOTE_GATE_GPIO_Port, NOTE_GATE_Pin, audible_tuning);

  uint16_t open_filter_dac_value = scaled_dac_value_for_primary_filter(127, 127);
	set_mcp4728_dac_value_channel(quad_dac1_i2c, QUAD_DAC_1_ADDRESS, 3, open_filter_dac_value);

	osc1_tuning_offset = dac_tunning_offset(quad_dac1_i2c, QUAD_DAC_1_ADDRESS, 0, osc1_tuning_timer, osc1_tuning_timer_channel);
	osc2_tuning_offset = dac_tunning_offset(quad_dac1_i2c, QUAD_DAC_1_ADDRESS, 1, osc2_tuning_timer, osc2_tuning_timer_channel);

  HAL_GPIO_WritePin(NOTE_GATE_GPIO_Port, NOTE_GATE_Pin, 0);
}

void dac_tracking_test_mode() {
	while(1) {
		uint16_t octave1_dac_value = scaled_dac_value_for_midi_note(36);

		uint16_t osc1_octace1_note_dac_value = octave1_dac_value + osc1_tuning_offset;
		set_mcp4728_dac_value_channel(quad_dac1_i2c, QUAD_DAC_1_ADDRESS, 0, osc1_octace1_note_dac_value);

		uint16_t osc2_octave1_note_dac_value = octave1_dac_value + osc2_tuning_offset;
		set_mcp4728_dac_value_channel(quad_dac1_i2c, QUAD_DAC_1_ADDRESS, 1, osc2_octave1_note_dac_value);

		HAL_Delay(4000);

		uint16_t octave2_dac_value = scaled_dac_value_for_midi_note(48);

		uint16_t osc1_octace2_note_dac_value = octave2_dac_value + osc1_tuning_offset;
		set_mcp4728_dac_value_channel(quad_dac1_i2c, QUAD_DAC_1_ADDRESS, 0, osc1_octace2_note_dac_value);

		uint16_t osc2_octave2_note_dac_value = octave2_dac_value + osc2_tuning_offset;
		set_mcp4728_dac_value_channel(quad_dac1_i2c, QUAD_DAC_1_ADDRESS, 1, osc2_octave2_note_dac_value);

		HAL_Delay(4000);
	}
}
