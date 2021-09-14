/*
 * is32led.c
 *
 *  Created on: Apr 13, 2021
 *      Author: jwilson
 */

#include <stdio.h>
#include "main.h"
#include "is32.h"
#include "i2c.h"

#define DEFAULT_IS32_ADDR 0x34

bool _is32_enable(uint8_t bus, uint8_t channel, uint8_t unit) {
	bool res;
	uint8_t data[2] = { 0x00, 0x00 };

	// 0x7F = RESET
	data[0] =  0x7F;
	data[1] = 0x00;
	res = i2c_tx(bus, channel, (DEFAULT_IS32_ADDR+unit), data, 2);
	if (!res) return false;

	// 0x00 = CONTROL
	data[0] = 0x00;
	data[1] = 0x00;
	data[1] |= 0b1 << 0;    // Enable normal operation
	data[1] |= 0b11 << 1;   // 16bit PWM Resolution
	data[1] |= 0b000 << 4;  // 16Mhz oscillator clock
	res = i2c_tx(bus, channel, (DEFAULT_IS32_ADDR+unit), data, 2);
	if (!res) return false;

	// 0x6e = GLOBAL CURRENT
	data[0] = 0x6E;
	data[1] = 0xFF;
	res = i2c_tx(bus, channel, (DEFAULT_IS32_ADDR+unit), data, 2);
	if (!res) return false;

	return true;
}


void is32_init() {
	bool res;

	// LEFT0:00
	res = _is32_enable(I2C_LEFT, 0, 0b00);
	if (!res) Error_Handler();

	// LEFT0:10
	res = _is32_enable(I2C_LEFT, 0, 0b10);
	if (!res) Error_Handler();

	// RIGHT1:00
	res = _is32_enable(I2C_RIGHT, 1, 0b00);
	if (!res) Error_Handler();

	// LEFT0:11
	res = _is32_enable(I2C_LEFT, 0, 0b11);
	if (!res) Error_Handler();

	// LEFT3:10
	res = _is32_enable(I2C_LEFT, 3, 0b10);
	if (!res) Error_Handler();

	// LEFT3:00
	res = _is32_enable(I2C_LEFT, 3, 0b00);
	if (!res) Error_Handler();

	// LEFT1:00
	res = _is32_enable(I2C_LEFT, 1, 0b00);
	if (!res) Error_Handler();

	// LEFT1:10
	res = _is32_enable(I2C_LEFT, 1, 0b10);
	if (!res) Error_Handler();

	// RIGHT2:01
	res = _is32_enable(I2C_RIGHT, 2, 0b01);
	if (!res) Error_Handler();

	// RIGHT1:01
	res = _is32_enable(I2C_RIGHT, 1, 0b01);
	if (!res) Error_Handler();

	// RIGHT2:10
	res = _is32_enable(I2C_RIGHT, 2, 0b10);
	if (!res) Error_Handler();

	// LEFT0:01
	res = _is32_enable(I2C_LEFT, 0, 0b01);
	if (!res) Error_Handler();

	// LEFT3:01
	res = _is32_enable(I2C_LEFT, 3, 0b01);
	if (!res) Error_Handler();

	// RIGHT2:00
	res = _is32_enable(I2C_RIGHT, 2, 0b00);
	if (!res) Error_Handler();

	// LEFT1:11
	res = _is32_enable(I2C_LEFT, 1, 0b11);
	if (!res) Error_Handler();

	// RIGHT1:10
	res = _is32_enable(I2C_RIGHT, 1, 0b10);
	if (!res) Error_Handler();

	// LEFT1:01
	res = _is32_enable(I2C_LEFT, 1, 0b01);
	if (!res) Error_Handler();

}

