/*
 * oled_preset.c
 *
 *  Created on: 6 Oct. 2021
 *      Author: jwilson
 */

#include "main.h"
#include "../../../lvgl/lvgl.h"
#include "preset.h"

lv_style_t large_label_style;
lv_style_t small_label_style;

void oled_preset_init()
{
  // Small Label
  lv_style_init(&small_label_style);
  lv_style_set_text_font(&small_label_style, &lv_font_montserrat_10);

  // Preset Name Label
  lv_style_init(&large_label_style);
  lv_style_set_text_font(&large_label_style, &lv_font_montserrat_14);
}

lv_obj_t *_preset_select_top_bar(lv_obj_t *parent)
{
  lv_obj_t *container = lv_obj_create(parent);
  lv_obj_set_size(container, LV_PCT(100), 12);
  lv_obj_align(container, LV_ALIGN_TOP_LEFT, 0, 0);

  lv_obj_t *save_btn_label = lv_label_create(container);
  lv_obj_align(save_btn_label, LV_ALIGN_TOP_RIGHT, -12, 0);
  lv_obj_add_style(save_btn_label, &small_label_style, LV_STATE_DEFAULT);
  lv_label_set_text(save_btn_label, "Save");

  return container;
}

lv_obj_t *_preset_select_bottom_bar(lv_obj_t *parent)
{
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

lv_obj_t *oled_preset_select_screen()
{

  // Get active preset
  preset_t *active_preset = preset_get_active();

  // Container
  lv_obj_t *container = lv_obj_create(NULL);
  lv_obj_align(container, LV_ALIGN_TOP_LEFT, 0, 0);

  // Top Row
  _preset_select_top_bar(container);

  // Bottom Row
  _preset_select_bottom_bar(container);

  // Preset Name Label
  lv_obj_t *name_label = lv_label_create(container);
  lv_obj_align(name_label, LV_ALIGN_LEFT_MID, 0, 0);
  lv_obj_add_style(name_label, &large_label_style, LV_STATE_DEFAULT);
  lv_label_set_long_mode(name_label, LV_LABEL_LONG_DOT);
  lv_label_set_text(name_label, active_preset->name);

  return container;
}
