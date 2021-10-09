/*
 * oled.c
 *
 *  Created on: 3 Oct. 2021
 *      Author: jwilson
 */

#include "main.h"
#include "../../../lvgl/lvgl.h"
#include "elw2701aa.h"

#define OLED_HORIZ_RES 176
#define OLED_VERT_RES 52

lv_disp_t *disp;
SPI_HandleTypeDef *spi;

void _oled_flush_cb(struct _lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p) {
  uint8_t _data_from_color(uint16_t i) {
    // Returns the 2x 4bit pixels as one byte
    uint16_t color_index = i * 2;
    uint8_t byte = 0x00;
    byte |= color_p[color_index].full & 0xf0;
    byte |= color_p[color_index+1].full >> 4;
    return byte;
  }

  elw2701aa_write_data(spi, area->x1, (area->x2 - area->x1)+1, area->y1, (area->y2 - area->y1)+1, _data_from_color);

  lv_disp_flush_ready(disp_drv);
}

void oled_init(SPI_HandleTypeDef *hspi) {

  // Init OLED Display Hardware
  spi = hspi;
  elw2701aa_init(spi);

  // Init LVGL
  lv_init();

  // Create draw buffers (x2)
  static lv_disp_draw_buf_t disp_buf;
  static lv_color_t buf_1[OLED_HORIZ_RES * 10];
  static lv_color_t buf_2[OLED_HORIZ_RES * 10];
  lv_disp_draw_buf_init(&disp_buf, buf_1, buf_2, OLED_HORIZ_RES*10);

  // Create driver
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.draw_buf = &disp_buf;
  disp_drv.hor_res = OLED_HORIZ_RES;
  disp_drv.ver_res = OLED_VERT_RES;
  disp_drv.flush_cb = _oled_flush_cb;

  // Register driver
  disp = lv_disp_drv_register(&disp_drv);

  // Init Screens
  oled_preset_init();
}

void oled_test() {
//  lv_obj_t * label1 = lv_label_create(lv_scr_act());
//  lv_label_set_long_mode(label1, LV_LABEL_LONG_WRAP);     /*Break the long lines*/
//  lv_label_set_recolor(label1, true);                      /*Enable re-coloring by commands in the text*/
//  lv_label_set_text(label1, "#0000ff Re-color# #ff00ff words# #ff0000 of a# label, align the lines to the center "
//      "and wrap long text automatically.");
//  lv_obj_set_width(label1, 150);  /*Set smaller width to make the lines wrap*/
//  lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
//  lv_obj_align(label1, LV_ALIGN_CENTER, 0, -40);
//

  lv_obj_t *label2 = lv_label_create(lv_scr_act());
  lv_label_set_long_mode(label2, LV_LABEL_LONG_SCROLL_CIRCULAR);     /*Circular scroll*/
  lv_obj_set_width(label2, 176);
  lv_label_set_text(label2, "Bass-1 Synth");
  lv_obj_align(label2, LV_ALIGN_CENTER, 0, 0);
}
