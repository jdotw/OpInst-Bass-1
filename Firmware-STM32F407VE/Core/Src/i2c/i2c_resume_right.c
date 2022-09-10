/*
 * i2c.c
 *
 *  Created on: Apr 19, 2021
 *      Author: jwilson
 */

#include "commit.h"
#include "ctrl.h"
#include "dac7678.h"
#include "i2c.h"
#include "is32.h"
#include "main.h"
#include "osc.h"
#include "rotpic.h"
#include <stdbool.h>

#define DEFAULT_BRIGHTNESS 0x15
#define HALF_BRIGHTNESS 0x04

#define OSC2_PATTERN_OFFSET -8

void _i2c_resume_right_rgbled_1_00(uint8_t bus, i2c_callback_t callback,
                                   void *userdata) {
  uint16_t pwm_seq[36] = {0};
  uint8_t scale_seq[36] = {0};
  ctrl_t *ctrl = ctrl_get_active();

  /* Osc1 Drive
   * RIGHT1:00
   * 0, 1, 3, 2 <--- Note 3 before 2
   */

  _set_pwm_seq_lab(_osc1_drive_lab(ctrl), pwm_seq, 4 * 3);
  _set_scale_seq_animated(pwm_seq, scale_seq, 2 * 3, 11, false);
  _set_scale_seq_animated(pwm_seq + (2 * 3), scale_seq + (2 * 3), 2 * 3, 13,
                          true);

  /* Osc Amp Out
   * RIGHT1:00
   * 4, 5, 6, 7, 8
   */

  _set_pwm_seq_lab(_osc_amp_out_lab(ctrl), pwm_seq + (4 * 3), 5 * 3);
  _set_scale_seq_animated(pwm_seq + (4 * 3), scale_seq + (4 * 3), 5 * 3,
                          20 + OSC2_PATTERN_OFFSET, false);

  /* Write */
  bool res = is32_set(bus, 1, 0b00, pwm_seq, scale_seq, callback, userdata);
  if (!res)
    Error_Handler();
}

void _i2c_resume_right_rgbled_1_10(uint8_t bus, i2c_callback_t callback,
                                   void *userdata) {
  uint16_t pwm_seq[36] = {0};
  uint8_t scale_seq[36] = {0};
  ctrl_t *ctrl = ctrl_get_active();
  ctrl_toggle_t *toggle = ctrl_get_active_toggle();

  /* Sub Amp ADSR Release LEDs
   * RIGHT1:10
   */

  uint8_t brightness = 0x80;
  switch (toggle->sub_amp_env_sustain_func) {
  case ENC_ENV_SUSTAIN:
    brightness = DEFAULT_BRIGHTNESS;
    break;
  case ENC_ENV_AMOUNT:
    brightness = HALF_BRIGHTNESS;
    break;
  default:
    break;
  }
  memset(scale_seq, brightness, 36);

  /* Attack: 6, 7, 8, 9, 10, 11
   * [0,2=2=08][1,2=4=10][2,2=5=11]
   * [0,1=1=07][1,1=3=09]
   * [0,0=0=06]
   */

  uint16_t a_val = ctrl->value[CTRL_SUB_AMP_ENV_A];

  _adsr_led_set_grid_curve(a_val);
  pwm_seq[6] = grid[0][0];
  pwm_seq[7] = grid[0][1];
  pwm_seq[8] = grid[0][2];
  pwm_seq[9] = grid[1][1];
  pwm_seq[10] = grid[1][2];
  pwm_seq[11] = grid[2][2];

  /* Decay: 0, 1, 2, 3, 4, 5
   * [0,2][1,2][2,2]
   * [0,1][1,1]
   * [0,0]
   *
   * transposes to:
   *
   * [0,0=08=02]
   * [0,1=07=01][1,1=10=04]
   * [0,2=06=00][1,2=09=03][2,2=11=05]
   */

  uint16_t d_val = ctrl->value[CTRL_SUB_AMP_ENV_D];
  _adsr_led_set_grid_curve(d_val);
  pwm_seq[0] = grid[0][2];
  pwm_seq[1] = grid[0][1];
  pwm_seq[2] = grid[0][0];
  pwm_seq[3] = grid[1][2];
  pwm_seq[4] = grid[1][1];
  pwm_seq[5] = grid[2][2];

  /* Sustain: 12, 13, 14, 15, 16, 17
   * [0,2=14=14][1,2=17=17]
   * [0,1=13=13][1,1=16=16]
   * [0,0=12=12][1,0=15=15]
   */

  uint16_t s_val = 0;
  switch (toggle->sub_amp_env_sustain_func) {
  case ENC_ENV_SUSTAIN:
    s_val = ctrl->value[CTRL_SUB_AMP_ENV_S];
    _adsr_led_set_grid_bar(s_val);
    break;
  case ENC_ENV_AMOUNT:
    s_val = ctrl->value[CTRL_SUB_AMP_ENV_AMT];
    _adsr_led_set_grid_stack(s_val);
    break;
  default:
    break;
  }

  // Sustain graph is always DEFAULT_BRIGHTNESS
  scale_seq[12] = DEFAULT_BRIGHTNESS;
  scale_seq[13] = DEFAULT_BRIGHTNESS;
  scale_seq[14] = DEFAULT_BRIGHTNESS;
  scale_seq[15] = DEFAULT_BRIGHTNESS;
  scale_seq[16] = DEFAULT_BRIGHTNESS;
  scale_seq[17] = DEFAULT_BRIGHTNESS;

  pwm_seq[12] = grid[0][0];
  pwm_seq[13] = grid[0][1];
  pwm_seq[14] = grid[0][2];
  pwm_seq[15] = grid[1][0];
  pwm_seq[16] = grid[1][1];
  pwm_seq[17] = grid[1][2];

  /* Release: 18, 19, 20, 21, 22, 23
   * [0,2][1,2][2,2]
   * [0,1][1,1]
   * [0,0]
   *
   * transposes to:
   *
   * [0,0=20=20]
   * [0,1=19=19][1,1=22=22]
   * [0,2=18=18][1,2=21=21][2,2=23=23]
   */

  uint16_t r_val = ctrl->value[CTRL_SUB_AMP_ENV_R];
  _adsr_led_set_grid_curve(r_val);
  pwm_seq[18] = grid[0][2];
  pwm_seq[19] = grid[0][1];
  pwm_seq[20] = grid[0][0];
  pwm_seq[21] = grid[1][2];
  pwm_seq[22] = grid[1][1];
  pwm_seq[23] = grid[2][2];

  /* Step Sequencer Buttons 13 to 16
   * RIGHT1:10 - Channels 24-35
   */

  seq_t *seq = seq_get();

  for (uint8_t i = 0; i < 4; i++) {
    _set_pwm_single(pwm_seq + 24 + (i * 3), _button_step_rgb(seq, i + 12));
  }
  _set_button_scale_seq(pwm_seq + 24, scale_seq + 24, 4 * 3);

  /* Write */
  bool res = is32_set(bus, 1, 0b10, pwm_seq, scale_seq, callback, userdata);
  if (!res)
    Error_Handler();
}

