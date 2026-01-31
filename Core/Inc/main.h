/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f1xx_hal.h"

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
#define NTC_Pin GPIO_PIN_0
#define NTC_GPIO_Port GPIOC
#define BATT_Pin GPIO_PIN_1
#define BATT_GPIO_Port GPIOC
#define DOWN_Pin GPIO_PIN_0
#define DOWN_GPIO_Port GPIOA
#define UP_Pin GPIO_PIN_2
#define UP_GPIO_Port GPIOA
#define RST_Pin GPIO_PIN_4
#define RST_GPIO_Port GPIOC
#define DC_Pin GPIO_PIN_5
#define DC_GPIO_Port GPIOC
#define SEL_Pin GPIO_PIN_0
#define SEL_GPIO_Port GPIOB
#define CS_Pin GPIO_PIN_1
#define CS_GPIO_Port GPIOB
#define DS18B20_Pin GPIO_PIN_10
#define DS18B20_GPIO_Port GPIOB
#define SpeedSense_Pin GPIO_PIN_11
#define SpeedSense_GPIO_Port GPIOB
#define SpeedSense_EXTI_IRQn EXTI15_10_IRQn
#define IGNSense_Pin GPIO_PIN_13
#define IGNSense_GPIO_Port GPIOB
#define IGNSense_EXTI_IRQn EXTI15_10_IRQn
#define PowerUp_Pin GPIO_PIN_15
#define PowerUp_GPIO_Port GPIOB
#define TimeINT_Pin GPIO_PIN_6
#define TimeINT_GPIO_Port GPIOC
#define TimeINT_EXTI_IRQn EXTI9_5_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
