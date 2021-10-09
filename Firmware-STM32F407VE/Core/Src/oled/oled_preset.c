/*
 * oled_preset.c
 *
 *  Created on: 6 Oct. 2021
 *      Author: jwilson
 */

#include "main.h"
#include "../../../lvgl/lvgl.h"
#include "preset.h"

lv_style_t style_large_label;
lv_style_t style_label;
lv_style_t container_style;

lv_obj_t* oled_preset_select_screen() {

  // Get active preset
  preset_t *active_preset = preset_get_active();

  // Container
  lv_obj_t *container = lv_obj_create(NULL);
  lv_obj_align(container, LV_ALIGN_TOP_LEFT, 0, 0);
  lv_obj_set_flex_flow(container, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(container,  LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);


  // Top Row
  lv_obj_t *top_row = lv_obj_create(container);
  lv_obj_set_flex_flow(top_row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(top_row,  LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  lv_obj_set_size(top_row, LV_PCT(100), LV_SIZE_CONTENT);

  // Content Row
  lv_obj_t *content_row = lv_obj_create(container);
  lv_obj_set_flex_flow(content_row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(content_row,  LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_flex_grow(content_row, 1);
  lv_obj_set_size(content_row, LV_PCT(100), LV_SIZE_CONTENT);

  // Bottom Row
  lv_obj_t *bottom_row = lv_obj_create(container);
  lv_obj_set_flex_flow(bottom_row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(bottom_row,  LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  lv_obj_set_size(bottom_row, LV_PCT(100), LV_SIZE_CONTENT);

  // Label Style
  lv_style_init(&style_large_label);
  lv_style_set_text_font(&style_large_label, &lv_font_montserrat_14);


  // Preset Name Label
  lv_obj_t *name_label = lv_label_create(content_row);
  lv_obj_add_style(name_label, &style_large_label, LV_STATE_DEFAULT);
  lv_label_set_long_mode(name_label, LV_LABEL_LONG_DOT);
  lv_label_set_text(name_label, active_preset->name);

  // Small Label
  lv_color_t label_bg_color = { .full = 0x00 };
  lv_color_t label_fg_color = { .full = 0x00 };
  lv_style_init(&style_label);
  lv_style_set_bg_color(&style_label, label_bg_color);
  lv_style_set_bg_opa(&style_label, LV_OPA_20);
  lv_style_set_text_color(&style_label, label_fg_color);
  lv_style_set_radius(&style_label, 3);
  lv_style_set_border_width(&style_label, 1);
  lv_style_set_border_color(&style_label, label_bg_color);
  lv_style_set_text_font(&style_label, &lv_font_montserrat_10);

  // Save Button
  lv_obj_t *save_btn_label = lv_label_create(top_row);
  lv_obj_add_style(save_btn_label, &style_label, LV_STATE_DEFAULT);
  lv_label_set_text(save_btn_label, "Save");

  // Control Label
  lv_obj_t *ctrl1_label = lv_label_create(bottom_row);
  lv_obj_add_style(ctrl1_label, &style_label, LV_STATE_DEFAULT);
  lv_label_set_text(ctrl1_label, "Mix");


  return container;
}
