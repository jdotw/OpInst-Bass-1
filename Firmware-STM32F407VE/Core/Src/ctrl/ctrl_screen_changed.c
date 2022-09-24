/*
 * ctrl_screen.c
 *
 *  Created on: 31 Oct 2021
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

ctrl_enum_t changed_ctrl1 = CTRL_ENUM_MAX;
ctrl_enum_t changed_ctrl2 = CTRL_ENUM_MAX;
uint32_t last_push = 0;
ctrl_enum_t last_pushed_ctrl = CTRL_ENUM_MAX;

#define LAST_PUSH_TIMEOUT_MS 2000

void ctrl_changed_screen_push_ctrl(ctrl_enum_t ctrl_enum) {
  if (ctrl_enum == changed_ctrl1) {
    // ctrl_enum is already changed_ctrl1
    // NO OP
    last_push = HAL_GetTick();
    last_pushed_ctrl = ctrl_enum;
    return;
  } else if (ctrl_enum == changed_ctrl2) {
    // ctrl_enum is changed_ctrl2 already
    // Treat this as a no-op so that the two
    // active ctrl values show on screen in
    // the same order
    last_push = HAL_GetTick();
    last_pushed_ctrl = ctrl_enum;
    return;
  } else if (HAL_GetTick() - last_push > LAST_PUSH_TIMEOUT_MS) {
    changed_ctrl1 = ctrl_enum;
    changed_ctrl2 = CTRL_ENUM_MAX;
    last_push = HAL_GetTick();
    last_pushed_ctrl = ctrl_enum;
  } else {
    // Push changed_ctrl1 to changed_ctrl2 and
    // then set changed_ctrl1 to ctrl_enum
    changed_ctrl2 = changed_ctrl1;
    changed_ctrl1 = ctrl_enum;
    last_push = HAL_GetTick();
    last_pushed_ctrl = ctrl_enum;
  }
}

/*
 * Top Bar
 */

lv_obj_t *top_bar = NULL;
lv_obj_t *min_btn_label = NULL;
lv_obj_t *max_btn_label = NULL;

void _ctrl_changed_top_bar_init(lv_obj_t *parent) {
  oled_theme_t *theme = oled_theme_get();

  top_bar = lv_obj_create(parent);
  lv_obj_set_size(top_bar, LV_PCT(100), 12);
  lv_obj_align(top_bar, LV_ALIGN_TOP_LEFT, 0, 0);

  min_btn_label = lv_label_create(top_bar);
  lv_obj_align(min_btn_label, LV_ALIGN_TOP_RIGHT, -48, 0);
  lv_obj_add_style(min_btn_label, &theme->small_label_style, LV_STATE_DEFAULT);

  max_btn_label = lv_label_create(top_bar);
  lv_obj_align(max_btn_label, LV_ALIGN_TOP_RIGHT, -12, 0);
  lv_obj_add_style(max_btn_label, &theme->small_label_style, LV_STATE_DEFAULT);
}

void _ctrl_changed_top_bar_update() {
  lv_label_set_text(min_btn_label, "Min");
  lv_label_set_text(max_btn_label, "Max");
}

/*
 * Botttom Bar
 */

lv_obj_t *bottom_bar = NULL;
lv_obj_t *ctrl1_label = NULL;

void _ctrl_changed_bottom_bar_init(lv_obj_t *parent) {
  oled_theme_t *theme = oled_theme_get();

  bottom_bar = lv_obj_create(parent);
  lv_obj_set_size(bottom_bar, LV_PCT(100), 12);
  lv_obj_align(bottom_bar, LV_ALIGN_BOTTOM_LEFT, 0, 0);

  // Control Label
  ctrl1_label = lv_label_create(bottom_bar);
  lv_obj_add_style(ctrl1_label, &theme->small_label_style, LV_STATE_DEFAULT);
  lv_obj_align(ctrl1_label, LV_ALIGN_TOP_LEFT, 0, 0);
}

void _ctrl_changed_bottom_bar_update() {
  lv_label_set_text(ctrl1_label, "Mix");
}

/*
 * Screen
 */

lv_obj_t *container = NULL;
lv_obj_t *value_label = NULL;
lv_obj_t *name_label = NULL;
lv_obj_t *value2_label = NULL;
lv_obj_t *name2_label = NULL;

