/*
 * commit_led_button.c
 *
 *  Created on: Jul 10, 2021
 *      Author: jwilson
 */

#include "blink.h"
#include "commit.h"
#include "ctrl.h"
#include "i2c.h"
#include "is32.h"
#include "main.h"
#include "string.h"

/*
 * RGB Calculations
 */

static uint16_t val[3];

#define MIN_PWM (uint16_t)0x0000
#define MAX_PWM (uint16_t)0xFFFF // 16bit

uint16_t *_button_step_rgb(seq_state_t *seq, uint8_t i) {
  seq_button_state_t button = seq->button_state[i];
  if (button.pressed) {
    val[0] = MAX_PWM;
    val[1] = MAX_PWM;
    val[2] = MAX_PWM;
  } else if (seq->active_page == seq->selected_page) {
    bool isActive = i == seq->active_step % 16 && seq->running;
    val[0] = MIN_PWM;
    val[1] = isActive ? MAX_PWM : MIN_PWM;
    val[2] = MIN_PWM;
  } else {
    val[0] = MIN_PWM;
    val[1] = MIN_PWM;
    val[2] = MIN_PWM;
  }
  return val;
}

uint16_t *_button_start_rgb(seq_state_t *seq, bool pressed) {
  if (pressed) {
    val[0] = MAX_PWM;
    val[1] = MAX_PWM;
    val[2] = MAX_PWM;
  } else {
    val[0] = MIN_PWM;
    val[1] = seq->running ? MAX_PWM : MIN_PWM;
    val[2] = MIN_PWM;
  }
  return val;
}

uint16_t *_button_shift_rgb(bool pressed) {
  val[0] = pressed ? MAX_PWM : MIN_PWM;
  val[1] = pressed ? MAX_PWM : MIN_PWM;
  val[2] = pressed ? MAX_PWM : MIN_PWM;
  return val;
}

/*
 * Utils
 */

#define DEFAULT_BUTTON_SCALE 0x15
#define DEFAULT_BUTTON_SCALE_R 0x37
#define DEFAULT_BUTTON_SCALE_G 0x27
#define DEFAULT_BUTTON_SCALE_B 0x36

