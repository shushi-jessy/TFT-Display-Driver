/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f0xx_hal.h"

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
#define RS_Pin GPIO_PIN_13
#define RS_GPIO_Port GPIOC
#define WR_Pin GPIO_PIN_14
#define WR_GPIO_Port GPIOC
#define RD_Pin GPIO_PIN_15
#define RD_GPIO_Port GPIOC
#define RST_Pin GPIO_PIN_0
#define RST_GPIO_Port GPIOF
#define FMARK_Pin GPIO_PIN_1
#define FMARK_GPIO_Port GPIOF
#define ADC0_Pin GPIO_PIN_0
#define ADC0_GPIO_Port GPIOA
#define ADC1_Pin GPIO_PIN_1
#define ADC1_GPIO_Port GPIOA
#define ADC2_Pin GPIO_PIN_2
#define ADC2_GPIO_Port GPIOA
#define ADC3_Pin GPIO_PIN_3
#define ADC3_GPIO_Port GPIOA
#define ADC4_Pin GPIO_PIN_4
#define ADC4_GPIO_Port GPIOA
#define D0_Pin GPIO_PIN_0
#define D0_GPIO_Port GPIOB
#define D1_Pin GPIO_PIN_1
#define D1_GPIO_Port GPIOB
#define D2_Pin GPIO_PIN_2
#define D2_GPIO_Port GPIOB
#define D10_Pin GPIO_PIN_10
#define D10_GPIO_Port GPIOB
#define D11_Pin GPIO_PIN_11
#define D11_GPIO_Port GPIOB
#define D12_Pin GPIO_PIN_12
#define D12_GPIO_Port GPIOB
#define D13_Pin GPIO_PIN_13
#define D13_GPIO_Port GPIOB
#define D14_Pin GPIO_PIN_14
#define D14_GPIO_Port GPIOB
#define D15_Pin GPIO_PIN_15
#define D15_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_8
#define LED_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define TX2_SWCLK_Pin GPIO_PIN_14
#define TX2_SWCLK_GPIO_Port GPIOA
#define RX2_Pin GPIO_PIN_15
#define RX2_GPIO_Port GPIOA
#define D3_Pin GPIO_PIN_3
#define D3_GPIO_Port GPIOB
#define D4_Pin GPIO_PIN_4
#define D4_GPIO_Port GPIOB
#define D5_Pin GPIO_PIN_5
#define D5_GPIO_Port GPIOB
#define D6_Pin GPIO_PIN_6
#define D6_GPIO_Port GPIOB
#define D7_Pin GPIO_PIN_7
#define D7_GPIO_Port GPIOB
#define D8_Pin GPIO_PIN_8
#define D8_GPIO_Port GPIOB
#define D9_Pin GPIO_PIN_9
#define D9_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