lv_obj_t *ctrl_changed_screen_init() {
  uint32_t ticks_before = 0;
  uint32_t ticks_after = 0;
  uint32_t ticks_cost = 0;

  ticks_before = HAL_GetTick();

  oled_theme_t *theme = oled_theme_get();

  // Container
  container = lv_obj_create(NULL);
  lv_obj_align(container, LV_ALIGN_TOP_LEFT, 0, 0);

  // Top Row
  _ctrl_changed_top_bar_init(container);

  // Bottom Row
  _ctrl_changed_bottom_bar_init(container);

  // Determine Offset
  lv_coord_t y_offset = 7;
  if (changed_ctrl2 == CTRL_ENUM_MAX) {
    y_offset = 0;
  }

  // Left Control Value Label
  value_label = lv_label_create(container);
  lv_obj_align(value_label, LV_ALIGN_LEFT_MID, 0, -1 * y_offset);
  lv_obj_add_style(value_label, &theme->large_label_style, LV_STATE_DEFAULT);
  lv_label_set_long_mode(value_label, LV_LABEL_LONG_DOT);

  // Left Control Name Label
  name_label = lv_label_create(container);
  lv_obj_align(name_label, LV_ALIGN_LEFT_MID, 40, -1 * y_offset);
  lv_obj_add_style(name_label, &theme->small_label_style, LV_STATE_DEFAULT);
  lv_label_set_long_mode(name_label, LV_LABEL_LONG_DOT);

  // Right Control Value Label
  value2_label = lv_label_create(container);
  lv_obj_align(value2_label, LV_ALIGN_LEFT_MID, 0, y_offset);
  lv_obj_add_style(value2_label, &theme->large_label_style, LV_STATE_DEFAULT);
  lv_label_set_long_mode(value2_label, LV_LABEL_LONG_DOT);

  // Right Control Name Label
  name2_label = lv_label_create(container);
  lv_obj_align(name2_label, LV_ALIGN_LEFT_MID, 40, y_offset);
  lv_obj_add_style(name2_label, &theme->small_label_style, LV_STATE_DEFAULT);
  lv_label_set_long_mode(name2_label, LV_LABEL_LONG_DOT);

  ticks_after = HAL_GetTick();
  ticks_cost = ticks_after - ticks_before;
  printf("%li", ticks_cost);

  return container;
}

lv_obj_t *_ctrl_changed_screen_update() {
  uint32_t ticks_before = 0;
  uint32_t ticks_after = 0;
  uint32_t ticks_cost = 0;

  ticks_before = HAL_GetTick();

  // Top Row
  _ctrl_changed_top_bar_update(container);

  // Bottom Row
  _ctrl_changed_bottom_bar_update(container);

  // Determine Offset
  lv_coord_t y_offset = 7;
  if (changed_ctrl2 == CTRL_ENUM_MAX) {
    y_offset = 0;
  }

  // Control Value Label
  char value_buf[6];
  lv_obj_align(value_label, LV_ALIGN_LEFT_MID, 0, -1 * y_offset);
  snprintf(value_buf, 6, "%3d%%", ctrl_value_get_percent(changed_ctrl1));
  lv_label_set_text(value_label, value_buf);

  // Control Name Label
  lv_obj_align(name_label, LV_ALIGN_LEFT_MID, 40, -1 * y_offset);
  lv_label_set_text(name_label, ctrl_get_name(changed_ctrl1));

  if (changed_ctrl2 != CTRL_ENUM_MAX) {
    // Secondary Control Value Label
    char value_buf[6];
    lv_obj_align(value2_label, LV_ALIGN_LEFT_MID, 0, y_offset);
    snprintf(value_buf, 6, "%3d%%", ctrl_value_get_percent(changed_ctrl2));
    lv_label_set_text(value2_label, value_buf);

    // Secondary Control Name Label
    lv_obj_align(name2_label, LV_ALIGN_LEFT_MID, 40, y_offset);
    lv_label_set_text(name2_label, ctrl_get_name(changed_ctrl2));
  } else {
    // Hide Secondary Control Labelss
    lv_label_set_text(value2_label, "");
    lv_label_set_text(name2_label, "");
  }

  ticks_after = HAL_GetTick();
  ticks_cost = ticks_after - ticks_before;
  printf("%li", ticks_cost);

  return container;
}

lv_obj_t *ctrl_screen() {
  _ctrl_changed_screen_update();
  return container;
}

void ctrl_changed_screen_commit() {
  mod_t *mod = mod_get();
  if (changed_ctrl1 != CTRL_ENUM_MAX || changed_ctrl2 != CTRL_ENUM_MAX) {
    oled_reload_screen();
  }
  if (mod->changed.down || mod->changed.up) {
    if (mod->state.down) {
      // Reset to Max
      ctrl_value_set_max(last_pushed_ctrl);
    } else if (mod->state.up) {
      // Reset to Min
      ctrl_value_set_min(last_pushed_ctrl);
    }
    oled_reload_screen();
  }
}