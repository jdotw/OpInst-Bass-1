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

const char *_preset_ctrl_name_func(uint8_t i);
void preset_catalog_save(void);

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
      if (seq->state.step_ctrl[step].value[i] != UINT16_MAX) {
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
  preset_t *preset = preset_get_user(index);
  preset->present = true;
  strncpy(preset->name, name, PRESET_NAME_MAX_LENGTH - 1);
  preset_catalog_save();

  return true;
}
