#ifndef __FOC
#define __FOC


#include "stdint.h"
#include "cordic.h"
#include "cordic_calc.h"
#include "main.h"
#include "upload.h"
#include "mc_lib.h"
#include "bsp_adc.h"
#include "adc.h"
#include "IF_StateMachine.h"




#define    Saturation    3.0F
#define    intCoeff      0.005F
#define    flux          0.006F


typedef struct _Vector_obj_
{
	float decouple_D;   // apply to Q axis
	float decouple_Q;   // apply to D axis
	float BemfCompensation;
} Vector_obj;

typedef struct _SpeedLoop_obj_
{
	float speedLoopRef;   
	float speedLoopFbk;   
	float speedLoopErr;
} SpeedLoop_obj;


extern Vector_obj   complex_Handler;
extern SpeedLoop_obj   speedLoop_Handler;

void currentLoopStep(void);
void complexVectorDecouple(Vector_obj * complex, Encoder_obj * encoder);


#endif /* FOC */
