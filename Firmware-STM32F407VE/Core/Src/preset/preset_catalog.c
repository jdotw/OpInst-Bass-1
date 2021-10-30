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

extern preset_t *user_presets;
extern preset_t *system_presets;

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
