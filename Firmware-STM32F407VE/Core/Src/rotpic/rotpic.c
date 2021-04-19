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
	if (bus == 0) {
		// Left I2C
		switch (channel) {
		case 0:
			// Left I2C 0
			break;
		case 1:
			// Left I2C 0
			break;
		case 2:
			// Left I2C 0
			printf("This! %i %i %i %i", state.enc1_delta, state.enc2_delta, state.enc3_delta, state.enc3_delta);
			break;
		case 3:
			// Left I2C 0
			break;
		}
	}
}

void rotpic_poll_all(uint8_t bus, uint8_t mux, uint8_t channel) {
	// Select the mux channel
	i2c_mux_select(bus, mux, channel);

	// Poll all (possible) 8 rotary PICs on that bus
	for (uint8_t i=0; i < 8; i++) {
		rotpic_state state = _rotpic_poll_selected(bus, DEFAULT_ROTPIC_ADDR + i);
		if (state.success) {
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
