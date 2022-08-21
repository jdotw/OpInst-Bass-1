/*
 * elw2701aa.c
 *
 *  Created on: Sep 15, 2021
 *      Author: jwilson
 */

#include "elw2701aa.h"
#include "main.h"
#include <math.h>
#include <stdlib.h>

#define GPIO_HIGH 1
#define GPIO_LOW 0

// NOTE: The x and y values here are memory addresses
// Each byte in GDRAM is 2 4-bit pixels.

void _elw2701aa_write_zero_data(SPI_HandleTypeDef *hspi);
void _elw2701aa_wait_for_ready(SPI_HandleTypeDef *hspi);

/*
 * API
 */

void elw2701aa_init(SPI_HandleTypeDef *hspi) {
  HAL_StatusTypeDef res;

  // Pull OLED_DATA_SELECT, OLED_SPI_CS, and OLED_RESET high
  HAL_GPIO_WritePin(GPIOC,
                    OLED_RESET_Pin | OLED_DATA_SELECT_Pin | OLED_SPI_CS_Pin,
                    GPIO_HIGH);

  //
  // Configure Display Driver
  //

  uint8_t cmds[32][2] = {
      {0x05, 0xB6}, // Line Clocks
      {0x06, 0x00}, // Scan Direction
      {0x07, 0x01}, // Invert Display
      {0x08, 0x00}, // Entire Display On
      {0x16, 0x00}, // Row Pin Config
      {0x34, 0x00}, // Y Line Start
      {0x36, 0x33}, // Number of Rows
      {0x37, 0x00}, // Row Start Line
      {0x38, 0x00}, // Column Start Line
      {0x39, 0x57}, // Column End Line
      {0x10, 0x08}, // Peak Width
      {0x11, 0x00}, // Peak Delay
      {0x12, 0x40}, // Contrast
      {0x13, 0x04}, // Pre-Charge Width
      {0x14, 0x00}, // Pre-Charge Width
      {0x15, 0x01}, // Row Overlap
      {0x1A, 0x00}, // Grey-scale Mode (4 bit)

      // PWM Gamma Table
      {0x20, 0x02},
      {0x21, 0x05},
      {0x22, 0x0B},
      {0x23, 0x11},
      {0x24, 0x19},
      {0x25, 0x23},
      {0x26, 0x2D},
      {0x27, 0x39},
      {0x28, 0x45},
      {0x29, 0x53},
      {0x2A, 0x61},
      {0x2B, 0x71},
      {0x2C, 0x81},
      {0x2D, 0x93},
      {0x2E, 0xA5},
  };

  // Pull OLD_DATA_SELECT LOW (Command)
  HAL_GPIO_WritePin(GPIOC, OLED_DATA_SELECT_Pin, GPIO_LOW);

  // Pull OLED_SPI_CS LOW (Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_LOW);

  for (uint8_t i = 0; i < (sizeof(cmds) / sizeof(uint8_t[2])); i++) {
    uint8_t *cmd = cmds[i];
    res = HAL_SPI_Transmit_DMA(hspi, cmd, 2);
    if (res != HAL_OK) {
      Error_Handler();
    }
    _elw2701aa_wait_for_ready(hspi);
  }

  // Pull OLD_DATA_SELECT HIGH (Data)
  HAL_GPIO_WritePin(GPIOC, OLED_DATA_SELECT_Pin, GPIO_HIGH);

  // Pull OLED_SPI_CS HIGH (In-Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_HIGH);

  //
  // Write zeros to GDRAM
  //

  _elw2701aa_write_zero_data(hspi);

  //
  // Send Display-On Command
  //

  // Pull OLD_DATA_SELECT LOW (Command)
  HAL_GPIO_WritePin(GPIOC, OLED_DATA_SELECT_Pin, GPIO_LOW);

  // Pull OLED_SPI_CS LOW (Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_LOW);

  uint8_t on_cmd[2] = {0x02, 0x01};

  _elw2701aa_wait_for_ready(hspi);
  res = HAL_SPI_Transmit_DMA(hspi, on_cmd, 2);
  if (res != HAL_OK) {
    Error_Handler();
  }
  _elw2701aa_wait_for_ready(hspi);

  // Pull OLED_SPI_CS HIGH (In-Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_HIGH);

  // Pull OLD_DATA_SELECT HIGH (Data)
  HAL_GPIO_WritePin(GPIOC, OLED_DATA_SELECT_Pin, GPIO_HIGH);
}

