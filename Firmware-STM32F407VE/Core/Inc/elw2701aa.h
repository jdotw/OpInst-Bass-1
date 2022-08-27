/*
 * elw2701aa.h
 *
 *  Created on: Sep 15, 2021
 *      Author: jwilson
 */

#ifndef INC_ELW2701AA_H_
#define INC_ELW2701AA_H_

#include "main.h"
#include <stdbool.h>

void elw2701aa_init(SPI_HandleTypeDef *hspi);

typedef void (*elw2701aa_write_data_cb_t)();

void elw2701aa_write_data(SPI_HandleTypeDef *hspi, uint8_t start_x,
                          uint8_t x_len, uint8_t start_y, uint8_t y_len,
                          uint8_t *data, uint16_t data_len,
                          elw2701aa_write_data_cb_t completion_callback,
                          void *completion_userdata);

#endif /* INC_ELW2701AA_H_ */
