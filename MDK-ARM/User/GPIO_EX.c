#include "GPIO_EX.h"
#include "usart.h"
extern int Run_Flag;
extern float speedRef;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(GPIO_Pin);
	if(GPIO_Pin == KEY1_Pin)
	{
		Run_Flag = !Run_Flag;
		
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		printf("Key1\r\n");
		
		if(Run_Flag == 0)
		{
			LL_HRTIM_DisableOutput(HRTIM1, LL_HRTIM_OUTPUT_TB1 | LL_HRTIM_OUTPUT_TB2 | LL_HRTIM_OUTPUT_TC1 |
                                    LL_HRTIM_OUTPUT_TC2 | LL_HRTIM_OUTPUT_TE1 | LL_HRTIM_OUTPUT_TE2);
		}
		else
		{
			LL_HRTIM_EnableOutput(HRTIM1, LL_HRTIM_OUTPUT_TB1 | LL_HRTIM_OUTPUT_TB2 | LL_HRTIM_OUTPUT_TC1 |
                                    LL_HRTIM_OUTPUT_TC2 | LL_HRTIM_OUTPUT_TE1 | LL_HRTIM_OUTPUT_TE2);
		}
	}
	
	else if(GPIO_Pin == KEY2_Pin)
	{
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		
		speedRef += 50.0F;
		if(speedRef > 1200.0F)
		{
			speedRef = 1200.0F;
		}
		
		printf("Key2\r\n");
	}
		
	else if(GPIO_Pin == KEY3_Pin)
	{
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
		
		speedRef -= 50.0F;
		if(speedRef < 50.0F)
		{
			speedRef = 50.0F;
		}
		
		printf("Key3\r\n");
	}
	
	else if(GPIO_Pin == KEY4_Pin)
	{
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
		printf("Key4\r\n");
	}
  /* NOTE: This function should not be modified, when the callback is needed,
           the HAL_GPIO_EXTI_Callback could be implemented in the user file
   */
}

