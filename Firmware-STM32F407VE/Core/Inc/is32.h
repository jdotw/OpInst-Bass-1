/*
 * is32.h
 *
 *  Created on: Apr 13, 2021
 *      Author: jwilson
 */

#ifndef INC_IS32_H_
#define INC_IS32_H_

#include "i2c.h"
#include "stm32f4xx_hal.h"
#include <stdbool.h>

void is32_init();
bool is32_set_single(uint8_t bus, uint8_t channel, uint8_t unit, uint8_t led,
                     uint8_t pwm, uint8_t brightness);
bool is32_set_single_uncommitted(uint8_t bus, uint8_t channel, uint8_t unit,
                                 uint8_t led, uint8_t pwm, uint8_t brightness);
bool is32_write_registers(uint8_t bus, uint8_t channel, uint8_t unit);
bool is32_set_single_pwm(uint8_t bus, uint8_t channel, uint8_t unit,
                         uint8_t led, uint8_t pwm);
bool is32_set_single_scale(uint8_t bus, uint8_t channel, uint8_t unit,
                           uint8_t led, uint8_t brightness);
bool is32_set_sequence_pwm(uint8_t bus, uint8_t channel, uint8_t unit,
                           uint8_t start, uint16_t *seq, uint8_t len);
bool is32_set_sequence_scale(uint8_t bus, uint8_t channel, uint8_t unit,
                             uint8_t start, uint8_t *seq, uint8_t len);
void is32_test(void);

bool is32_set(uint8_t bus, uint8_t channel, uint8_t unit, uint16_t *pwm,
              uint8_t *scale, i2c_callback_t callback, void *userdata);

#endif /* INC_IS32_H_ */