bool is32_set_rgb(uint8_t bus, uint8_t channel, uint8_t unit, uint8_t led, uint16_t red, uint16_t green, uint16_t blue, uint8_t red_scale, uint8_t green_scale, uint8_t blue_scale) {
	uint8_t data[3] = {  0x00, 0x00, 0x00 };
	bool res;

	// PWM
	data[0] =  0x01 + (led * 6);
	data[1] = red & 0x00FF;
	data[2] = (red & 0xFF00) >> 8;
	res = i2c_tx(bus, channel, (DEFAULT_IS32_ADDR+unit), data, 3);
	if (!res) return false;

	data[0] =  0x03 + (led * 6);
  data[1] = green & 0x00FF;
  data[2] = (green & 0xFF00) >> 8;
	res = i2c_tx(bus, channel, (DEFAULT_IS32_ADDR+unit), data, 3);
	if (!res) return false;

	data[0] =  0x05 + (led * 6);
  data[1] = blue & 0x00FF;
  data[2] = (blue & 0xFF00) >> 8;
	res = i2c_tx(bus, channel, (DEFAULT_IS32_ADDR+unit), data, 3);
	if (!res) return false;

	// Scaling (current)
	data[0] = 0x4A + (led * 3);
	data[1] = red_scale;
	res = i2c_tx(bus, channel, (DEFAULT_IS32_ADDR+unit), data, 2);
	if (!res) return false;

	data[0] = 0x4B + (led * 3);
	data[1] = green_scale;
	res = i2c_tx(bus, channel, (DEFAULT_IS32_ADDR+unit), data, 2);
	if (!res) return false;

	data[0] = 0x4C + (led * 3);
	data[1] = blue_scale;
	res = i2c_tx(bus, channel, (DEFAULT_IS32_ADDR+unit), data, 2);
	if (!res) return false;

	// Write the registers
	data[0] = 0x49;
	data[1] = 0x00;
	res = i2c_tx(bus, channel, (DEFAULT_IS32_ADDR+unit), data, 2);
	if (!res) return false;

	return true;
}

bool is32_write_registers(uint8_t bus, uint8_t channel, uint8_t unit) {
	// Write the registers
	uint8_t data[2] = {  0x00, 0x00 };
	bool res;
	data[0] = 0x49;
	data[1] = 0x00;
	res = i2c_tx(bus, channel, (DEFAULT_IS32_ADDR+unit), data, 2);
	if (!res) return false;
	return true;
}

bool is32_set_single_pwm(uint8_t bus, uint8_t channel, uint8_t unit, uint8_t led, uint8_t pwm) {
	// PWM
	uint8_t data[2] = {  0x00, 0x00 };
	bool res;
	data[0] = 0x01 + (led * 2);
	data[1] = pwm;
	res = i2c_tx(bus, channel, (DEFAULT_IS32_ADDR+unit), data, 2);
	if (!res) return false;
	return true;
}

bool is32_set_single_scale(uint8_t bus, uint8_t channel, uint8_t unit, uint8_t led, uint8_t brightness) {
	// Scaling (current)
	uint8_t data[2] = {  0x00, 0x00 };
	bool res;
	data[0] = 0x4A + led;
	data[1] = brightness;
	res = i2c_tx(bus, channel, (DEFAULT_IS32_ADDR+unit), data, 2);
	if (!res) return false;
	return true;
}

bool _is32_set_single(uint8_t bus, uint8_t channel, uint8_t unit, uint8_t led, uint8_t pwm, uint8_t brightness, bool write_registers) {
	bool res;

	// PWM
	res = is32_set_single_pwm(bus, channel, unit, led, pwm);
	if (!res) return false;

	// Scaling (current)
	res = is32_set_single_scale(bus, channel, unit, led, brightness);
	if (!res) return false;

	// Write the registers
	if (write_registers) {
		res = is32_write_registers(bus, channel, unit);
		if (!res) return false;
	}

	return true;
}

bool is32_set_single(uint8_t bus, uint8_t channel, uint8_t unit, uint8_t led, uint8_t pwm, uint8_t brightness) {
	return _is32_set_single(bus, channel, unit, led, pwm, brightness, true);
}

bool is32_set_single_uncommitted(uint8_t bus, uint8_t channel, uint8_t unit, uint8_t led, uint8_t pwm, uint8_t brightness) {
	return _is32_set_single(bus, channel, unit, led, pwm, brightness, false);
}

bool is32_set_sequence_pwm(uint8_t bus, uint8_t channel, uint8_t unit, uint8_t start, uint16_t *seq, uint8_t len) {
	const uint8_t data_len = 1+(len*2);
	uint8_t data[data_len];
	data[0] = 0x01 + (start * 2);	// Registry address (start)
	for (uint8_t i=0; i < len; i++) {
	  data[(i*2)+1] = 0x00FF & seq[i];        // PWM_L
	  data[(i*2)+2] = (0xFF00 & seq[i]) >> 8;	// PWM_H
	}
	bool res;
	res = i2c_tx(bus, channel, (DEFAULT_IS32_ADDR+unit), data, data_len);
	if (!res) return false;
	return true;
}

bool is32_set_sequence_scale(uint8_t bus, uint8_t channel, uint8_t unit, uint8_t start, uint8_t *seq, uint8_t len) {
	const uint8_t data_len = 1+len;
	uint8_t data[data_len];
	data[0] = 0x4A + start; // Registry address (start)
	for (uint8_t i=0; i < len; i++) {
		data[i+1] = seq[i];
	}
	bool res;
	res = i2c_tx(bus, channel, (DEFAULT_IS32_ADDR+unit), data, data_len);
	if (!res) return false;
	return true;
}

