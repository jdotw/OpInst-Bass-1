/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

#include "hci_tl_interface.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OLED_RESET_Pin GPIO_PIN_15
#define OLED_RESET_GPIO_Port GPIOC
#define BT_SPI_IRQ_Pin GPIO_PIN_0
#define BT_SPI_IRQ_GPIO_Port GPIOA
#define BT_SPI_CS_Pin GPIO_PIN_1
#define BT_SPI_CS_GPIO_Port GPIOA
#define OSC1SCALEDPOTCS_Pin GPIO_PIN_4
#define OSC1SCALEDPOTCS_GPIO_Port GPIOC
#define OSC2SCALEDPOTCS_Pin GPIO_PIN_5
#define OSC2SCALEDPOTCS_GPIO_Port GPIOC
#define I2C2_INT_Pin GPIO_PIN_12
#define I2C2_INT_GPIO_Port GPIOB
#define I2C2_INT_EXTI_IRQn EXTI15_10_IRQn
#define SHIFTSW_Pin GPIO_PIN_10
#define SHIFTSW_GPIO_Port GPIOD
#define SHIFTSW_EXTI_IRQn EXTI15_10_IRQn
#define OLED_DATA_SELECT_Pin GPIO_PIN_6
#define OLED_DATA_SELECT_GPIO_Port GPIOC
#define OLED_SPI_CS_Pin GPIO_PIN_7
#define OLED_SPI_CS_GPIO_Port GPIOC
#define BT_RESET_Pin GPIO_PIN_8
#define BT_RESET_GPIO_Port GPIOA
#define MIDI_TX_Pin GPIO_PIN_9
#define MIDI_TX_GPIO_Port GPIOA
#define MIDI_RX_Pin GPIO_PIN_10
#define MIDI_RX_GPIO_Port GPIOA
#define I2C1_INT_Pin GPIO_PIN_5
#define I2C1_INT_GPIO_Port GPIOB
#define I2C1_INT_EXTI_IRQn EXTI9_5_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