void _set_button_scale_seq(uint16_t *pwm_seq, uint8_t *scale_seq, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    switch (i % 3) {
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

bool _commit_led_steps1to12_changed(seq_state_t *seq, seq_changed_t *changed) {
  for (uint8_t i = 0; i < 12; i++) {
    if (seq->button_changed[i]) {
      return true;
    }
  }
  if (changed->active_step || changed->running) {
    return true;
  }
  return false;
}

bool _commit_led_steps13to16_changed(seq_state_t *seq, seq_changed_t *changed) {
  for (uint8_t i = 11; i < 16; i++) {
    if (seq->button_changed[i]) {
      return true;
    }
  }
  if (changed->active_step || changed->running) {
    return true;
  }
  return false;
}

bool _commit_led_shiftpage_changed(mod_state_t *mod, seq_changed_t *changed) {
  return mod->button_changed.shift || mod->button_changed.page ||
         changed->active_page || changed->running || blink_changed;
}

bool _commit_led_start_changed(mod_state_t *mod, seq_changed_t *changed) {
  return mod->button_changed.start || changed->running;
}

/*
 * Commit Functions
 */

void _set_pwm_single(uint16_t *seqptr, uint16_t *val) {
  memcpy(seqptr, val, 3 * (sizeof(uint16_t)));
}

void _commit_led_button_steps1to12(seq_state_t *seq, seq_changed_t *changed,
                                   mod_state_t *mod) {
  bool res;
  uint16_t pwm_seq[36];
  uint8_t scale_seq[36];

  /* Steps 1 to 12
   * LEFT1:01 - Channels 0-35
   */

  if (!_commit_led_steps1to12_changed(seq, changed))
    return;

  for (uint8_t i = 0; i < 12; i++) {
    _set_pwm_single(pwm_seq + (i * 3), _button_step_rgb(seq, i));
  }

  res = is32_set_sequence_pwm(I2C_LEFT, 1, 1, 0, pwm_seq, 12 * 3);
  if (!res)
    Error_Handler();

  _set_button_scale_seq(pwm_seq, scale_seq, 12 * 3);
  res = is32_set_sequence_scale(I2C_LEFT, 1, 1, 0, scale_seq, 12 * 3);
  if (!res)
    Error_Handler();

  res = is32_write_registers(I2C_LEFT, 1, 1);
  if (!res)
    Error_Handler();
}

void _commit_led_button_steps13to16(seq_state_t *seq, seq_changed_t *changed) {
  bool res;
  uint16_t pwm_seq[36];
  uint8_t scale_seq[36];

  /* Steps 13 to 16
   * RIGHT1:10 - Channels 24-35
   */

  if (!_commit_led_steps13to16_changed(seq, changed))
    return;

  for (uint8_t i = 0; i < 4; i++) {
    _set_pwm_single(pwm_seq + (i * 3), _button_step_rgb(seq, i + 12));
  }

  res = is32_set_sequence_pwm(I2C_RIGHT, 1, 0b10, 24, pwm_seq, 4 * 3);
  if (!res)
    Error_Handler();

  _set_button_scale_seq(pwm_seq, scale_seq, 4 * 3);
  res = is32_set_sequence_scale(I2C_RIGHT, 1, 0b10, 24, scale_seq, 4 * 3);
  if (!res)
    Error_Handler();

  res = is32_write_registers(I2C_RIGHT, 1, 0b10);
  if (!res)
    Error_Handler();
}

void _commit_led_button_mod_shiftpage(seq_state_t *seq, seq_changed_t *changed,
                                      mod_state_t *mod) {
  bool res;
  uint16_t pwm_seq[36];
  uint8_t scale_seq[36];

  /*
   * RIGHT2:10
   * - Shift is 24,25,26
   * - Page is 32,33,34,35
   * - NOTE: Channels 27,28,29,30,31 are unused
   *         This allows a contiguous write to set these LED value
   */

  if (!_commit_led_shiftpage_changed(mod, changed))
    return;

  // Shift is 0,1,2 [actually 24,25,26]
  _set_pwm_single(pwm_seq, _button_shift_rgb(mod->button_state.shift));
  _set_button_scale_seq(pwm_seq, scale_seq, 4 * 3);

  // Page is 8,9,10,11 [actually 32,33,34,35]
  for (uint8_t i = 0; i < 4; i++) {
    if (i == seq->selected_page) {
      pwm_seq[i + 8] = 0xFFFF;
      scale_seq[i + 8] = 0x90;
    } else if (i == seq->active_page) {
      pwm_seq[i + 8] = 0xFFFF;
      scale_seq[i + 8] = blink ? 0x10 : 0x00;
    } else {
      pwm_seq[i + 8] = 0x0000;
      scale_seq[i + 8] = 0x00;
    }
  }

  res = is32_set_sequence_pwm(I2C_RIGHT, 2, 0b10, 24, pwm_seq, 4 * 3);
  if (!res)
    Error_Handler();

  res = is32_set_sequence_scale(I2C_RIGHT, 2, 0b10, 24, scale_seq, 4 * 3);
  if (!res)
    Error_Handler();

  res = is32_write_registers(I2C_RIGHT, 2, 0b10);
  if (!res)
    Error_Handler();
}

void _commit_led_button_mod_start(seq_state_t *seq, seq_changed_t *changed,
                                  mod_state_t *mod) {
  bool res;
  uint16_t pwm_seq[36];
  uint8_t scale_seq[36];

  /* Start LED
   * LEFT1:00 Channels 33,34,35
   */

  if (!_commit_led_start_changed(mod, changed))
    return;

  _set_pwm_single(pwm_seq, _button_start_rgb(seq, mod->button_state.start));

  res = is32_set_sequence_pwm(I2C_LEFT, 1, 0b00, 33, pwm_seq, 1 * 3);
  if (!res)
    Error_Handler();

  _set_button_scale_seq(pwm_seq, scale_seq, 1 * 3);
  res = is32_set_sequence_scale(I2C_LEFT, 1, 0b00, 33, scale_seq, 1 * 3);
  if (!res)
    Error_Handler();

  res = is32_write_registers(I2C_LEFT, 1, 0b00);
  if (!res)
    Error_Handler();
}

void commit_led_button(commit_cycle_t cycle, seq_state_t *seq,
                       seq_changed_t *changed, mod_state_t *mod) {
  uint32_t ticks_before = HAL_GetTick();

  switch (cycle) {
  case COMMIT_LED_BUTTON_STEP1TO12:
    _commit_led_button_steps1to12(seq, changed, mod);
    break;
  case COMMIT_LED_BUTTON_STEP13TO16:
    _commit_led_button_steps13to16(seq, changed);
    break;
  case COMMIT_LED_BUTTON_SHIFTPAGE:
    _commit_led_button_mod_shiftpage(seq, changed, mod);
    break;
  case COMMIT_LED_BUTTON_START:
    _commit_led_button_mod_start(seq, changed, mod);
    break;
  default:
    break;
  }

  uint32_t ticks_after = HAL_GetTick();
  uint32_t ticks_cost = ticks_after - ticks_before;

  UNUSED(ticks_cost);
}
