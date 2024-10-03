#include "IF_StateMachine.h"

StateMachine_obj   state_Handler;
IF_output_obj      IF_output_Handler;
IF_input_obj       IF_input_Handler;

#define one_IF_time   (1 / IF_Open_Time)

extern int Run_Flag;
float thetaIntegrator = 0;
static float tmp = 0;

void freGeneration(StateMachine_obj * state_Handler, IF_input_obj * input_Handler);

void IF_StateMachine(StateMachine_obj * state_Handler, IF_output_obj * output_Handler, IF_input_obj * input_Handler
											,Encoder_obj * encoder, PIController_obj * spdController)
{
	state_Handler->zCounter++;
	
	switch(state_Handler->condition)
	{
		case Idle:
			state_Handler->state = 0;
			if(Run_Flag == 1)
			{
				state_Handler->zCounter = 0;
				state_Handler->state = 1;
				state_Handler->condition = FisrtAlign;
			}
		break;
			
		case FisrtAlign:
			state_Handler->state = 1;
			if(state_Handler->zCounter >= 1000)
			{
				state_Handler->zCounter = 0;
				state_Handler->condition = SecAlign;
			}
			else if(Run_Flag == 0)
			{
				state_Handler->condition = Idle;
				state_Handler->state = 0;
			}
		break;
			
		case SecAlign:
			state_Handler->state = 2;
			if(state_Handler->zCounter >= 1000)
			{
				state_Handler->zCounter = 0;
				state_Handler->condition = Ramp;
				state_Handler->state = 3;
			}
			else if(Run_Flag == 0)
			{
				state_Handler->condition = Idle;
				state_Handler->state = 0;
			}
		break;
			
		case Ramp:
			state_Handler->state = 3;
			if(state_Handler->zCounter >= IF_Open_Time * 20000.0F)
			{
				state_Handler->state = 4;
				state_Handler->condition = Stable;
				state_Handler->zCounter = 0;
			}
			else if(Run_Flag == 0)
			{
				state_Handler->condition = Idle;
				state_Handler->state = 0;
			}
		break;
			
		case Stable:
			state_Handler->state = 4;
//			Speed_Controller.Out = 0;
//			Speed_Controller.integrator = 0;
			if(state_Handler->zCounter >= preTime * 20000.0F)
			{
				state_Handler->state = 5;
				state_Handler->condition = FOC_STAGE;
				state_Handler->zCounter = 0;
			}
			if(Run_Flag == 0)
			{
				state_Handler->condition = Idle;
				state_Handler->state = 0;
			}
		break;
			
		case FOC_STAGE:
			state_Handler->state = 5;
			if(Run_Flag == 0)
			{
				state_Handler->condition = Idle;
				state_Handler->state = 0;
			}
	}
			

		
		switch(state_Handler->state)
		{
			
			case 0:
				state_Handler->IF_speed = 0;
				state_Handler->thetaMerge = 0;
				state_Handler->D_Merge = 0;
				state_Handler->Q_Merge = 0;
				thetaIntegrator = 0;
				complex_Handler.decouple_D = 0;
				complex_Handler.decouple_Q = 0;
				complex_Handler.BemfCompensation = 0;
			break;
			
			case 1:
				state_Handler->thetaMerge = firstAlignTheta;  // 第一次d轴对齐角度 pi/2
				state_Handler->D_Merge = D_current;      //d轴对齐电流
			  state_Handler->Q_Merge = 0;
			break;
						
		 	case 2:
				state_Handler->thetaMerge = 0;  //第二次d轴对齐角度 0
				state_Handler->D_Merge = D_current;  //d轴对齐电流
			  state_Handler->Q_Merge = 0;
				state_Handler->IF_speed = 0;
			break;
					
			case 3:

				thetaIntegrator = fmodf(thetaIntegrator, 1.0F);
				state_Handler->thetaMerge = thetaIntegrator;
				tmp = state_Handler->IF_speed;
				state_Handler->IF_speed += One_By_Sixty * IF_Ramp_Speed * Pn * Ts * one_IF_time; //极对数为5
				thetaIntegrator += Ts * tmp;
				state_Handler->D_Merge = 0;
			  state_Handler->Q_Merge = Q_current; //Q轴对齐电流
			break;
			
			case 4:
				thetaIntegrator = fmodf(thetaIntegrator, 1.0F);
				state_Handler->thetaMerge = thetaIntegrator;
				thetaIntegrator += One_By_Sixty * IF_Ramp_Speed * Pn * Ts; //极对数为5
				
				state_Handler->D_Merge = 0;
			  state_Handler->Q_Merge = Q_current; //Q轴对齐电流

			break;
			
			case 5:
				state_Handler->thetaMerge = encoder->encEleAngle;
				state_Handler->D_Merge = 0;
				state_Handler->Q_Merge = spdController->Out;
			break;
	  }
		
		output_Handler->id_ref = state_Handler->D_Merge;
		output_Handler->iq_ref = state_Handler->Q_Merge;	
	  output_Handler->theta_ref = state_Handler->thetaMerge;
		
}




