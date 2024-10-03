#include "serial.h"


uint8_t rxBuffer_uart1[BUFFER_SIZE_UART1];
uint8_t rxBuffer_uart3[BUFFER_SIZE_UART3];

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef * huart, uint16_t Size)
{
		
//		if(huart->Instance == USART1)
//		{	
//			 HAL_UARTEx_ReceiveToIdle_DMA(&huart1, rxBuffer_uart1, BUFFER_SIZE_UART1); 
//			 HAL_UART_Transmit_DMA(&huart1, rxBuffer_uart1, sizeof(rxBuffer_uart1));
//		}
		
//		if(huart->Instance == USART3)
//    {
//			 uart_IT_Cnt++;
//			 HAL_UARTEx_ReceiveToIdle_DMA(&huart3, rxBuffer_uart3, BUFFER_SIZE_UART3);
////			 encoder_Handler.absPos = (uint32_t)rxBuffer_uart3[2] | (uint32_t)rxBuffer_uart3[3] << 8 | (uint32_t)rxBuffer_uart3[4] << 16;
////			__HAL_UART_CLEAR_FLAG(&huart3, UART_CLEAR_IDLEF);
////			 if(encoder_Handler.absPos >= 131072)
////			 {
////				 encoder_Handler.absPos = 131072;
////			 }
////			 if(encoder_Handler.absPos <= 0)
////			 {
////				 encoder_Handler.absPos = 0;
////			 }			   
//    }
}

