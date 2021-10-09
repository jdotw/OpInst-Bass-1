/*
 * oled.h
 *
 *  Created on: 3 Oct. 2021
 *      Author: jwilson
 */

#ifndef INC_OLED_H_
#define INC_OLED_H_

#include "../../../lvgl/lvgl.h"

void oled_init(SPI_HandleTypeDef *hspi);
void oled_test(void);

// Preset Screens
lv_obj_t* oled_preset_select_screen(void);

#endif /* INC_OLED_H_ */
