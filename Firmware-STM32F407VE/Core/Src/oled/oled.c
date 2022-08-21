/*
 * oled.c
 *
 *  Created on: 3 Oct. 2021
 *      Author: jwilson
 */

#include "oled.h"
#include "../../../lvgl/lvgl.h"
#include "elw2701aa.h"
#include "main.h"

#define OLED_HORIZ_RES 176
#define OLED_VERT_RES 52

lv_disp_t *disp;
SPI_HandleTypeDef *spi;

oled_screen_t active_screen = OLED_SCREEN_PRESET;
uint32_t timeout = 0;
uint32_t timeout_start = 0;
bool reload_requested = true; // true to perform initial load

void _oled_flush_callback(struct _lv_disp_drv_t *disp_drv,
                          const lv_area_t *area, lv_color_t *color_p);

// Init

void oled_init(SPI_HandleTypeDef *hspi) {

  // Init OLED Display Hardware
  spi = hspi;
  elw2701aa_init(spi);

  // Init LVGL
  lv_init();

  // Create draw buffers (x2)
  static lv_disp_draw_buf_t disp_buf;
  static lv_color_t buf_1[OLED_HORIZ_RES * OLED_VERT_RES];
  static lv_color_t buf_2[OLED_HORIZ_RES * OLED_VERT_RES];
  lv_disp_draw_buf_init(&disp_buf, buf_1, buf_2,
                        OLED_HORIZ_RES * OLED_VERT_RES);

  // Create driver
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.draw_buf = &disp_buf;
  disp_drv.hor_res = OLED_HORIZ_RES;
  disp_drv.ver_res = OLED_VERT_RES;
  disp_drv.flush_cb = _oled_flush_callback;

  // Register driver
  disp = lv_disp_drv_register(&disp_drv);

  // Init Theme
  oled_theme_init();

  // Init Screens
  preset_select_screen_init();
  preset_set_name_screen_init();
  ctrl_changed_screen_init();
}

// State

lv_obj_t *oled_render_active_screen() {
  lv_obj_t *screen = NULL;
  switch (active_screen) {
  case OLED_SCREEN_PRESET:
    screen = preset_screen();
    break;
  case OLED_SCREEN_CTRL:
    screen = ctrl_screen();
    break;
  }
  return screen;
}

oled_screen_t oled_get_screen() { return active_screen; }

void oled_set_screen(oled_screen_t screen, uint32_t timeout_ms) {
  timeout = timeout_ms;
  timeout_start = HAL_GetTick();
  active_screen = screen;
  reload_requested = true;
}

void oled_reload_screen() { reload_requested = true; }

// LVGL Flush Callback
// This function is called by LVGL
// to write the graphics buffer to the display

void _oled_flush_callback(struct _lv_disp_drv_t *disp_drv,
                          const lv_area_t *area, lv_color_t *color_p) {
  uint8_t _data_from_color(uint16_t i) {
    // Returns the 2x 4bit pixels as one byte
    uint16_t color_index = i * 2;
    uint8_t byte = 0x00;
    byte |= color_p[color_index].full & 0xf0;
    byte |= color_p[color_index + 1].full >> 4;
    return byte;
  }

  uint32_t total_ticks_before = HAL_GetTick();
  uint32_t ticks_before = 0;
  uint32_t ticks_after = 0;
  uint32_t ticks_cost = 0;

  ticks_before = HAL_GetTick();
  elw2701aa_write_data(spi, area->x1, (area->x2 - area->x1) + 1, area->y1,
                       (area->y2 - area->y1) + 1, _data_from_color);
  ticks_after = HAL_GetTick();
  ticks_cost = ticks_after - ticks_before;
  printf("%li", ticks_cost);

  lv_disp_flush_ready(disp_drv);
}
