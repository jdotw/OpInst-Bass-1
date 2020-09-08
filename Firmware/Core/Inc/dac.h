/*
 * dac.h
 *
 *  Created on: Feb 29, 2020
 *      Author: jwilson
 */

#ifndef INC_DAC_H_
#define INC_DAC_H_

#include "main.h"
#include "midi.h"

#define MIDI_NOTE_OUT_OF_RANGE 0
#define MIDI_NOTE_MINIMUM 24 // C1
#define MIDI_NOTE_MAXIMUM 72 // C5

#define SINGLE_DAC_1_ADDRESS 0x62
#define SINGLE_DAC_2_ADDRESS 0x63

#define QUAD_DAC_1_ADDRESS 0x60
#define QUAD_DAC_2_ADDRESS 0x61

#define DAC_VALUE_MAX 0xFFF
#define DAC_VALUE_MIN 0x0

/*
 * dac.c
 */

void set_single_dac1_i2c(I2C_HandleTypeDef *value);
void set_single_dac2_i2c(I2C_HandleTypeDef *value);
void set_quad_dac1_i2c(I2C_HandleTypeDef *value);
void set_quad_dac2_i2c(I2C_HandleTypeDef *value);

void set_osc1_tuning_timer(TIM_HandleTypeDef *value, uint32_t channel);
void set_osc2_tuning_timer(TIM_HandleTypeDef *value, uint32_t channel);

void dac_calibrate_all();
void update_dac_control_voltages();

void dac_tracking_test_mode();

void set_mcp4728_dac_value_channel(I2C_HandleTypeDef *i2c, uint8_t dacAddr, uint8_t channel, uint16_t value);

/*
 * dac_note.c
 */

uint16_t scaled_dac_value_for_midi_note(uint8_t note);
uint16_t dac_value_for_midi_state(midi_state midi, uint8_t transpose_steps, bool allow_pitch_bend);

/*
 * dac_velocity.c
 */
uint16_t dac_value_for_midi_velocity(uint8_t velocity);

/*
 * dac_tuning.c
 */


int16_t dac_tunning_offset(I2C_HandleTypeDef *dac_bus, uint8_t dac_addr, uint8_t dac_channel, TIM_HandleTypeDef *timer, uint32_t timer_channel);
void dac_timer_input_capture_callback(TIM_HandleTypeDef *htim, uint32_t channel);


#endif /* INC_DAC_H_ */
