/*
 * elw2701aa.c
 *
 *  Created on: Sep 15, 2021
 *      Author: jwilson
 */

#include "main.h"
#include "elw2701aa.h"

#define GPIO_HIGH 1
#define GPIO_LOW 0

void elw2701aa_init(SPI_HandleTypeDef *hspi) {
  // Pull OLED_DATA_SELECT, OLED_SPI_CS, and OLED_RESET high
  HAL_GPIO_WritePin(GPIOC, OLED_RESET_Pin|OLED_DATA_SELECT_Pin|OLED_SPI_CS_Pin, GPIO_HIGH);
}

void elw2701aa_test(SPI_HandleTypeDef *hspi) {
  HAL_StatusTypeDef res;
  uint8_t buf[4];

  uint8_t cmds[32][2] = {
		  { 0x05, 0xB6 },
		  { 0x06, 0x00 },
		  { 0x07, 0x00 },
		  { 0x08, 0x00 },
		  { 0x16, 0x00 },
		  { 0x34, 0x00 },
		  { 0x36, 0x33 },
		  { 0x37, 0x00 },
		  { 0x38, 0x00 },
		  { 0x39, 0x57 },
		  { 0x10, 0x08 },
		  { 0x11, 0x00 },
		  { 0x12, 0x40 },
		  { 0x13, 0x04 },
		  { 0x14, 0x00 },
		  { 0x15, 0x01 },
		  { 0x1A, 0x00 },

		  // PWM
		  { 0x20, 0x02 },
		  { 0x21, 0x05 },
		  { 0x22, 0x0B },
		  { 0x23, 0x11 },
		  { 0x24, 0x19 },
		  { 0x25, 0x23 },
		  { 0x26, 0x2D },
		  { 0x27, 0x39 },
		  { 0x28, 0x45 },
		  { 0x29, 0x53 },
		  { 0x2A, 0x61 },
		  { 0x2B, 0x71 },
		  { 0x2C, 0x81 },
		  { 0x2D, 0x93 },
		  { 0x2E, 0xA5 },
  };

  // Pull OLD_DATA_SELECT LOW (Command)
  HAL_GPIO_WritePin(GPIOC, OLED_DATA_SELECT_Pin, GPIO_LOW);

  // Pull OLED_SPI_CS LOW (Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_LOW);

  for (uint8_t i=0; i < (sizeof(cmds)/sizeof(uint8_t[2])); i++) {
	  uint8_t *cmd = cmds[i];
	  res = HAL_SPI_Transmit(hspi, cmd, 2, HAL_MAX_DELAY);
	  if (res != HAL_OK) {
	    Error_Handler();
	  }
  }

  // Pull OLD_DATA_SELECT HIGH (Data)
  HAL_GPIO_WritePin(GPIOC, OLED_DATA_SELECT_Pin, GPIO_HIGH);

  // Pull OLED_SPI_CS HIGH (In-Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_HIGH);

  //
  // Write to GDRAM
  //

  // Pull OLD_DATA_SELECT LOW (Command)
  HAL_GPIO_WritePin(GPIOC, OLED_DATA_SELECT_Pin, GPIO_LOW);

  // Pull OLED_SPI_CS LOW (Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_LOW);

  uint8_t write_cmds[4][2] = {
		  { 0x30, 0x00 },
		  { 0x32, 0x57 },
		  { 0x31, 0x00 },
		  { 0x33, 0x33 },
  };

  for (uint8_t i=0; i < (sizeof(write_cmds)/sizeof(uint8_t[2])); i++) {
	  uint8_t *cmd = write_cmds[i];
	  res = HAL_SPI_Transmit(hspi, cmd, 2, HAL_MAX_DELAY);
	  if (res != HAL_OK) {
	    Error_Handler();
	  }
  }

  // Pull OLD_DATA_SELECT HIGH (Data)
  HAL_GPIO_WritePin(GPIOC, OLED_DATA_SELECT_Pin, GPIO_HIGH);

  for (uint16_t i=0; i < 4576; i++) {
	  uint8_t data[2] = { 0x00, 0b11110000 };
	  res = HAL_SPI_Transmit(hspi, i == 0 ? data : (&data[1]), i == 0 ? 2 : 1, HAL_MAX_DELAY);
	  if (res != HAL_OK) {
	    Error_Handler();
	  }
  }

  // Pull OLED_SPI_CS HIGH (In-Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_HIGH);


  while(1) {
    // Send Command
    printf("Test");
  }
}
