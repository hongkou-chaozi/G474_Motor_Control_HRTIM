#include "upload.h"
#include <stdint.h>
#include "encoder.h"
#include "IF_StateMachine.h"
extern UART_HandleTypeDef huart1;
extern uint16_t lft_Flag;
extern StateMachine_obj   state_Handler;



extern float squ;
static float temp[12];
static uint8_t tempData[52] = 
{	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0x80,0x7F};

void upLoad(void)
{
			temp[0] = regCOMPARE[0];
			temp[1] = regCOMPARE[1];
			temp[2] = regCOMPARE[2];
			temp[3] = encoder_Handler.absPos;
			temp[4] = Speed_Controller.Out;
			temp[5] = encoder_Handler.encRPM;
			temp[6] = encoder_Handler.encEleAngle;
			temp[7] = sqrtf((uDQ_handler.d * uDQ_handler.d) + (uDQ_handler.q * uDQ_handler.q));
			temp[8] = iDQ_handler.d;
			temp[9] = iDQ_handler.q;
			temp[10] = uDQ_handler.d;
			temp[11] = uDQ_handler.q;	
			memcpy(tempData, temp, sizeof(temp));
		  HAL_UART_Transmit_DMA(&huart1,(uint8_t *)tempData,sizeof(tempData));
}
