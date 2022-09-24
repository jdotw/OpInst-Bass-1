/*
 * ctrl.h
 *
 *  Created on: 20 Apr 2021
 *      Author: jwilson
 */

#ifndef INC_CTRL_H_
#define INC_CTRL_H_

#include "../../../lvgl/lvgl.h"
#include "mod.h"
#include <stdbool.h>
#include <stdint.h>

typedef enum {
  CTRL_OSC1_SAW_LVL,
  CTRL_OSC1_SQU_LVL,
  CTRL_OSC1_SQU_PWM,
  CTRL_OSC1_TO_OSC1_MIX,
  CTRL_OSC1_TO_OSC2_MIX,

  CTRL_OSC1_TUNE_COARSE,
  CTRL_OSC1_TUNE_FINE,

  CTRL_OSC1_FILT_CUTOFF,
  CTRL_OSC1_FILT_RES,
  CTRL_OSC1_DRIVE_AMT,

  CTRL_OSC2_SAW_LVL,
  CTRL_OSC2_SQU_LVL,
  CTRL_OSC2_SQU_PWM,
  CTRL_OSC2_NOISE_LVL,

  CTRL_OSC2_FILT_CUTOFF,
  CTRL_OSC2_FILT_RES,
  CTRL_OSC2_DRIVE_AMT,

  CTRL_SUB_LVL,
  CTRL_SUB_NOISE_LVL,
  CTRL_SUB_TO_OSC2_MIX,

  CTRL_SUB_FILT_CUTOFF,
  CTRL_SUB_FILT_RES,

  CTRL_OSC_FILT_ENV1_A,
  CTRL_OSC_FILT_ENV1_D,
  CTRL_OSC_FILT_ENV1_S,
  CTRL_OSC_FILT_ENV1_R,
  CTRL_OSC_FILT_ENV1_AMT,

  CTRL_OSC_FILT_ENV2_A,
  CTRL_OSC_FILT_ENV2_D,
  CTRL_OSC_FILT_ENV2_S,
  CTRL_OSC_FILT_ENV2_R,
  CTRL_OSC_FILT_ENV2_AMT,

  CTRL_SUB_FILT_ENV1_A,
  CTRL_SUB_FILT_ENV1_D,
  CTRL_SUB_FILT_ENV1_S,
  CTRL_SUB_FILT_ENV1_R,
  CTRL_SUB_FILT_ENV1_AMT,

  CTRL_SUB_FILT_ENV2_A,
  CTRL_SUB_FILT_ENV2_D,
  CTRL_SUB_FILT_ENV2_S,
  CTRL_SUB_FILT_ENV2_R,
  CTRL_SUB_FILT_ENV2_AMT,

  CTRL_OSC_AMP_ENV_A,
  CTRL_OSC_AMP_ENV_D,
  CTRL_OSC_AMP_ENV_S,
  CTRL_OSC_AMP_ENV_R,
  CTRL_OSC_AMP_ENV_AMT,

  CTRL_SUB_AMP_ENV_A,
  CTRL_SUB_AMP_ENV_D,
  CTRL_SUB_AMP_ENV_S,
  CTRL_SUB_AMP_ENV_R,
  CTRL_SUB_AMP_ENV_AMT,

  CTRL_FX_WETDRY,
  CTRL_FX_VAL1,
  CTRL_FX_VAL2,
  CTRL_FX_VAL3,
  CTRL_FX_FEEDBACK,

  CTRL_ENUM_MAX // MUST BE LAST

} ctrl_enum_t;

#define CTRL_DEFAULT_SCALE 100
#define CTRL_SCALE_HALT_TURN 100 * (4096 / 12)
#define CTRL_SCALE_WHOLE_TURN 100 * (4096 / 24)
#define CTRL_SCALE_TWO_TURNS 100 * (4096 / 48)
#define CTRL_SCALE_FOUR_TURNS 100 * (4096 / 96)
#define CTRL_DEFAULT_MIN 0
#define CTRL_DEFAULT_MID 0x800
#define CTRL_DEFAULT_MAX 0xFFF // 12-bit

