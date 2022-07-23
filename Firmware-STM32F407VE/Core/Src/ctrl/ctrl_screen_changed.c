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

lv_obj_t *_ctrl_changed_top_bar(lv_obj_t *parent) {
  oled_theme_t *theme = oled_theme_get();

  lv_obj_t *container = lv_obj_create(parent);
  lv_obj_set_size(container, LV_PCT(100), 12);
  lv_obj_align(container, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *min_btn_label = lv_label_create(container);
  lv_obj_align(min_btn_label, LV_ALIGN_TOP_RIGHT, -48, 0);
  lv_obj_add_style(min_btn_label, &theme->small_label_style, LV_STATE_DEFAULT);
  lv_label_set_text(min_btn_label, "Min");

  lv_obj_t *max_btn_label = lv_label_create(container);
  lv_obj_align(max_btn_label, LV_ALIGN_TOP_RIGHT, -12, 0);
  lv_obj_add_style(max_btn_label, &theme->small_label_style, LV_STATE_DEFAULT);
  lv_label_set_text(max_btn_label, "Max");

  return container;
}

lv_obj_t *_ctrl_changed_bottom_bar(lv_obj_t *parent) {
  // oled_theme_t *theme = oled_theme_get();

  lv_obj_t *container = lv_obj_create(parent);
  lv_obj_set_size(container, LV_PCT(100), 12);
  lv_obj_align(container, LV_ALIGN_BOTTOM_LEFT, 0, 0);

  // // Control Label
  // lv_obj_t *ctrl1_label = lv_label_create(container);
  // lv_obj_add_style(ctrl1_label, &theme->small_label_style, LV_STATE_DEFAULT);
  // lv_label_set_text(ctrl1_label, "Mix");
  // lv_obj_align(ctrl1_label, LV_ALIGN_TOP_LEFT, 0, 0);

  return container;
}

lv_obj_t *ctrl_changed_screen() {
  oled_theme_t *theme = oled_theme_get();

  // Container
  lv_obj_t *container = lv_obj_create(NULL);
  lv_obj_align(container, LV_ALIGN_TOP_LEFT, 0, 0);

  // Top Row
  _ctrl_changed_top_bar(container);

  // Bottom Row
  // _ctrl_changed_bottom_bar(container);

  // Determine Offset
  lv_coord_t y_offset = 7;
  if (changed_ctrl2 == CTRL_ENUM_MAX) {
    y_offset = 0;
  }

  // Left Control Value Label
  char value_buf[6];
  lv_obj_t *value_label = lv_label_create(container);
  lv_obj_align(value_label, LV_ALIGN_LEFT_MID, 0, -1 * y_offset);
  lv_obj_add_style(value_label, &theme->large_label_style, LV_STATE_DEFAULT);
  lv_label_set_long_mode(value_label, LV_LABEL_LONG_DOT);
  snprintf(value_buf, 6, "%3d%%", ctrl_value_get_percent(changed_ctrl1));
  lv_label_set_text(value_label, value_buf);

  // Left Control Name Label
  lv_obj_t *name_label = lv_label_create(container);
  lv_obj_align(name_label, LV_ALIGN_LEFT_MID, 40, -1 * y_offset);
  lv_obj_add_style(name_label, &theme->small_label_style, LV_STATE_DEFAULT);
  lv_label_set_long_mode(name_label, LV_LABEL_LONG_DOT);
  lv_label_set_text(name_label, ctrl_get_name(changed_ctrl1));

  if (changed_ctrl2 != CTRL_ENUM_MAX) {
    // Right Control Value Label
    char value_buf[6];
    lv_obj_t *value_label = lv_label_create(container);
    lv_obj_align(value_label, LV_ALIGN_LEFT_MID, 0, y_offset);
    lv_obj_add_style(value_label, &theme->large_label_style, LV_STATE_DEFAULT);
    lv_label_set_long_mode(value_label, LV_LABEL_LONG_DOT);
    snprintf(value_buf, 6, "%3d%%", ctrl_value_get_percent(changed_ctrl2));
    lv_label_set_text(value_label, value_buf);

    // Right Control Name Label
    lv_obj_t *name_label = lv_label_create(container);
    lv_obj_align(name_label, LV_ALIGN_LEFT_MID, 40, y_offset);
    lv_obj_add_style(name_label, &theme->small_label_style, LV_STATE_DEFAULT);
    lv_label_set_long_mode(name_label, LV_LABEL_LONG_DOT);
    lv_label_set_text(name_label, ctrl_get_name(changed_ctrl2));
  }

  return container;
}

void ctrl_changed_screen_commit(ctrl_t *ctrl, mod_t *mod) {
  if ((changed_ctrl1 != CTRL_ENUM_MAX && ctrl->changed[changed_ctrl1]) ||
      ((changed_ctrl2 != CTRL_ENUM_MAX) && ctrl->changed[changed_ctrl2])) {
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