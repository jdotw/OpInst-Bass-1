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

lv_style_t large_label_style;
lv_style_t small_label_style;
lv_style_t selection_style;

lv_obj_t *_preset_select_top_bar(lv_obj_t *parent) {
  lv_obj_t *container = lv_obj_create(parent);
  lv_obj_set_size(container, LV_PCT(100), 12);
  lv_obj_align(container, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *save_btn_label = lv_label_create(container);
  lv_obj_align(save_btn_label, LV_ALIGN_TOP_RIGHT, -12, 0);
  lv_obj_add_style(save_btn_label, &small_label_style, LV_STATE_DEFAULT);
  lv_label_set_text(save_btn_label, "Save");

  return container;
}

lv_obj_t *_preset_select_bottom_bar(lv_obj_t *parent) {
  lv_obj_t *container = lv_obj_create(parent);
  lv_obj_set_size(container, LV_PCT(100), 12);
  lv_obj_align(container, LV_ALIGN_BOTTOM_LEFT, 0, 0);

  // Control Label
  lv_obj_t *ctrl1_label = lv_label_create(container);
  lv_obj_add_style(ctrl1_label, &small_label_style, LV_STATE_DEFAULT);
  lv_label_set_text(ctrl1_label, "Mix");
  lv_obj_align(ctrl1_label, LV_ALIGN_TOP_LEFT, 0, 0);

  return container;
}

lv_obj_t *preset_select_screen() {
  oled_theme_t *theme = oled_theme_get();

  // Get active preset
  preset_t *active_preset = preset_get_active();

  // Container
  lv_obj_t *container = lv_obj_create(NULL);
  lv_obj_align(container, LV_ALIGN_TOP_LEFT, 0, 0);

  // Top Row
  _preset_select_top_bar(container);

  // Bottom Row
  _preset_select_bottom_bar(container);

  // Preset Number Label
  char number_buf[4];
  lv_obj_t *number_label = lv_label_create(container);
  lv_obj_align(number_label, LV_ALIGN_LEFT_MID, 0, 0);
  lv_obj_add_style(number_label, &theme->large_label_style, LV_STATE_DEFAULT);
  lv_label_set_long_mode(number_label, LV_LABEL_LONG_DOT);
  snprintf(number_buf, 4, "%02d", preset_get_active_index());
  lv_label_set_text(number_label, number_buf);

  // Preset Separator Label
  lv_obj_t *separator_label = lv_label_create(container);
  lv_obj_align(separator_label, LV_ALIGN_LEFT_MID, 22, 0);
  lv_obj_add_style(separator_label, &theme->large_label_style,
                   LV_STATE_DEFAULT);
  lv_label_set_long_mode(separator_label, LV_LABEL_LONG_DOT);
  lv_label_set_text(separator_label, "-");

  char *preset_name = "Untitled";
  if (active_preset && active_preset->name && strlen(active_preset->name) > 0) {
    preset_name = active_preset->name;
  }

  // Preset Name Label
  lv_obj_t *name_label = lv_label_create(container);
  lv_obj_align(name_label, LV_ALIGN_LEFT_MID, 32, 0);
  lv_obj_add_style(name_label, &theme->large_label_style, LV_STATE_DEFAULT);
  lv_label_set_long_mode(name_label, LV_LABEL_LONG_DOT);
  lv_label_set_text(name_label, preset_name);

  return container;
}

void preset_select_screen_commit(mod_t *mod) {
  if (mod->changed.down && mod->state.down) {
    // Down (save) button is pressed
    // Switch to Preset Set Name screen
    preset_set_screen(PRESET_SET_NAME_SCREEN);
    preset_set_name_screen_set_preset(preset_get_active(),
                                      preset_get_active_index());
    oled_reload_screen();
  }
}