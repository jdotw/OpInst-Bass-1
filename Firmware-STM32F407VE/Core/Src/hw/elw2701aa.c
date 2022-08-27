/*
 * elw2701aa.c
 *
 *  Created on: Sep 15, 2021
 *      Author: jwilson
 */

#include "elw2701aa.h"
#include "main.h"
#include "oled.h"
#include <math.h>
#include <stdlib.h>

#define GPIO_HIGH 1
#define GPIO_LOW 0

// NOTE: The x and y values here are memory addresses
// Each byte in GDRAM is 2 4-bit pixels.

void _elw2701aa_wait_for_ready(SPI_HandleTypeDef *hspi);

/*
 * State Machine
 */

typedef enum {
  STATE_IDLE = 0,
  STATE_COMMAND,
  STATE_DATA,
} _elw2701aa_state_enum_t;

_elw2701aa_state_enum_t _state = STATE_IDLE;

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

  // Send Commands
  res = HAL_SPI_Transmit_DMA(hspi, *cmds, sizeof(cmds));
  if (res != HAL_OK) {
    Error_Handler();
  }
  _elw2701aa_wait_for_ready(hspi);

  // Pull OLD_DATA_SELECT HIGH (Data)
  HAL_GPIO_WritePin(GPIOC, OLED_DATA_SELECT_Pin, GPIO_HIGH);

  // Pull OLED_SPI_CS HIGH (In-Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_HIGH);

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

elw2701aa_write_data_cb_t _completion_callback = NULL;
void *_completion_userdata = NULL;
uint8_t *_frame_buf;
uint16_t _frame_buf_len = 0;

uint8_t _write_cmds[4][2] = {
    {0x30, 0},
    {0x32, 0},
    {0x31, 0},
    {0x33, 0},
};

void _elw2701aa_write_data_commands(SPI_HandleTypeDef *hspi, uint8_t start_x,
                                    uint8_t x_len, uint8_t start_y,
                                    uint8_t y_len) {
  HAL_StatusTypeDef res;

  _state = STATE_COMMAND;

  // Suppress BLUENRG-2 IRQ
  // This prevents the bluetooth IRQ from trying to
  // use the SPI bus while we are writing to the display
  HAL_NVIC_DisableIRQ(EXTI0_IRQn);

  // Pull OLD_DATA_SELECT LOW (Command)
  HAL_GPIO_WritePin(GPIOC, OLED_DATA_SELECT_Pin, GPIO_LOW);

  // Pull OLED_SPI_CS LOW (Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_LOW);

  _write_cmds[0][1] = ((175 - start_x) - (x_len - 1)) / 2;
  _write_cmds[1][1] = (175 - start_x) / 2;
  _write_cmds[2][1] = (51 - start_y) - (y_len - 1);
  _write_cmds[3][1] = (51 - start_y);

  res = HAL_SPI_Transmit_DMA(hspi, *_write_cmds, sizeof(_write_cmds));
  if (res != HAL_OK) {
    Error_Handler();
  }
}

void _elw2701aa_write_data_bytes(SPI_HandleTypeDef *hspi) {
  static HAL_StatusTypeDef res;

  _state = STATE_DATA;

  // Pull OLED_SPI_CS HIGH (In-Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_HIGH);

  // Pull OLD_DATA_SELECT HIGH (Data)
  HAL_GPIO_WritePin(GPIOC, OLED_DATA_SELECT_Pin, GPIO_HIGH);

  // Pull OLED_SPI_CS LOW (Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_LOW);

  // Write bytes
  res = HAL_SPI_Transmit_DMA(hspi, _frame_buf, _frame_buf_len);
  if (res != HAL_OK) {
    Error_Handler();
  }
}

void _elw2701aa_write_data_cleanup() {
  // Pull OLED_SPI_CS HIGH (In-Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_HIGH);

  // Re-enable BLUENRG-2 IRQ
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  _state = STATE_IDLE;
}

// NOTE: The x and y coordinates here are in pixel-space
//       When writing data to the OLED display we must
//       divide the x value by 2 for two-pixels-per-byte

uint8_t _gdram_byte_from_framebuffer(uint16_t i, uint8_t *color_p) {
  // Returns the 2x 4bit pixels as one byte
  uint16_t color_index = i * 2;
  uint8_t byte = 0x00;
  byte |= color_p[color_index] & 0xf0;
  byte |= color_p[color_index + 1] >> 4;
  return byte;
}

void elw2701aa_write_data(SPI_HandleTypeDef *hspi, uint8_t start_x,
                          uint8_t x_len, uint8_t start_y, uint8_t y_len,
                          uint8_t *data, uint16_t data_len,
                          elw2701aa_write_data_cb_t completion_callback,
                          void *completion_userdata) {
  if (_state != STATE_IDLE) {
    // Must not be called when an existing
    // operation is already in progress
    Error_Handler();
  }

  _completion_callback = completion_callback;
  _completion_userdata = completion_userdata;

  // Use local buffer that represents what will
  // be written to the GDRAM to do the row reversing
  // and bit packing.
  // This buffer is half data_len because we're writing
  // 2 x 4-bit pixels per 8-bit uint8_t byte
  uint16_t buf_len = (data_len / 2);
  uint8_t buf[buf_len];

  // Check that we won't overflow the buffer
  _frame_buf_len = buf_len;
  if (_frame_buf_len > OLED_BUFFER_SIZE) {
    // Asked to write more then we can handle
    Error_Handler();
  }

  // Iterate over buf_len. Remember, this
  // is in GDRAM space where 1 byte is
  // two 4-bit pixels
  for (uint16_t i = 0; i < buf_len; i++) {
    // First row-reverse the index so that
    // we're reading from the back of the buffer
    // first, and with the row content
    // flipped horizontally
    uint16_t reverse_i = (buf_len - 1) - i;
    uint16_t remainder = reverse_i % 88;
    uint16_t row_reversed_i = (reverse_i - remainder) +
                              (87 - remainder); // Always gets us start of row!

    // Pack two consecutive 4-bit pixels (stored at 2 bytes)
    // from the data buffer that's passed in, into the
    // GDRAM 2-pixel per-byte local buffer
    uint16_t color_index = row_reversed_i * 2;
    uint8_t byte = 0x00;
    byte |= data[color_index] & 0xf0;
    byte |= data[color_index + 1] >> 4;
    buf[i] = byte;
  }

  // Write the local buffer back to the frame
  // buffer because this will persist long enough
  // for the SPI peripheral to read it from DRAM
  for (uint16_t i = 0; i < buf_len; i++) {
    data[i] = buf[i];
  }
  _frame_buf = data;

  // Begin the write process with command first
  // Data will be done once the commands write completes
  _elw2701aa_write_data_commands(hspi, start_x, x_len, start_y, y_len);
}

/*
 * Callback
 */

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
  switch (_state) {
  case STATE_COMMAND:
    // Move on to data
    _elw2701aa_write_data_bytes(hspi);
    break;
  case STATE_DATA:
    // Clean up
    _elw2701aa_write_data_cleanup();

    // Fire callback
    if (_completion_callback) {
      _completion_callback(_completion_userdata);
    }
    break;
  default:
    break;
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
