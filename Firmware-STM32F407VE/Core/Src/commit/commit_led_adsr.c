/*
 * commit_led_adsr.c
 *
 *  Created on: 3 May 2021
 *      Author: jwilson
 */

#include "commit.h"
#include "ctrl.h"
#include "i2c.h"
#include "is32.h"
#include "main.h"

#define MASK_2_MSB 0b110000000000
#define MASK_10_LSB 0b1111111111

#define THIRD_1 0b010000000000
#define THIRD_2 0b100000000000
#define THIRD_3 0b110000000000

#define MAX_PWM 0xFFFF // 12bit

static uint16_t grid[3][3];

void _adsr_led_set_grid_curve(uint16_t val) {
  /* Produces graph like this:
   *
   * [0,2][1,2][2,2]
   * [0,1][1,1]
   * [0,0]
   *
   */

  grid[0][0] = 0;
  grid[0][1] = 0;
  grid[0][2] = 0;
  grid[1][0] = 0;
  grid[1][1] = 0;
  grid[1][2] = 0;
  grid[2][0] = 0;
  grid[2][1] = 0;
  grid[2][2] = 0;

  grid[0][0] = MAX_PWM; // Always on

  double v = val / 4095.0;

  if (v < 0.33) {
    // 0% - 33%
    // [0,2] -> [1,2]
    double d = v / 0.33;
    grid[0][2] = MAX_PWM * (1.0 - d);
    grid[1][2] = MAX_PWM * d;
    grid[0][1] = MAX_PWM;
  } else if (v < 0.66) {
    // 33% - 66%
    // [0,1] -> [1,1]
    double d = (v - 0.33) / 0.33;
    grid[0][1] = MAX_PWM * (1.0 - d);
    grid[1][1] = MAX_PWM * d;
    grid[1][2] = MAX_PWM;
  } else {
    // 66% - 100%
    // [1,2] -> [2,2]
    double d = (v - 0.66) / 0.33;
    if (d > 1.0)
      d = 1.0;
    grid[1][2] = MAX_PWM * (1.0 - d);
    grid[2][2] = MAX_PWM * d;
    grid[1][1] = MAX_PWM;
  }
}

void _adsr_led_set_grid_stack(uint16_t val) {
  /* Produces graph like this:
   *
   * [0,2][1,2]
   * [0,1][1,1]
   * [0,0][1,0]
   *
   */

  grid[0][0] = 0;
  grid[0][1] = 0;
  grid[0][2] = 0;
  grid[1][0] = 0;
  grid[1][1] = 0;
  grid[1][2] = 0;
  grid[2][0] = 0;
  grid[2][1] = 0;
  grid[2][2] = 0;

  double v = val / 4095.0;

  if (v < 0.33) {
    // 0% - 33%
    // [0,2] -> [1,2]
    double d = v / 0.33;
    grid[0][0] = MAX_PWM * d;
    grid[1][0] = MAX_PWM * d;
  } else if (v < 0.66) {
    // 33% - 66%
    // [0,1] -> [1,1]
    double d = (v - 0.33) / 0.33;
    grid[0][0] = MAX_PWM;
    grid[1][0] = MAX_PWM;
    grid[0][1] = MAX_PWM * d;
    grid[1][1] = MAX_PWM * d;
  } else {
    // 66% - 100%
    // [2,1] -> [2,2]
    double d = (v - 0.66) / 0.33;
    if (d > 1.0)
      d = 1.0;
    grid[0][0] = MAX_PWM;
    grid[1][0] = MAX_PWM;
    grid[0][1] = MAX_PWM;
    grid[1][1] = MAX_PWM;
    grid[0][2] = MAX_PWM * d;
    grid[1][2] = MAX_PWM * d;
  }
}

