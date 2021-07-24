/*
 * ctrl.h
 *
 *  Created on: 20 Apr 2021
 *      Author: jwilson
 */

#ifndef INC_CTRL_H_
#define INC_CTRL_H_

#include <stdint.h>
#include <stdbool.h>

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

  CTRL_ENUM_MAX  // MUST BE LAST

} ctrl_enum_t;


#define CTRL_DEFAULT_SCALE 4096
#define CTRL_SCALE_HALT_TURN 100 * (4096/12)
#define CTRL_SCALE_WHOLE_TURN 100 * (4096/24)
#define CTRL_SCALE_TWO_TURNS 100 * (4096/48)
#define CTRL_DEFAULT_MIN 0
#define CTRL_DEFAULT_MID 0x800
#define CTRL_DEFAULT_MAX 0xFFF // 12-bit

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

typedef struct {
  bool osc1_saw_lvl_changed;
  bool osc1_squ_lvl_changed;
  bool osc1_squ_pwm_changed;
  bool osc1_to_osc1_changed;
  bool osc1_to_osc2_changed;

  bool osc1_tune_coarse_changed;
  bool osc1_tune_fine_changed;
  bool osc1_filt_cutoff_changed;
  bool osc1_filt_res_changed;
  bool osc1_drive_changed;

  bool osc2_saw_lvl_changed;
  bool osc2_squ_lvl_changed;
  bool osc2_squ_pwm_changed;
  bool osc2_noise_lvl_changed;

  bool osc2_filt_cutoff_changed;
  bool osc2_filt_res_changed;
  bool osc2_drive_changed;

  bool osc_filt_env1_a_changed;
  bool osc_filt_env1_d_changed;
  bool osc_filt_env1_s_changed;
  bool osc_filt_env1_r_changed;
  bool osc_filt_env1_amt_changed;

  bool osc_filt_env2_a_changed;
  bool osc_filt_env2_d_changed;
  bool osc_filt_env2_s_changed;
  bool osc_filt_env2_r_changed;
  bool osc_filt_env2_amt_changed;

  bool osc_amp_env_a_changed;
  bool osc_amp_env_d_changed;
  bool osc_amp_env_s_changed;
  bool osc_amp_env_r_changed;
  bool osc_amp_env_amt_changed;
  bool sub_lvl_changed;
  bool sub_noise_lvl_changed;
  bool sub_to_osc2_changed;

  bool sub_filt_cutoff_changed;
  bool sub_filt_res_changed;

  bool sub_filt_env1_a_changed;
  bool sub_filt_env1_d_changed;
  bool sub_filt_env1_s_changed;
  bool sub_filt_env1_r_changed;
  bool sub_filt_env1_amt_changed;

  bool sub_filt_env2_a_changed;
  bool sub_filt_env2_d_changed;
  bool sub_filt_env2_s_changed;
  bool sub_filt_env2_r_changed;
  bool sub_filt_env2_amt_changed;

  bool sub_amp_env_a_changed;
  bool sub_amp_env_d_changed;
  bool sub_amp_env_s_changed;
  bool sub_amp_env_r_changed;
  bool sub_amp_env_amt_changed;

  bool fx_wetdry_changed;
  bool fx_val1_changed;
  bool fx_val2_changed;
  bool fx_val3_changed;
  bool fx_feedback_changed;

} ctrl_changed_t;

typedef struct {

  uint16_t value[CTRL_ENUM_MAX];
  bool changed[CTRL_ENUM_MAX];

	uint16_t osc1_saw_lvl;
	uint16_t osc1_squ_lvl;
  uint16_t osc1_squ_pwm;
  uint16_t osc1_to_osc1;
  uint16_t osc1_to_osc2;

  int16_t osc1_tune_coarse;
  int16_t osc1_tune_fine;
  uint16_t osc1_filt_cutoff;
  uint16_t osc1_filt_res;
  uint16_t osc1_drive;

  uint16_t osc2_saw_lvl;
  uint16_t osc2_squ_lvl;
  uint16_t osc2_squ_pwm;
  uint16_t osc2_noise_lvl;

  uint16_t osc2_filt_cutoff;
  uint16_t osc2_filt_res;
  uint16_t osc2_drive;

  uint16_t osc_filt_env1_a;
  uint16_t osc_filt_env1_d;
  uint16_t osc_filt_env1_s;
  uint16_t osc_filt_env1_r;
  uint16_t osc_filt_env1_amt;

  uint16_t osc_filt_env2_a;
  uint16_t osc_filt_env2_d;
  uint16_t osc_filt_env2_s;
  uint16_t osc_filt_env2_r;
  uint16_t osc_filt_env2_amt;

  uint16_t osc_amp_env_a;
  uint16_t osc_amp_env_d;
  uint16_t osc_amp_env_s;
  uint16_t osc_amp_env_r;
  uint16_t osc_amp_env_amt;

  uint16_t sub_lvl;
  uint16_t sub_noise_lvl;
  uint16_t sub_to_osc2;

  uint16_t sub_filt_cutoff;
  uint16_t sub_filt_res;

  uint16_t sub_filt_env1_a;
  uint16_t sub_filt_env1_d;
  uint16_t sub_filt_env1_s;
  uint16_t sub_filt_env1_r;
  uint16_t sub_filt_env1_amt;

  uint16_t sub_filt_env2_a;
  uint16_t sub_filt_env2_d;
  uint16_t sub_filt_env2_s;
  uint16_t sub_filt_env2_r;
  uint16_t sub_filt_env2_amt;

  uint16_t sub_amp_env_a;
  uint16_t sub_amp_env_d;
  uint16_t sub_amp_env_s;
  uint16_t sub_amp_env_r;
  uint16_t sub_amp_env_amt;

  uint16_t fx_wetdry;
  uint16_t fx_val1;
  uint16_t fx_val2;
  uint16_t fx_val3;
  uint16_t fx_feedback;

} ctrl_value_t;

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

} ctrl_toggle_t;

extern ctrl_value_t ctrl_value;
extern ctrl_changed_t ctrl_changed;
extern ctrl_toggle_t ctrl_toggle;
extern bool ctrl_enabled;

void ctrl_value_init(void);
void ctrl_changed_init(void);
void ctrl_changed_reset(void);
void ctrl_toggle_init(void);

void ctrl_apply_delta(enc_enum_t ctrl, int8_t delta);
void ctrl_apply_toggle(enc_enum_t ctrl, bool changed, bool state);

#endif /* INC_CTRL_H_ */