typedef enum {
  ENC_OSC1_SAW,
  ENC_OSC1_SQU,
  ENC_OSC1_TO_OSC2,

  ENC_OSC1_TUNE,

  ENC_OSC1_FILT_CUTOFF,
  ENC_OSC1_FILT_RES,
  ENC_OSC1_DRIVE,

  ENC_OSC2_SAW,
  ENC_OSC2_SQU,
  ENC_OSC2_NOISE,

  ENC_OSC2_FILT_CUTOFF,
  ENC_OSC2_FILT_RES,
  ENC_OSC2_DRIVE,

  ENC_SUB,
  ENC_SUB_NOISE,
  ENC_SUB_TO_OSC2,

  ENC_SUB_FILT_CUTOFF,
  ENC_SUB_FILT_RES,

  ENC_OSC_FILT_ENV_A,
  ENC_OSC_FILT_ENV_D,
  ENC_OSC_FILT_ENV_S,
  ENC_OSC_FILT_ENV_R,

  ENC_SUB_FILT_ENV_A,
  ENC_SUB_FILT_ENV_D,
  ENC_SUB_FILT_ENV_S,
  ENC_SUB_FILT_ENV_R,

  ENC_OSC_AMP_ENV_A,
  ENC_OSC_AMP_ENV_D,
  ENC_OSC_AMP_ENV_S,
  ENC_OSC_AMP_ENV_R,

  ENC_SUB_AMP_ENV_A,
  ENC_SUB_AMP_ENV_D,
  ENC_SUB_AMP_ENV_S,
  ENC_SUB_AMP_ENV_R,

  ENC_FX_WETDRY,
  ENC_FX_VAL1,
  ENC_FX_VAL2,
  ENC_FX_VAL3,
  ENC_FX_VAL4,

} enc_enum_t;

typedef enum {
  ENC_OSC_SQU_LEVEL,
  ENC_OSC_SQU_PWM,

  ENC_OSC_SQU_MAX,
} osc_pwm_func_t;

typedef enum {
  ENC_OSC_TUNE_COARSE,
  ENC_OSC_TUNE_FINE,

  ENC_OSC_TUNE_MAX,
} osc_tune_func_t;

typedef enum {
  ENC_SELECT_ENV_1,
  ENC_SELECT_ENV_2,

  ENC_SELECT_ENV_MAX,
} env_attack_func_t;

typedef enum {
  ENC_ENV_SUSTAIN,
  ENC_ENV_AMOUNT,

  ENC_ENV_SUSTAIN_FUNC_MAX,
} env_sustain_func_t;

typedef enum {
  ENC_FX_WETDRY_FX,
  ENC_FX_WETDRY_PRESET,

  ENC_FX_WETDRY_FUNC_MAX,
} fx_wetdry_func_t;

typedef struct {
  uint16_t value[CTRL_ENUM_MAX];
  bool changed[CTRL_ENUM_MAX];
} ctrl_t;

typedef struct {
  osc_pwm_func_t osc1_squ_func;
  osc_pwm_func_t osc2_squ_func;
  osc_tune_func_t osc1_tune_func;

  env_attack_func_t osc_filt_env_attack_func;
  env_sustain_func_t osc_filt_env_sustain_func;
  env_sustain_func_t osc_amp_env_sustain_func;

  env_attack_func_t sub_filt_env_attack_func;
  env_sustain_func_t sub_filt_env_sustain_func;
  env_sustain_func_t sub_amp_env_sustain_func;

  fx_wetdry_func_t fx_wetdry_func;

} ctrl_toggle_t;

// ctrl.c

ctrl_t *ctrl_get_active(void);
ctrl_toggle_t *ctrl_get_active_toggle(void);
bool ctrl_get_enabled(void);
void ctrl_set_enabled(bool enabled);

void ctrl_changed_init(void);
void ctrl_changed_reset(void);
void ctrl_toggle_init(void);

void ctrl_overflow_handler(void);

void ctrl_apply_delta(enc_enum_t enc, int8_t delta);
void ctrl_apply_toggle(enc_enum_t ctrl_enum, bool changed, bool state);

double ctrl_double(uint16_t value);
double ctrl_double_inverse(uint16_t value);

// ctrl_name.c

const char *ctrl_get_name(ctrl_enum_t ctrl_enum);

// ctrl_value.c

void ctrl_value_init(void);
int8_t ctrl_value_get_percent(ctrl_enum_t ctrl_enum);
void ctrl_value_set_min(ctrl_enum_t ctrl_enum);
void ctrl_value_set_max(ctrl_enum_t ctrl_enum);

// ctrl_screen_changed.c

lv_obj_t *ctrl_screen(void);
lv_obj_t *ctrl_changed_screen_init();
void ctrl_changed_screen_push_ctrl(ctrl_enum_t ctrl_enum);
void ctrl_changed_screen_commit();

#endif /* INC_CTRL_H_ */
