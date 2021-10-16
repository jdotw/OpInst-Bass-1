/*
 * preset.c
 *
 *  Created on: 6 Oct. 2021
 *      Author: jwilson
 */

#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "main.h"
#include "preset.h"
#include "cJSON.h"
#include "sd.h"

#define str(x) #x
#define xstr(x) str(x)

typedef struct
{
  preset_t *active;
} preset_state_t;

preset_t default_preset;

preset_state_t state;

void preset_init(void)
{
  default_preset.name = "DefaultLongName";
  default_preset.ctrl = &ctrl;
  default_preset.toggle = &ctrl_toggle;
  default_preset.seq = &seq_state;
  state.active = &default_preset;
}

preset_t *preset_get_active(void)
{
  return state.active;
}

static const char *_preset_ctrl_name_func(uint8_t i)
{
  switch (i)
  {
  case CTRL_OSC1_SAW_LVL:
    return "osc1_saw_lvl";
  case CTRL_OSC1_SQU_LVL:
    return "osc1_squ_lvl";
  case CTRL_OSC1_SQU_PWM:
    return "osc1_squ_pwm";
  case CTRL_OSC1_TO_OSC1_MIX:
    return "osc1_to_osc1_mix";
  case CTRL_OSC1_TO_OSC2_MIX:
    return "osc1_to_osc2_mix";

  case CTRL_OSC1_TUNE_COARSE:
    return "osc1_tune_coarse";
  case CTRL_OSC1_TUNE_FINE:
    return "osc1_tune_fine";

  case CTRL_OSC1_FILT_CUTOFF:
    return "osc1_filt_cutoff";
  case CTRL_OSC1_FILT_RES:
    return "osc1_filt_res";
  case CTRL_OSC1_DRIVE_AMT:
    return "osc1_drive_amt";

  case CTRL_OSC2_SAW_LVL:
    return "osc2_saw_lvl";
  case CTRL_OSC2_SQU_LVL:
    return "osc2_squ_lvl";
  case CTRL_OSC2_SQU_PWM:
    return "osc2_squ_pwm";
  case CTRL_OSC2_NOISE_LVL:
    return "osc2_noise_lvl";

  case CTRL_OSC2_FILT_CUTOFF:
    return "osc2_filt_cutoff";
  case CTRL_OSC2_FILT_RES:
    return "osc2_filt_res";
  case CTRL_OSC2_DRIVE_AMT:
    return "osc2_drive_amt";

  case CTRL_SUB_LVL:
    return "sub_lvl";
  case CTRL_SUB_NOISE_LVL:
    return "sub_noise_lvl";
  case CTRL_SUB_TO_OSC2_MIX:
    return "sub_to_osc2_mix";

  case CTRL_SUB_FILT_CUTOFF:
    return "sub_filt_cutoff";
  case CTRL_SUB_FILT_RES:
    return "sub_filt_res";

  case CTRL_OSC_FILT_ENV1_A:
    return "osc_filt_env1_a";
  case CTRL_OSC_FILT_ENV1_D:
    return "osc_filt_env1_d";
  case CTRL_OSC_FILT_ENV1_S:
    return "osc_filt_env1_s";
  case CTRL_OSC_FILT_ENV1_R:
    return "osc_filt_env1_r";
  case CTRL_OSC_FILT_ENV1_AMT:
    return "osc_filt_env1_amt";

  case CTRL_OSC_FILT_ENV2_A:
    return "osc_filt_env2_a";
  case CTRL_OSC_FILT_ENV2_D:
    return "osc_filt_env2_d";
  case CTRL_OSC_FILT_ENV2_S:
    return "osc_filt_env2_s";
  case CTRL_OSC_FILT_ENV2_R:
    return "osc_filt_env2_r";
  case CTRL_OSC_FILT_ENV2_AMT:
    return "osc_filt_env2_amt";

  case CTRL_SUB_FILT_ENV1_A:
    return "sub_filt_env1_a";
  case CTRL_SUB_FILT_ENV1_D:
    return "sub_filt_env1_d";
  case CTRL_SUB_FILT_ENV1_S:
    return "sub_filt_env1_s";
  case CTRL_SUB_FILT_ENV1_R:
    return "sub_filt_env1_r";
  case CTRL_SUB_FILT_ENV1_AMT:
    return "sub_filt_env1_a_amt";

  case CTRL_SUB_FILT_ENV2_A:
    return "sub_filt_env2_a";
  case CTRL_SUB_FILT_ENV2_D:
    return "sub_filt_env2_d";
  case CTRL_SUB_FILT_ENV2_S:
    return "sub_filt_env2_s";
  case CTRL_SUB_FILT_ENV2_R:
    return "sub_filt_env2_r";
  case CTRL_SUB_FILT_ENV2_AMT:
    return "sub_filt_env2_amt";

  case CTRL_OSC_AMP_ENV_A:
    return "osc_amp_env_a";
  case CTRL_OSC_AMP_ENV_D:
    return "osc_amp_env_d";
  case CTRL_OSC_AMP_ENV_S:
    return "osc_amp_env_s";
  case CTRL_OSC_AMP_ENV_R:
    return "osc_amp_env_r";
  case CTRL_OSC_AMP_ENV_AMT:
    return "osc_amp_env_amt";

  case CTRL_SUB_AMP_ENV_A:
    return "sub_amp_env_a";
  case CTRL_SUB_AMP_ENV_D:
    return "sub_amp_env_d";
  case CTRL_SUB_AMP_ENV_S:
    return "sub_amp_env_s";
  case CTRL_SUB_AMP_ENV_R:
    return "sub_amp_env_r";
  case CTRL_SUB_AMP_ENV_AMT:
    return "sub_amp_env_amt";

  case CTRL_FX_WETDRY:
    return "fx_wetdry";
  case CTRL_FX_VAL1:
    return "fx_val1";
  case CTRL_FX_VAL2:
    return "fx_val2";
  case CTRL_FX_VAL3:
    return "fx_val3";
  case CTRL_FX_FEEDBACK:
    return "fx_feedback";

  default:
    Error_Handler();
    return NULL;
  }
}

