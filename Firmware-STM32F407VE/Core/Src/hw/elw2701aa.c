/*
 * elw2701aa.c
 *
 *  Created on: Sep 15, 2021
 *      Author: jwilson
 */

#include "main.h"
#include "elw2701aa.h"
#include <math.h>
#include <stdlib.h>

#define GPIO_HIGH 1
#define GPIO_LOW 0

// NOTE: The x and y values here are memory addresses
// Each byte in GDRAM is 2 4-bit pixels.

void _elw2701aa_write_zero_data(SPI_HandleTypeDef *hspi);

/*
 * API
 */

void elw2701aa_init(SPI_HandleTypeDef *hspi)
{
  HAL_StatusTypeDef res;

  // Pull OLED_DATA_SELECT, OLED_SPI_CS, and OLED_RESET high
  HAL_GPIO_WritePin(GPIOC, OLED_RESET_Pin | OLED_DATA_SELECT_Pin | OLED_SPI_CS_Pin, GPIO_HIGH);

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

  for (uint8_t i = 0; i < (sizeof(cmds) / sizeof(uint8_t[2])); i++)
  {
    uint8_t *cmd = cmds[i];
    res = HAL_SPI_Transmit(hspi, cmd, 2, HAL_MAX_DELAY);
    if (res != HAL_OK)
    {
      Error_Handler();
    }
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

  res = HAL_SPI_Transmit(hspi, on_cmd, 2, HAL_MAX_DELAY);
  if (res != HAL_OK)
  {
    Error_Handler();
  }

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

void elw2701aa_write_data(SPI_HandleTypeDef *hspi, uint8_t start_x, uint8_t x_len, uint8_t start_y, uint8_t y_len, uint8_t (*data_callback)(uint16_t))
{
  HAL_StatusTypeDef res;

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

  for (uint8_t i = 0; i < (sizeof(write_cmds) / sizeof(uint8_t[2])); i++)
  {
    uint8_t *cmd = write_cmds[i];
    res = HAL_SPI_Transmit(hspi, cmd, 2, HAL_MAX_DELAY);
    if (res != HAL_OK)
    {
      Error_Handler();
    }
  }

  // Pull OLED_SPI_CS HIGH (In-Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_HIGH);

  // Pull OLD_DATA_SELECT HIGH (Data)
  HAL_GPIO_WritePin(GPIOC, OLED_DATA_SELECT_Pin, GPIO_HIGH);

  // Pull OLED_SPI_CS LOW (Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_LOW);

  uint16_t num_bytes = ((uint16_t)x_len * (uint16_t)y_len) / 2;
  for (uint16_t i = 0; i < num_bytes; i++)
  {
    uint16_t reverse_i = (num_bytes - 1) - i;
    uint16_t remainder = reverse_i % 88;
    uint16_t row_reversed_i = (reverse_i - remainder) + (87 - remainder); // Always gets us start of row!
    uint8_t data = data_callback(row_reversed_i);
    //    uint8_t reversed = (data & 0xF0) >> 4;
    //    reversed |= (data & 0x0F) << 4;
    res = HAL_SPI_Transmit(hspi, &data, 1, HAL_MAX_DELAY);
    if (res != HAL_OK)
    {
      Error_Handler();
    }
  }

  // Pull OLED_SPI_CS HIGH (In-Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_HIGH);

  // Re-enable BLUENRG-2 IRQ
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

/*
 * Zero-Data
 */

void _elw2701aa_write_zero_data(SPI_HandleTypeDef *hspi)
{
  uint8_t _data_from_buf(uint16_t i)
  {
    return 0x00;
  }
  elw2701aa_write_data(hspi, 0, 176, 0, 52, _data_from_buf);
}

/*
 * Test-Data
 */

const uint8_t image[4576] = {
    0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x3c, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xe4, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xee, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0x6a, 0xff, 0xf9, 0x6d, 0x28, 0xd2, 0x8a,
    0x25, 0x61, 0xd7, 0x34, 0x8f, 0x7b, 0xf8, 0x6f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x81, 0x33, 0x4d, 0xfd, 0x35, 0x4d,
    0xf7, 0x34, 0x8f, 0x34, 0x54, 0x7f, 0xe0, 0xdf,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xb2, 0x54, 0x4e, 0xe2, 0x46, 0x2f, 0x54, 0x45,
    0xfa, 0x2b, 0x73, 0x55, 0x3b, 0x7e, 0x4f, 0xf5,
    0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff,
    0xf9, 0x6f, 0x73, 0x47, 0xf3, 0xfd, 0x35, 0x4d,
    0xd2, 0x55, 0x2a, 0x54, 0x45, 0xfb, 0x35, 0x5f,
    0xff, 0xc4, 0xff, 0xf3, 0xcb, 0x6f, 0xb6, 0xf6,
    0xaf, 0xb4, 0xc4, 0xff, 0x6e, 0x7b, 0xc2, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x88, 0xff, 0xb2, 0xf5, 0xcf, 0xda,
    0xc4, 0xff, 0x6d, 0x2f, 0xf3, 0xcf, 0x87, 0x7f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x1e, 0xff, 0x88, 0xa6, 0xfe, 0x1e, 0x9f, 0xf2,
    0xd7, 0xaf, 0x2e, 0xf7, 0x8b, 0x7e, 0x4f, 0xf5,
    0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff,
    0xf8, 0x9d, 0x4f, 0xf9, 0xe3, 0xf5, 0xcf, 0xda,
    0xa6, 0xff, 0x1e, 0x9f, 0xf2, 0xd2, 0xff, 0xbc,
    0xff, 0xf2, 0x34, 0x22, 0xfb, 0x6f, 0xb6, 0xfe,
    0x66, 0x54, 0xa7, 0xff, 0xff, 0x72, 0x2c, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x88, 0xff, 0xf5, 0xb2, 0x44, 0x45,
    0xa7, 0xff, 0xff, 0x96, 0x62, 0xcf, 0x2f, 0x3f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xcf, 0xfe, 0x49, 0xa8, 0xff, 0x2f, 0xb4, 0x38,
    0xf7, 0xaf, 0xc6, 0x64, 0x8b, 0x7e, 0x4f, 0xf5,
    0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xee,
    0xd3, 0xca, 0x24, 0x44, 0xb3, 0xf2, 0x44, 0x45,
    0xf7, 0x66, 0x1f, 0xb4, 0x38, 0xe1, 0x44, 0x48,
    0xff, 0xf9, 0x7f, 0x69, 0xfb, 0x6f, 0xb6, 0xfc,
    0xaf, 0xb5, 0xd3, 0xff, 0x7e, 0x79, 0x4c, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x88, 0xff, 0xf6, 0xa5, 0xcf, 0xc5,
    0xd3, 0xff, 0x7e, 0x7f, 0xf3, 0xda, 0x6f, 0x8a,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xd6, 0x23, 0x9f, 0xa8, 0xff, 0x2f, 0x1e, 0xf8,
    0xf7, 0xaf, 0x9c, 0xf7, 0x9b, 0x7e, 0x1f, 0xf3,
    0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x55,
    0x44, 0xfd, 0x4f, 0xf4, 0xd3, 0xf5, 0xcf, 0xc5,
    0xe7, 0xff, 0x1f, 0x1e, 0xf8, 0xf3, 0xff, 0x89,
    0xff, 0xff, 0x2f, 0x2f, 0xe3, 0x28, 0x32, 0x9e,
    0x44, 0x3a, 0xf9, 0x33, 0x7f, 0x7b, 0xf5, 0xbf,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x88, 0xff, 0xf4, 0xbe, 0x54, 0x4e,
    0xf9, 0x33, 0x7f, 0xa3, 0x44, 0xf5, 0xdf, 0xd5,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x39, 0xff, 0xde, 0xa9, 0xff, 0x3f, 0x64, 0x38,
    0xb2, 0x3c, 0xc3, 0x43, 0xdb, 0x8e, 0x45, 0x35,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff,
    0xf8, 0x9f, 0x94, 0x49, 0xf3, 0xfe, 0x54, 0x4e,
    0xf6, 0x43, 0x7f, 0x64, 0x38, 0xfc, 0x44, 0x6f,
    0xff, 0xff, 0x57, 0x6f, 0xfb, 0x6f, 0xb6, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x7b, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x88, 0xff, 0xb2, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x3d, 0xff, 0x1d, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xf7, 0xaf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff,
    0xf5, 0xaf, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xc0, 0xcf, 0xfd, 0xaf, 0xda, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x7b, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x81, 0x33, 0x4d, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xb2, 0x51, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfb, 0xdf, 0xff, 0xff, 0xfb, 0x7f, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x33,
    0x25, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xde, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xef, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xeb, 0xac, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x53, 0x4e, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xeb, 0xbf,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x56, 0xff, 0xff, 0xf9, 0x5f, 0x95, 0xfd, 0x10,
    0xff, 0x70, 0x13, 0xcf, 0x76, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff,
    0xff, 0xff, 0x81, 0x00, 0x00, 0x02, 0x7f, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xa0, 0x03, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xb1, 0x00, 0x00,
    0x3e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x56, 0xff, 0xff, 0xf9, 0x5f, 0x95, 0xf9, 0x4f,
    0xf9, 0x3e, 0xfb, 0x2f, 0x76, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x20, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff,
    0xff, 0xb1, 0x00, 0x00, 0x00, 0x00, 0x02, 0xcf,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf6, 0x00, 0x8f,
    0xff, 0xff, 0xff, 0xff, 0xf9, 0x00, 0x05, 0x30,
    0x01, 0xdf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x56, 0xff, 0xff, 0xf9, 0x5f, 0x95, 0xf9, 0x4f,
    0xf4, 0x9f, 0xff, 0xdf, 0x76, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x60, 0x02,
    0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0xaf, 0xff,
    0xfd, 0x10, 0x00, 0x48, 0xa9, 0x60, 0x00, 0x0b,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x20, 0x0c,
    0xff, 0xff, 0xff, 0xff, 0xf1, 0x02, 0xff, 0xfc,
    0x00, 0x4f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x56, 0xff, 0xff, 0xf9, 0x5f, 0x95, 0xf9, 0x4f,
    0xf2, 0x01, 0x11, 0x1b, 0x76, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x90, 0x03,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xf3, 0x00, 0x1c, 0xff, 0xff, 0xfd, 0x30, 0x02,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xd0, 0x01,
    0xef, 0xff, 0xff, 0xff, 0x90, 0x0b, 0xff, 0xff,
    0x70, 0x0d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x52, 0x55, 0x56, 0xf9, 0x5f, 0x95, 0xf9, 0x4f,
    0xf5, 0x8e, 0xee, 0x0d, 0x72, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0x00,
    0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xc0, 0x00, 0xcf, 0xff, 0xff, 0xff, 0xe0, 0x00,
    0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0x00,
    0x5f, 0xff, 0xff, 0xff, 0x50, 0x0f, 0xff, 0xff,
    0xb0, 0x09, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x54, 0x99, 0x9a, 0xf9, 0x5f, 0x95, 0xe8, 0x4d,
    0xfb, 0x2c, 0xf7, 0x3f, 0x72, 0x5b, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf9, 0x00,
    0x1d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x90, 0x04, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x00,
    0x2f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x40,
    0x09, 0xff, 0xff, 0xff, 0x30, 0x2f, 0xff, 0xff,
    0xe0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x56, 0xff, 0xff, 0xfa, 0x6f, 0x95, 0x81, 0x02,
    0xff, 0xa2, 0x15, 0xef, 0x89, 0x82, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x30,
    0x01, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x61, 0x19, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
    0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe1,
    0x00, 0xdf, 0xff, 0xff, 0x20, 0x2f, 0xff, 0xff,
    0xf0, 0x05, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x56, 0xff, 0xff, 0xff, 0xff, 0x95, 0xf9, 0x4f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe3,
    0x00, 0x08, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
    0x0c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb,
    0x00, 0x3f, 0xff, 0xff, 0x20, 0x2f, 0xff, 0xff,
    0xf0, 0x06, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x51, 0x33, 0x33, 0xa9, 0x5f, 0x95, 0xfb, 0x7f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x40, 0x00, 0x3c, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00,
    0x0c, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0x70, 0x07, 0xff, 0xff, 0x40, 0x1f, 0xff, 0xff,
    0xe0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xdb, 0xbb, 0xbb, 0xde, 0xcf, 0xec, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xf9, 0x00, 0x00, 0x6f, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0x00,
    0x0e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xf3, 0x00, 0xbf, 0xff, 0x60, 0x0d, 0xff, 0xff,
    0xa0, 0x09, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xd4, 0x00, 0x01, 0xbf, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfa, 0x00,
    0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xfd, 0x00, 0x1e, 0xff, 0xc0, 0x06, 0xff, 0xff,
    0x40, 0x0e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x90, 0x00, 0x09, 0xff, 0xff, 0xff,
    0xfd, 0xcc, 0xff, 0xff, 0xff, 0xff, 0xf2, 0x00,
    0x4f, 0xff, 0xff, 0xff, 0xfa, 0x63, 0x26, 0xaf,
    0xff, 0xa0, 0x04, 0xff, 0xf3, 0x00, 0x8e, 0xd6,
    0x00, 0x5f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfe, 0x30, 0x00, 0x8f, 0xff, 0xff,
    0xf1, 0x00, 0x7f, 0xff, 0xff, 0xff, 0x60, 0x00,
    0xbf, 0xff, 0xff, 0xff, 0x60, 0x00, 0x00, 0x05,
    0xff, 0xf5, 0x00, 0x8f, 0xfe, 0x30, 0x00, 0x00,
    0x04, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xf5, 0x00, 0x0b, 0xff, 0xff,
    0xf3, 0x00, 0x04, 0xae, 0xfe, 0xb2, 0x00, 0x05,
    0xff, 0xff, 0xff, 0xf7, 0x00, 0x5c, 0xc6, 0x00,
    0x5f, 0xff, 0x20, 0x0c, 0xff, 0xf8, 0x41, 0x15,
    0x8f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x30, 0x02, 0xff, 0xff,
    0xf6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2e,
    0xff, 0xff, 0xff, 0xf1, 0x03, 0xff, 0xff, 0x60,
    0x0d, 0xff, 0xc0, 0x02, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xdf,
    0xff, 0xff, 0xff, 0xee, 0xff, 0xff, 0xfe, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xd0, 0x00, 0xbf, 0xff,
    0xf9, 0x00, 0x84, 0x00, 0x00, 0x00, 0x07, 0xff,
    0xff, 0xff, 0xff, 0xa0, 0x0a, 0xff, 0xff, 0xd0,
    0x07, 0xff, 0xf8, 0x00, 0x6f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x50, 0x10,
    0x3d, 0xff, 0xc2, 0x11, 0x2d, 0xfa, 0x11, 0x29,
    0xff, 0xff, 0xff, 0xff, 0x3b, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0x7a,
    0xff, 0xff, 0xff, 0xff, 0xf3, 0x00, 0x8f, 0xff,
    0xfc, 0x00, 0x7f, 0xa7, 0x44, 0x69, 0xdf, 0xff,
    0xff, 0xff, 0xff, 0x80, 0x0d, 0xff, 0xff, 0xf1,
    0x04, 0xff, 0xff, 0x40, 0x0a, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0x4e, 0xff,
    0x71, 0xef, 0x38, 0xff, 0x75, 0xd0, 0xbf, 0xd1,
    0xdf, 0xff, 0xff, 0xff, 0x3b, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xb0, 0x04,
    0xff, 0xff, 0xff, 0xff, 0xf6, 0x00, 0x5f, 0xff,
    0xff, 0x00, 0x4f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x60, 0x0f, 0xff, 0xff, 0xf2,
    0x03, 0xff, 0xff, 0xe1, 0x01, 0xef, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xa1, 0xff, 0xff,
    0xf4, 0x6f, 0xdf, 0xfd, 0x35, 0x93, 0xff, 0xfa,
    0xcf, 0xff, 0xff, 0xff, 0x3b, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x02,
    0xff, 0xff, 0xff, 0xff, 0xf7, 0x00, 0x4f, 0xff,
    0xff, 0x20, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x50, 0x0f, 0xff, 0xff, 0xf3,
    0x01, 0xff, 0xff, 0xfb, 0x00, 0x3f, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x56, 0xff, 0xff,
    0xfa, 0x2f, 0xe6, 0x10, 0x5e, 0x85, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x3b, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00,
    0xcf, 0xff, 0xff, 0xff, 0xf4, 0x00, 0x7f, 0xff,
    0xff, 0x50, 0x0e, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x70, 0x0e, 0xff, 0xff, 0xf2,
    0x03, 0xff, 0xff, 0xff, 0x60, 0x07, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x39, 0xff, 0xff,
    0xfc, 0x0f, 0x63, 0xcf, 0xee, 0xa2, 0xff, 0xfb,
    0xdf, 0xff, 0xff, 0xff, 0x3b, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0x00,
    0x5f, 0xff, 0xff, 0xff, 0xd0, 0x00, 0xaf, 0xff,
    0xff, 0x70, 0x0b, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0x80, 0x0c, 0xff, 0xff, 0xf0,
    0x04, 0xff, 0xff, 0xff, 0xf2, 0x00, 0xcf, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x48, 0xff, 0xff,
    0xfb, 0x1f, 0x56, 0xfd, 0x2b, 0xf1, 0x8e, 0xb0,
    0xbf, 0xff, 0xff, 0xff, 0x3b, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00,
    0x07, 0xff, 0xff, 0xfe, 0x20, 0x01, 0xef, 0xff,
    0xff, 0xa0, 0x08, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xff, 0xb0,
    0x08, 0xff, 0xff, 0xff, 0xfd, 0x00, 0x1e, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0x73, 0xff, 0xff,
    0xf7, 0x4f, 0xd4, 0x01, 0x7f, 0xfd, 0x40, 0x29,
    0xff, 0xff, 0xff, 0xbb, 0x2b, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x40,
    0x00, 0x48, 0xca, 0x70, 0x00, 0x0b, 0xff, 0xff,
    0xff, 0xd0, 0x02, 0x66, 0x66, 0x66, 0x66, 0x67,
    0xff, 0xff, 0xff, 0xf3, 0x00, 0xdf, 0xff, 0x20,
    0x0e, 0xff, 0xff, 0xff, 0xff, 0x90, 0x05, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x9f, 0xff,
    0xc0, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xfe, 0x61, 0x0b, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf5,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x8f, 0xff, 0xff,
    0xff, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0xff, 0xff, 0xff, 0xfc, 0x00, 0x05, 0x61, 0x00,
    0x7f, 0xff, 0xff, 0xff, 0xff, 0xf5, 0x00, 0x9f,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x13, 0x64,
    0x0a, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0x2b, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xa1, 0x00, 0x00, 0x00, 0x4d, 0xff, 0xff, 0xff,
    0xff, 0xf3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0xff, 0xff, 0xff, 0xff, 0xc1, 0x00, 0x00, 0x08,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x10, 0x0d,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfb, 0x8a,
    0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xde, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xeb, 0x89, 0xbe, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xfe, 0xee, 0xee, 0xee, 0xee, 0xee, 0xee,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xd9, 0x8c, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x03,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

void _elw2701aa_write_test_data(SPI_HandleTypeDef *hspi)
{
  uint8_t _data_from_buf(uint16_t i)
  {
    //    uint8_t data = 0x00;
    //    data |= rand() % 16;
    //    data |= rand() % 16 << 4;
    //    return data;
    return image[i];
  }
  elw2701aa_write_data(hspi, 0, 176, 0, 52, _data_from_buf);
}

void elw2701aa_test(SPI_HandleTypeDef *hspi, bool loop)
{
  do
  {
    _elw2701aa_write_test_data(hspi);
    HAL_Delay(10);
  } while (loop);
}
