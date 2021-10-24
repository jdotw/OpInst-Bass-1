/*
 * oled_preset.c
 *
 *  Created on: 6 Oct. 2021
 *      Author: jwilson
 */

#include "../../../lvgl/lvgl.h"
#include "main.h"
#include "preset.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

lv_style_t large_label_style;
lv_style_t small_label_style;
lv_style_t selection_style;

void oled_preset_init() {
  // Small Label
  lv_style_init(&small_label_style);
  lv_style_set_text_font(&small_label_style, &lv_font_montserrat_10);

  // Preset Name Label
  lv_style_init(&large_label_style);
  lv_style_set_text_font(&large_label_style, &lv_font_montserrat_14);

  // Text Selection Style
  lv_color_t selection_bg = {.full = 0x80};
  lv_color_t selection_fg = {.full = 0xFF};
  lv_style_init(&selection_style);
  lv_style_set_bg_color(&selection_style, selection_bg);
  lv_style_set_text_color(&selection_style, selection_fg);
}

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

lv_obj_t *oled_preset_select_screen() {
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
  lv_obj_add_style(number_label, &large_label_style, LV_STATE_DEFAULT);
  lv_label_set_long_mode(number_label, LV_LABEL_LONG_DOT);
  snprintf(number_buf, 4, "%02d", preset_get_active_index());
  lv_label_set_text(number_label, number_buf);

  // Preset Separator Label
  lv_obj_t *separator_label = lv_label_create(container);
  lv_obj_align(separator_label, LV_ALIGN_LEFT_MID, 22, 0);
  lv_obj_add_style(separator_label, &large_label_style, LV_STATE_DEFAULT);
  lv_label_set_long_mode(separator_label, LV_LABEL_LONG_DOT);
  lv_label_set_text(separator_label, "-");

  char *preset_name = "Untitled";
  if (active_preset && active_preset->name && strlen(active_preset->name) > 0) {
    preset_name = active_preset->name;
  }

  // Preset Name Label
  lv_obj_t *name_label = lv_label_create(container);
  lv_obj_align(name_label, LV_ALIGN_LEFT_MID, 32, 0);
  lv_obj_add_style(name_label, &large_label_style, LV_STATE_DEFAULT);
  lv_label_set_long_mode(name_label, LV_LABEL_LONG_DOT);
  lv_label_set_text(name_label, preset_name);

  return container;
}

#define NAME_BUF_SIZE 128

char name_buf[NAME_BUF_SIZE] = {0};
uint8_t name_buf_len = 0;

uint8_t selected_char_index = 0;

#ifndef MIN
#define MIN(a, b) ((a) < (b)) ? (a) : (b)
#endif
#ifndef MAX
#define MAX(a, b) ((a) > (b)) ? (a) : (b)
#endif

uint8_t preset_index;

void oled_preset_name_set_preset(preset_t *preset, uint8_t index) {
  memset(name_buf, 0, 128);
  name_buf_len = MIN(strlen(preset->name), NAME_BUF_SIZE - 1);
  strncpy(name_buf, preset->name, name_buf_len);
  selected_char_index = 0;
  preset_index = index;
}

lv_obj_t *oled_preset_name_screen() {
  // Container
  lv_obj_t *container = lv_obj_create(NULL);
  lv_obj_align(container, LV_ALIGN_TOP_LEFT, 0, 0);

  // Top Row
  lv_obj_t *save_btn_label = lv_label_create(container);
  lv_obj_align(save_btn_label, LV_ALIGN_TOP_RIGHT, -5, 0);
  lv_obj_add_style(save_btn_label, &small_label_style, LV_STATE_DEFAULT);
  lv_label_set_text(save_btn_label, "Save");

  lv_obj_t *cancel_btn_label = lv_label_create(container);
  lv_obj_align(cancel_btn_label, LV_ALIGN_TOP_RIGHT, -40, 0);
  lv_obj_add_style(cancel_btn_label, &small_label_style, LV_STATE_DEFAULT);
  lv_label_set_text(cancel_btn_label, "Cancel");

  // Preset Name Label
  lv_obj_t *name_label = lv_label_create(container);
  lv_obj_align(name_label, LV_ALIGN_LEFT_MID, 0, 0);
  lv_obj_add_style(name_label, &large_label_style, LV_STATE_DEFAULT);
  lv_obj_add_style(name_label, &selection_style,
                   LV_PART_SELECTED | LV_STATE_DEFAULT);
  lv_label_set_long_mode(name_label, LV_LABEL_LONG_DOT);
  lv_label_set_text(name_label, name_buf);

  // Set Selection
  lv_label_set_text_sel_start(name_label, selected_char_index);
  lv_label_set_text_sel_end(name_label, selected_char_index + 1);

  return container;
}

void oled_preset_apply_char_delta(int8_t delta) {
  bool is_valid = false;
  uint8_t new_char = ((uint8_t)name_buf[selected_char_index]) + delta;

  if ((new_char >= 48 && new_char <= 57) ||
      (new_char >= 65 && new_char <= 90) ||
      (new_char >= 97 && new_char <= 122)) {
    is_valid = true;
  } else if (new_char < 48) {
    new_char = 122;
    is_valid = true;
  } else if (new_char < 65) {
    new_char = 57;
    is_valid = true;
  } else if (new_char < 97) {
    new_char = 90;
    is_valid = true;
  } else {
    new_char = 48; // wrap from z to 0
    is_valid = true;
  }

  if (is_valid) {
    name_buf[selected_char_index] = new_char;
    lv_scr_load_anim(oled_preset_name_screen(), LV_SCR_LOAD_ANIM_NONE, 0, 0,
                     true);
  }
}

void oled_preset_apply_index_delta(int8_t delta) {
  bool did_apply = false;
  if (delta < 0) {
    if (selected_char_index >= (delta * -1)) {
      selected_char_index += delta;
      did_apply = true;
    }
  } else {
    if (selected_char_index + delta < strlen(name_buf)) {
      selected_char_index += delta;
      did_apply = true;
    }
  }
  if (did_apply) {
    lv_scr_load_anim(oled_preset_name_screen(), LV_SCR_LOAD_ANIM_NONE, 0, 0,
                     true);
  }
}

void oled_preset_name_save() { preset_save(preset_index, name_buf); }
