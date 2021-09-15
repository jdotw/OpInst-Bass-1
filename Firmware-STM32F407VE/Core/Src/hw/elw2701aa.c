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

  // Pull OLD_DATA_SELECT LOW (Command)
  HAL_GPIO_WritePin(GPIOC, OLED_DATA_SELECT_Pin, GPIO_LOW);

  // Pull OLED_SPI_CS LOW (Active)
  HAL_GPIO_WritePin(GPIOC, OLED_SPI_CS_Pin, GPIO_LOW);

  // Set 2bit Mode
  buf[0] = 0x1a;
  buf[1] = 0x01;
  res = HAL_SPI_Transmit(hspi, buf, 2, HAL_MAX_DELAY);
  if (res != HAL_OK) {
    Error_Handler();
  }

  // Invert display
  buf[0] = 0x07;
  buf[1] = 0x00;
  res = HAL_SPI_Transmit(hspi, buf, 2, HAL_MAX_DELAY);
  if (res != HAL_OK) {
    Error_Handler();
  }


  // Set Entire Dislay On
  buf[0] = 0x08;
  buf[1] = 0x00;
  res = HAL_SPI_Transmit(hspi, buf, 2, HAL_MAX_DELAY);
  if (res != HAL_OK) {
    Error_Handler();
  }

  // Set Contrast
  buf[0] = 0x12;
  buf[1] = 0x40;
  res = HAL_SPI_Transmit(hspi, buf, 2, HAL_MAX_DELAY);
  if (res != HAL_OK) {
    Error_Handler();
  }

  while(1) {
    // Send Command
    printf("Test");
  }
}
