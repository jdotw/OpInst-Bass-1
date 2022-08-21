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

void preset_set_name_screen_set_preset(preset_t *preset, uint8_t index) {
  memset(name_buf, 0, 128);
  name_buf_len = MIN(strlen(preset->name), NAME_BUF_SIZE - 1);
  strncpy(name_buf, preset->name, name_buf_len);
  selected_char_index = 0;
  preset_index = index;
}

lv_obj_t *_preset_set_name_container = NULL;
lv_obj_t *_preset_set_name_label = NULL;

void preset_set_name_screen_init() {
  oled_theme_t *theme = oled_theme_get();

  // Container
  _preset_set_name_container = lv_obj_create(NULL);
  lv_obj_align(_preset_set_name_container, LV_ALIGN_TOP_LEFT, 0, 0);

  // Top Row
  lv_obj_t *save_btn_label = lv_label_create(_preset_set_name_container);
  lv_obj_align(save_btn_label, LV_ALIGN_TOP_RIGHT, -5, 0);
  lv_obj_add_style(save_btn_label, &theme->small_label_style, LV_STATE_DEFAULT);
  lv_label_set_text(save_btn_label, "Save");

  lv_obj_t *cancel_btn_label = lv_label_create(_preset_set_name_container);
  lv_obj_align(cancel_btn_label, LV_ALIGN_TOP_RIGHT, -40, 0);
  lv_obj_add_style(cancel_btn_label, &theme->small_label_style,
                   LV_STATE_DEFAULT);
  lv_label_set_text(cancel_btn_label, "Cancel");

  // Preset Name Label
  _preset_set_name_label = lv_label_create(_preset_set_name_container);
  lv_obj_align(_preset_set_name_label, LV_ALIGN_LEFT_MID, 0, 0);
  lv_obj_add_style(_preset_set_name_label, &theme->large_label_style,
                   LV_STATE_DEFAULT);
  lv_obj_add_style(_preset_set_name_label, &theme->selection_style,
                   LV_PART_SELECTED | LV_STATE_DEFAULT);
  lv_label_set_long_mode(_preset_set_name_label, LV_LABEL_LONG_DOT);

  // Set Selection
  lv_label_set_text_sel_start(_preset_set_name_label, selected_char_index);
  lv_label_set_text_sel_end(_preset_set_name_label, selected_char_index + 1);
}

void _preset_set_name_screen_update() {
  // Preset Name Label
  lv_label_set_text(_preset_set_name_label, name_buf);

  // Set Selection
  lv_label_set_text_sel_start(_preset_set_name_label, selected_char_index);
  lv_label_set_text_sel_end(_preset_set_name_label, selected_char_index + 1);
}

lv_obj_t *preset_set_name_screen() {
  _preset_set_name_screen_update();
  return _preset_set_name_container;
}

void preset_set_name_screen_commit(mod_t *mod) {
  if (mod->changed.up && mod->state.up) {
    // Up (cancel) button is pressed
    // Cancel the preset naming
    preset_set_screen(PRESET_SELECT_SCREEN);
    oled_reload_screen();
  } else if (mod->changed.down && mod->state.down) {
    // Down (save) button is pressed
    // Save to SD Card
    preset_save(preset_index, name_buf);
    preset_set_screen(PRESET_SELECT_SCREEN);
    oled_reload_screen();
  }
}

void preset_set_name_screen_apply_char_delta(int8_t delta) {
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
    oled_reload_screen();
  }
}

void preset_set_name_screen_apply_index_delta(int8_t delta) {
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
    oled_reload_screen();
  }
}
