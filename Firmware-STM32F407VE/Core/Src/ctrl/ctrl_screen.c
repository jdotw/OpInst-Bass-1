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

lv_obj_t *ctrl_screen() { return ctrl_changed_screen(); }

void ctrl_screen_commit(ctrl_t *ctrl, mod_t *mod) {
  ctrl_changed_screen_commit(ctrl, mod);
}
