/*
 * i2c.h
 *
 *  Created on: Apr 12, 2021
 *      Author: jwilson
 */

#ifndef INC_I2C_H_
#define INC_I2C_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

#define I2C_LEFT 0
#define I2C_RIGHT 1

typedef enum {
  I2C_STATE_IDLE = 0,
  I2C_STATE_CHANNEL_SELECT,
  I2C_STATE_DATA,
} i2c_state_enum_t;

typedef enum {
  I2C_DIRECTION_UNKNOWN = 0,
  I2C_DIRECTION_TX,
  I2C_DIRECTION_RX,
} i2c_direction_enum_t;

typedef void (*i2c_callback_t)(uint8_t bus, void *userdata);

typedef struct i2c_bus_s {
  bool interrupted;
  i2c_state_enum_t state;
  i2c_direction_enum_t direction;
  uint8_t mux_addr;
  uint8_t mux_desired_channel;
  uint8_t mux_selected_channel;
  uint8_t dest_addr;
  uint8_t *data;
  uint16_t data_len;
  I2C_HandleTypeDef *hi2c;
  i2c_callback_t callback;
  void *callback_userdata;
} i2c_bus_t;

/* i2c.c */
extern i2c_bus_t i2c_bus[];
bool i2c_tx(uint8_t bus, uint8_t channel, uint8_t address, uint8_t *data,
            uint8_t len, i2c_callback_t callback, void *userdata);
bool i2c_tx_sync(uint8_t bus, uint8_t channel, uint8_t address, uint8_t *data,
                 uint8_t len);
bool i2c_rx(uint8_t bus, uint8_t channel, uint8_t address, uint8_t *data,
            uint8_t len, i2c_callback_t callback, void *userdata);
bool i2c_rx_sync(uint8_t bus, uint8_t channel, uint8_t address, uint8_t *data,
                 uint8_t len);

/* i2c_scan.c */
void i2c_scan_bus(uint8_t bus, uint8_t channel);

/* i2c_isr.c */
void i2c_isr(uint8_t bus);

/* i2c_resume.c */
void i2c_resume(uint8_t bus);

/* i2c_resume_left.c */
typedef enum {
  I2C_LEFT_START = 0,
  I2C_LEFT_0_00,
  I2C_LEFT_0_01,
  I2C_LEFT_0_10,
  I2C_LEFT_0_11,
  I2C_LEFT_1_00,
  I2C_LEFT_1_01,
  I2C_LEFT_1_10,
  I2C_LEFT_1_11,
} i2c_left_device_enum_t;

#endif /* INC_I2C_H_ */
