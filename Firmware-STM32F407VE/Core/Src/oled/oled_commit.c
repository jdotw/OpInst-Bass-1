#include "main.h"
#include "oled.h"
#include "commit.h"
#include "preset.h"

oled_state_t oled_state = OLED_NONE;

void oled_commit()
{
  oled_state_t new_state = oled_state;

  switch (oled_state)
  {
  case OLED_NONE:
    // Initialization
    new_state = OLED_SELECT_PRESET;
    break;
  case OLED_SELECT_PRESET:
    if (commit_mod_state.button_changed.down && commit_mod_state.button_state.down)
    {
      new_state = OLED_NAME_PRESET;
      oled_preset_name_set_preset(preset_get_active());
    }
    break;
  case OLED_NAME_PRESET:
    if (commit_mod_state.button_changed.up && commit_mod_state.button_state.up)
    {
      new_state = OLED_SELECT_PRESET;
    }
    else if (commit_mod_state.button_changed.down && commit_mod_state.button_state.down)
    {
      // Save preset to SDCard
      new_state = OLED_SELECT_PRESET;
    }
  }

  // Handle screen changes
  if (oled_state != new_state)
  {
    lv_obj_t *screen;
    switch (new_state)
    {
    case OLED_SELECT_PRESET:
      screen = oled_preset_select_screen();
      break;
    case OLED_NAME_PRESET:
      screen = oled_preset_name_screen();
      break;
    default:
      screen = NULL;
    }
    if (screen)
    {
      lv_scr_load(screen);
    }
    oled_state = new_state;
  }

  // LAST: Let LVGL render updates
  lv_timer_handler();
}
