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

ctrl_enum_t _preset_ctrl_enum(char *name);

/*
 * Preset Load
 */

void _preset_load_ctrl(cJSON *ctrl_obj, ctrl_t *ctrlptr) {
  memset(&ctrlptr->value, 0, sizeof(ctrlptr->value));

  for (cJSON *ctrl_value = ctrl_obj->child; ctrl_value;
       ctrl_value = ctrl_value->next) {
    ctrl_enum_t ctrl_enum = _preset_ctrl_enum(ctrl_value->string);
    ctrlptr->value[ctrl_enum] = ctrl_value->valueint;
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

  // Set presence flag
  // If load operation gets to here, then we have
  // successfully loaded a preset from disk and
  // so we should set to the present flag in the catalog
  preset_t *preset = system ? preset_get_system(index) : preset_get_user(index);
  preset->present = true;

  return true;
}
