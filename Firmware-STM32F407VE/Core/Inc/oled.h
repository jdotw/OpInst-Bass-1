/*
 * oled.h
 *
 *  Created on: 3 Oct. 2021
 *      Author: jwilson
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "../../../lvgl/lvgl.h"
#include "mod.h"
#include "preset.h"

// State
typedef enum {
  OLED_NONE = 0,
  OLED_SELECT_PRESET,
  OLED_NAME_PRESET,
} oled_state_t;
extern oled_state_t oled_state;
extern bool reload_oled;

// Init
void oled_init(SPI_HandleTypeDef *hspi);
void oled_test(void);

// Commit
void oled_commit(ctrl_t *ctrl, mod_t *mod);

// Preset Screens
void oled_preset_init(void);
lv_obj_t *oled_preset_select_screen(void);
lv_obj_t *oled_preset_name_screen(void);
void oled_preset_name_set_preset(preset_t *preset, uint8_t index);
void oled_preset_apply_char_delta(int8_t delta);
void oled_preset_apply_index_delta(int8_t delta);
void oled_preset_name_save(void);

#endif /* INC_OLED_H_ */
