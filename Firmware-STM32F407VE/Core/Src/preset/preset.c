/*
 * preset.c
 *
 *  Created on: 6 Oct. 2021
 *      Author: jwilson
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "cJSON.h"
#include "main.h"
#include "oled.h"
#include "preset.h"
#include "sd.h"

typedef struct {
  uint8_t selected_index;
} preset_state_t;

preset_state_t state = {0};

#define PRESET_CATALOG_MAX 100

preset_t user_presets[PRESET_CATALOG_MAX] = {0};
preset_t system_presets[PRESET_CATALOG_MAX] = {0};

void preset_catalog_load(bool system);
void preset_load_selected(void);

/*
 * Init
 */

void preset_init(void) {
  // Get list of presets from SDcard
  preset_catalog_load(true);  // System
  preset_catalog_load(false); // User
  state.selected_index = 0;
  preset_load_selected();
}

/*
 * Catalog
 */

void preset_catalog_load(bool system) {
  char path[128] = {0};
  char filename[128] = {0};
  bool result;
  size_t read_len = 0;
  char read_buf[2048] = {0};

  // Create Root Path
  snprintf(path, 128 - 1, "\\PRESETS\\%s", system ? "SYSTEM" : "USER");
  sd_mkdir(path);

  // Load Controls
  snprintf(filename, 128 - 1, "\\PRESETS\\%s\\PRESETS.JSN",
           system ? "SYSTEM" : "USER");
  result = sd_read(filename, read_buf, 2047, &read_len);
  if (result) {
    const char *parse_end = 0;
    cJSON *catalog_obj = cJSON_ParseWithOpts(read_buf, &parse_end, true);
    if (catalog_obj) {
      preset_t *presets = system ? system_presets : user_presets;
      for (cJSON *preset_obj = catalog_obj->child; preset_obj;
           preset_obj = preset_obj->next) {
        cJSON *index_obj = cJSON_GetObjectItem(preset_obj, "index");
        cJSON *name_obj = cJSON_GetObjectItem(preset_obj, "name");
        preset_t *preset = &presets[index_obj->valueint];
        preset->present = true;
        snprintf(preset->name, PRESET_NAME_MAX_LENGTH, "%s", name_obj->string);
      }
    }
  }
}

void preset_catalog_save() {
  char path[128] = {0};
  char filename[128] = {0};
  char *json;
  bool result;

  // Create Root Path
  snprintf(path, 128 - 1, "\\PRESETS\\USER");
  sd_mkdir(path);

  // Create Catalog
  cJSON *catalog_array = cJSON_CreateArray();
  for (ctrl_enum_t i = 0; i < PRESET_CATALOG_MAX; i++) {
    if (user_presets[i].present) {
      cJSON *preset_obj = cJSON_CreateObject();
      cJSON_AddStringToObject(preset_obj, "name", user_presets[i].name);
      cJSON_AddNumberToObject(preset_obj, "index", (double)i);
      cJSON_AddItemToArray(catalog_array, preset_obj);
    }
  }

  // Encode
  json = cJSON_Print(catalog_array);
  cJSON_Delete(catalog_array);

  // Write to disk
  snprintf(filename, 128 - 1, "\\PRESETS\\USER\\PRESETS.JSN");
  result = sd_write(filename, json, strlen(json));
  if (!result) {
    Error_Handler();
  }
  cJSON_free(json);
}

/*
 * State
 */

preset_t *preset_get_active(void) {
  if (user_presets[state.selected_index].present) {
    return &user_presets[state.selected_index];
  } else {
    return &system_presets[state.selected_index];
  }
}

uint8_t preset_get_active_index(void) { return state.selected_index; }

/*
 * Selection
 */

void preset_select_apply_delta(uint8_t delta) {
  // Increment Index (and handle wrap)
  state.selected_index += delta;
  if (state.selected_index >= PRESET_CATALOG_MAX) {
    state.selected_index = 0;
  }

  // Load the preset
  preset_load_selected();

  // Refresh OLED Screen
  reload_oled = true;
}

/*
 * Mapping
 */

