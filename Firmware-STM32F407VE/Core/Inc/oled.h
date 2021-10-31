/*
 * oled.h
 *
 *  Created on: 3 Oct. 2021
 *      Author: jwilson
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "../../../lvgl/lvgl.h"
#include "mod.h"
#include "preset.h"

// State
typedef enum {
  OLED_SCREEN_PRESET = 0,
  OLED_SCREEN_CTRL,
} oled_screen_t;

// Init
void oled_init(SPI_HandleTypeDef *hspi);
void oled_test(void);

// Screen
oled_screen_t oled_get_screen(void);
void oled_set_screen(oled_screen_t screen, uint32_t timeout_ms);
void oled_reload_screen();

// Theme
typedef struct {
  lv_style_t large_label_style;
  lv_style_t small_label_style;
  lv_style_t selection_style;
} oled_theme_t;

void oled_theme_init(void);
oled_theme_t *oled_theme_get(void);

// Commit
lv_obj_t *oled_render_active_screen();
void oled_commit(ctrl_t *ctrl, mod_t *mod);

#endif /* INC_OLED_H_ */
