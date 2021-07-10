/*
 * commit_led_button.c
 *
 *  Created on: Jul 10, 2021
 *      Author: jwilson
 */


#include "main.h"
#include "commit.h"
#include "is32.h"
#include "i2c.h"
#include "ctrl.h"
#include "string.h"

/*
 * RGB Calculations
 */

static uint8_t val[3];

uint8_t* _button_led_rgb(seq_button_state_t button) {
  val[0] = button.pressed ? 0xFF : 0x00;
  val[1] = button.pressed ? 0xFF : 0x00;
  val[2] = button.pressed ? 0xFF : 0x00;
  return val;
}

/*
 * Utils
 */

#define DEFAULT_BUTTON_SCALE 0x15
#define DEFAULT_BUTTON_SCALE_R 0x27
#define DEFAULT_BUTTON_SCALE_G 0x17
#define DEFAULT_BUTTON_SCALE_B 0x26

void _set_button_scale_seq(uint8_t *pwm_seq, uint8_t *scale_seq, uint8_t len) {
  for (uint8_t i=0; i < len; i++) {
    switch(i%3) {
    case 0:
      scale_seq[i] = DEFAULT_BUTTON_SCALE_R;
      break;
    case 1:
      scale_seq[i] = DEFAULT_BUTTON_SCALE_G;
      break;
    case 2:
      scale_seq[i] = DEFAULT_BUTTON_SCALE_B;
      break;
    }
  }
}

/*
 * Change Detection
 */

bool _commit_led_steps1to12_changed() {
  for(uint8_t i=0; i < 12; i++) {
    if (commit_seq_state.button_changed[i]) {
      return true;
    }
  }
  return false;
}

bool _commit_led_steps13to16_changed() {
  for(uint8_t i=11; i < 16; i++) {
    if (commit_seq_state.button_changed[i]) {
      return true;
    }
  }
  return false;
}

/*
 * Commit Functions
 */

void _set_pwm_single(uint8_t *seqptr, uint8_t *val) {
  memcpy(seqptr, val, 3);
}

void _commit_led_button_steps1to12() {
  bool res;
  uint8_t pwm_seq[36];
  uint8_t scale_seq[36];

  /* Steps 1 to 12
   * LEFT1:01 - Channels 0-35
   */

  if (!_commit_led_steps1to12_changed()) return;

  for (uint8_t i=0; i < 12; i++) {
    _set_pwm_single(pwm_seq+(i*3), _button_led_rgb(commit_seq_state.button_state[i]));
  }

  res = is32_set_sequence_pwm(I2C_LEFT, 1, 1, 0, pwm_seq, 12*3);
  if (!res) Error_Handler();

  _set_button_scale_seq(pwm_seq, scale_seq, 12*3);
  res = is32_set_sequence_scale(I2C_LEFT, 1, 1, 0, scale_seq, 12*3);
  if (!res) Error_Handler();

  res = is32_write_registers(I2C_LEFT, 1, 1);
  if (!res) Error_Handler();
}

void _commit_led_button_steps13to16() {
  bool res;
  uint8_t pwm_seq[36];
  uint8_t scale_seq[36];

  /* Steps 13 to 16
   * RIGHT1:10 - Channels 24-35
   */

  if (!_commit_led_steps13to16_changed()) return;

  for (uint8_t i=0; i < 4; i++) {
    _set_pwm_single(pwm_seq+(i*3), _button_led_rgb(commit_seq_state.button_state[i+12]));
  }

  res = is32_set_sequence_pwm(I2C_RIGHT, 1, 0b10, 24, pwm_seq, 4*3);
  if (!res) Error_Handler();

  _set_button_scale_seq(pwm_seq, scale_seq, 4*3);
  res = is32_set_sequence_scale(I2C_RIGHT, 1, 0b10, 24, scale_seq, 4*3);
  if (!res) Error_Handler();

  res = is32_write_registers(I2C_RIGHT, 1, 0b10);
  if (!res) Error_Handler();
}

void commit_led_button(commit_cycle_t cycle) {
  uint32_t ticks_before = HAL_GetTick();

  switch(cycle) {
  case COMMIT_LED_BUTTON_STEP1TO12:
    _commit_led_button_steps1to12();
    break;
  case COMMIT_LED_BUTTON_STEP13TO16:
    _commit_led_button_steps13to16();
    break;
  default:
    break;
  }

  uint32_t ticks_after = HAL_GetTick();
  uint32_t ticks_cost = ticks_after - ticks_before;

  UNUSED(ticks_cost);
}

