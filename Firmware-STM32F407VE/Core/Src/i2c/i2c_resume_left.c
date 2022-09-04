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

  // Animation: Osc1 Saw (0, 1)
  _set_scale_seq_animated(pwm_seq, scale_seq, 2 * 3, 0, false);

  // Animation: Osc1 Squ (2, 3)
  _set_scale_seq_animated(pwm_seq + (2 * 3), scale_seq + (2 * 3), 2 * 3, 0,
                          false);

  // Animation: Osc 1 pre-filt (4, 5, 6, 7, 8)
  _set_scale_seq_animated(pwm_seq + (4 * 3), scale_seq + (4 * 3), 5 * 3, 2,
                          false);

  // Animation: Osc1 to Osc2 pre-mix (9, 10)
  _set_scale_seq_animated(pwm_seq + (9 * 3), scale_seq + (9 * 3), 2 * 3, 4,
                          false);

  // Write to driver
  res = is32_set(I2C_LEFT, 0, 0, pwm_seq, scale_seq, callback, userdata);
  if (!res)
    Error_Handler();
}

void _i2c_resume_left_0_01(uint8_t bus, i2c_callback_t callback,
                           void *userdata) {

  uint16_t pwm_seq[36] = {0};
  uint8_t scale_seq[36] = {0};

  memset(scale_seq, 0xFF, 36);

  /* Tuning LEDs
   * LEFT0:01
   * 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12
   * 13 LEDs arranged in a circle around the tune knob
   */

  ctrl_toggle_t *toggle = ctrl_get_active_toggle();
  ctrl_t *ctrl = ctrl_get_active();

  int16_t tuning_step = 0;
  double tuning_percent = 0.0;
  switch (toggle->osc1_tune_func) {
  case ENC_OSC_TUNE_COARSE:
    tuning_step = (uint8_t)(ctrl->value[CTRL_OSC1_TUNE_COARSE] / 2);
    pwm_seq[tuning_step] = 0x7000;
    if (ctrl->value[CTRL_OSC1_TUNE_COARSE] % 2) {
      pwm_seq[tuning_step + 1] = 0x7000;
    }
    break;

  case ENC_OSC_TUNE_FINE:
    tuning_step = (int16_t)ctrl->value[CTRL_OSC1_TUNE_FINE];
    tuning_percent = (double)tuning_step / CTRL_DEFAULT_MAX;
    pwm_seq[(uint8_t)(tuning_percent * 12.0)] = 0x7000;
    break;

  default:
    break;
  }

  bool res = is32_set(bus, 0, 1, pwm_seq, scale_seq, callback, userdata);
  if (!res)
    Error_Handler();
}

void _i2c_resume_left_0_10(uint8_t bus, i2c_callback_t callback,
                           void *userdata) {
  uint16_t pwm_seq[36] = {0};
  uint8_t scale_seq[36] = {0};
  ctrl_t *ctrl = ctrl_get_active();

  /* Osc1toOsc2 Mix Level
   * LEFT0:10
   * Mix: 0, 1
   */

  _set_pwm_seq_lab(_osc1_to_2_mix_lab(ctrl), pwm_seq, (2 * 3));
  _set_scale_seq_animated(pwm_seq, scale_seq, (2 * 3), 6, false);

  /* Osc1 Filter
   * LEFT0:10
   * Freq: 3, 4
   */

  _set_pwm_seq_lab(_osc1_filt_freq_lab(ctrl), pwm_seq + (3 * 3), (2 * 3));
  _set_scale_seq_animated(pwm_seq, scale_seq + (3 * 3), (2 * 3), 7, false);

  /* Osc1 Filter
   * LEFT0:10
   * Reso: 5, 6
   */

  _set_pwm_seq_lab(_osc1_filt_reso_lab(ctrl), pwm_seq + (5 * 3), (2 * 3));
  _set_scale_seq_animated(pwm_seq, scale_seq + (5 * 3), (2 * 3), 9, false);

  bool res = is32_set(bus, 0, 0b10, pwm_seq, scale_seq, callback, userdata);
  if (!res)
    Error_Handler();
}

