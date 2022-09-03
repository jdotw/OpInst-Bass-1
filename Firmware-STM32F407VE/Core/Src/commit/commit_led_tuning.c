#include "commit.h"
#include "i2c.h"
#include "is32.h"
#include "main.h"

void commit_led_tuning(ctrl_t *ctrl, seq_t *seq, mod_t *mod,
                       ctrl_toggle_t *toggle) {
  bool res;
  uint16_t pwm_seq[36] = {0};
  uint8_t scale_seq[36] = {0};

  memset(scale_seq, 0xFF, 36);

  /* Tuning LEDs
   * LEFT0:01
   * 0, 1
   */

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

  res = is32_set_sequence_pwm(I2C_LEFT, 0, 1, 0, pwm_seq, 13);
  if (!res)
    Error_Handler();

  res = is32_set_sequence_scale(I2C_LEFT, 0, 1, 0, scale_seq, 13);
  if (!res)
    Error_Handler();

  res = is32_write_registers(I2C_LEFT, 0, 1);
  if (!res)
    Error_Handler();
}