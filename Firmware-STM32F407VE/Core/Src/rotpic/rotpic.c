/*
 * rotpic.c
 *
 *  Created on: 17 Apr 2021
 *      Author: jwilson
 */

#include <stdio.h>
#include <stdbool.h>

#include "main.h"
#include "rotpic.h"
#include "tca9544a.h"
#include "i2c.h"
#include "osc.h"
#include "ctrl.h"

#define DEFAULT_ROTPIC_ADDR 0x50

#define ROTPIC_SW1_STATE_MASK   0b00000001
#define ROTPIC_SW1_CHANGED_MASK 0b00000010
#define ROTPIC_SW2_STATE_MASK   0b00000100
#define ROTPIC_SW2_CHANGED_MASK 0b00001000

rotpic_state _rotpic_poll_selected(uint8_t bus, uint8_t rotpic_addr) {
	rotpic_state state;

	uint8_t rx[5];
	HAL_StatusTypeDef res = HAL_I2C_Master_Receive(i2c_bus[bus], rotpic_addr << 1, rx, 5, HAL_MAX_DELAY);
	if (res != HAL_OK) {
		state.success = false;
		return state;
	}

	state.success = true;
	state.enc1_delta = rx[0];
	state.enc2_delta = rx[1];
	state.enc3_delta = rx[2];
	state.enc4_delta = rx[3];
	state.sw1_state = rx[4] & ROTPIC_SW1_STATE_MASK;
	state.sw1_changed = rx[4] & ROTPIC_SW1_CHANGED_MASK;
	state.sw2_state = rx[4] & ROTPIC_SW2_STATE_MASK;
	state.sw2_changed = rx[4] & ROTPIC_SW2_CHANGED_MASK;

	return state;
}

void _rotpic_handle_state(uint8_t bus, uint8_t mux, uint8_t channel, uint8_t pic, rotpic_state state) {
	// Depending on which PIC this is, update actual state values
	if (state.enc1_delta != 0 || state.enc2_delta != 0 || state.enc3_delta != 0 || state.enc4_delta != 0) {
		printf("ENC CHANGE!");
	}
	if (state.sw1_changed || state.sw2_changed) {
		printf("SW CHANGE!");
	}
	if (bus == 0) {
		// Left I2C
		switch (channel) {
		case 0:
			// Left I2C 0
			switch(pic) {
			case 0:
				// Left 0:000
				ctrl_apply_delta(CTRL_OSC1_SAW, state.enc1_delta);
				ctrl_apply_delta(CTRL_OSC1_SQU, state.enc2_delta);
				ctrl_apply_delta(CTRL_OSC1_TO_OSC2, state.enc4_delta);
				ctrl_apply_toggle(CTRL_OSC1_SQU, state.sw2_changed, state.sw2_state);
				break;
			case 1:
				// Left 0:001
				ctrl_apply_delta(CTRL_OSC1_DRIVE, state.enc1_delta);
				ctrl_apply_delta(CTRL_OSC1_FILT_RES, state.enc2_delta);
				ctrl_apply_delta(CTRL_OSC1_TUNE, state.enc3_delta);
				ctrl_apply_delta(CTRL_OSC1_FILT_CUTOFF, state.enc4_delta);
				ctrl_apply_toggle(CTRL_OSC1_TUNE, state.sw1_changed, state.sw1_state);
				break;
			case 2:
				// Left 0:010
				ctrl_apply_delta(CTRL_OSC_FILT_ENV_A, state.enc1_delta);
				ctrl_apply_delta(CTRL_OSC_FILT_ENV_S, state.enc2_delta);
				ctrl_apply_delta(CTRL_OSC_FILT_ENV_D, state.enc3_delta);
				ctrl_apply_delta(CTRL_OSC_FILT_ENV_R, state.enc4_delta);
				ctrl_apply_toggle(CTRL_OSC_FILT_ENV_A, state.sw1_changed, state.sw1_state);
				ctrl_apply_toggle(CTRL_OSC_FILT_ENV_S, state.sw2_changed, state.sw2_state);
				break;
			case 3:
				// Left 0:011
				ctrl_apply_delta(CTRL_OSC2_SAW, state.enc1_delta);
				ctrl_apply_delta(CTRL_OSC2_NOISE, state.enc2_delta);
				ctrl_apply_delta(CTRL_OSC2_SQU, state.enc3_delta);
				ctrl_apply_toggle(CTRL_OSC2_SQU, state.sw1_changed, state.sw1_state);
				break;
			case 4:
				// Left 0:100
				ctrl_apply_delta(CTRL_SUB_NOISE, state.enc1_delta);
				ctrl_apply_delta(CTRL_SUB_TO_OSC2, state.enc2_delta);
				ctrl_apply_delta(CTRL_SUB, state.enc3_delta);
				break;
			}
			break;
		case 2:
			// Left I2C 2
			switch(pic) {
			case 0:
				// Left 2:000
				ctrl_apply_delta(CTRL_SUB_FILT_ENV_D, state.enc1_delta);
				ctrl_apply_delta(CTRL_SUB_FILT_ENV_R, state.enc2_delta);
				ctrl_apply_delta(CTRL_SUB_FILT_ENV_A, state.enc3_delta);
				ctrl_apply_delta(CTRL_SUB_FILT_ENV_S, state.enc4_delta);
				ctrl_apply_toggle(CTRL_SUB_FILT_ENV_A, state.sw1_changed, state.sw1_state);
				ctrl_apply_toggle(CTRL_SUB_FILT_ENV_S, state.sw2_changed, state.sw2_state);
				break;
			case 1:
				// Left 2:001
				ctrl_apply_delta(CTRL_SUB_FILT_CUTOFF, state.enc1_delta);
				ctrl_apply_delta(CTRL_SUB_FILT_RES, state.enc2_delta);
			}
			break;
		case 3:
			// Left I2C 3
			switch(pic) {
			case 0:
				// Left 3:000
				ctrl_apply_delta(CTRL_OSC2_FILT_CUTOFF, state.enc1_delta);
				ctrl_apply_delta(CTRL_OSC2_DRIVE, state.enc2_delta);
				ctrl_apply_delta(CTRL_OSC2_FILT_RES, state.enc3_delta);
				ctrl_apply_delta(CTRL_FX_WETDRY, state.enc4_delta);
				ctrl_apply_toggle(CTRL_FX_WETDRY, state.sw1_changed, state.sw1_state);
				break;
			}
			break;
		}
	} else if (bus == 1) {
		// Right I2C
		switch (channel) {
		case 0:
			// Right I2C 0
			switch(pic) {
			case 0:
				// Right 0:000
				ctrl_apply_delta(CTRL_OSC_AMP_ENV_D, state.enc1_delta);
				ctrl_apply_delta(CTRL_OSC_AMP_ENV_R, state.enc2_delta);
				ctrl_apply_delta(CTRL_OSC_AMP_ENV_A, state.enc3_delta);
				ctrl_apply_delta(CTRL_OSC_AMP_ENV_S, state.enc4_delta);
				ctrl_apply_toggle(CTRL_OSC_AMP_ENV_S, state.sw2_changed, state.sw2_state);
				break;
			}
			break;
		case 1:
			// Right I2C 1
			switch(pic) {
			case 0:
				// Right 1:000
				ctrl_apply_delta(CTRL_FX_VAL2, state.enc1_delta);
				ctrl_apply_delta(CTRL_FX_VAL4, state.enc2_delta);
				ctrl_apply_delta(CTRL_FX_VAL3, state.enc3_delta);
				ctrl_apply_delta(CTRL_FX_VAL1, state.enc4_delta);
				break;
			case 1:
				// Right 1:001
				ctrl_apply_delta(CTRL_SUB_AMP_ENV_A, state.enc1_delta);
				ctrl_apply_delta(CTRL_SUB_AMP_ENV_S, state.enc2_delta);
				ctrl_apply_delta(CTRL_SUB_AMP_ENV_D, state.enc3_delta);
				ctrl_apply_delta(CTRL_SUB_AMP_ENV_R, state.enc4_delta);
				ctrl_apply_toggle(CTRL_SUB_AMP_ENV_S, state.sw2_changed, state.sw2_state);
			}
			break;
		}
	}
}

