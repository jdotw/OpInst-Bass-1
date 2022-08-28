/*
 * i2c.c
 *
 *  Created on: Apr 19, 2021
 *      Author: jwilson
 */

#include "i2c.h"
#include "main.h"
#include <stdbool.h>

i2c_bus_t i2c_bus[2] = {{
                          mux_addr : 0x70,
                          mux_selected_channel : 0xFF,
                        },
                        {
                          mux_addr : 0x71,
                          mux_selected_channel : 0xFF,
                        }};

void _i2c_txrx_complete_callback(I2C_HandleTypeDef *hi2c);

bool _i2c_mux_select_channel(uint8_t bus, uint8_t channel) {
  i2c_bus[bus].mux_desired_channel = channel;
  if (i2c_bus[bus].mux_desired_channel == i2c_bus[bus].mux_selected_channel) {
    _i2c_txrx_complete_callback(i2c_bus[bus].hi2c);
    return true;
  } else {
    I2C_HandleTypeDef *hi2c = i2c_bus[bus].hi2c;
    uint8_t data = (channel & 0b11) | 0b100;
    HAL_StatusTypeDef res;
    res =
        HAL_I2C_Master_Transmit_DMA(hi2c, i2c_bus[bus].mux_addr << 1, &data, 1);
    if (!res)
      return false;
    else
      return true;
  }
}

bool _i2c_tx_bytes(uint8_t bus) {
  // Transmit via DMA
  I2C_HandleTypeDef *hi2c = i2c_bus[bus].hi2c;
  HAL_StatusTypeDef res;
  res = HAL_I2C_Master_Transmit_DMA(hi2c, i2c_bus[bus].dest_addr << 1,
                                    i2c_bus[bus].data, i2c_bus[bus].data_len);
  return (res == HAL_OK);
}

bool _i2c_rx_bytes(uint8_t bus) {
  // Receive via DMA
  I2C_HandleTypeDef *hi2c = i2c_bus[bus].hi2c;
  HAL_StatusTypeDef res;
  res = HAL_I2C_Master_Receive_DMA(hi2c, i2c_bus[bus].dest_addr << 1,
                                   i2c_bus[bus].data, i2c_bus[bus].data_len);
  return (res == HAL_OK);
}

/*
 * Callback
 */

uint8_t _bus_index_for_handle(I2C_HandleTypeDef *hi2c) {
  for (uint8_t i = 0; i < (sizeof(i2c_bus) / sizeof(i2c_bus_t)); i++) {
    if (i2c_bus[i].hi2c == hi2c) {
      return i;
    }
  }
  return UINT8_MAX;
}

void _i2c_txrx_complete_callback(I2C_HandleTypeDef *hi2c) {
  uint8_t bus = _bus_index_for_handle(hi2c);
  if (bus == UINT8_MAX) {
    // Unknown bus
    Error_Handler();
  }

  switch (i2c_bus[bus].state) {
  case I2C_STATE_CHANNEL_SELECT:
    /* Move on to transmit of data */
    i2c_bus[bus].mux_selected_channel = i2c_bus[bus].mux_desired_channel;
    i2c_bus[bus].state = I2C_STATE_DATA;
    switch (i2c_bus[bus].direction) {
    case I2C_DIRECTION_TRANSMIT:
      if (!_i2c_tx_bytes(bus)) {
        Error_Handler();
      }
      break;
    case I2C_DIRECTION_RECEIVE:
      if (!_i2c_rx_bytes(bus)) {
        Error_Handler();
      }
      break;
    default:
      Error_Handler();
    }
    break;
  case I2C_STATE_DATA:
    /* Progress to IDLE */
    i2c_bus[bus].state = I2C_STATE_IDLE;
    /* Fire callback */
    if (i2c_bus[bus].callback) {
      i2c_bus[bus].callback(i2c_bus[bus].callback_userdata);
    }
    break;
  default:
    Error_Handler();
  }
}

void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c) {
  _i2c_txrx_complete_callback(hi2c);
}

void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c) {
  _i2c_txrx_complete_callback(hi2c);
}

/*
 * API
 */

bool i2c_tx(uint8_t bus, uint8_t channel, uint8_t address, uint8_t *data,
            uint8_t len, i2c_callback_t callback, void *userdata) {
  // Make sure state machine is IDLE
  if (i2c_bus[bus].state != I2C_STATE_IDLE) {
    Error_Handler();
  }

  // Setup state
  i2c_bus[bus].data = data;
  i2c_bus[bus].data_len = len;
  i2c_bus[bus].direction = I2C_DIRECTION_TX;
  i2c_bus[bus].callback = callback;
  i2c_bus[bus].callback_userdata = userdata;

  // Select Channel
  i2c_bus[bus].state = I2C_STATE_CHANNEL_SELECT;
  bool res = _i2c_mux_select_channel(bus, channel);

  // HACK: Enable synchronous behavior
  while (i2c_bus[bus].state != I2C_STATE_IDLE) {
  }

  return res;
}

bool i2c_rx(uint8_t bus, uint8_t channel, uint8_t address, uint8_t *data,
            uint8_t len, i2c_callback_t callback, void *userdata) {
  // Make sure state machine is IDLE
  if (i2c_bus[bus].state != I2C_STATE_IDLE) {
    Error_Handler();
  }

  // Setup state
  i2c_bus[bus].data = data;
  i2c_bus[bus].data_len = len;
  i2c_bus[bus].direction = I2C_DIRECTION_RX;
  i2c_bus[bus].callback = callback;
  i2c_bus[bus].callback_userdata = userdata;

  // Select Channel
  i2c_bus[bus].state = I2C_STATE_CHANNEL_SELECT;
  bool res = _i2c_mux_select_channel(bus, channel);

  // HACK: Enable synchronous behavior
  while (i2c_bus[bus].state != I2C_STATE_IDLE) {
  }

  return res;
}