#define OSC2_PATTERN_OFFSET -8

void _i2c_resume_left_0_11(uint8_t bus, i2c_callback_t callback,
                           void *userdata) {
  uint16_t pwm_seq[36] = {0};
  uint8_t scale_seq[36] = {0};
  ctrl_t *ctrl = ctrl_get_active();

  /* Osc2 Squ
   * LEFT0:11
   * 0
   */

  _set_pwm_seq_lab(_osc2_squ_lab(ctrl), pwm_seq, (1 * 3));
  _set_scale_seq_animated(pwm_seq, scale_seq, (1 * 3), 1 + OSC2_PATTERN_OFFSET,
                          false);

  /* Osc2 Saw
   * LEFT0:11
   * 1, 2
   */

  _set_pwm_seq_lab(_osc2_saw_lab(ctrl), pwm_seq + (1 * 3), (2 * 3));
  _set_scale_seq_animated(pwm_seq + (1 * 3), scale_seq + (1 * 3), (2 * 3),
                          0 + OSC2_PATTERN_OFFSET, false);

  /* Osc2 Noise
   * LEFT0:11
   * 3, 4
   */

  _set_pwm_seq_lab(_osc2_noise_lab(ctrl), pwm_seq + (3 * 3), (2 * 3));

  // Work around Red and Blue pins being transposed
  uint16_t tmp;
  tmp = pwm_seq[0];
  pwm_seq[0] = pwm_seq[2];
  pwm_seq[2] = tmp;
  tmp = pwm_seq[3];
  pwm_seq[3] = pwm_seq[5];
  pwm_seq[5] = tmp;

  _set_scale_seq_animated(pwm_seq + (3 * 3), scale_seq + (3 * 3), (2 * 3),
                          0 + OSC2_PATTERN_OFFSET, false);

  /* Osc2-Only Mix
   * LEFT0:11
   * 5, 6, 7, 8
   */

  _set_pwm_seq_lab(_osc2_mix_lab(ctrl), pwm_seq + (5 * 3), (4 * 3));

  _set_scale_seq_animated(pwm_seq + (5 * 3), scale_seq + (5 * 3), (4 * 3),
                          2 + OSC2_PATTERN_OFFSET, false);

  /* Osc2-PreFilter Mix
   * LEFT0:11
   * 9, 10
   */

  _set_pwm_seq_lab(_osc2_prefilt_lab(ctrl), pwm_seq + (9 * 3), (2 * 3));

  _set_scale_seq_animated(pwm_seq + (9 * 3), scale_seq + (9 * 3), (2 * 3),
                          6 + OSC2_PATTERN_OFFSET, false);

  bool res = is32_set(bus, 0, 0b11, pwm_seq, scale_seq, callback, userdata);
  if (!res)
    Error_Handler();
}

void _i2c_resume_left_bus(uint8_t bus, i2c_callback_t callback,
                          void *userdata) {
  static uint8_t cycle = I2C_LEFT_START;

  cycle++;

  switch (cycle) {
  case I2C_LEFT_0_00:
    // Osc1 Saw, Squ and Mix
    _i2c_resume_left_0_00(bus, callback, userdata);
    break;
  case I2C_LEFT_0_01:
    // Tuning LEDs
    _i2c_resume_left_0_01(bus, callback, userdata);
    break;
  case I2C_LEFT_0_10:
    // Osc1 Filter Freq & Res, and Osc1-to-2 Mix
    _i2c_resume_left_0_10(bus, callback, userdata);
    break;
  case I2C_LEFT_0_11:
    // Osc2 Saw, Squ, Noise, Osc2-Only, Osc2 Pre-Filt
    _i2c_resume_left_0_11(bus, callback, userdata);
    break;
  default:
    cycle = I2C_LEFT_START;
    callback(bus, userdata);
  }
}
