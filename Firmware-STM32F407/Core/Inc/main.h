/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#define BT_IRQ_Pin GPIO_PIN_0
#define BT_IRQ_GPIO_Port GPIOA
#define BT_TX_Pin GPIO_PIN_2
#define BT_TX_GPIO_Port GPIOA
#define BT_RX_Pin GPIO_PIN_3
#define BT_RX_GPIO_Port GPIOA
#define OSC1_SCALE_CS_Pin GPIO_PIN_4
#define OSC1_SCALE_CS_GPIO_Port GPIOC
#define OSC2_SCALE_CS_Pin GPIO_PIN_5
#define OSC2_SCALE_CS_GPIO_Port GPIOC
#define OSC1_TUNE_Pin GPIO_PIN_9
#define OSC1_TUNE_GPIO_Port GPIOE
#define OSC2_TUNE_Pin GPIO_PIN_11
#define OSC2_TUNE_GPIO_Port GPIOE
#define I2C2_EXTI_Pin GPIO_PIN_12
#define I2C2_EXTI_GPIO_Port GPIOB
#define MIDI_TX_Pin GPIO_PIN_9
#define MIDI_TX_GPIO_Port GPIOA
#define MIDI_RX_Pin GPIO_PIN_10
#define MIDI_RX_GPIO_Port GPIOA
#define SDIO_DETECT_Pin GPIO_PIN_0
#define SDIO_DETECT_GPIO_Port GPIOD
#define I2C1_EXTI_Pin GPIO_PIN_5
#define I2C1_EXTI_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
