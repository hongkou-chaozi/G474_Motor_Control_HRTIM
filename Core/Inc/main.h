/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32g4xx_hal.h"

#include "stm32g4xx_ll_adc.h"
#include "stm32g4xx_ll_cordic.h"
#include "stm32g4xx_ll_dac.h"
#include "stm32g4xx_ll_dma.h"
#include "stm32g4xx_ll_hrtim.h"
#include "stm32g4xx_ll_i2c.h"
#include "stm32g4xx_ll_rcc.h"
#include "stm32g4xx_ll_bus.h"
#include "stm32g4xx_ll_crs.h"
#include "stm32g4xx_ll_system.h"
#include "stm32g4xx_ll_exti.h"
#include "stm32g4xx_ll_cortex.h"
#include "stm32g4xx_ll_utils.h"
#include "stm32g4xx_ll_pwr.h"
#include "stm32g4xx_ll_tim.h"
#include "stm32g4xx_ll_gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>

#define ADC1_BUFFER_SIZE                  3
#define ADC2_BUFFER_SIZE                  2
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
#define LED3_Pin GPIO_PIN_14
#define LED3_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_15
#define LED2_GPIO_Port GPIOC
#define U_FB_Pin GPIO_PIN_0
#define U_FB_GPIO_Port GPIOC
#define V_FB_Pin GPIO_PIN_1
#define V_FB_GPIO_Port GPIOC
#define W_FB_Pin GPIO_PIN_2
#define W_FB_GPIO_Port GPIOC
#define KEY4_Pin GPIO_PIN_3
#define KEY4_GPIO_Port GPIOC
#define KEY4_EXTI_IRQn EXTI3_IRQn
#define INA_U_Pin GPIO_PIN_0
#define INA_U_GPIO_Port GPIOA
#define INA_V_Pin GPIO_PIN_1
#define INA_V_GPIO_Port GPIOA
#define INA_W_Pin GPIO_PIN_2
#define INA_W_GPIO_Port GPIOA
#define Vbus_Pin GPIO_PIN_3
#define Vbus_GPIO_Port GPIOA
#define Test_PIN_Pin GPIO_PIN_5
#define Test_PIN_GPIO_Port GPIOA
#define IU_Pin GPIO_PIN_6
#define IU_GPIO_Port GPIOA
#define IV_Pin GPIO_PIN_7
#define IV_GPIO_Port GPIOA
#define IW_Pin GPIO_PIN_4
#define IW_GPIO_Port GPIOC
#define KEY1_Pin GPIO_PIN_0
#define KEY1_GPIO_Port GPIOB
#define KEY1_EXTI_IRQn EXTI0_IRQn
#define KEY2_Pin GPIO_PIN_1
#define KEY2_GPIO_Port GPIOB
#define KEY2_EXTI_IRQn EXTI1_IRQn
#define KEY3_Pin GPIO_PIN_2
#define KEY3_GPIO_Port GPIOB
#define KEY3_EXTI_IRQn EXTI2_IRQn
#define Adjust_Pin GPIO_PIN_14
#define Adjust_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_12
#define LED1_GPIO_Port GPIOA
#define SPI3_CS_Pin GPIO_PIN_2
#define SPI3_CS_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
