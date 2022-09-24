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

preset_t user_presets[PRESET_CATALOG_MAX] = {0};
preset_t system_presets[PRESET_CATALOG_MAX] = {0};

void preset_catalog_load(bool system);
void preset_catalog_save();

preset_t *preset_get_system(uint8_t index) { return &system_presets[index]; }
preset_t *preset_get_user(uint8_t index) { return &user_presets[index]; }

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
  oled_reload_screen();
}

void preset_load_selected() {
  bool loaded_user = preset_load(false, state.selected_index);
  if (!loaded_user) {
    bool loaded_system = preset_load(true, state.selected_index);
    if (!loaded_system) {
      // Fallback to init
      ctrl_value_init();
      seq_init();
    }
  }
}
