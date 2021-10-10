/*
 * preset.c
 *
 *  Created on: 6 Oct. 2021
 *      Author: jwilson
 */

#include "main.h"
#include "preset.h"
#include "cJSON.h"

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
  state.active = &default_preset;
}

preset_t *preset_get_active(void)
{
  return state.active;
}

char *_preset_json(preset_t preset)
{
  cJSON *preset_obj = cJSON_CreateObject();

  cJSON *ctrl_obj = cJSON_CreateObject();
  cJSON *toggle_obj = cJSON_CreateObject();
  cJSON *seq_obj = cJSON_CreateObject();

  cJSON_AddItemToObject(preset_obj, "ctrl", ctrl_obj);
  cJSON_AddItemToObject(preset_obj, "toggle", toggle_obj);
  cJSON_AddItemToObject(preset_obj, "seq", seq_obj);
  cJSON *steps_array = cJSON_AddArrayToObject(seq_obj, "steps");

  for (ctrl_enum_t i = 0; i < CTRL_ENUM_MAX; i++)
  {
    const char *name = ctrl_name[i];
    int16_t value = preset.ctrl->value[i];
    cJSON_AddNumberToObject(ctrl_obj, name, (double)value);
  }

  char *string = cJSON_Print(preset_obj);
  cJSON_Delete(preset_obj);

  return string;
}

bool preset_save(preset_t preset)
{
  char buf[128] = {0};
  char *json = _preset_json(preset);
  snprintf(buf, 128 - 1, "%s.JSN", preset.name);
  sd_write(buf, json, strlen(json));
}
