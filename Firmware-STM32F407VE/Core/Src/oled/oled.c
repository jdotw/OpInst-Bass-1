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

lv_disp_t *disp;
SPI_HandleTypeDef *spi;

oled_screen_t active_screen = OLED_SCREEN_PRESET;
uint32_t timeout = 0;
uint32_t timeout_start = 0;
bool reload_requested = true; // true to perform initial load

void _oled_flush_callback(struct _lv_disp_drv_t *disp_drv,
                          const lv_area_t *area, lv_color_t *color_p);

// Init

static lv_disp_draw_buf_t _disp_buf;
static lv_color_t _buf_1[OLED_BUFFER_SIZE];
static lv_color_t _buf_2[OLED_BUFFER_SIZE];
static lv_disp_drv_t disp_drv;

void oled_init(SPI_HandleTypeDef *hspi) {

  // Init OLED Display Hardware
  spi = hspi;
  elw2701aa_init(spi);

  // Init LVGL
  lv_init();

  // Create draw buffers (x2)
  lv_disp_draw_buf_init(&_disp_buf, _buf_1, _buf_2, OLED_BUFFER_SIZE);

  // Create driver
  lv_disp_drv_init(&disp_drv);
  disp_drv.draw_buf = &_disp_buf;
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

void _oled_flush_callback_write_completed(void *userdata) {
  struct _lv_disp_drv_t *disp_drv = userdata;
  lv_disp_flush_ready(disp_drv);
}

void _oled_flush_callback(struct _lv_disp_drv_t *disp_drv,
                          const lv_area_t *area, lv_color_t *color_p) {

  uint8_t x_len = (area->x2 - area->x1) + 1;
  uint8_t y_len = (area->y2 - area->y1) + 1;

  // color_p is essentially an array of uint8_t's
  // therefore we cast it as uint8_t* when passing
  // it to elw2701aa_write_data as our data buffer
  elw2701aa_write_data(spi, area->x1, x_len, area->y1, y_len,
                       (uint8_t *)color_p, x_len * y_len,
                       &_oled_flush_callback_write_completed, disp_drv);
}
