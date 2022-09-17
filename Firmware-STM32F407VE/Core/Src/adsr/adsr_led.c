/*
 * led_adsr.c
 *
 *  Created on: 3 May 2021
 *      Author: jwilson
 */

#include "adsr.h"
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

adsr_grid_t _grid;

adsr_grid_t *adsr_led_set_grid_curve(uint16_t val) {
  /* Produces graph like this:
   *
   * [0,2][1,2][2,2]
   * [0,1][1,1]
   * [0,0]
   *
   */

  _grid.led[0][0] = 0;
  _grid.led[0][1] = 0;
  _grid.led[0][2] = 0;
  _grid.led[1][0] = 0;
  _grid.led[1][1] = 0;
  _grid.led[1][2] = 0;
  _grid.led[2][0] = 0;
  _grid.led[2][1] = 0;
  _grid.led[2][2] = 0;

  _grid.led[0][0] = MAX_PWM; // Always on

  double v = val / 4095.0;

  if (v < 0.33) {
    // 0% - 33%
    // [0,2] -> [1,2]
    double d = v / 0.33;
    _grid.led[0][2] = MAX_PWM * (1.0 - d);
    _grid.led[1][2] = MAX_PWM * d;
    _grid.led[0][1] = MAX_PWM;
  } else if (v < 0.66) {
    // 33% - 66%
    // [0,1] -> [1,1]
    double d = (v - 0.33) / 0.33;
    _grid.led[0][1] = MAX_PWM * (1.0 - d);
    _grid.led[1][1] = MAX_PWM * d;
    _grid.led[1][2] = MAX_PWM;
  } else {
    // 66% - 100%
    // [1,2] -> [2,2]
    double d = (v - 0.66) / 0.33;
    if (d > 1.0)
      d = 1.0;
    _grid.led[1][2] = MAX_PWM * (1.0 - d);
    _grid.led[2][2] = MAX_PWM * d;
    _grid.led[1][1] = MAX_PWM;
  }

  return &_grid;
}

adsr_grid_t *adsr_led_set_grid_stack(uint16_t val) {
  /* Produces graph like this:
   *
   * [0,2][1,2]
   * [0,1][1,1]
   * [0,0][1,0]
   *
   */

  _grid.led[0][0] = 0;
  _grid.led[0][1] = 0;
  _grid.led[0][2] = 0;
  _grid.led[1][0] = 0;
  _grid.led[1][1] = 0;
  _grid.led[1][2] = 0;
  _grid.led[2][0] = 0;
  _grid.led[2][1] = 0;
  _grid.led[2][2] = 0;

  double v = val / 4095.0;

  if (v < 0.33) {
    // 0% - 33%
    // [0,2] -> [1,2]
    double d = v / 0.33;
    _grid.led[0][0] = MAX_PWM * d;
    _grid.led[1][0] = MAX_PWM * d;
  } else if (v < 0.66) {
    // 33% - 66%
    // [0,1] -> [1,1]
    double d = (v - 0.33) / 0.33;
    _grid.led[0][0] = MAX_PWM;
    _grid.led[1][0] = MAX_PWM;
    _grid.led[0][1] = MAX_PWM * d;
    _grid.led[1][1] = MAX_PWM * d;
  } else {
    // 66% - 100%
    // [2,1] -> [2,2]
    double d = (v - 0.66) / 0.33;
    if (d > 1.0)
      d = 1.0;
    _grid.led[0][0] = MAX_PWM;
    _grid.led[1][0] = MAX_PWM;
    _grid.led[0][1] = MAX_PWM;
    _grid.led[1][1] = MAX_PWM;
    _grid.led[0][2] = MAX_PWM * d;
    _grid.led[1][2] = MAX_PWM * d;
  }

  return &_grid;
}

adsr_grid_t *adsr_led_set_grid_bar(uint16_t val) {
  /* Produces graph like this:
   *
   * [0,2][1,2]
   * [0,1][1,1]
   * [0,0][1,0]
   *
   */

  double v = val / 4095.0;

  _grid.led[0][0] = 0;
  _grid.led[0][1] = 0;
  _grid.led[0][2] = 0;
  _grid.led[1][0] = 0;
  _grid.led[1][1] = 0;
  _grid.led[1][2] = 0;
  _grid.led[2][0] = 0;
  _grid.led[2][1] = 0;
  _grid.led[2][2] = 0;

  if (v < 0.5) {
    // 0% - 50%
    // [0,0][1,0] -> [0,1][1,1]
    double d = v / 0.50;
    _grid.led[0][0] = MAX_PWM * (1.0 - d);
    _grid.led[1][0] = MAX_PWM * (1.0 - d);
    _grid.led[0][1] = MAX_PWM * d;
    _grid.led[1][1] = MAX_PWM * d;
  } else {
    // 50-100%
    // [0,1][1,1] -> [0,2][1,2]
    double d = (v - 0.50) / 0.50;
    _grid.led[0][1] = MAX_PWM * (1.0 - d);
    _grid.led[1][1] = MAX_PWM * (1.0 - d);
    _grid.led[0][2] = MAX_PWM * d;
    _grid.led[1][2] = MAX_PWM * d;
  }

  return &_grid;
}
