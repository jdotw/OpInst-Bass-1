/*
 * oled.c
 *
 *  Created on: 3 Oct. 2021
 *      Author: jwilson
 */

#include "../../../lvgl/lvgl.h"
#include "main.h"
#include "oled.h"

oled_theme_t default_theme;

oled_theme_t *oled_theme_get() { return &default_theme; }

void oled_theme_init() {
  // Small Label
  lv_style_init(&default_theme.small_label_style);
  lv_style_set_text_font(&default_theme.small_label_style,
                         &lv_font_montserrat_10);

  // Preset Name Label
  lv_style_init(&default_theme.large_label_style);
  lv_style_set_text_font(&default_theme.large_label_style,
                         &lv_font_montserrat_14);

  // Text Selection Style
  lv_color_t selection_bg = {.full = 0x80};
  lv_color_t selection_fg = {.full = 0xFF};
  lv_style_init(&default_theme.selection_style);
  lv_style_set_bg_color(&default_theme.selection_style, selection_bg);
  lv_style_set_text_color(&default_theme.selection_style, selection_fg);
}
