#include "commit.h"
#include "ctrl.h"
#include "main.h"
#include "oled.h"
#include "preset.h"

extern uint32_t timeout;
extern uint32_t timeout_start;
extern bool reload_requested;

void oled_commit(void) {
  uint32_t total_ticks_before = HAL_GetTick();
  uint32_t ticks_before = 0;
  uint32_t ticks_after = 0;
  uint32_t ticks_cost = 0;

  ticks_before = HAL_GetTick();

  ctrl_t *ctrl = ctrl_get_active();
  mod_t *mod = mod_get();

  // Check for timeout or pending reload
  if (timeout > 0 && (HAL_GetTick() - timeout_start) >= timeout) {
    // Timed out, reset screen to default
    oled_set_screen(OLED_SCREEN_PRESET, 0);
  } else if (reload_requested) {
    lv_obj_t *screen = oled_render_active_screen();
    if (screen) {
      lv_scr_load(screen);
    }
    reload_requested = false;
  }

  // Call current screens commit function
  oled_screen_t screen = oled_get_screen();
  switch (screen) {
  case OLED_SCREEN_PRESET:
    preset_screen_commit(mod);
    break;
  case OLED_SCREEN_CTRL:
    ctrl_changed_screen_commit(ctrl, mod);
    break;
  default:
    break;
  }

  ticks_after = HAL_GetTick();
  ticks_cost = ticks_after - ticks_before;
  if (reload_requested) {
    printf("%li", ticks_cost);
  }

  // LAST: Let LVGL render updates
  lv_timer_handler();

  ticks_after = HAL_GetTick();
  ticks_cost = ticks_after - ticks_before;
  if (reload_requested) {
    printf("%li", ticks_cost);
  }
}