static const char *_preset_ctrl_name_func(uint8_t i) {
  switch (i) {
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

ctrl_enum_t _preset_ctrl_enum(char *name) {
  for (ctrl_enum_t i = 0; i < CTRL_ENUM_MAX; i++) {
    const char *candidate = _preset_ctrl_name_func(i);
    if (strcmp(candidate, name) == 0) {
      return i;
    }
  }
  Error_Handler();
  return CTRL_ENUM_MAX;
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

/*
 * Preset Save
 */

bool preset_save(uint8_t index, char *name) {
  char path[128] = {0};
  char filename[128] = {0};
  char *json;
  bool result;
  ctrl_t *ctrl = ctrl_get_active();
  ctrl_toggle_t *toggle = ctrl_get_active_toggle();
  seq_t *seq = seq_get();

  // Create Root Path
  snprintf(path, 128 - 1, "\\PRESETS\\USER\\%02d", index);
  sd_mkdir(path);

  // Encode Controls
  cJSON *ctrl_obj = cJSON_CreateObject();
  for (ctrl_enum_t i = 0; i < CTRL_ENUM_MAX; i++) {
    const char *name = _preset_ctrl_name_func(i);
    int16_t value = ctrl->value[i];
    cJSON_AddNumberToObject(ctrl_obj, name, (double)value);
  }
  json = cJSON_Print(ctrl_obj);
  cJSON_Delete(ctrl_obj);

  // Write Controls
  snprintf(filename, 128 - 1, "\\PRESETS\\USER\\%02d\\CONTROLS.JSN", index);
  result = sd_write(filename, json, strlen(json));
  if (!result) {
    Error_Handler();
  }
  cJSON_free(json);
  json = NULL;

  // Encode Toggle
  cJSON *toggle_obj = cJSON_CreateObject();

  cJSON_AddNumberToObject(toggle_obj, "osc1_squ_func", toggle->osc1_squ_func);
  cJSON_AddNumberToObject(toggle_obj, "osc2_squ_func", toggle->osc2_squ_func);
  cJSON_AddNumberToObject(toggle_obj, "osc1_tune_func", toggle->osc1_tune_func);

  cJSON_AddNumberToObject(toggle_obj, "osc_filt_env_attack_func",
                          toggle->osc_filt_env_attack_func);
  cJSON_AddNumberToObject(toggle_obj, "osc_filt_env_sustain_func",
                          toggle->osc_filt_env_sustain_func);
  cJSON_AddNumberToObject(toggle_obj, "osc_amp_env_sustain_func",
                          toggle->osc_amp_env_sustain_func);

  cJSON_AddNumberToObject(toggle_obj, "sub_filt_env_attack_func",
                          toggle->sub_filt_env_attack_func);
  cJSON_AddNumberToObject(toggle_obj, "sub_filt_env_sustain_func",
                          toggle->sub_filt_env_sustain_func);
  cJSON_AddNumberToObject(toggle_obj, "sub_amp_env_sustain_func",
                          toggle->sub_amp_env_sustain_func);

  json = cJSON_Print(toggle_obj);
  cJSON_Delete(toggle_obj);

  // Write Toggle
  snprintf(filename, 128 - 1, "\\PRESETS\\USER\\%02d\\TOGGLES.JSN", index);
  result = sd_write(filename, json, strlen(json));
  if (!result) {
    Error_Handler();
  }
  cJSON_free(json);
  json = NULL;

  // Create Steps Path
  snprintf(path, 128 - 1, "\\PRESETS\\USER\\%02d\\STEPS", index);
  sd_mkdir(path);

  // Sequencer
  for (uint8_t step = 0; step < SEQ_MAX_STEPS; step++) {
    cJSON *step_obj = cJSON_CreateObject();
    for (ctrl_enum_t i = 0; i < CTRL_ENUM_MAX; i++) {
      if (seq->state.step_ctrl[step].changed[i]) {
        const char *name = _preset_ctrl_name_func(i);
        int16_t value = seq->state.step_ctrl[step].value[i];
        cJSON_AddNumberToObject(step_obj, name, (double)value);
      }
    }
    json = cJSON_Print(step_obj);
    cJSON_Delete(step_obj);

    snprintf(filename, 128 - 1, "\\PRESETS\\USER\\%02d\\STEPS\\STEP_%d.JSN",
             index, step + 1);
    result = sd_write(filename, json, strlen(json));
    if (!result) {
      Error_Handler();
    }
    cJSON_free(json);
  }

  // Update Catalog
  strncpy(user_presets[index].name, name, PRESET_NAME_MAX_LENGTH - 1);
  preset_catalog_save();

  return true;
}

/*
 * Preset Load
 */

void _preset_load_ctrl(cJSON *ctrl_obj, ctrl_t *ctrlptr) {
  memset(&ctrlptr->value, 0, sizeof(ctrlptr->value));
  memset(&ctrlptr->changed, 0, sizeof(ctrlptr->changed));

  for (cJSON *ctrl_value = ctrl_obj->child; ctrl_value;
       ctrl_value = ctrl_value->next) {
    ctrl_enum_t ctrl_enum = _preset_ctrl_enum(ctrl_value->string);
    ctrlptr->value[ctrl_enum] = ctrl_value->valueint;
    ctrlptr->changed[ctrl_enum] = true;
  }
}

bool preset_load(bool system, uint8_t index) {
  char path[128] = {0};
  char filename[128] = {0};
  bool result;
  size_t read_len = 0;
  char read_buf[2048] = {0};
  ctrl_t *ctrl = ctrl_get_active();

  // Create Root Path
  snprintf(path, 128 - 1, "\\PRESETS\\%s\\%02d", system ? "SYSTEM" : "USER",
           index);

  // Load Controls
  snprintf(filename, 128 - 1, "\\PRESETS\\%s\\%02d\\CONTROLS.JSN",
           system ? "SYSTEM" : "USER", index);
  result = sd_read(filename, read_buf, 2047, &read_len);
  if (result) {
    const char *parse_end = 0;
    cJSON *ctrl_obj = cJSON_ParseWithOpts(read_buf, &parse_end, true);
    if (ctrl_obj) {
      _preset_load_ctrl(ctrl_obj, ctrl);
    }
  } else {
    return false;
  }

  // Load Toggles
  snprintf(filename, 128 - 1, "\\PRESETS\\%s\\%02d\\TOGGLES.JSN",
           system ? "SYSTEM" : "USER", index);
  memset(read_buf, 0, sizeof(read_buf));
  result = sd_read(filename, read_buf, 2047, &read_len);
  if (result) {
    const char *parse_end = 0;
    cJSON *toggles_obj = cJSON_ParseWithOpts(read_buf, &parse_end, true);
    ctrl_toggle_t *toggle = ctrl_get_active_toggle();

    cJSON *osc1_squ_func_obj =
        cJSON_GetObjectItem(toggles_obj, "osc1_squ_func");
    if (osc1_squ_func_obj)
      toggle->osc1_squ_func = osc1_squ_func_obj->valueint;

    cJSON *osc2_squ_func_obj =
        cJSON_GetObjectItem(toggles_obj, "osc2_squ_func");
    if (osc2_squ_func_obj)
      toggle->osc2_squ_func = osc2_squ_func_obj->valueint;

    cJSON *osc1_tune_func_obj =
        cJSON_GetObjectItem(toggles_obj, "osc1_tune_func");
    if (osc1_tune_func_obj)
      toggle->osc1_tune_func = osc1_tune_func_obj->valueint;

    cJSON *osc_filt_env_attack_func_obj =
        cJSON_GetObjectItem(toggles_obj, "osc_filt_env_attack_func");
    if (osc_filt_env_attack_func_obj)
      toggle->osc_filt_env_attack_func = osc_filt_env_attack_func_obj->valueint;

    cJSON *osc_filt_env_sustain_func_obj =
        cJSON_GetObjectItem(toggles_obj, "osc_filt_env_sustain_func");
    if (osc_filt_env_sustain_func_obj)
      toggle->osc_filt_env_sustain_func =
          osc_filt_env_sustain_func_obj->valueint;

    cJSON *osc_amp_env_sustain_func_obj =
        cJSON_GetObjectItem(toggles_obj, "osc_amp_env_sustain_func");
    if (osc_amp_env_sustain_func_obj)
      toggle->osc_amp_env_sustain_func = osc_amp_env_sustain_func_obj->valueint;

    cJSON *sub_filt_env_attack_func_obj =
        cJSON_GetObjectItem(toggles_obj, "sub_filt_env_attack_func");
    if (sub_filt_env_attack_func_obj)
      toggle->sub_filt_env_attack_func = sub_filt_env_attack_func_obj->valueint;

    cJSON *sub_filt_env_sustain_func_obj =
        cJSON_GetObjectItem(toggles_obj, "sub_filt_env_sustain_func");
    if (sub_filt_env_sustain_func_obj)
      toggle->sub_filt_env_sustain_func =
          sub_filt_env_sustain_func_obj->valueint;

    cJSON *sub_amp_env_sustain_func_obj =
        cJSON_GetObjectItem(toggles_obj, "sub_amp_env_sustain_func");
    if (sub_amp_env_sustain_func_obj)
      toggle->sub_amp_env_sustain_func = sub_amp_env_sustain_func_obj->valueint;
  }

  // Load Steps
  seq_t *seq = seq_get();
  for (uint8_t i = 0; i < SEQ_MAX_STEPS; i++) {
    snprintf(filename, 128 - 1, "\\PRESETS\\USER\\%02d\\STEPS\\STEP_%d.JSN",
             index, i + 1);
    memset(read_buf, 0, sizeof(read_buf));
    result = sd_read(filename, read_buf, 2047, &read_len);
    if (result) {
      const char *parse_end = 0;
      cJSON *step_obj = cJSON_ParseWithOpts(read_buf, &parse_end, true);
      _preset_load_ctrl(step_obj, &seq->state.step_ctrl[i]);
    }
  }

  return true;
}

void preset_load_selected() {
  bool loaded_user = preset_load(false, state.selected_index);
  if (!loaded_user) {
    preset_load(true, state.selected_index);
  }
}
