/*
 * preset.h
 *
 *  Created on: 6 Oct. 2021
 *      Author: jwilson
 */

#ifndef INC_PRESET_H_
#define INC_PRESET_H_

typedef struct {
  char *name;
} preset_t;


void preset_init(void);
preset_t* preset_get_active(void);

#endif /* INC_PRESET_H_ */
