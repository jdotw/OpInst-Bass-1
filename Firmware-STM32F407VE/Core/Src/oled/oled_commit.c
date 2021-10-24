#include "commit.h"
#include "main.h"
#include "oled.h"
#include "preset.h"

oled_state_t oled_state = OLED_NONE;
bool reload_oled = false;

void oled_commit() {
  oled_state_t new_state = oled_state;

  switch (oled_state) {
  case OLED_NONE:
    // Initialization
    new_state = OLED_SELECT_PRESET;
    break;
  case OLED_SELECT_PRESET:
    if (commit_mod_state.button_changed.down &&
        commit_mod_state.button_state.down) {
      new_state = OLED_NAME_PRESET;
      oled_preset_name_set_preset(preset_get_active(),
                                  preset_get_active_index());
    }
    break;
  case OLED_NAME_PRESET:
    if (commit_mod_state.button_changed.up &&
        commit_mod_state.button_state.up) {
      new_state = OLED_SELECT_PRESET;
    } else if (commit_mod_state.button_changed.down &&
               commit_mod_state.button_state.down) {
      // Save preset to SDCard
      oled_preset_name_save();
      new_state = OLED_SELECT_PRESET;
    }
  }

  // Handle screen changes
  if (oled_state != new_state || reload_oled) {
    lv_obj_t *screen;
    switch (new_state) {
    case OLED_SELECT_PRESET:
      screen = oled_preset_select_screen();
      break;
    case OLED_NAME_PRESET:
      screen = oled_preset_name_screen();
      break;
    default:
      screen = NULL;
    }
    if (screen) {

      lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_FADE_ON, 125, 0, true);
    }
    oled_state = new_state;
    reload_oled = false;
  }

  // LAST: Let LVGL render updates
  lv_timer_handler();
}
