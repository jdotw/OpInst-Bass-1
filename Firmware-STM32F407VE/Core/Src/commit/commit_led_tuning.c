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

  /* Osc1 Saw
   * LEFT0:00
   * 0, 1
   */

  if (!ctrl->changed[CTRL_OSC1_TUNE_FINE] &&
      !ctrl->changed[CTRL_OSC1_TUNE_COARSE]) {
    return;
  }

  switch (toggle->osc1_tune_func) {
  case ENC_OSC_TUNE_COARSE: {
    int8_t tuning_step = (int8_t)ctrl->value[CTRL_OSC1_TUNE_COARSE];
    pwm_seq[6 + (tuning_step / 2)] = 0xFF;
    break;
  }
  case ENC_OSC_TUNE_FINE: {
    int16_t tuning_step = (int16_t)ctrl->value[CTRL_OSC1_TUNE_FINE];
    double tuning_percent = (double)tuning_step / 2047.0;
    pwm_seq[6 + (int16_t)(6.0 * tuning_percent)] = 0xFF;
    break;
  }
  }

  // _set_pwm_seq_lab(_osc1_saw_lab(ctrl), pwm_seq, 2 * 3);
  res = is32_set_sequence_pwm(I2C_LEFT, 0, 1, 0, pwm_seq, 12);
  if (!res)
    Error_Handler();

  // _set_scale_seq_animated(pwm_seq, scale_seq, 6, 0, false);
  res = is32_set_sequence_scale(I2C_LEFT, 0, 1, 0, scale_seq, 12);
  if (!res)
    Error_Handler();

  res = is32_write_registers(I2C_LEFT, 0, 1);
  if (!res)
    Error_Handler();
}