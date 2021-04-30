/*
 * pca9555.c
 *
 *  Created on: 16 Apr 2021
 *      Author: jwilson
 */

#include <stdio.h>
#include "main.h"
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

HAL_StatusTypeDef pca9555_set_port_config(uint8_t bus, uint8_t addr, uint8_t ports[2]) {
	uint8_t port_0_data[2] = { CMD_CONFIG_PORT0, ports[0] };
	HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(i2c_bus[bus], DEFAULT_PCA9555_ADDRESS << 1, port_0_data, 2, HAL_MAX_DELAY);
	if (result != HAL_OK) return result;

	uint8_t port_1_data[2] = { CMD_CONFIG_PORT1, ports[1] };
	result = HAL_I2C_Master_Transmit(i2c_bus[bus], DEFAULT_PCA9555_ADDRESS << 1, port_1_data, 2, HAL_MAX_DELAY);
	if (result != HAL_OK) return result;

	return result;
}

HAL_StatusTypeDef pca9555_set_port_output(uint8_t bus, uint8_t addr, uint8_t outputs[2]) {
	uint8_t port_0_data[2] = { CMD_OUTPUT_PORT0, outputs[0] };
	HAL_StatusTypeDef result = HAL_I2C_Master_Transmit(i2c_bus[bus], DEFAULT_PCA9555_ADDRESS << 1, port_0_data, 2, HAL_MAX_DELAY);
	if (result != HAL_OK) return result;

	uint8_t port_1_data[2] = { CMD_OUTPUT_PORT1, outputs[1] };
	result = HAL_I2C_Master_Transmit(i2c_bus[bus], DEFAULT_PCA9555_ADDRESS << 1, port_1_data, 2, HAL_MAX_DELAY);
	if (result != HAL_OK) return result;

	return result;
}

void pca9555_init() {

  // Reset DACs
	HAL_StatusTypeDef res;

	// I2C Left 2
	res = i2c_mux_select(I2C_LEFT, I2C_LEFT_MUX, 2);
	if (res != HAL_OK) {
		printf("Failed to select I2C Left 2\n");
		Error_Handler();
	}

	// Configure ports on Left PCA9555
	uint8_t left_ports[2] = { ALL_INPUT, ALL_INPUT };
	res = pca9555_set_port_config(I2C_LEFT, DEFAULT_PCA9555_ADDRESS, left_ports);
	if (res != HAL_OK) {
		printf("Failed to set LEFT PCA9555 port configuration");
		Error_Handler();
	}

	// I2C Right 2
	res = i2c_mux_select(I2C_RIGHT, I2C_RIGHT_MUX, 2);
	if (res != HAL_OK) {
		printf("Failed to select I2C Left 2\n");
		Error_Handler();
	}

	// Configure ports on Right PCA9555
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
	res = pca9555_set_port_config(I2C_RIGHT, DEFAULT_PCA9555_ADDRESS, right_ports);
	if (res != HAL_OK) {
		printf("Failed to set RIGHT PCA9555 port configuration");
		Error_Handler();
	}

	// Set all outputs to 0
	uint8_t right_outputs[2] = { 0xFF, 0xFF };
	res = pca9555_set_port_output(I2C_RIGHT, DEFAULT_PCA9555_ADDRESS, right_outputs);
	if (res != HAL_OK) {
		printf("Failed to set RIGHT PCA9555 port outputs");
		Error_Handler();
	}
}