void _adsr_led_set_grid_bar(uint16_t val) {
  /* Produces graph like this:
   *
   * [0,2][1,2]
   * [0,1][1,1]
   * [0,0][1,0]
   *
   */

  double v = val / 4095.0;

  grid[0][0] = 0;
  grid[0][1] = 0;
  grid[0][2] = 0;
  grid[1][0] = 0;
  grid[1][1] = 0;
  grid[1][2] = 0;
  grid[2][0] = 0;
  grid[2][1] = 0;
  grid[2][2] = 0;

  if (v < 0.5) {
    // 0% - 50%
    // [0,0][1,0] -> [0,1][1,1]
    double d = v / 0.50;
    grid[0][0] = MAX_PWM * (1.0 - d);
    grid[1][0] = MAX_PWM * (1.0 - d);
    grid[0][1] = MAX_PWM * d;
    grid[1][1] = MAX_PWM * d;
  } else {
    // 50-100%
    // [0,1][1,1] -> [0,2][1,2]
    double d = (v - 0.50) / 0.50;
    grid[0][1] = MAX_PWM * (1.0 - d);
    grid[1][1] = MAX_PWM * (1.0 - d);
    grid[0][2] = MAX_PWM * d;
    grid[1][2] = MAX_PWM * d;
  }
}

#define DEFAULT_BRIGHTNESS 0x15
#define HALF_BRIGHTNESS 0x04

/*
 * Osc Filter Envelope
 */

