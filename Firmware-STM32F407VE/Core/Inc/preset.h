/*
 * preset.h
 *
 *  Created on: 6 Oct. 2021
 *      Author: jwilson
 */

#ifndef INC_PRESET_H_
#define INC_PRESET_H_

#include "ctrl.h"
#include "mod.h"
#include "seq.h"

#define PRESET_CATALOG_MAX 100

#define PRESET_NAME_MAX_LENGTH 128

typedef struct preset_struct {
  bool present;
  char name[PRESET_NAME_MAX_LENGTH];
} preset_t;

// preset.c
void preset_init(void);
preset_t *preset_get_active(void);
preset_t *preset_get_system(uint8_t index);
preset_t *preset_get_user(uint8_t index);
uint8_t preset_get_active_index(void);

void preset_select_apply_delta(uint8_t delta);

bool preset_save(uint8_t index, char *name);
bool preset_load(bool system, uint8_t index);

// preset_screen.c

typedef enum {
  PRESET_SELECT_SCREEN = 0,
  PRESET_SET_NAME_SCREEN,
} preset_screen_t;

void preset_set_screen(preset_screen_t screen);
preset_screen_t preset_get_screen();

lv_obj_t *preset_screen();
void preset_screen_commit();

// preset_screen_set_name.c
lv_obj_t *preset_set_name_screen(void);
void preset_set_name_screen_init(void);
void preset_set_name_screen_commit(void);
void preset_set_name_screen_set_preset(preset_t *preset, uint8_t index);
void preset_set_name_screen_apply_char_delta(int8_t delta);
void preset_set_name_screen_apply_index_delta(int8_t delta);

// preset_screen_select.c
lv_obj_t *preset_select_screen(void);
void preset_select_screen_init(void);
void preset_select_screen_commit(void);

#endif /* INC_PRESET_H_ */
