/*
 * i2c.c
 *
 *  Created on: Apr 19, 2021
 *      Author: jwilson
 */

#include "commit.h"
#include "ctrl.h"
#include "i2c.h"
#include "is32.h"
#include "main.h"
#include <stdbool.h>

void _i2c_resume_left_0_00(uint8_t bus, i2c_callback_t callback,
                           void *userdata) {

  bool res;
  uint16_t pwm_seq[36] = {0};
  uint8_t scale_seq[36] = {0};

  ctrl_t *ctrl = ctrl_get_active();

  // Osc1 Saw: LEDs 0, 1
  _set_pwm_seq_lab(_osc1_saw_lab(ctrl), pwm_seq, 2 * 3);

  // Osc1 Squ: LEDS 2, 3
  _set_pwm_seq_lab(_osc1_squ_lab(ctrl), pwm_seq + (2 * 3), 2 * 3);

  // Work around Red and Blue pins being transposed
  uint16_t tmp;
  tmp = pwm_seq[6];
  pwm_seq[6] = pwm_seq[8];
  pwm_seq[8] = tmp;
  tmp = pwm_seq[9];
  pwm_seq[9] = pwm_seq[11];
  pwm_seq[11] = tmp;

  // Osc1 Mix: LEDs 4, 5, 6, 7, 8, 9, 10

  _set_pwm_seq_lab(_osc1_mix_lab(ctrl), pwm_seq + (4 * 3), 7 * 3);

  // Animation: Osc1 Saw
  _set_scale_seq_animated(pwm_seq, scale_seq, 2 * 3, 0, false);

  // Animation: Osc1 Squ
  _set_scale_seq_animated(pwm_seq + (2 * 3), scale_seq, 2 * 3, 0, false);

  // Animation: Osc 1 pre-filt
  _set_scale_seq_animated(pwm_seq + (4 * 3), scale_seq, 5 * 3, 2, false);

  // Animation: Osc1 to Osc2 pre-mix
  _set_scale_seq_animated(pwm_seq + (9 * 3), scale_seq + (5 * 3), 2 * 3, 4,
                          false);

  // Write to driver
  res = is32_set(I2C_LEFT, 0, 0, pwm_seq, scale_seq, callback, userdata);
  if (!res)
    Error_Handler();
}

uint8_t cycle = 0;

void _i2c_resume_left_bus(uint8_t bus, i2c_callback_t callback,
                          void *userdata) {

  switch (cycle) {
  case I2C_LEFT_0_00:
    // Osc1 Saw, Squ and Mix
    _i2c_resume_left_0_00(bus, callback, userdata);
    break;
  // case I2C_LEFT_0_01:
  //   // Tuning LEDs
  //   break;
  // case I2C_LEFT_0_10:
  //   // Osc1 Filter Freq & Res, and Osc1-to-2 Mix
  //   break;
  // case I2C_LEFT_0_11:
  //   // Osc2 Saw, Squ, Noise, Osc2-Only, Osc2 Pre-Filt
  //   break;
  default:
    cycle = 0;
    callback(bus, userdata);
  }

  cycle++;
}