// static const char *
//     _preset_ctrl_name_array[] = {
//         "osc1_saw_lvl",
//         "osc1_squ_lvl",
//         "osc1_squ_pwm",
//         "osc1_to_osc1_mix",
//         "osc1_to_osc2_mix",

//         "osc1_tune_coarse",
//         "osc1_tune_fine",

//         "osc1_filt_cutoff",
//         "osc1_filt_res",
//         "osc1_drive_amt",

//         "osc2_saw_lvl",
//         "osc2_squ_lvl",
//         "osc2_squ_pwm",
//         "osc2_noise_lvl",

//         "osc2_filt_cutoff",
//         "osc2_filt_res",
//         "osc2_drive_amt",

//         "sub_lvl",
//         "sub_noise_lvl",
//         "sub_to_osc2_mix",

//         "sub_filt_cutoff",
//         "sub_filt_res",

//         "osc_filt_env1_a",
//         "osc_filt_env1_d",
//         "osc_filt_env1_s",
//         "osc_filt_env1_r",
//         "osc_filt_env1_amt",

//         "osc_filt_env2_a",
//         "osc_filt_env2_d",
//         "osc_filt_env2_s",
//         "osc_filt_env2_r",
//         "osc_filt_env2_amt",

//         "sub_filt_env1_a",
//         "sub_filt_env1_d",
//         "sub_filt_env1_s",
//         "sub_filt_env1_r",
//         "sub_filt_env1_amt",

//         "sub_filt_env2_a",
//         "sub_filt_env2_d",
//         "sub_filt_env2_s",
//         "sub_filt_env2_r",
//         "sub_filt_env2_amt",

//         "osc_amp_env_a",
//         "osc_amp_env_d",
//         "osc_amp_env_s",
//         "osc_amp_env_r",
//         "osc_amp_env_amt",

//         "sub_amp_env_a",
//         "sub_amp_env_d",
//         "sub_amp_env_s",
//         "sub_amp_env_r",
//         "sub_amp_env_amt",

//         "fx_wetdry",
//         "fx_val1",
//         "fx_val2",
//         "fx_val3",
//         "fx_feedback",
// };

