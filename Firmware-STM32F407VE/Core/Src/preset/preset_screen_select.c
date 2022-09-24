/*
 * oled_preset.c
 *
 *  Created on: 6 Oct. 2021
 *      Author: jwilson
 */

#include "../../../lvgl/lvgl.h"
#include "main.h"
#include "mod.h"
#include "oled.h"
#include "preset.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

lv_obj_t *_preset_top_bar = NULL;

void _preset_select_top_bar_init(lv_obj_t *parent) {
  oled_theme_t *theme = oled_theme_get();

  _preset_top_bar = lv_obj_create(parent);
  lv_obj_set_size(_preset_top_bar, LV_PCT(100), 12);
  lv_obj_align(_preset_top_bar, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *save_btn_label = lv_label_create(_preset_top_bar);
  lv_obj_align(save_btn_label, LV_ALIGN_TOP_RIGHT, -12, 0);
  lv_obj_add_style(save_btn_label, &theme->small_label_style, LV_STATE_DEFAULT);
  lv_label_set_text(save_btn_label, "Save");
}

lv_obj_t *_preset_bottom_bar = NULL;

void _preset_select_bottom_bar_init(lv_obj_t *parent) {
  oled_theme_t *theme = oled_theme_get();

  _preset_bottom_bar = lv_obj_create(parent);
  lv_obj_set_size(_preset_bottom_bar, LV_PCT(100), 12);
  lv_obj_align(_preset_bottom_bar, LV_ALIGN_BOTTOM_LEFT, 0, 0);

  // Control Label
  lv_obj_t *ctrl1_label = lv_label_create(_preset_bottom_bar);
  lv_obj_add_style(ctrl1_label, &theme->small_label_style, LV_STATE_DEFAULT);
  lv_label_set_text(ctrl1_label, "Mix");
  lv_obj_align(ctrl1_label, LV_ALIGN_TOP_LEFT, 0, 0);
}

lv_obj_t *_preset_container = NULL;
lv_obj_t *_preset_number_label = NULL;
lv_obj_t *_preset_name_label = NULL;

void preset_select_screen_init() {
  oled_theme_t *theme = oled_theme_get();

  // Container
  _preset_container = lv_obj_create(NULL);
  lv_obj_align(_preset_container, LV_ALIGN_TOP_LEFT, 0, 0);

  // Top Row
  _preset_select_top_bar_init(_preset_container);

  // Bottom Row
  _preset_select_bottom_bar_init(_preset_container);

  // Preset Number Label
  _preset_number_label = lv_label_create(_preset_container);
  lv_obj_align(_preset_number_label, LV_ALIGN_LEFT_MID, 0, 0);
  lv_obj_add_style(_preset_number_label, &theme->large_label_style,
                   LV_STATE_DEFAULT);
  lv_label_set_long_mode(_preset_number_label, LV_LABEL_LONG_DOT);

  // Preset Separator Label
  lv_obj_t *separator = lv_label_create(_preset_container);
  lv_obj_align(separator, LV_ALIGN_LEFT_MID, 22, 0);
  lv_obj_add_style(separator, &theme->large_label_style, LV_STATE_DEFAULT);
  lv_label_set_long_mode(separator, LV_LABEL_LONG_DOT);
  lv_label_set_text(separator, "-");

  // Preset Name Label
  _preset_name_label = lv_label_create(_preset_container);
  lv_obj_align(_preset_name_label, LV_ALIGN_LEFT_MID, 32, 0);
  lv_obj_add_style(_preset_name_label, &theme->large_label_style,
                   LV_STATE_DEFAULT);
  lv_label_set_long_mode(_preset_name_label, LV_LABEL_LONG_DOT);
}

void _preset_select_screen_update() {
  // Get active preset
  preset_t *active_preset = preset_get_active();

  // Preset Number Label
  char number_buf[4];
  snprintf(number_buf, 4, "%02d", preset_get_active_index());
  lv_label_set_text(_preset_number_label, number_buf);

  // Preset Name Label
  char *preset_name = "Untitled";
  if (active_preset && active_preset->name && strlen(active_preset->name) > 0) {
    preset_name = active_preset->name;
  }
  lv_label_set_text(_preset_name_label, preset_name);
}

lv_obj_t *preset_select_screen() {
  _preset_select_screen_update();
  return _preset_container;
}

void preset_select_screen_commit() {
  mod_t *mod = mod_get();
  if (mod->state.down) {
    // Down (save) button is pressed
    // Switch to Preset Set Name screen
    preset_set_screen(PRESET_SET_NAME_SCREEN);
    preset_set_name_screen_set_preset(preset_get_active(),
                                      preset_get_active_index());
    oled_reload_screen();
  }
}