void _commit_led_adsr_osc_filt_env_a(ctrl_t *ctrl, ctrl_toggle_t *toggle) {
  /* Osc Filter ADSR Attack LEDs
   * LEFT3:001
   */

  if (!ctrl->changed[CTRL_OSC_FILT_ENV1_A] &&
      !ctrl->changed[CTRL_OSC_FILT_ENV2_A]) {
    return;
  }

  uint8_t brightness = 0x80;
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

  /* [0,2=3][1,2=1][2,2=0]
   * [0,1=4][1,1=2]
   * [0,0=5]
   */

  _adsr_led_set_grid_curve(a_val);
  uint16_t pwm_seq[6] = {grid[2][2], grid[1][2], grid[1][1],
                         grid[0][2], grid[0][1], grid[0][0]};
  uint8_t scale_seq[6] = {brightness, brightness, brightness,
                          brightness, brightness, brightness};

  bool res;
  res = is32_set_sequence_pwm(I2C_LEFT, 3, 1, 0, pwm_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_set_sequence_scale(I2C_LEFT, 3, 1, 0, scale_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_write_registers(I2C_LEFT, 3, 1);
  if (!res)
    Error_Handler();
}

void _commit_led_adsr_osc_filt_env_d(ctrl_t *ctrl, ctrl_toggle_t *toggle) {
  /* Osc Filter ADSR Decay LEDs
   * LEFT3:001
   */

  if (!ctrl->changed[CTRL_OSC_FILT_ENV1_D] &&
      !ctrl->changed[CTRL_OSC_FILT_ENV2_D]) {
    return;
  }

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

  /* [0,2][1,2][2,2]
   * [0,1][1,1]
   * [0,0]
   *
   * transposes to:
   *
   * [0,0=27]
   * [0,1=28][1,1=25]
   * [0,2=29][1,2=26][2,2=24]
   */

  _adsr_led_set_grid_curve(d_val);
  uint16_t pwm_seq[6] = {grid[2][2], grid[1][1], grid[1][2],
                         grid[0][0], grid[0][1], grid[0][2]};
  uint8_t scale_seq[6] = {brightness, brightness, brightness,
                          brightness, brightness, brightness};

  bool res;
  res = is32_set_sequence_pwm(I2C_LEFT, 3, 1, 24, pwm_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_set_sequence_scale(I2C_LEFT, 3, 1, 24, scale_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_write_registers(I2C_LEFT, 3, 1);
  if (!res)
    Error_Handler();
}

void _commit_led_adsr_osc_filt_env_s(ctrl_t *ctrl, ctrl_toggle_t *toggle) {
  /* Osc Filter ADSR Sustain LEDs
   * LEFT3:001
   */

  if (!ctrl->changed[CTRL_OSC_FILT_ENV1_S] &&
      !ctrl->changed[CTRL_OSC_FILT_ENV1_AMT] &&
      !ctrl->changed[CTRL_OSC_FILT_ENV2_S] &&
      !ctrl->changed[CTRL_OSC_FILT_ENV2_AMT]) {
    return;
  }

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

  /* [0,2=14=23][1,2=17=18]
   * [0,1=13=22][1,1=16=19]
   * [0,0=12=21][1,0=15=20]
   */

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
  uint8_t brightness = DEFAULT_BRIGHTNESS;
  uint16_t pwm_seq[6] = {grid[1][2], grid[1][1], grid[1][0],
                         grid[0][0], grid[0][1], grid[0][2]};
  uint8_t scale_seq[6] = {brightness, brightness, brightness,
                          brightness, brightness, brightness};

  bool res;
  res = is32_set_sequence_pwm(I2C_LEFT, 3, 1, 18, pwm_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_set_sequence_scale(I2C_LEFT, 3, 1, 18, scale_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_write_registers(I2C_LEFT, 3, 1);
  if (!res)
    Error_Handler();
}

void _commit_led_adsr_osc_filt_env_r(ctrl_t *ctrl, ctrl_toggle_t *toggle) {
  /* Osc Filter ADSR Release LEDs
   * LEFT3:001
   */

  if (!ctrl->changed[CTRL_OSC_FILT_ENV1_R] &&
      !ctrl->changed[CTRL_OSC_FILT_ENV2_R]) {
    return;
  }

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

  /* [0,2][1,2][2,2]
   * [0,1][1,1]
   * [0,0]
   *
   * transposes to:
   *
   *
   * [0,0=20=17]
   * [0,1=19=16][1,1=22=13]
   * [0,2=18=15][1,2=21=14][2,2=23=12]
   */

  _adsr_led_set_grid_curve(r_val);

  uint16_t pwm_seq[6] = {grid[2][2], grid[1][1], grid[1][2],
                         grid[0][2], grid[0][1], grid[0][0]};
  uint8_t scale_seq[6] = {brightness, brightness, brightness,
                          brightness, brightness, brightness};

  bool res;
  res = is32_set_sequence_pwm(I2C_LEFT, 3, 1, 12, pwm_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_set_sequence_scale(I2C_LEFT, 3, 1, 12, scale_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_write_registers(I2C_LEFT, 3, 1);
  if (!res)
    Error_Handler();
}

/*
 * Sub Filter ADSR Envelope
 */

void _commit_led_adsr_sub_filt_env_a(ctrl_t *ctrl, ctrl_toggle_t *toggle) {
  /* Sub Filter ADSR Attack LEDs
   * LEFT1:11
   */

  if (!ctrl->changed[CTRL_SUB_FILT_ENV1_A] &&
      !ctrl->changed[CTRL_SUB_FILT_ENV2_A]) {
    return;
  }

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

  uint8_t brightness = 0x80;
  switch (toggle->sub_filt_env_sustain_func) {
  case ENC_ENV_SUSTAIN:
    brightness = DEFAULT_BRIGHTNESS;
    break;
  case ENC_ENV_AMOUNT:
    brightness = HALF_BRIGHTNESS;
    break;
  default:
    break;
  }

  /* [0,2=2][1,2=4][2,2=5]
   * [0,1=1][1,1=3]
   * [0,0=0]
   */

  _adsr_led_set_grid_curve(a_val);

  uint16_t pwm_seq[6] = {grid[0][0], grid[0][1], grid[0][2],
                         grid[1][1], grid[1][2], grid[2][2]};
  uint8_t scale_seq[6] = {brightness, brightness, brightness,
                          brightness, brightness, brightness};

  bool res;
  res = is32_set_sequence_pwm(I2C_LEFT, 1, 3, 0, pwm_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_set_sequence_scale(I2C_LEFT, 1, 3, 0, scale_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_write_registers(I2C_LEFT, 1, 3);
  if (!res)
    Error_Handler();
}

void _commit_led_adsr_sub_filt_env_d(ctrl_t *ctrl, ctrl_toggle_t *toggle) {
  /* Sub Filter Decay ADSR LEDs
   * LEFT1:11
   */

  if (!ctrl->changed[CTRL_SUB_FILT_ENV1_D] &&
      !ctrl->changed[CTRL_SUB_FILT_ENV2_D]) {
    return;
  }

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

  uint8_t brightness = 0x80;
  switch (toggle->sub_filt_env_sustain_func) {
  case ENC_ENV_SUSTAIN:
    brightness = DEFAULT_BRIGHTNESS;
    break;
  case ENC_ENV_AMOUNT:
    brightness = HALF_BRIGHTNESS;
    break;
  default:
    break;
  }

  /* [0,2][1,2][2,2]
   * [0,1][1,1]
   * [0,0]
   *
   * transposes to:
   *
   * [0,0=08]
   * [0,1=07][1,1=10]
   * [0,2=06][1,2=09][2,2=11]
   */

  _adsr_led_set_grid_curve(d_val);

  uint16_t pwm_seq[6] = {grid[0][2], grid[0][1], grid[0][0],
                         grid[1][2], grid[1][1], grid[2][2]};
  uint8_t scale_seq[6] = {brightness, brightness, brightness,
                          brightness, brightness, brightness};

  bool res;
  res = is32_set_sequence_pwm(I2C_LEFT, 1, 3, 6, pwm_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_set_sequence_scale(I2C_LEFT, 1, 3, 6, scale_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_write_registers(I2C_LEFT, 1, 3);
  if (!res)
    Error_Handler();
}

void _commit_led_adsr_sub_filt_env_s(ctrl_t *ctrl, ctrl_toggle_t *toggle) {
  /* Sub Filter ADSR Sustain LEDs
   * LEFT1:11
   */

  if (!ctrl->changed[CTRL_SUB_FILT_ENV1_S] &&
      !ctrl->changed[CTRL_SUB_FILT_ENV1_AMT] &&
      !ctrl->changed[CTRL_SUB_FILT_ENV2_S] &&
      !ctrl->changed[CTRL_SUB_FILT_ENV2_AMT]) {
    return;
  }

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

  /* [0,2=14=14][1,2=17=17]
   * [0,1=13=13][1,1=16=16]
   * [0,0=12=12][1,0=15=15]
   */

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

  uint16_t pwm_seq[6] = {grid[0][0], grid[0][1], grid[0][2],
                         grid[1][0], grid[1][1], grid[1][2]};
  uint8_t scale_seq[6] = {DEFAULT_BRIGHTNESS, DEFAULT_BRIGHTNESS,
                          DEFAULT_BRIGHTNESS, DEFAULT_BRIGHTNESS,
                          DEFAULT_BRIGHTNESS, DEFAULT_BRIGHTNESS};

  bool res;
  res = is32_set_sequence_pwm(I2C_LEFT, 1, 3, 12, pwm_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_set_sequence_scale(I2C_LEFT, 1, 3, 12, scale_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_write_registers(I2C_LEFT, 1, 3);
  if (!res)
    Error_Handler();
}

void _commit_led_adsr_sub_filt_env_r(ctrl_t *ctrl, ctrl_toggle_t *toggle) {
  /* Sub Filter ADSR Release LEDs
   * LEFT1:11
   */

  if (!ctrl->changed[CTRL_SUB_FILT_ENV1_R] &&
      !ctrl->changed[CTRL_SUB_FILT_ENV2_R]) {
    return;
  }

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

  uint8_t brightness = 0x80;
  switch (toggle->sub_filt_env_sustain_func) {
  case ENC_ENV_SUSTAIN:
    brightness = DEFAULT_BRIGHTNESS;
    break;
  case ENC_ENV_AMOUNT:
    brightness = HALF_BRIGHTNESS;
    break;
  default:
    break;
  }

  /* [0,2][1,2][2,2]
   * [0,1][1,1]
   * [0,0]
   *
   * transposes to:
   *
   *
   * [0,0=20]
   * [0,1=19][1,1=22]
   * [0,2=18][1,2=21][2,2=23]
   */

  _adsr_led_set_grid_curve(r_val);

  uint16_t pwm_seq[6] = {grid[0][2], grid[0][1], grid[0][0],
                         grid[1][2], grid[1][1], grid[2][2]};
  uint8_t scale_seq[6] = {brightness, brightness, brightness,
                          brightness, brightness, brightness};

  bool res;
  res = is32_set_sequence_pwm(I2C_LEFT, 1, 3, 18, pwm_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_set_sequence_scale(I2C_LEFT, 1, 3, 18, scale_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_write_registers(I2C_LEFT, 1, 3);
  if (!res)
    Error_Handler();
}

/*
 * Osc Amp ADSR Envelope
 */

void _commit_led_adsr_osc_amp_env_a(ctrl_t *ctrl, ctrl_toggle_t *toggle) {
  /* Osc Amp ADSR Attack LEDs
   * RIGHT2:00
   */

  if (!ctrl->changed[CTRL_OSC_AMP_ENV_A]) {
    return;
  }

  uint16_t a_val = ctrl->value[CTRL_OSC_AMP_ENV_A];
  uint8_t brightness = 0x80;
  switch (toggle->osc_amp_env_sustain_func) {
  case ENC_ENV_SUSTAIN:
    brightness = DEFAULT_BRIGHTNESS;
    break;
  case ENC_ENV_AMOUNT:
    brightness = HALF_BRIGHTNESS;
    break;
  default:
    break;
  }

  /* [0,2=2=20][1,2=4=22][2,2=5=23]
   * [0,1=1=19][1,1=3=21]
   * [0,0=0=18]
   */

  _adsr_led_set_grid_curve(a_val);

  uint16_t pwm_seq[6] = {grid[0][0], grid[0][1], grid[0][2],
                         grid[1][1], grid[1][2], grid[2][2]};
  uint8_t scale_seq[6] = {brightness, brightness, brightness,
                          brightness, brightness, brightness};

  bool res;
  res = is32_set_sequence_pwm(I2C_RIGHT, 2, 0, 18, pwm_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_set_sequence_scale(I2C_RIGHT, 2, 0, 18, scale_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_write_registers(I2C_RIGHT, 2, 0);
  if (!res)
    Error_Handler();
}

void _commit_led_adsr_osc_amp_env_d(ctrl_t *ctrl, ctrl_toggle_t *toggle) {
  /* Osc Amp ADSR Decay LEDs
   * RIGHT2:00
   */

  if (!ctrl->changed[CTRL_OSC_AMP_ENV_D]) {
    return;
  }

  uint16_t d_val = ctrl->value[CTRL_OSC_AMP_ENV_D];
  uint8_t brightness = 0x80;
  switch (toggle->osc_amp_env_sustain_func) {
  case ENC_ENV_SUSTAIN:
    brightness = DEFAULT_BRIGHTNESS;
    break;
  case ENC_ENV_AMOUNT:
    brightness = HALF_BRIGHTNESS;
    break;
  default:
    break;
  }

  /* [0,2][1,2][2,2]
   * [0,1][1,1]
   * [0,0]
   *
   * transposes to:
   *
   * [0,0=08=12]
   * [0,1=07=14][1,1=10=13]
   * [0,2=06=15][1,2=09=16][2,2=11=17]
   */

  _adsr_led_set_grid_curve(d_val);

  uint16_t pwm_seq[6] = {grid[0][0], grid[1][1], grid[0][1],
                         grid[0][2], grid[1][2], grid[2][2]};
  uint8_t scale_seq[6] = {brightness, brightness, brightness,
                          brightness, brightness, brightness};

  bool res;
  res = is32_set_sequence_pwm(I2C_RIGHT, 2, 0, 12, pwm_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_set_sequence_scale(I2C_RIGHT, 2, 0, 12, scale_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_write_registers(I2C_RIGHT, 2, 0);
  if (!res)
    Error_Handler();
}

void _commit_led_adsr_osc_amp_env_s(ctrl_t *ctrl, ctrl_toggle_t *toggle) {
  /* Osc Amp ADSR Sustain LEDs
   *
   * RIGHT2:00
   */

  if (!ctrl->changed[CTRL_OSC_AMP_ENV_S] &&
      !ctrl->changed[CTRL_OSC_AMP_ENV_AMT]) {
    return;
  }

  uint8_t brightness = DEFAULT_BRIGHTNESS;
  uint16_t s_val;
  switch (toggle->osc_amp_env_sustain_func) {
  case ENC_ENV_SUSTAIN:
    s_val = ctrl->value[CTRL_OSC_AMP_ENV_S];
    break;
  case ENC_ENV_AMOUNT:
    s_val = ctrl->value[CTRL_OSC_AMP_ENV_AMT];
    break;
  default:
    s_val = 0;
  }

  /* [0,2=14=11][1,2=17=08]
   * [0,1=13=10][1,1=16=07]
   * [0,0=12=09][1,0=15=06]
   */

  switch (toggle->osc_amp_env_sustain_func) {
  case ENC_ENV_SUSTAIN:
    _adsr_led_set_grid_bar(s_val);
    break;
  case ENC_ENV_AMOUNT:
    _adsr_led_set_grid_stack(s_val);
    break;
  default:
    break;
  }

  uint16_t pwm_seq[6] = {grid[1][0], grid[1][1], grid[1][2],
                         grid[0][0], grid[0][1], grid[0][2]};
  uint8_t scale_seq[6] = {brightness, brightness, brightness,
                          brightness, brightness, brightness};

  bool res;
  res = is32_set_sequence_pwm(I2C_RIGHT, 2, 0, 6, pwm_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_set_sequence_scale(I2C_RIGHT, 2, 0, 6, scale_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_write_registers(I2C_RIGHT, 2, 0);
  if (!res)
    Error_Handler();
}

void _commit_led_adsr_osc_amp_env_r(ctrl_t *ctrl, ctrl_toggle_t *toggle) {
  /* Osc Amp ADSR Release LEDs
   * RIGHT2:00
   */

  if (!ctrl->changed[CTRL_OSC_AMP_ENV_R]) {
    return;
  }

  uint16_t r_val = ctrl->value[CTRL_OSC_AMP_ENV_R];
  uint8_t brightness = 0x80;
  switch (toggle->osc_amp_env_sustain_func) {
  case ENC_ENV_SUSTAIN:
    brightness = DEFAULT_BRIGHTNESS;
    break;
  case ENC_ENV_AMOUNT:
    brightness = HALF_BRIGHTNESS;
    break;
  default:
    break;
  }

  /* [0,2][1,2][2,2]
   * [0,1][1,1]
   * [0,0]
   *
   * transposes to:
   *
   * [0,0=20=2]
   * [0,1=19=1][1,1=22=4]
   * [0,2=18=0][1,2=21=3][2,2=23=5]
   */

  _adsr_led_set_grid_curve(r_val);

  uint16_t pwm_seq[6] = {grid[0][2], grid[0][1], grid[0][0],
                         grid[1][2], grid[1][1], grid[2][2]};
  uint8_t scale_seq[6] = {brightness, brightness, brightness,
                          brightness, brightness, brightness};

  bool res;
  res = is32_set_sequence_pwm(I2C_RIGHT, 2, 0, 0, pwm_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_set_sequence_scale(I2C_RIGHT, 2, 0, 0, scale_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_write_registers(I2C_RIGHT, 2, 0);
  if (!res)
    Error_Handler();
}

/*
 * Sub Amp ADSR Envelope
 */

void _commit_led_adsr_sub_amp_env_a(ctrl_t *ctrl, ctrl_toggle_t *toggle) {
  /* Sub Amp ADSR Attack LEDs
   * RIGHT1:10
   */

  if (!ctrl->changed[CTRL_SUB_AMP_ENV_A]) {
    return;
  }

  uint16_t a_val = ctrl->value[CTRL_SUB_AMP_ENV_A];
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

  /* [0,2=2=08][1,2=4=10][2,2=5=11]
   * [0,1=1=07][1,1=3=09]
   * [0,0=0=06]
   */

  _adsr_led_set_grid_curve(a_val);

  uint16_t pwm_seq[6] = {grid[0][0], grid[0][1], grid[0][2],
                         grid[1][1], grid[1][2], grid[2][2]};
  uint8_t scale_seq[6] = {brightness, brightness, brightness,
                          brightness, brightness, brightness};

  bool res;
  res = is32_set_sequence_pwm(I2C_RIGHT, 1, 2, 6, pwm_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_set_sequence_scale(I2C_RIGHT, 1, 2, 6, scale_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_write_registers(I2C_RIGHT, 1, 2);
  if (!res)
    Error_Handler();
}

void _commit_led_adsr_sub_amp_env_d(ctrl_t *ctrl, ctrl_toggle_t *toggle) {
  /* Sub Amp ADSR Decay LEDs
   * RIGHT1:10
   */

  if (!ctrl->changed[CTRL_SUB_AMP_ENV_D]) {
    return;
  }

  uint16_t d_val = ctrl->value[CTRL_SUB_AMP_ENV_D];
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

  /* [0,2][1,2][2,2]
   * [0,1][1,1]
   * [0,0]
   *
   * transposes to:
   *
   * [0,0=08=02]
   * [0,1=07=01][1,1=10=04]
   * [0,2=06=00][1,2=09=03][2,2=11=05]
   */

  _adsr_led_set_grid_curve(d_val);

  uint16_t pwm_seq[6] = {grid[0][2], grid[0][1], grid[0][0],
                         grid[1][2], grid[1][1], grid[2][2]};
  uint8_t scale_seq[6] = {brightness, brightness, brightness,
                          brightness, brightness, brightness};

  bool res;
  res = is32_set_sequence_pwm(I2C_RIGHT, 1, 2, 0, pwm_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_set_sequence_scale(I2C_RIGHT, 1, 2, 0, scale_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_write_registers(I2C_RIGHT, 1, 2);
  if (!res)
    Error_Handler();
}

void _commit_led_adsr_sub_amp_env_s(ctrl_t *ctrl, ctrl_toggle_t *toggle) {
  /* Sub Amp ADSR Sustain LEDs
   * RIGHT1:10
   */

  if (!ctrl->changed[CTRL_SUB_AMP_ENV_S] &&
      !ctrl->changed[CTRL_SUB_AMP_ENV_AMT]) {
    return;
  }

  uint8_t brightness = DEFAULT_BRIGHTNESS;
  uint16_t s_val = 0;
  switch (toggle->sub_amp_env_sustain_func) {
  case ENC_ENV_SUSTAIN:
    s_val = ctrl->value[CTRL_SUB_AMP_ENV_S];
    break;
  case ENC_ENV_AMOUNT:
    s_val = ctrl->value[CTRL_SUB_AMP_ENV_AMT];
    break;
  default:
    break;
  }

  /* [0,2=14=14][1,2=17=17]
   * [0,1=13=13][1,1=16=16]
   * [0,0=12=12][1,0=15=15]
   */

  switch (toggle->sub_amp_env_sustain_func) {
  case ENC_ENV_SUSTAIN:
    _adsr_led_set_grid_bar(s_val);
    break;
  case ENC_ENV_AMOUNT:
    _adsr_led_set_grid_stack(s_val);
    break;
  default:
    break;
  }

  uint16_t pwm_seq[6] = {grid[0][0], grid[0][1], grid[0][2],
                         grid[1][0], grid[1][1], grid[1][2]};
  uint8_t scale_seq[6] = {brightness, brightness, brightness,
                          brightness, brightness, brightness};

  bool res;
  res = is32_set_sequence_pwm(I2C_RIGHT, 1, 2, 12, pwm_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_set_sequence_scale(I2C_RIGHT, 1, 2, 12, scale_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_write_registers(I2C_RIGHT, 1, 2);
  if (!res)
    Error_Handler();
}

void _commit_led_adsr_sub_amp_env_r(ctrl_t *ctrl, ctrl_toggle_t *toggle) {
  /* Sub Amp ADSR Release LEDs
   * A: RIGHT1:10
   */

  if (!ctrl->changed[CTRL_SUB_AMP_ENV_R]) {
    return;
  }

  uint16_t r_val = ctrl->value[CTRL_SUB_AMP_ENV_R];
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

  /* [0,2][1,2][2,2]
   * [0,1][1,1]
   * [0,0]
   *
   * transposes to:
   *
   * [0,0=20=20]
   * [0,1=19=19][1,1=22=22]
   * [0,2=18=18][1,2=21=21][2,2=23=23]
   */

  _adsr_led_set_grid_curve(r_val);

  uint16_t pwm_seq[6] = {grid[0][2], grid[0][1], grid[0][0],
                         grid[1][2], grid[1][1], grid[2][2]};
  uint8_t scale_seq[6] = {brightness, brightness, brightness,
                          brightness, brightness, brightness};

  bool res;
  res = is32_set_sequence_pwm(I2C_RIGHT, 1, 2, 18, pwm_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_set_sequence_scale(I2C_RIGHT, 1, 2, 18, scale_seq, 6);
  if (!res)
    Error_Handler();
  res = is32_write_registers(I2C_RIGHT, 1, 2);
  if (!res)
    Error_Handler();
}

/*
 * ADSR LED Graphs
 */

void commit_led_adsr(commit_cycle_t cycle, ctrl_t *ctrl,
                     ctrl_toggle_t *toggle) {
  switch (cycle) {
  case COMMIT_LED_ADSR_OSC_FILT_A:
    _commit_led_adsr_osc_filt_env_a(ctrl, toggle);
    break;
  case COMMIT_LED_ADSR_OSC_FILT_D:
    _commit_led_adsr_osc_filt_env_d(ctrl, toggle);
    break;
  case COMMIT_LED_ADSR_OSC_FILT_S:
    _commit_led_adsr_osc_filt_env_s(ctrl, toggle);
    break;
  case COMMIT_LED_ADSR_OSC_FILT_R:
    _commit_led_adsr_osc_filt_env_r(ctrl, toggle);
    break;
  case COMMIT_LED_ADSR_SUB_FILT_A:
    _commit_led_adsr_sub_filt_env_a(ctrl, toggle);
    break;
  case COMMIT_LED_ADSR_SUB_FILT_D:
    _commit_led_adsr_sub_filt_env_d(ctrl, toggle);
    break;
  case COMMIT_LED_ADSR_SUB_FILT_S:
    _commit_led_adsr_sub_filt_env_s(ctrl, toggle);
    break;
  case COMMIT_LED_ADSR_SUB_FILT_R:
    _commit_led_adsr_sub_filt_env_r(ctrl, toggle);
    break;
  case COMMIT_LED_ADSR_OSC_AMP_A:
    _commit_led_adsr_osc_amp_env_a(ctrl, toggle);
    break;
  case COMMIT_LED_ADSR_OSC_AMP_D:
    _commit_led_adsr_osc_amp_env_d(ctrl, toggle);
    break;
  case COMMIT_LED_ADSR_OSC_AMP_S:
    _commit_led_adsr_osc_amp_env_s(ctrl, toggle);
    break;
  case COMMIT_LED_ADSR_OSC_AMP_R:
    _commit_led_adsr_osc_amp_env_r(ctrl, toggle);
    break;
  case COMMIT_LED_ADSR_SUB_AMP_A:
    _commit_led_adsr_sub_amp_env_a(ctrl, toggle);
    break;
  case COMMIT_LED_ADSR_SUB_AMP_D:
    _commit_led_adsr_sub_amp_env_d(ctrl, toggle);
    break;
  case COMMIT_LED_ADSR_SUB_AMP_S:
    _commit_led_adsr_sub_amp_env_s(ctrl, toggle);
    break;
  case COMMIT_LED_ADSR_SUB_AMP_R:
    _commit_led_adsr_sub_amp_env_r(ctrl, toggle);
    break;
  default:
    break;
  }
}
