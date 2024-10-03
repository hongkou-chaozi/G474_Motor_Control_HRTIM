#ifndef __ENCODER
#define __ENCODER


#include <stdint.h>
#include "cordic_calc.h"
#include "usart.h"

#define   POSpuConFactor     (float)(1 / 131072.0F)


typedef struct _Encoder_obj_
{
	uint32_t absPos;
	float puPos;
	float offsetPos;
	float encEleAngle;
	float encRPM;
	uint32_t cntPerRev;
	uint16_t CircBufIdx;                 
  uint32_t DelaySamps[50];
} Encoder_obj;



extern Encoder_obj   encoder_Handler;

void decoder(Encoder_obj * encoder);
void sendEncoderCmd(void);

#endif /* Encoder */