void rotpic_poll_all(uint8_t bus, uint8_t mux, uint8_t channel, bool ignore_state) {
	// Select the mux channel
	i2c_mux_select(bus, mux, channel);

	// Poll all (possible) 8 rotary PICs on that bus
	for (uint8_t i=0; i < 8; i++) {
		rotpic_state state = _rotpic_poll_selected(bus, DEFAULT_ROTPIC_ADDR + i);
		if (state.success && !ignore_state) {
			_rotpic_handle_state(bus, mux, channel, i, state);
		}
	}
}

void rotpic_test(I2C_HandleTypeDef *bus, uint8_t channel, uint8_t index) {
	HAL_StatusTypeDef res = tca9544a_select(bus, LEFT_I2C_MUX_ADDR, 2);
	if (res != HAL_OK) {
		return;
	}

	uint8_t data[2] = { 0b10, 0x00 };
	uint8_t shifted_addr = (DEFAULT_ROTPIC_ADDR + index) << 1;

	while(1) {
		res = HAL_I2C_Master_Transmit(bus, shifted_addr, NULL, 0, HAL_MAX_DELAY);
		if (res != HAL_OK) {
			printf("Damn");
//			return state;
		}
		res = HAL_I2C_Master_Transmit(bus, shifted_addr, data, 2, HAL_MAX_DELAY);
		if (res != HAL_OK) {
			printf("Damn");
//			return state;
		}
		uint8_t recv[4] = { 0x0, 0x0, 0x0, 0x0 };
		res = HAL_I2C_Master_Receive(bus, shifted_addr, recv, 4, HAL_MAX_DELAY);
		if (res != HAL_OK) {
			printf("Damn");
//			return state;
		}
		if (recv[3] != 3) {
			printf("Awew what?!");
		} else {
			printf("Yesss mate");
		}
	}
}