/*
 * Test
 */

#define TEST_PWM 0x8000
#define TEST_SCALE 0x10

#define TEST_RED_SCALE 0x37
#define TEST_GREEN_SCALE 0x25
#define TEST_BLUE_SCALE 0x25

#define TEST_GAMMA_R 1.0
#define TEST_GAMMA_G 2.2
#define TEST_GAMMA_B 2.8

uint16_t test_gamma_correct(float gamma, uint16_t val) {
  const uint16_t max_in = 0xFFFF;
  const uint16_t max_out = 0xFFFF;
  return (uint16_t)(pow((float)val / (float)max_in, gamma) * max_out + 0.5);
}

void _is32_test_led(uint8_t bus, uint8_t channel, uint8_t unit) {
  bool shift_key = HAL_GPIO_ReadPin(SHIFTSW_GPIO_Port, SHIFTSW_Pin) == GPIO_PIN_RESET; // Pulled down
//  uint16_t pwm = shift_key ? 0x4000 : 0xFFFF;
  uint16_t pwm = 0x4000;
  uint16_t red = shift_key ? test_gamma_correct(TEST_GAMMA_R, pwm) : pwm;
  uint16_t green = shift_key ? test_gamma_correct(TEST_GAMMA_G, pwm) : pwm;
  uint16_t blue = shift_key ? test_gamma_correct(TEST_GAMMA_B, pwm) : pwm;
	for (uint8_t i=0; i < 12; i++) {
		is32_set_rgb(bus, channel, unit, i, red, green, blue, TEST_RED_SCALE, TEST_GREEN_SCALE, TEST_BLUE_SCALE);
	}
}

void is32_test() {
	_is32_test_led(I2C_LEFT,  0, 0b00); // ** Nothing
	_is32_test_led(I2C_LEFT,  0, 0b10); // OK
//	_is32_test_led(I2C_RIGHT, 1, 0b00); // OK
//	_is32_test_led(I2C_LEFT,  0, 0b11); // ** Some pins missing
//	_is32_test_led(I2C_LEFT,  3, 0b10); // OK
//	_is32_test_led(I2C_LEFT,  3, 0b00); // OK
//	_is32_test_led(I2C_LEFT,  1, 0b00); // OK
//	_is32_test_led(I2C_LEFT,  1, 0b10); // OK
//	_is32_test_led(I2C_RIGHT, 2, 0b01); // OK
//	_is32_test_led(I2C_RIGHT, 1, 0b01); // OK
//	_is32_test_led(I2C_RIGHT, 2, 0b10); // OK
//	_is32_test_led(I2C_LEFT,  0, 0b01); // OK
//	_is32_test_led(I2C_LEFT,  3, 0b01); // OK (Graph)
//	_is32_test_led(I2C_RIGHT, 2, 0b00); // OK (Graph)
//	_is32_test_led(I2C_LEFT,  1, 0b11); // OK (Graph)
//	_is32_test_led(I2C_RIGHT, 1, 0b10); // OK (Graph + Steps)
//	_is32_test_led(I2C_LEFT,  1, 0b01); // OK (Steps)
}


//#define PATTERN_LENGTH 12
//
//void is32_flux_cap(uint8_t bus, uint8_t mux, uint8_t channel, uint8_t unit) {
//	uint8_t red, green, blue;
//
//	HAL_StatusTypeDef res = i2c_mux_select(bus, mux, channel);
//	if (res != HAL_OK) {
//		Error_Handler();
//	}
//
////	uint8_t brightness[PATTERN_LENGTH] = { 10, 10, 15, 20, 30, 45, 65, 90, 130, 90, 65, 45, 30, 20, 10, 10, 10, 10, 10, 5  };
//	uint8_t brightness[PATTERN_LENGTH] = { 10, 10, 15, 20, 30, 45, 65, 90, 130, 90, 65, 45, };
//	uint8_t i = 0;
//
//	red = 0xFF;
//	green = 0x22;
//	blue = 0xAA;
//
//	while(1) {
//		uint8_t offset;
//		for (offset=0; offset < PATTERN_LENGTH; offset++) {
//			for (i=0; i < 10; i++) {
//				HAL_StatusTypeDef result = is32_turn_on_led_rgb(bus, unit, i, red, green, blue, brightness[(i+offset)%PATTERN_LENGTH]);
//				if (result != HAL_OK) {
//					printf("Failed...");
//				}
//			}
//			HAL_Delay(48);
//		}
//	}
//}
//
