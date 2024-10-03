/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "cordic.h"
#include "dac.h"
#include "dma.h"
#include "fdcan.h"
#include "hrtim.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "serial.h"

#include "mc_lib.h"
#include "cordic_calc.h"
#include "IF_StateMachine.h"
#include "PI.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
int Run_Flag = 1;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint16_t aDST_Buffer1[ADC1_BUFFER_SIZE]; //Adjust, V_FB, Vbus,
uint16_t aDST_Buffer2[ADC2_BUFFER_SIZE]; //U_FB, W_FB
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern uint8_t rxBuffer_uart1[BUFFER_SIZE_UART1]; 
extern uint8_t rxBuffer_uart3[BUFFER_SIZE_UART3];
float Adjust, V_FB, Vbus, U_FB, W_FB;
extern float speedRef;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void ADC_Regular_DMA_Start(void);
void UART_RX_Idle_Start(void);
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_HRTIM1_Init();
  MX_ADC1_Init();
  MX_ADC2_Init();
  MX_DAC1_Init();
  MX_FDCAN3_Init();
  MX_I2C1_Init();
  MX_I2C3_Init();
  MX_SPI3_Init();
  MX_TIM8_Init();
  MX_USART1_UART_Init();
  MX_USART3_UART_Init();
  MX_CORDIC_Init();
  /* USER CODE BEGIN 2 */
//	__HAL_UART_CLEAR_FLAG(&huart3, UART_CLEAR_IDLEF);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rxBuffer_uart1, BUFFER_SIZE_UART1);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rxBuffer_uart3, BUFFER_SIZE_UART3);
	Run_Flag = 0;
	HRTIM_Start();
	ADC_Start();
	PI_Initialize();

	FOC_handler.Vbus = 30.0F;
	SVM_handler.PWMtimerCounts = 17000.0F;
	encoder_Handler.offsetPos = 0.16F;
	speedRef = 800.0F;
	
	LL_DAC_Enable(DAC1, LL_DAC_CHANNEL_1);
	
	
  /* Set duty to 0 */
  LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_B, 0);
  LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_C, 0);
  LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_E, 0);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
  while(LL_FLASH_GetLatency() != LL_FLASH_LATENCY_4)
  {
  }
  LL_PWR_EnableRange1BoostMode();
  LL_RCC_HSE_Enable();
   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {
  }

  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_3, 85, LL_RCC_PLLR_DIV_2);
  LL_RCC_PLL_ConfigDomain_ADC(LL_RCC_PLLSOURCE_HSE, LL_RCC_PLLM_DIV_3, 85, LL_RCC_PLLP_DIV_2);
  LL_RCC_PLL_EnableDomain_SYS();
  LL_RCC_PLL_EnableDomain_ADC();
  LL_RCC_PLL_Enable();
   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {
  }

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_2);
   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  }

  /* Insure 1us transition state at intermediate medium speed clock*/
  for (__IO uint32_t i = (170 >> 1); i !=0; i--);

  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_2);
  LL_SetSystemCoreClock(170000000);

   /* Update the time base */
  if (HAL_InitTick (TICK_INT_PRIORITY) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void ADC_Regular_DMA_Start(void)
{
	LL_DMA_SetDataLength(DMA1,LL_DMA_CHANNEL_5,ADC1_BUFFER_SIZE);
	LL_DMA_SetPeriphAddress(DMA1,LL_DMA_CHANNEL_5,LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA));
	LL_DMA_SetMemoryAddress(DMA1,LL_DMA_CHANNEL_5,(uint32_t)aDST_Buffer1);
	LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_5);
	LL_ADC_REG_SetDMATransfer(ADC1,LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
	
	LL_DMA_SetDataLength(DMA1,LL_DMA_CHANNEL_6,ADC2_BUFFER_SIZE);
	LL_DMA_SetPeriphAddress(DMA1,LL_DMA_CHANNEL_6,LL_ADC_DMA_GetRegAddr(ADC2, LL_ADC_DMA_REG_REGULAR_DATA));
	LL_DMA_SetMemoryAddress(DMA1,LL_DMA_CHANNEL_6,(uint32_t)aDST_Buffer2);
	LL_DMA_EnableChannel(DMA1,LL_DMA_CHANNEL_6);
	LL_ADC_REG_SetDMATransfer(ADC1,LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
}

void UART_RX_Idle_Start(void)
{
	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rxBuffer_uart1, BUFFER_SIZE_UART1);
	HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rxBuffer_uart3, BUFFER_SIZE_UART3);
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
