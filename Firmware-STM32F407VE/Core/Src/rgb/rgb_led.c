/*
 * rgb_led.c
 *
 *  Created on: Jul 10, 2021
 *      Author: jwilson
 */

#include "ctrl.h"
#include "i2c.h"
#include "is32.h"
#include "main.h"
#include "rgb.h"
#include <math.h>

/*
 * Pulse Width Modulation (Color)
 */

#define GAMMA_R 1.2
#define GAMMA_G 1.5
#define GAMMA_B 1.8

uint16_t gamma_correct(float gamma, uint16_t val) {
  const uint16_t max_in = 0xFFFF;
  const uint16_t max_out = 0xFFFF;
  return (uint16_t)(pow((float)val / (float)max_in, gamma) * max_out);
}

void rgb_led_set_pwm_lab(lab_t in, uint16_t *pwm, uint8_t len) {
  bool use_gamma = HAL_GPIO_ReadPin(SHIFTSW_GPIO_Port, SHIFTSW_Pin) !=
                   GPIO_PIN_RESET; // Pulled down

  rgb_t rgb = oklab_to_rgb(in);
  if (rgb.r < 0.0)
    rgb.r = 0.0;
  if (rgb.g < 0.0)
    rgb.g = 0.0;
  if (rgb.b < 0.0)
    rgb.b = 0.0;
  if (rgb.r > 1.0)
    rgb.r = 1.0;
  if (rgb.g > 1.0)
    rgb.g = 1.0;
  if (rgb.b > 1.0)
    rgb.b = 1.0;
  for (uint8_t i = 0; i < len; i++) {
    switch (i % 3) {
    case 0:
      pwm[i] =
          use_gamma ? gamma_correct(GAMMA_R, rgb.r * 65535.0) : rgb.r * 65535.0;
      break;
    case 1:
      pwm[i] =
          use_gamma ? gamma_correct(GAMMA_G, rgb.g * 65535.0) : rgb.g * 65535.0;
      break;
    case 2:
      pwm[i] =
          use_gamma ? gamma_correct(GAMMA_B, rgb.b * 65535.0) : rgb.b * 65535.0;
      break;
    }
  }
}

/*
 * Scaling (current / brightness)
 */

#define DEFAULT_SCALE 0x15

#define DEFAULT_SCALE_R 0x35
#define DEFAULT_SCALE_G 0x12
#define DEFAULT_SCALE_B 0x18

#define PATTERN_STEPS 10
double pattern[PATTERN_STEPS] = {0.5, 0.5, 0.5, 0.5, 0.6,
                                 0.7, 1.0, 0.5, 0.5, 0.5};
uint8_t pattern_step = 0;
void rgb_led_increment_pattern_step() { pattern_step--; }

void rgb_led_set_scale_animated(uint16_t *pwm, uint8_t *scale, uint8_t len,
                                uint8_t offset, bool invert) {
  uint8_t pattern_offset = pattern_step + offset;
  for (uint8_t i = 0; i < len; i++) {
    uint8_t bank_offset = (pattern_offset + (i / 3)) % PATTERN_STEPS;
    double pattern_modulation = pattern[bank_offset];
    switch (i % 3) {
    case 0:
      scale[invert ? (len - 1) - i : i] =
          ((double)DEFAULT_SCALE_R * pattern_modulation);
      break;
    case 1:
      scale[invert ? (len - 1) - i : i] =
          ((double)DEFAULT_SCALE_G * pattern_modulation);
      break;
    case 2:
      scale[invert ? (len - 1) - i : i] =
          ((double)DEFAULT_SCALE_B * pattern_modulation);
      break;
    }
  }
}