/*
 * GDRAM Write
 */

// NOTE: The x and y coordinates here are in pixel-space
//       When writing data to the OLED display we must
//       divide the x value by 2 for two-pixels-per-byte

void elw2701aa_write_data(SPI_HandleTypeDef *hspi, uint8_t start_x,
                          uint8_t x_len, uint8_t start_y, uint8_t y_len,
                          uint8_t (*data_callback)(uint16_t),
                          elw2701aa_write_data_cb_t completion_callback,
                          void *completion_userdata) {
  HAL_StatusTypeDef res;
  uint8_t buf[176 * 52];

  // Suppress BLUENRG-2 IRQ
  // This prevents the bluetooth IRQ from trying to
  // use the SPI bus while we are writing to the display
  HAL_NVIC_DisableIRQ(EXTI0_IRQn);

  // Pull OLD_DATA_SELECT LOW (Command)
  HAL_GPIO_WritePin(GPIOC, OLED_DATA_SELECT_Pin, GPIO_LOW);

  // Pull OLED_SPI_CS LOW (Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_LOW);

  uint8_t write_cmds[4][2] = {
      {0x30, ((175 - start_x) - (x_len - 1)) / 2},
      {0x32, (175 - start_x) / 2},
      {0x31, (51 - start_y) - (y_len - 1)},
      {0x33, (51 - start_y)},
  };

  _elw2701aa_wait_for_ready(hspi);
  for (uint8_t i = 0; i < (sizeof(write_cmds) / sizeof(uint8_t[2])); i++) {
    uint8_t *cmd = write_cmds[i];
    res = HAL_SPI_Transmit_DMA(hspi, cmd, 2);
    if (res != HAL_OK) {
      Error_Handler();
    }
    _elw2701aa_wait_for_ready(hspi);
  }

  // Pull OLED_SPI_CS HIGH (In-Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_HIGH);

  // Pull OLD_DATA_SELECT HIGH (Data)
  HAL_GPIO_WritePin(GPIOC, OLED_DATA_SELECT_Pin, GPIO_HIGH);

  // Pull OLED_SPI_CS LOW (Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_LOW);

  uint16_t num_bytes = ((uint16_t)x_len * (uint16_t)y_len) / 2;
  for (uint16_t i = 0; i < num_bytes; i++) {
    uint16_t reverse_i = (num_bytes - 1) - i;
    uint16_t remainder = reverse_i % 88;
    uint16_t row_reversed_i = (reverse_i - remainder) +
                              (87 - remainder); // Always gets us start of row!
    uint8_t data = data_callback(row_reversed_i);
    //    uint8_t reversed = (data & 0xF0) >> 4;
    //    reversed |= (data & 0x0F) << 4;
    buf[i] = data;
  }

  res = HAL_SPI_Transmit_DMA(hspi, buf, num_bytes);
  if (res != HAL_OK) {
    Error_Handler();
  }
  _elw2701aa_wait_for_ready(hspi);

  // Pull OLED_SPI_CS HIGH (In-Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_HIGH);

  // Re-enable BLUENRG-2 IRQ
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  if (completion_callback) {
    completion_callback(completion_userdata);
  }
}

/*
 * Bus
 */

void _elw2701aa_wait_for_ready(SPI_HandleTypeDef *hspi) {
  // Wait for bus to become available again
  while (hspi->State != HAL_SPI_STATE_READY) {
    // Do nothing...
  }
}

/*
 * Zero-Data
 */

void _elw2701aa_write_zero_data(SPI_HandleTypeDef *hspi) {
  uint8_t _data_from_buf(uint16_t i) { return 0x00; }
  elw2701aa_write_data(hspi, 0, 176, 0, 52, _data_from_buf, NULL, NULL);
}
