#include "commit.h"
#include "main.h"
#include "oled.h"
#include "preset.h"

extern uint32_t timeout;
extern uint32_t timeout_start;

void oled_commit(ctrl_t *ctrl, mod_t *mod) {
  // Check for timeout
  if (timeout > 0 && HAL_GetTick() - timeout_start >= timeout) {
    // Timed out, reset screen to default
    oled_set_screen(OLED_SCREEN_PRESET, 0);
  }

  // Call current screens commit function
  oled_screen_t screen = oled_get_screen();
  switch (screen) {
  case OLED_SCREEN_PRESET:
    preset_screen_commit(mod);
    break;
  default:
    break;
  }

  // LAST: Let LVGL render updates
  lv_timer_handler();
}
