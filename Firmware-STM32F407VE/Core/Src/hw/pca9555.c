/*
 * pca9555.c
 *
 *  Created on: 16 Apr 2021
 *      Author: jwilson
 */

#include <stdbool.h>
#include "main.h"
#include "pca9555.h"
#include "i2c.h"

#define DEFAULT_PCA9555_ADDRESS 0x20

#define ALL_INPUT 0xFF
#define PIN0_INPUT 1 << 0
#define PIN1_INPUT 1 << 1
#define PIN2_INPUT 1 << 2
#define PIN3_INPUT 1 << 3
#define PIN4_INPUT 1 << 4
#define PIN5_INPUT 1 << 5
#define PIN6_INPUT 1 << 6
#define PIN7_INPUT 1 << 7

#define ALL_OUTPUT 0x00
#define PIN0_OUTPUT 0 << 0
#define PIN1_OUTPUT 0 << 1
#define PIN2_OUTPUT 0 << 2
#define PIN3_OUTPUT 0 << 3
#define PIN4_OUTPUT 0 << 4
#define PIN5_OUTPUT 0 << 5
#define PIN6_OUTPUT 0 << 6
#define PIN7_OUTPUT 0 << 7

#define CMD_OUTPUT_PORT0 2
#define CMD_OUTPUT_PORT1 3

#define CMD_CONFIG_PORT0 6
#define CMD_CONFIG_PORT1 7

bool pca9555_set_port_output(uint8_t bus, uint8_t channel, uint8_t unit, uint8_t outputs[2]) {
	bool res;
	uint8_t port_0_data[2] = { CMD_OUTPUT_PORT0, outputs[0] };
	res = i2c_tx(bus, channel, DEFAULT_PCA9555_ADDRESS + unit, port_0_data, 2);
	if (!res) return false;

	uint8_t port_1_data[2] = { CMD_OUTPUT_PORT1, outputs[1] };
	res = i2c_tx(bus, channel, DEFAULT_PCA9555_ADDRESS + unit, port_1_data, 2);
	if (!res) return false;

	return true;
}

bool _pca9555_set_port_config(uint8_t bus, uint8_t channel, uint8_t unit, uint8_t ports[2]) {
	bool res;
	uint8_t port_0_data[2] = { CMD_CONFIG_PORT0, ports[0] };
	res = i2c_tx(bus, channel, DEFAULT_PCA9555_ADDRESS + unit, port_0_data, 2);
	if (!res) return false;

	uint8_t port_1_data[2] = { CMD_CONFIG_PORT1, ports[1] };
	res = i2c_tx(bus, channel, DEFAULT_PCA9555_ADDRESS + unit, port_1_data, 2);
	if (!res) return false;

	return true;
}

void pca9555_init() {
	bool res;

	// Configure ports on Left:2 PCA9555
	uint8_t left_ports[2] = { ALL_INPUT, ALL_INPUT };
	res = _pca9555_set_port_config(I2C_LEFT, 2, 0, left_ports);
	if (!res) Error_Handler();

	// Configure ports on Right:2 PCA9555
	uint8_t right_ports[2] = { 0x0, 0x0 };
	right_ports[0] |= PIN0_INPUT;
	right_ports[0] |= PIN1_INPUT;
	right_ports[0] |= PIN2_OUTPUT;
	right_ports[0] |= PIN3_OUTPUT;
	right_ports[0] |= PIN4_OUTPUT;
	right_ports[0] |= PIN5_OUTPUT;
	right_ports[0] |= PIN6_OUTPUT;
	right_ports[0] |= PIN7_OUTPUT;
	right_ports[1] |= PIN0_OUTPUT;
	right_ports[1] |= PIN1_OUTPUT;
	right_ports[1] |= PIN2_OUTPUT;
	right_ports[1] |= PIN3_OUTPUT;
	right_ports[1] |= PIN4_OUTPUT;
	right_ports[1] |= PIN5_OUTPUT;
	right_ports[1] |= PIN6_INPUT;
	right_ports[1] |= PIN7_INPUT;
	res = _pca9555_set_port_config(I2C_RIGHT, 2, 0, right_ports);
	if (!res) Error_Handler();

	// Set all outputs to 0
	uint8_t right_outputs[2] = { 0x00, 0x00 };
	res = pca9555_set_port_output(I2C_RIGHT, 2, 0, right_outputs);
	if (!res) Error_Handler();
}
