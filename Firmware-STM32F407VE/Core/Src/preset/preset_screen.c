/*
 * oled_preset.c
 *
 *  Created on: 6 Oct. 2021
 *      Author: jwilson
 */

#include "../../../lvgl/lvgl.h"
#include "main.h"
#include "mod.h"
#include "preset.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

preset_screen_t active_preset_screen;

void preset_set_screen(preset_screen_t screen) {
  active_preset_screen = screen;
}
preset_screen_t preset_get_screen() { return active_preset_screen; }

lv_obj_t *preset_screen() {
  switch (active_preset_screen) {
  case PRESET_SELECT_SCREEN:
    return preset_select_screen();
  case PRESET_SET_NAME_SCREEN:
    return preset_set_name_screen();
  default:
    return NULL;
  }
}

void preset_screen_commit() {
  switch (active_preset_screen) {
  case PRESET_SELECT_SCREEN:
    preset_select_screen_commit();
    break;
  case PRESET_SET_NAME_SCREEN:
    preset_set_name_screen_commit();
    break;
  }
}