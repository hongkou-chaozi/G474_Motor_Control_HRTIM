#include "encoder.h"

Encoder_obj   encoder_Handler;

static uint8_t cmd = 0x1A;

void decoder(Encoder_obj * encoder)
{
	encoder->puPos = (float)encoder->absPos * POSpuConFactor;
	
	float product;
	uint32_t temp;
	if(encoder->puPos <= encoder->offsetPos)
	{
		product = encoder->puPos + 1.0F - encoder->offsetPos;
	}
	else
	{
		product = encoder->puPos - encoder->offsetPos;
	}
	
	product *= 5.0F;
	
	encoder->encEleAngle = product - floorf(product);
	
	temp = (uint32_t)(4294967296 * encoder->puPos);
	
	encoder->encRPM = (float)((int32_t)temp - (int32_t)encoder->DelaySamps[encoder->CircBufIdx]) * 5.5879E-6F;
	
	encoder->DelaySamps[encoder->CircBufIdx] = temp;
	
  if (encoder->CircBufIdx < 49) 
	{
    encoder->CircBufIdx++;
  } 
	else 
  {
    encoder->CircBufIdx = 0;
  }
	
}

void sendEncoderCmd(void)
{
	HAL_UART_Transmit(&huart3, &cmd, 1, 5);
}