#include "commit.h"
#include "ctrl.h"
#include "main.h"
#include "oled.h"
#include "preset.h"

extern uint32_t timeout;
extern uint32_t timeout_start;
extern bool reload_requested;

void oled_commit(ctrl_t *ctrl, mod_t *mod) {
  // Check for timeout or pending reload
  if (timeout > 0 && (HAL_GetTick() - timeout_start) >= timeout) {
    // Timed out, reset screen to default
    oled_set_screen(OLED_SCREEN_PRESET, 0);
  } else if (reload_requested) {
    lv_obj_t *screen = oled_render_active_screen();
    if (screen) {
      lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
    }
  }

  // Call current screens commit function
  oled_screen_t screen = oled_get_screen();
  switch (screen) {
  case OLED_SCREEN_PRESET:
    preset_screen_commit(mod);
    break;
  case OLED_SCREEN_CTRL:
    ctrl_screen_commit(ctrl, mod);
    break;
  default:
    break;
  }

  // LAST: Let LVGL render updates
  lv_timer_handler();
}
