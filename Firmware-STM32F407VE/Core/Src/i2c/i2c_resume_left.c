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

void _i2c_resume_left_rgbled_0_00(uint8_t bus, i2c_callback_t callback,
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

void _i2c_resume_left_rgbled_0_01(uint8_t bus, i2c_callback_t callback,
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

void _i2c_resume_left_rgbled_0_10(uint8_t bus, i2c_callback_t callback,
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

void _i2c_resume_left_rgbled_0_11(uint8_t bus, i2c_callback_t callback,
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

#define SUB_PATTERN_OFFSET 0

void _i2c_resume_left_rgbled_1_00(uint8_t bus, i2c_callback_t callback,
                                  void *userdata) {
  uint16_t pwm_seq[36];
  uint8_t scale_seq[36];
  ctrl_t *ctrl = ctrl_get_active();
  seq_t *seq = seq_get();
  mod_t *mod = mod_get();

  /* Sub Squ
   * LEFT1:00
   * 0, 1
   */

  _set_pwm_seq_lab(_sub_squ_lab(ctrl), pwm_seq, 2 * 3);
  _set_scale_seq_animated(pwm_seq, scale_seq, 2 * 3, 0 + SUB_PATTERN_OFFSET,
                          false);

  /* Sub Noise
   * LEFT1:00
   * 2, 3
   */

  _set_pwm_seq_lab(_sub_noise_lab(ctrl), pwm_seq + (2 * 3), 2 * 3);
  _set_scale_seq_animated(pwm_seq + (2 * 3), scale_seq + (2 * 3), 2 * 3,
                          0 + SUB_PATTERN_OFFSET, false);

  /* Sub Mix
   * LEFT1:00
   * 4, 5, 6, 7, 8, 9, 10
   */

  _set_pwm_seq_lab(_sub_mix_lab(ctrl), pwm_seq + (4 * 3), 7 * 3);

  _set_scale_seq_animated(pwm_seq + (4 * 3), scale_seq + (4 * 3), 5 * 3,
                          2 + SUB_PATTERN_OFFSET, false);
  _set_scale_seq_animated(pwm_seq + (9 * 3), scale_seq + (9 * 3), 2 * 3,
                          4 + SUB_PATTERN_OFFSET, false);

  /* Start LED
   * LEFT1:00
   * 11
   */

  _set_pwm_single(pwm_seq + (11 * 3), _button_start_rgb(seq, mod->state.start));

  _set_button_scale_seq(pwm_seq + (11 * 3), scale_seq + (11 * 3), 1 * 3);

  bool res = is32_set(bus, 1, 0b00, pwm_seq, scale_seq, callback, userdata);
  if (!res)
    Error_Handler();
}

void _i2c_resume_left_rgbled_1_01(uint8_t bus, i2c_callback_t callback,
                                  void *userdata) {
  uint16_t pwm_seq[36];
  uint8_t scale_seq[36];
  seq_t *seq = seq_get();

  /* Steps 1 to 12
   * LEFT1:01 - Channels 0-35
   */

  for (uint8_t i = 0; i < 12; i++) {
    _set_pwm_single(pwm_seq + (i * 3), _button_step_rgb(seq, i));
  }
  _set_button_scale_seq(pwm_seq, scale_seq, 12 * 3);

  bool res = is32_set(bus, 1, 0b01, pwm_seq, scale_seq, callback, userdata);
  if (!res)
    Error_Handler();
}

void _i2c_resume_left_rgbled_1_10(uint8_t bus, i2c_callback_t callback,
                                  void *userdata) {
  uint16_t pwm_seq[36];
  uint8_t scale_seq[36];
  ctrl_t *ctrl = ctrl_get_active();

  /* Sub Filter Freq Cutoff
   * LEFT1:10
   * 0, 1,
   */

  _set_pwm_seq_lab(_sub_filt_freq_lab(ctrl), pwm_seq, (2 * 3));

  _set_scale_seq_animated(pwm_seq, scale_seq, (2 * 3), 7 + SUB_PATTERN_OFFSET,
                          false);

  /* Sub Filter Out
   * LEFT1:10
   * 2, 3, 4, 5, 6, 7, 8
   */

  _set_pwm_seq_lab(_sub_filt_reso_lab(ctrl), pwm_seq + (2 * 3), 7 * 3);

  _set_scale_seq_animated(pwm_seq + (2 * 3), scale_seq + (2 * 3), 7 * 3,
                          9 + SUB_PATTERN_OFFSET, false);

  bool res = is32_set(bus, 1, 0b10, pwm_seq, scale_seq, callback, userdata);
  if (!res)
    Error_Handler();
}

#define DEFAULT_BRIGHTNESS 0x15
#define HALF_BRIGHTNESS 0x04

void _i2c_resume_left_rgbled_1_11(uint8_t bus, i2c_callback_t callback,
                                  void *userdata) {

  ctrl_toggle_t *toggle = ctrl_get_active_toggle();
  ctrl_t *ctrl = ctrl_get_active();

  /* Sub Filter ADSR Attack LEDs
   * LEFT1:11
   */

  uint8_t brightness = 0x80;
  switch (toggle->sub_filt_env_sustain_func) {
  case ENC_ENV_SUSTAIN:
    brightness = DEFAULT_BRIGHTNESS;
    break;
  case ENC_ENV_AMOUNT:
    brightness = HALF_BRIGHTNESS;
    break;
  default:
    Error_Handler();
  }

  /* Attack
   * [0,2=2][1,2=4][2,2=5]
   * [0,1=1][1,1=3]
   * [0,0=0]
   */

  uint8_t scale_seq[36] = {0};
  memset(scale_seq, brightness, 36);

  uint16_t pwm_seq[36] = {0x00};

  uint16_t a_val = 0;
  switch (toggle->sub_filt_env_attack_func) {
  case ENC_SELECT_ENV_1:
    a_val = ctrl->value[CTRL_SUB_FILT_ENV1_A];
    break;
  case ENC_SELECT_ENV_2:
    a_val = ctrl->value[CTRL_SUB_FILT_ENV2_A];
    break;
  default:
    break;
  }

  _adsr_led_set_grid_curve(a_val);

  pwm_seq[0] = grid[0][0];
  pwm_seq[1] = grid[0][1];
  pwm_seq[2] = grid[0][2];
  pwm_seq[3] = grid[1][1];
  pwm_seq[4] = grid[1][2];
  pwm_seq[5] = grid[2][2];

  /* Decay:
   * [0,2][1,2][2,2]
   * [0,1][1,1]
   * [0,0]
   *
   * transposes to:
   *
   * [0,0=08]
   * [0,1=07][1,1=10]
   * [0,2=06][1,2=09][2,2=11]
   */

  uint16_t d_val = 0;
  switch (toggle->sub_filt_env_attack_func) {
  case ENC_SELECT_ENV_1:
    d_val = ctrl->value[CTRL_SUB_FILT_ENV1_D];
    break;
  case ENC_SELECT_ENV_2:
    d_val = ctrl->value[CTRL_SUB_FILT_ENV2_D];
    break;
  default:
    break;
  }

  _adsr_led_set_grid_curve(d_val);

  pwm_seq[6] = grid[0][2];
  pwm_seq[7] = grid[0][1];
  pwm_seq[8] = grid[0][0];
  pwm_seq[9] = grid[1][2];
  pwm_seq[10] = grid[1][1];
  pwm_seq[11] = grid[2][2];

  /* Sustain:
   * [0,2=14=14][1,2=17=17]
   * [0,1=13=13][1,1=16=16]
   * [0,0=12=12][1,0=15=15]
   */

  uint16_t s_val = 0;
  switch (toggle->sub_filt_env_attack_func) {
  case ENC_SELECT_ENV_1:
    switch (toggle->sub_filt_env_sustain_func) {
    case ENC_ENV_SUSTAIN:
      s_val = ctrl->value[CTRL_SUB_FILT_ENV1_S];
      break;
    case ENC_ENV_AMOUNT:
      s_val = ctrl->value[CTRL_SUB_FILT_ENV1_AMT];
      break;
    default:
      break;
    }
    break;
  case ENC_SELECT_ENV_2:
    switch (toggle->sub_filt_env_sustain_func) {
    case ENC_ENV_SUSTAIN:
      s_val = ctrl->value[CTRL_SUB_FILT_ENV2_S];
      break;
    case ENC_ENV_AMOUNT:
      s_val = ctrl->value[CTRL_SUB_FILT_ENV2_AMT];
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }

  switch (toggle->sub_filt_env_sustain_func) {
  case ENC_ENV_SUSTAIN:
    _adsr_led_set_grid_bar(s_val);
    break;
  case ENC_ENV_AMOUNT:
    _adsr_led_set_grid_stack(s_val);
    break;
  default:
    break;
  }

  pwm_seq[12] = grid[0][0];
  pwm_seq[13] = grid[0][1];
  pwm_seq[14] = grid[0][2];
  pwm_seq[15] = grid[1][0];
  pwm_seq[16] = grid[1][1];
  pwm_seq[17] = grid[1][2];

  /* Release:
   * [0,2][1,2][2,2]
   * [0,1][1,1]
   * [0,0]
   *
   * transposes to:
   *
   * [0,0=20]
   * [0,1=19][1,1=22]
   * [0,2=18][1,2=21][2,2=23]
   */

  uint16_t r_val = 0;
  switch (toggle->sub_filt_env_attack_func) {
  case ENC_SELECT_ENV_1:
    r_val = ctrl->value[CTRL_SUB_FILT_ENV1_R];
    break;
  case ENC_SELECT_ENV_2:
    r_val = ctrl->value[CTRL_SUB_FILT_ENV2_R];
    break;
  default:
    break;
  }

  _adsr_led_set_grid_curve(r_val);

  pwm_seq[18] = grid[0][2];
  pwm_seq[19] = grid[0][1];
  pwm_seq[20] = grid[0][0];
  pwm_seq[21] = grid[1][2];
  pwm_seq[22] = grid[1][1];
  pwm_seq[23] = grid[2][2];

  /* Write */

  bool res = is32_set(bus, 1, 0b11, pwm_seq, scale_seq, callback, userdata);
  if (!res)
    Error_Handler();
}

void _i2c_resume_left_rgbled_3_00(uint8_t bus, i2c_callback_t callback,
                                  void *userdata) {

  uint16_t pwm_seq[36] = {0};
  uint8_t scale_seq[36] = {0};
  ctrl_t *ctrl = ctrl_get_active();

  /* Osc2 Filt
   * LEFT3:00
   * Freq: 0, 1
   */

  _set_pwm_seq_lab(_osc2_filt_freq_lab(ctrl), pwm_seq, (2 * 3));

  _set_scale_seq_animated(pwm_seq, scale_seq, 2 * 3, 8 + OSC2_PATTERN_OFFSET,
                          false);

  /* Osc2 Filt
   * LEFT3:00
   * Reso: 2, 3
   */

  _set_pwm_seq_lab(_osc2_filt_reso_lab(ctrl), pwm_seq + (2 * 3), (2 * 3));

  _set_scale_seq_animated(pwm_seq + (2 * 3), scale_seq + (2 * 3), 2 * 3,
                          10 + OSC2_PATTERN_OFFSET, false);

  /* SubToOsc2 Mix Level
   * LEFT3:00
   * 7, 8
   */

  _set_pwm_seq_lab(_sub_to_osc2_mix_lab(ctrl), pwm_seq + (7 * 3), 2 * 3);

  _set_scale_seq_animated(pwm_seq + (7 * 3), scale_seq + (7 * 3), 2 * 3,
                          6 + SUB_PATTERN_OFFSET, false);

  /* Write */
  bool res = is32_set(bus, 3, 0b00, pwm_seq, scale_seq, callback, userdata);
  if (!res)
    Error_Handler();
}

void _i2c_resume_left_rgbled_3_01(uint8_t bus, i2c_callback_t callback,
                                  void *userdata) {
  uint16_t pwm_seq[36] = {0};
  uint8_t scale_seq[36] = {0};
  ctrl_t *ctrl = ctrl_get_active();
  ctrl_toggle_t *toggle = ctrl_get_active_toggle();

  /* Osc Filter ADSR Attack LEDs
   * LEFT3:01
   */

  uint8_t brightness = 0x80;
  switch (toggle->osc_filt_env_sustain_func) {
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

  /* Attack: 0, 1, 2, 3, 4, 5
   * [0,2=3][1,2=1][2,2=0]
   * [0,1=4][1,1=2]
   * [0,0=5]
   */

  uint16_t a_val = 0;
  switch (toggle->osc_filt_env_attack_func) {
  case ENC_SELECT_ENV_1:
    a_val = ctrl->value[CTRL_OSC_FILT_ENV1_A];
    break;
  case ENC_SELECT_ENV_2:
    a_val = ctrl->value[CTRL_OSC_FILT_ENV2_A];
    break;
  default:
    break;
  }

  _adsr_led_set_grid_curve(a_val);
  pwm_seq[0] = grid[2][2];
  pwm_seq[1] = grid[1][2];
  pwm_seq[2] = grid[1][1];
  pwm_seq[3] = grid[0][2];
  pwm_seq[4] = grid[0][1];
  pwm_seq[5] = grid[0][0];

  /* Decay: 24, 25, 26, 27, 28, 29
   * [0,2][1,2][2,2]
   * [0,1][1,1]
   * [0,0]
   *
   * transposes to:
   *
   * [0,0=27]
   * [0,1=28][1,1=25]
   * [0,2=29][1,2=26][2,2=24]
   */

  uint16_t d_val = 0;
  switch (toggle->osc_filt_env_attack_func) {
  case ENC_SELECT_ENV_1:
    d_val = ctrl->value[CTRL_OSC_FILT_ENV1_D];
    break;
  case ENC_SELECT_ENV_2:
    d_val = ctrl->value[CTRL_OSC_FILT_ENV2_D];
    break;
  default:
    break;
  }

  _adsr_led_set_grid_curve(d_val);
  pwm_seq[24] = grid[2][2];
  pwm_seq[25] = grid[1][1];
  pwm_seq[26] = grid[1][2];
  pwm_seq[27] = grid[0][0];
  pwm_seq[28] = grid[0][1];
  pwm_seq[29] = grid[0][2];

  /* Sustain: 18, 19, 20, 21, 22, 23
   * [0,2=14=23][1,2=17=18]
   * [0,1=13=22][1,1=16=19]
   * [0,0=12=21][1,0=15=20]
   */

  uint16_t s_val = 0;
  switch (toggle->osc_filt_env_attack_func) {
  case ENC_SELECT_ENV_1:
    switch (toggle->osc_filt_env_sustain_func) {
    case ENC_ENV_SUSTAIN:
      s_val = ctrl->value[CTRL_OSC_FILT_ENV1_S];
      break;
    case ENC_ENV_AMOUNT:
      s_val = ctrl->value[CTRL_OSC_FILT_ENV1_AMT];
      break;
    default:
      break;
    }
    break;
  case ENC_SELECT_ENV_2:
    switch (toggle->osc_filt_env_sustain_func) {
    case ENC_ENV_SUSTAIN:
      s_val = ctrl->value[CTRL_OSC_FILT_ENV2_S];
      break;
    case ENC_ENV_AMOUNT:
      s_val = ctrl->value[CTRL_OSC_FILT_ENV2_AMT];
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }

  switch (toggle->osc_filt_env_sustain_func) {
  case ENC_ENV_SUSTAIN:
    _adsr_led_set_grid_bar(s_val);
    break;
  case ENC_ENV_AMOUNT:
    _adsr_led_set_grid_stack(s_val);
    break;
  default:
    break;
  }

  // Sustain graph is always DEFAULT_BRIGHTNESS
  scale_seq[18] = DEFAULT_BRIGHTNESS;
  scale_seq[19] = DEFAULT_BRIGHTNESS;
  scale_seq[20] = DEFAULT_BRIGHTNESS;
  scale_seq[21] = DEFAULT_BRIGHTNESS;
  scale_seq[22] = DEFAULT_BRIGHTNESS;
  scale_seq[23] = DEFAULT_BRIGHTNESS;

  pwm_seq[18] = grid[1][2];
  pwm_seq[19] = grid[1][1];
  pwm_seq[20] = grid[1][0];
  pwm_seq[21] = grid[0][0];
  pwm_seq[22] = grid[0][1];
  pwm_seq[23] = grid[0][2];

  /* Release: 12, 13, 14, 15, 16, 17
   * [0,2][1,2][2,2]
   * [0,1][1,1]
   * [0,0]
   *
   * transposes to:
   *
   * [0,0=20=17]
   * [0,1=19=16][1,1=22=13]
   * [0,2=18=15][1,2=21=14][2,2=23=12]
   */

  uint16_t r_val = 0;
  switch (toggle->osc_filt_env_attack_func) {
  case ENC_SELECT_ENV_1:
    r_val = ctrl->value[CTRL_OSC_FILT_ENV1_R];
    break;
  case ENC_SELECT_ENV_2:
    r_val = ctrl->value[CTRL_OSC_FILT_ENV2_R];
    break;
  default:
    break;
  }

  _adsr_led_set_grid_curve(r_val);
  pwm_seq[12] = grid[2][2];
  pwm_seq[13] = grid[1][1];
  pwm_seq[14] = grid[1][2];
  pwm_seq[15] = grid[0][2];
  pwm_seq[16] = grid[0][1];
  pwm_seq[17] = grid[0][0];

  /* Write */
  bool res = is32_set(bus, 3, 0b01, pwm_seq, scale_seq, callback, userdata);
  if (!res)
    Error_Handler();
}

void _i2c_resume_left_rotpic_0_000(uint8_t bus, i2c_callback_t callback,
                                   void *userdata) {
  // LEFT0:000
  ctrl_toggle_t *toggle = ctrl_get_active_toggle();
  uint8_t led = 0x0;
  led = 0x00;
  led |= (toggle->osc1_squ_func == ENC_OSC_SQU_LEVEL) << ROTPIC_LED1_BIT_SHIFT;
  led |= (toggle->osc1_squ_func == ENC_OSC_SQU_PWM) << ROTPIC_LED2_BIT_SHIFT;
  bool res = rotpic_led_set_state(I2C_LEFT, 0, 0b000, led, callback, userdata);
  if (!res)
    Error_Handler();
}

void _i2c_resume_left_rotpic_0_011(uint8_t bus, i2c_callback_t callback,
                                   void *userdata) {
  // LEFT0:011
  ctrl_toggle_t *toggle = ctrl_get_active_toggle();
  uint8_t led = 0x0;
  led = 0x00;
  led |= (toggle->osc2_squ_func == ENC_OSC_SQU_LEVEL) << ROTPIC_LED1_BIT_SHIFT;
  led |= (toggle->osc2_squ_func == ENC_OSC_SQU_PWM) << ROTPIC_LED2_BIT_SHIFT;
  bool res = rotpic_led_set_state(I2C_LEFT, 0, 0b011, led, callback, userdata);
  if (!res)
    Error_Handler();
}

void _i2c_resume_left_rotpic_0_001(uint8_t bus, i2c_callback_t callback,
                                   void *userdata) {
  // LEFT0:001
  ctrl_toggle_t *toggle = ctrl_get_active_toggle();
  uint8_t led = 0x0;
  led = 0x00;
  led |= (toggle->osc_filt_env_attack_func == ENC_SELECT_ENV_1)
         << ROTPIC_LED1_BIT_SHIFT;
  led |= (toggle->osc_filt_env_attack_func == ENC_SELECT_ENV_2)
         << ROTPIC_LED2_BIT_SHIFT;
  led |= (toggle->osc_filt_env_sustain_func == ENC_ENV_SUSTAIN)
         << ROTPIC_LED4_BIT_SHIFT;
  led |= (toggle->osc_filt_env_sustain_func == ENC_ENV_AMOUNT)
         << ROTPIC_LED3_BIT_SHIFT;
  bool res = rotpic_led_set_state(I2C_LEFT, 0, 0b001, led, callback, userdata);
  if (!res)
    Error_Handler();
}

void _i2c_resume_left_rotpic_2_000(uint8_t bus, i2c_callback_t callback,
                                   void *userdata) {
  // LEFT2:000
  ctrl_toggle_t *toggle = ctrl_get_active_toggle();
  uint8_t led = 0x0;
  led |= (toggle->sub_filt_env_attack_func == ENC_SELECT_ENV_1)
         << ROTPIC_LED1_BIT_SHIFT;
  led |= (toggle->sub_filt_env_attack_func == ENC_SELECT_ENV_2)
         << ROTPIC_LED2_BIT_SHIFT;
  led |= (toggle->sub_filt_env_sustain_func == ENC_ENV_SUSTAIN)
         << ROTPIC_LED3_BIT_SHIFT;
  led |= (toggle->sub_filt_env_sustain_func == ENC_ENV_AMOUNT)
         << ROTPIC_LED4_BIT_SHIFT;
  bool res = rotpic_led_set_state(I2C_LEFT, 2, 0b000, led, callback, userdata);
  if (!res)
    Error_Handler();
}

void _i2c_resume_left_dac_0_0_0(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(I2C_LEFT, 0, 0, 0, ctrl->value[CTRL_OSC1_FILT_RES],
                    callback, userdata);
}

void _i2c_resume_left_dac_0_0_1(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  note_t *note = note_get_active();
  dac7678_set_value(
      I2C_LEFT, 0, 0, 1,
      _commit_filt_cutoff_dac_value(
          note, ctrl->value[CTRL_OSC1_FILT_CUTOFF],
          ctrl->value[CTRL_OSC_FILT_ENV1_A], ctrl->value[CTRL_OSC_FILT_ENV1_D],
          ctrl->value[CTRL_OSC_FILT_ENV1_S], ctrl->value[CTRL_OSC_FILT_ENV1_R],
          ctrl->value[CTRL_OSC_FILT_ENV1_AMT], 0, 0, 0, 0, 0),
      callback, userdata);
}

void _i2c_resume_left_dac_0_0_2(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  note_t *note = note_get_active();
  uint8_t osc1_note =
      note->value.note_number + (12 - ctrl->value[CTRL_OSC1_TUNE_COARSE]);
  uint16_t osc1_note_dac_val = osc_dac_value_for_note(OSC1, osc1_note);
  osc1_note_dac_val +=
      ((int16_t)CTRL_DEFAULT_MID -
       ctrl->value[CTRL_OSC1_TUNE_FINE]); // TODO: Handle wrapping, maybe
                                          // add it to osc1_note_dac_val?
  dac7678_set_value(I2C_LEFT, 0, 0, 2, osc1_note_dac_val, callback, userdata);
}

void _i2c_resume_left_dac_0_0_3(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(I2C_LEFT, 0, 0, 3,
                    _vca_lin_to_log(ctrl->value[CTRL_OSC1_TO_OSC1_MIX]),
                    callback, userdata);
}

void _i2c_resume_left_dac_0_0_4(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(I2C_LEFT, 0, 0, 4,
                    _vca_lin_to_log(ctrl->value[CTRL_OSC1_TO_OSC2_MIX]),
                    callback, userdata);
}

void _i2c_resume_left_dac_0_0_5(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(I2C_LEFT, 0, 0, 5, ctrl->value[CTRL_OSC1_SQU_PWM], callback,
                    userdata);
}

void _i2c_resume_left_dac_0_0_6(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(I2C_LEFT, 0, 0, 6,
                    _vca_lin_to_log(ctrl->value[CTRL_OSC1_SAW_LVL]), callback,
                    userdata);
}

void _i2c_resume_left_dac_0_0_7(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(I2C_LEFT, 0, 0, 7,
                    _vca_lin_to_log(ctrl->value[CTRL_OSC1_SQU_LVL]), callback,
                    userdata);
}

void _i2c_resume_left_dac_0_2_0(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(I2C_LEFT, 0, 2, 0,
                    _vca_lin_to_log(ctrl->value[CTRL_OSC2_NOISE_LVL]), callback,
                    userdata);
}

void _i2c_resume_left_dac_0_2_1(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(I2C_LEFT, 0, 2, 1,
                    _vca_lin_to_log(ctrl->value[CTRL_SUB_NOISE_LVL]), callback,
                    userdata);
}

void _i2c_resume_left_dac_0_2_2(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(I2C_LEFT, 0, 2, 2,
                    _vca_lin_to_log(ctrl->value[CTRL_SUB_LVL]), callback,
                    userdata);
}

void _i2c_resume_left_dac_0_2_3(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(I2C_LEFT, 0, 2, 3,
                    _vca_lin_to_log(ctrl->value[CTRL_SUB_TO_OSC2_MIX]),
                    callback, userdata);
}

void _i2c_resume_left_dac_0_2_4(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  note_t *note = note_get_active();
  dac7678_set_value(I2C_LEFT, 0, 2, 4,
                    osc_dac_value_for_note(OSC2, note->value.note_number),
                    callback, userdata);
}

void _i2c_resume_left_dac_0_2_5(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(I2C_LEFT, 0, 2, 5, ctrl->value[CTRL_OSC2_SQU_PWM], callback,
                    userdata);
}

void _i2c_resume_left_dac_0_2_6(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(I2C_LEFT, 0, 2, 6,
                    _vca_lin_to_log(ctrl->value[CTRL_OSC2_SQU_LVL]), callback,
                    userdata);
}

void _i2c_resume_left_dac_0_2_7(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(I2C_LEFT, 0, 2, 7,
                    _vca_lin_to_log(ctrl->value[CTRL_OSC2_SAW_LVL]), callback,
                    userdata);
}

void _i2c_resume_left_dac_0_4_2(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(
      I2C_LEFT, 0, 4, 2,
      _env_amt_lin_to_log(CTRL_DEFAULT_MAX - ctrl->value[CTRL_OSC_AMP_ENV_AMT]),
      callback, userdata);
}

void _i2c_resume_left_dac_0_4_3(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(
      I2C_LEFT, 0, 4, 3,
      _env_amt_lin_to_log(CTRL_DEFAULT_MAX - ctrl->value[CTRL_SUB_AMP_ENV_AMT]),
      callback, userdata);
}

void _i2c_resume_left_dac_0_4_6(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(I2C_LEFT, 0, 4, 6,
                    _vca_lin_to_log(ctrl->value[CTRL_FX_WETDRY]), callback,
                    userdata);
}

void _i2c_resume_left_dac_0_4_7(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(I2C_LEFT, 0, 4, 7,
                    _vca_lin_to_log(ctrl->value[CTRL_FX_FEEDBACK]), callback,
                    userdata);
}

void _i2c_resume_left_dac_2_0_0(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(I2C_LEFT, 2, 0, 0, ctrl->value[CTRL_OSC2_FILT_RES],
                    callback, userdata);
}
void _i2c_resume_left_dac_2_0_2(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  note_t *note = note_get_active();
  dac7678_set_value(
      I2C_LEFT, 2, 0, 2,
      _commit_filt_cutoff_dac_value(
          note, ctrl->value[CTRL_SUB_FILT_CUTOFF],
          ctrl->value[CTRL_SUB_FILT_ENV1_A], ctrl->value[CTRL_SUB_FILT_ENV1_D],
          ctrl->value[CTRL_SUB_FILT_ENV1_S], ctrl->value[CTRL_SUB_FILT_ENV1_R],
          ctrl->value[CTRL_SUB_FILT_ENV1_AMT],
          ctrl->value[CTRL_SUB_FILT_ENV2_A], ctrl->value[CTRL_SUB_FILT_ENV2_D],
          ctrl->value[CTRL_SUB_FILT_ENV2_S], ctrl->value[CTRL_SUB_FILT_ENV2_R],
          ctrl->value[CTRL_SUB_FILT_ENV2_AMT]),
      callback, userdata);
}
void _i2c_resume_left_dac_2_0_4(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  dac7678_set_value(I2C_LEFT, 2, 0, 4, ctrl->value[CTRL_SUB_FILT_RES], callback,
                    userdata);
}

void _i2c_resume_left_dac_2_0_5(uint8_t bus, i2c_callback_t callback,
                                void *userdata) {
  ctrl_t *ctrl = ctrl_get_active();
  note_t *note = note_get_active();
  dac7678_set_value(
      I2C_LEFT, 2, 0, 5,
      _commit_filt_cutoff_dac_value(
          note, ctrl->value[CTRL_OSC2_FILT_CUTOFF],
          ctrl->value[CTRL_OSC_FILT_ENV2_A], ctrl->value[CTRL_OSC_FILT_ENV2_D],
          ctrl->value[CTRL_OSC_FILT_ENV2_S], ctrl->value[CTRL_OSC_FILT_ENV2_R],
          ctrl->value[CTRL_OSC_FILT_ENV2_AMT], 0, 0, 0, 0, 0),
      callback, userdata);
}

void _i2c_resume_left_bus(uint8_t bus, i2c_callback_t callback,
                          void *userdata) {
  static uint8_t cycle = I2C_LEFT_START;

  cycle++;

  switch (cycle) {
  case I2C_LEFT_RGBLED_0_00:
    // Osc1 Saw, Squ and Mix
    _i2c_resume_left_rgbled_0_00(bus, callback, userdata);
    break;
  case I2C_LEFT_RGBLED_0_01:
    // Tuning LEDs
    _i2c_resume_left_rgbled_0_01(bus, callback, userdata);
    break;
  case I2C_LEFT_RGBLED_0_10:
    // Osc1 Filter Freq & Res, and Osc1-to-2 Mix
    _i2c_resume_left_rgbled_0_10(bus, callback, userdata);
    break;
  case I2C_LEFT_RGBLED_0_11:
    // Osc2 Saw, Squ, Noise, Osc2-Only, Osc2 Pre-Filt
    _i2c_resume_left_rgbled_0_11(bus, callback, userdata);
    break;
  case I2C_LEFT_RGBLED_1_00:
    // Sub Squ, Noise, Mix, and Start LED
    _i2c_resume_left_rgbled_1_00(bus, callback, userdata);
    break;
  case I2C_LEFT_RGBLED_1_01:
    // Sequencer Step 1 to 12
    _i2c_resume_left_rgbled_1_01(bus, callback, userdata);
    break;
  case I2C_LEFT_RGBLED_1_10:
    // Sub Filter Freq and Reso
    _i2c_resume_left_rgbled_1_10(bus, callback, userdata);
    break;
  case I2C_LEFT_RGBLED_1_11:
    // Sub Filter Env ADSR
    _i2c_resume_left_rgbled_1_11(bus, callback, userdata);
    break;
  case I2C_LEFT_RGBLED_3_00:
    //  Osc2 Filt, Reso, and Sub to Osc2 Mix
    _i2c_resume_left_rgbled_3_00(bus, callback, userdata);
    break;
  case I2C_LEFT_RGBLED_3_01:
    // Osc2 Filter ADSR
    _i2c_resume_left_rgbled_3_01(bus, callback, userdata);
    break;
  case I2C_LEFT_ROTPIC_0_000:
    // Osc1 Square Level/PWM Toggle
    _i2c_resume_left_rotpic_0_000(bus, callback, userdata);
    break;
  case I2C_LEFT_ROTPIC_0_001:
    // Osc Envelope Toggles
    _i2c_resume_left_rotpic_0_001(bus, callback, userdata);
    break;
  case I2C_LEFT_ROTPIC_0_011:
    // Osc2 Square Level/PWM Toggle
    _i2c_resume_left_rotpic_0_011(bus, callback, userdata);
    break;
  case I2C_LEFT_ROTPIC_2_000:
    // Sub Envelope Toggles
    _i2c_resume_left_rotpic_2_000(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_0_0:
    // Osc1 Filt Resonance
    _i2c_resume_left_dac_0_0_0(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_0_1:
    // Osc1 Filt CUTOFF
    _i2c_resume_left_dac_0_0_1(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_0_2:
    // Osc1 Note
    _i2c_resume_left_dac_0_0_2(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_0_3:
    // Osc1 to Osc1 Mix
    _i2c_resume_left_dac_0_0_3(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_0_4:
    // Osc1 to Osc2 Mix
    _i2c_resume_left_dac_0_0_4(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_0_5:
    // Osc1 Square PWM
    _i2c_resume_left_dac_0_0_5(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_0_6:
    // Osc1 Saw Level
    _i2c_resume_left_dac_0_0_6(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_0_7:
    // Osc1 Square Level
    _i2c_resume_left_dac_0_0_7(bus, callback, userdata);
    break;

  case I2C_LEFT_DAC_0_2_0:
    // Osc2 Noise Level
    _i2c_resume_left_dac_0_2_0(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_2_1:
    // Sub Noise Level
    _i2c_resume_left_dac_0_2_1(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_2_2:
    // Sub Level
    _i2c_resume_left_dac_0_2_2(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_2_3:
    // Sub to Osc 2 Mix
    _i2c_resume_left_dac_0_2_3(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_2_4:
    // Osc2 Note
    _i2c_resume_left_dac_0_2_4(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_2_5:
    // Osc2 Square PWM
    _i2c_resume_left_dac_0_2_5(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_2_6:
    // Osc2 Square Level
    _i2c_resume_left_dac_0_2_6(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_2_7:
    // Osc2 Saw Level
    _i2c_resume_left_dac_0_2_7(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_4_2:
    // Osc Amp Envelope Amount
    _i2c_resume_left_dac_0_4_2(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_4_3:
    // Sub Amp Envelope Amount
    _i2c_resume_left_dac_0_4_3(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_4_6:
    // FX Wet/Dry
    _i2c_resume_left_dac_0_4_6(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_0_4_7:
    // FX Feedback
    _i2c_resume_left_dac_0_4_7(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_2_0_0:
    // Osc2 Filt Resonance
    _i2c_resume_left_dac_2_0_0(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_2_0_2:
    // Sub Filt Cutoff
    _i2c_resume_left_dac_2_0_2(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_2_0_4:
    // Sub Filt Resonance
    _i2c_resume_left_dac_2_0_4(bus, callback, userdata);
    break;
  case I2C_LEFT_DAC_2_0_5:
    // Osc2 Filt Cutoff
    _i2c_resume_left_dac_2_0_5(bus, callback, userdata);
    break;

  default:
    cycle = I2C_LEFT_START;
    increment_pattern_step();
    callback(bus, userdata);
  }
}
