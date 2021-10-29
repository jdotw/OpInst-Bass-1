/*
 * commit_led.c
 *
 *  Created on: 30 Apr 2021
 *      Author: jwilson
 */

#include "ctrl.h"
#include "dac7678.h"
#include "i2c.h"
#include "main.h"
#include "osc.h"
#include "rotpic.h"
#include <math.h>
#include <stdbool.h>

void commit_led_rotpic(ctrl_toggle_t *toggle) {
  uint8_t led = 0x0;
  bool res;

  // LEFT0:000
  led = 0x00;
  led |= (toggle->osc1_squ_func == ENC_OSC_SQU_LEVEL) << ROTPIC_LED1_BIT_SHIFT;
  led |= (toggle->osc1_squ_func == ENC_OSC_SQU_PWM) << ROTPIC_LED2_BIT_SHIFT;
  res = rotpic_led_set_state(I2C_LEFT, 0, 0b000, led);
  if (!res)
    Error_Handler();

  // LEFT0:001
  led = 0x00;
  led |= (toggle->osc_filt_env_attack_func == ENC_SELECT_ENV_1)
         << ROTPIC_LED1_BIT_SHIFT;
  led |= (toggle->osc_filt_env_attack_func == ENC_SELECT_ENV_2)
         << ROTPIC_LED2_BIT_SHIFT;
  led |= (toggle->osc_filt_env_sustain_func == ENC_ENV_SUSTAIN)
         << ROTPIC_LED4_BIT_SHIFT;
  led |= (toggle->osc_filt_env_sustain_func == ENC_ENV_AMOUNT)
         << ROTPIC_LED3_BIT_SHIFT;
  res = rotpic_led_set_state(I2C_LEFT, 0, 0b001, led);
  if (!res)
    Error_Handler();

  // LEFT0:011
  led = 0x00;
  led |= (toggle->osc2_squ_func == ENC_OSC_SQU_LEVEL) << ROTPIC_LED1_BIT_SHIFT;
  led |= (toggle->osc2_squ_func == ENC_OSC_SQU_PWM) << ROTPIC_LED2_BIT_SHIFT;
  res = rotpic_led_set_state(I2C_LEFT, 0, 0b011, led);
  if (!res)
    Error_Handler();

  // LEFT2:000
  led = 0x00;
  led |= (toggle->sub_filt_env_attack_func == ENC_SELECT_ENV_1)
         << ROTPIC_LED1_BIT_SHIFT;
  led |= (toggle->sub_filt_env_attack_func == ENC_SELECT_ENV_2)
         << ROTPIC_LED2_BIT_SHIFT;
  led |= (toggle->sub_filt_env_sustain_func == ENC_ENV_SUSTAIN)
         << ROTPIC_LED3_BIT_SHIFT;
  led |= (toggle->sub_filt_env_sustain_func == ENC_ENV_AMOUNT)
         << ROTPIC_LED4_BIT_SHIFT;
  res = rotpic_led_set_state(I2C_LEFT, 2, 0b000, led);
  if (!res)
    Error_Handler();

  // RIGHT0:000
  led = 0x00;
  led |= (toggle->osc_amp_env_sustain_func == ENC_ENV_SUSTAIN)
         << ROTPIC_LED1_BIT_SHIFT;
  led |= (toggle->osc_amp_env_sustain_func == ENC_ENV_AMOUNT)
         << ROTPIC_LED2_BIT_SHIFT;
  res = rotpic_led_set_state(I2C_RIGHT, 0, 0b000, led);
  if (!res)
    Error_Handler();

  // RIGHT1:001
  led = 0x00;
  led |= (toggle->sub_amp_env_sustain_func == ENC_ENV_SUSTAIN)
         << ROTPIC_LED1_BIT_SHIFT;
  led |= (toggle->sub_amp_env_sustain_func == ENC_ENV_AMOUNT)
         << ROTPIC_LED2_BIT_SHIFT;
  res = rotpic_led_set_state(I2C_RIGHT, 1, 0b001, led);
  if (!res)
    Error_Handler();
}