char *_preset_ctrl_json_string(preset_t preset)
{
  cJSON *ctrl_obj = cJSON_CreateObject();
  for (ctrl_enum_t i = 0; i < CTRL_ENUM_MAX; i++)
  {
    const char *name = _preset_ctrl_name_func(i);
    int16_t value = preset.ctrl->value[i];
    cJSON_AddNumberToObject(ctrl_obj, name, (double)value);
  }
  char *string = cJSON_Print(ctrl_obj);
  cJSON_Delete(ctrl_obj);
  return string;
}

char *_preset_toggle_json_string(preset_t preset)
{
  cJSON *toggle_obj = cJSON_CreateObject();

  if (preset.toggle)
  {
    cJSON_AddNumberToObject(toggle_obj, "osc1_squ_func", preset.toggle->osc1_squ_func);
    cJSON_AddNumberToObject(toggle_obj, "osc2_squ_func", preset.toggle->osc2_squ_func);
    cJSON_AddNumberToObject(toggle_obj, "osc1_tune_func", preset.toggle->osc1_tune_func);

    cJSON_AddNumberToObject(toggle_obj, "osc_filt_env_attack_func", preset.toggle->osc_filt_env_attack_func);
    cJSON_AddNumberToObject(toggle_obj, "osc_filt_env_sustain_func", preset.toggle->osc_filt_env_sustain_func);
    cJSON_AddNumberToObject(toggle_obj, "osc_amp_env_sustain_func", preset.toggle->osc_amp_env_sustain_func);

    cJSON_AddNumberToObject(toggle_obj, "sub_filt_env_attack_func", preset.toggle->sub_filt_env_attack_func);
    cJSON_AddNumberToObject(toggle_obj, "sub_filt_env_sustain_func", preset.toggle->sub_filt_env_sustain_func);
    cJSON_AddNumberToObject(toggle_obj, "sub_amp_env_sustain_func", preset.toggle->sub_amp_env_sustain_func);
  }

  char *string = cJSON_Print(toggle_obj);
  cJSON_Delete(toggle_obj);

  return string;
}

char *_preset_step_json_string(preset_t preset, uint8_t step)
{
  cJSON *step_obj = cJSON_CreateObject();
  for (ctrl_enum_t i = 0; i < CTRL_ENUM_MAX; i++)
  {
    if (preset.seq->step_ctrl[step].changed[i])
    {
      const char *name = _preset_ctrl_name_func(i);
      int16_t value = preset.ctrl->value[i];
      cJSON_AddNumberToObject(step_obj, name, (double)value);
    }
  }
  char *string = cJSON_Print(step_obj);
  cJSON_Delete(step_obj);
  return string;
}

bool preset_save(preset_t preset)
{
  char path[128] = {0};
  char filename[128] = {0};
  char *json;
  bool result;

  // Create Root Path
  snprintf(path, 128 - 1, "\\PRESETS\\USER\\%02d", preset.index);
  sd_mkdir(path);

  // Controls
  json = _preset_ctrl_json_string(preset);
  snprintf(filename, 128 - 1, "\\PRESETS\\USER\\%02d\\CONTROLS.JSN", preset.index);
  result = sd_write(filename, json, strlen(json));
  if (!result)
  {
    Error_Handler();
  }
  cJSON_free(json);

  // Toggle
  json = _preset_toggle_json_string(preset);
  snprintf(filename, 128 - 1, "\\PRESETS\\USER\\%02d\\TOGGLES.JSN", preset.index);
  result = sd_write(filename, json, strlen(json));
  if (!result)
  {
    Error_Handler();
  }
  cJSON_free(json);

  // Create Steps Path
  snprintf(path, 128 - 1, "\\PRESETS\\USER\\%02d\\STEPS", preset.index);
  sd_mkdir(path);

  // Sequencer
  for (uint8_t i = 0; i < SEQ_MAX_STEPS; i++)
  {
    json = _preset_step_json_string(preset, i);
    snprintf(filename, 128 - 1, "\\PRESETS\\USER\\%02d\\STEPS\\STEP_%d.JSN", preset.index, i + 1);
    result = sd_write(filename, json, strlen(json));
    if (!result)
    {
      Error_Handler();
    }
    cJSON_free(json);
  }

  return true;
}