void _i2c_resume_right_rotpic_0_000(uint8_t bus, i2c_callback_t callback,
                                    void *userdata) {

  // RIGHT0:000
  ctrl_toggle_t *toggle = ctrl_get_active_toggle();
  uint8_t led = 0x0;
  led |= (toggle->osc_amp_env_sustain_func == ENC_ENV_SUSTAIN)
         << ROTPIC_LED1_BIT_SHIFT;
  led |= (toggle->osc_amp_env_sustain_func == ENC_ENV_AMOUNT)
         << ROTPIC_LED2_BIT_SHIFT;
  bool res = rotpic_led_set_state(bus, 0, 0b000, led, callback, userdata);
  if (!res)
    Error_Handler();
}

void _i2c_resume_right_rotpic_1_001(uint8_t bus, i2c_callback_t callback,
                                    void *userdata) {

  // RIGHT0:000
  ctrl_toggle_t *toggle = ctrl_get_active_toggle();
  uint8_t led = 0x0;
  led |= (toggle->sub_amp_env_sustain_func == ENC_ENV_SUSTAIN)
         << ROTPIC_LED1_BIT_SHIFT;
  led |= (toggle->sub_amp_env_sustain_func == ENC_ENV_AMOUNT)
         << ROTPIC_LED2_BIT_SHIFT;
  bool res = rotpic_led_set_state(bus, 0, 0b000, led, callback, userdata);
  if (!res)
    Error_Handler();
}

void _i2c_resume_right_bus(uint8_t bus, i2c_callback_t callback,
                           void *userdata) {
  static uint8_t cycle = I2C_RIGHT_START;

  cycle++;

  switch (cycle) {
  case I2C_RIGHT_RGBLED_1_00:
    // Osc1 Drive and Osc Amp Out
    _i2c_resume_right_rgbled_1_00(bus, callback, userdata);
    break;
  case I2C_RIGHT_RGBLED_1_10:
    _i2c_resume_right_rgbled_1_10(bus, callback, userdata);
    break;
  case I2C_RIGHT_ROTPIC_0_000:
    // // Osc Amp Env Toggle
    _i2c_resume_right_rotpic_0_000(bus, callback, userdata);
    break;
  case I2C_RIGHT_ROTPIC_1_001:
    // // Sub Amp Env Toggle
    _i2c_resume_right_rotpic_1_001(bus, callback, userdata);
    break;
  default:
    cycle = I2C_RIGHT_START;
    callback(bus, userdata);
  }
}
