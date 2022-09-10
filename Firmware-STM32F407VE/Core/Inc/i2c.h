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
#define I2C_BUS_MAX 2

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
  I2C_LEFT_RGBLED_0_00,
  I2C_LEFT_RGBLED_0_01,
  I2C_LEFT_RGBLED_0_10,
  I2C_LEFT_RGBLED_0_11,
  I2C_LEFT_RGBLED_1_00,
  I2C_LEFT_RGBLED_1_01,
  I2C_LEFT_RGBLED_1_10,
  I2C_LEFT_RGBLED_1_11,
  I2C_LEFT_RGBLED_3_00,
  I2C_LEFT_RGBLED_3_01,
  I2C_LEFT_ROTPIC_0_000,
  I2C_LEFT_ROTPIC_0_001,
  I2C_LEFT_ROTPIC_0_011,
  I2C_LEFT_ROTPIC_2_000,
  I2C_LEFT_DAC_0_0_0,
  I2C_LEFT_DAC_0_0_1,
  I2C_LEFT_DAC_0_0_2,
  I2C_LEFT_DAC_0_0_3,
  I2C_LEFT_DAC_0_0_4,
  I2C_LEFT_DAC_0_0_5,
  I2C_LEFT_DAC_0_0_6,
  I2C_LEFT_DAC_0_0_7,
  I2C_LEFT_DAC_0_2_0,
  I2C_LEFT_DAC_0_2_1,
  I2C_LEFT_DAC_0_2_2,
  I2C_LEFT_DAC_0_2_3,
  I2C_LEFT_DAC_0_2_4,
  I2C_LEFT_DAC_0_2_5,
  I2C_LEFT_DAC_0_2_6,
  I2C_LEFT_DAC_0_2_7,
  I2C_LEFT_DAC_0_4_2,
  I2C_LEFT_DAC_0_4_3,
  I2C_LEFT_DAC_0_4_6,
  I2C_LEFT_DAC_0_4_7,
  I2C_LEFT_DAC_2_0_0,
  I2C_LEFT_DAC_2_0_2,
  I2C_LEFT_DAC_2_0_4,
  I2C_LEFT_DAC_2_0_5,
} i2c_list_device_enum_t;

typedef enum {
  I2C_RIGHT_START = 0,
  I2C_RIGHT_RGBLED_1_00,
  I2C_RIGHT_RGBLED_1_01,
  I2C_RIGHT_RGBLED_1_10,
  I2C_RIGHT_ROTPIC_0_000,
  I2C_RIGHT_ROTPIC_1_001
} i2c_right_device_enum_t;

#endif /* INC_I2C_H_ */
