/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#define UI_LED_CENTER_Pin GPIO_PIN_13
#define UI_LED_CENTER_GPIO_Port GPIOC
#define SENLED_R_Pin GPIO_PIN_15
#define SENLED_R_GPIO_Port GPIOC
#define SEN_RF_Pin GPIO_PIN_0
#define SEN_RF_GPIO_Port GPIOA
#define SEN_R_Pin GPIO_PIN_1
#define SEN_R_GPIO_Port GPIOA
#define SEN_L_Pin GPIO_PIN_2
#define SEN_L_GPIO_Port GPIOA
#define SEN_LF_Pin GPIO_PIN_3
#define SEN_LF_GPIO_Port GPIOA
#define UI_LED_LEFT_BO_Pin GPIO_PIN_4
#define UI_LED_LEFT_BO_GPIO_Port GPIOA
#define SENLED_L_Pin GPIO_PIN_5
#define SENLED_L_GPIO_Port GPIOA
#define SENLED_LF_Pin GPIO_PIN_6
#define SENLED_LF_GPIO_Port GPIOA
#define Speaker_Pin GPIO_PIN_7
#define Speaker_GPIO_Port GPIOA
#define UI_LED_LEFT_Pin GPIO_PIN_0
#define UI_LED_LEFT_GPIO_Port GPIOB
#define Battery_Pin GPIO_PIN_1
#define Battery_GPIO_Port GPIOB
#define SCLK_GY_Pin GPIO_PIN_10
#define SCLK_GY_GPIO_Port GPIOB
#define SCLK_EN_Pin GPIO_PIN_12
#define SCLK_EN_GPIO_Port GPIOB
#define SWITCH_Pin GPIO_PIN_13
#define SWITCH_GPIO_Port GPIOB
#define MISO_GY_Pin GPIO_PIN_14
#define MISO_GY_GPIO_Port GPIOB
#define MOSI_GY_Pin GPIO_PIN_15
#define MOSI_GY_GPIO_Port GPIOB
#define CS_GYRO_Pin GPIO_PIN_8
#define CS_GYRO_GPIO_Port GPIOA
#define FLAG_LED_Pin GPIO_PIN_9
#define FLAG_LED_GPIO_Port GPIOA
#define UI_LED_RIGHT_Pin GPIO_PIN_11
#define UI_LED_RIGHT_GPIO_Port GPIOA
#define SEN_LED_RF_Pin GPIO_PIN_12
#define SEN_LED_RF_GPIO_Port GPIOA
#define MOTER_R_PWM_Pin GPIO_PIN_15
#define MOTER_R_PWM_GPIO_Port GPIOA
#define MOTER_L_PWM_Pin GPIO_PIN_3
#define MOTER_L_PWM_GPIO_Port GPIOB
#define MISO_EN_Pin GPIO_PIN_4
#define MISO_EN_GPIO_Port GPIOB
#define MOSI_EN_Pin GPIO_PIN_5
#define MOSI_EN_GPIO_Port GPIOB
#define CS_R_EN_Pin GPIO_PIN_6
#define CS_R_EN_GPIO_Port GPIOB
#define CS_L_EN_Pin GPIO_PIN_7
#define CS_L_EN_GPIO_Port GPIOB
#define MOTER_R_CWCCW_Pin GPIO_PIN_8
#define MOTER_R_CWCCW_GPIO_Port GPIOB
#define MOTER_L_CWCCW_Pin GPIO_PIN_9
#define MOTER_L_CWCCW_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
