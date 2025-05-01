/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "d_main.h"
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void record_cnt(uint32_t cnt);
void main_sleep_func(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define VBUS_FS_Pin GPIO_PIN_3
#define VBUS_FS_GPIO_Port GPIOE
#define DAC_DEEM_Pin GPIO_PIN_0
#define DAC_DEEM_GPIO_Port GPIOA
#define DAC_MUTE_Pin GPIO_PIN_2
#define DAC_MUTE_GPIO_Port GPIOA
#define VGA_Pin GPIO_PIN_0
#define VGA_GPIO_Port GPIOB
#define LED_PIN_Pin GPIO_PIN_7
#define LED_PIN_GPIO_Port GPIOH

/* USER CODE BEGIN Private defines */

extern volatile uint32_t g_systime_ms;
extern volatile uint32_t g_systime_ms_delay;
extern volatile uint8_t *g_vga_feed_buffer;


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
