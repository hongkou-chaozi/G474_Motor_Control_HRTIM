#ifndef __STATE_MACHINE
#define __STATE_MACHINE

#include <stdint.h>
#include "cordic_calc.h"
#include "FOC.h"
#include "PI.h"

#define   Idle         0U
#define   FisrtAlign   1U
#define   SecAlign     2U
#define   Ramp         3U
#define   Stable       4U
#define   FOC_STAGE    5U



#define   Pn                    5.0F
#define   Q_current             1.2F
#define   D_current             1.0F
#define   IF_Open_Time          2.0F
#define   IF_Ramp_Speed         400.0F      
//#define   firstAlignTheta       M_PI2
#define   firstAlignTheta       0.0F
#define   secAlignTheta         0.0F

#define   preTime               0.001F

typedef struct _StateMachine_obj_
{
	uint32_t zCounter;
	uint8_t condition;
	uint8_t state;
	float   thetaMerge;
	float   D_Merge;
	float   Q_Merge;
	float   IF_speed;
} StateMachine_obj;

typedef struct _IF_input_obj_
{
	float IFtime;
	float rampSpeed;
	
} IF_input_obj;

typedef struct _IF_output_obj_
{
	float theta_ref;
	float id_ref;
	float iq_ref;
	
} IF_output_obj;

extern StateMachine_obj   state_Handler;
extern IF_output_obj      IF_output_Handler;
extern IF_input_obj       IF_input_Handler;

void IF_StateMachine(StateMachine_obj * state_Handler, IF_output_obj * output_Handler,
	IF_input_obj * input_Handler,Encoder_obj * encoder, PIController_obj * spd);


                                                                                                                                    

#endif /* State_Machine */
