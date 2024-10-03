#include "HFT.h"

float regCOMPARE[3];
int offsetFlag = 1;
uint16_t lft_Flag = 0;
uint32_t spdCnt = 0;

extern void speedLoop(SpeedLoop_obj * spd_Handler, Encoder_obj * encoder,PIController_obj * PI_Handler);

void ADC_High_Freq_Task(void)
{
	sendEncoderCmd();
	
	if(offsetFlag)
	{
		ADC_Zero_Offset(&adcRawData_Handler);
	}
	else
	{
		lft_Flag++;

		
		LL_GPIO_SetOutputPin(Test_PIN_GPIO_Port, Test_PIN_Pin);

		IF_StateMachine(&state_Handler, &IF_output_Handler, &IF_input_Handler, 0, 0);
		
		if(state_Handler.condition == FOC_STAGE)
		{
			SC_handler.Angle = encoder_Handler.encEleAngle;
		}
		else
		{
			SC_handler.Angle = IF_output_Handler.theta_ref;
		}
		
		cordic_sincos_calc(SC_handler.Angle, &SC_handler.Sin, &SC_handler.Cos);
		
		getPhaseCurrent(&adcRawData_Handler, &adcConvData_Handler);
		
		
		iABC_handler.a = -adcConvData_Handler.currentINA_A;
		iABC_handler.b = -adcConvData_Handler.currentINA_B;
		iABC_handler.c = -adcConvData_Handler.currentINA_C;
		
//		iABC_handler.a = adcConvData_Handler.currentOPA_A;
//		iABC_handler.b = adcConvData_Handler.currentOPA_B;
//		iABC_handler.c = adcConvData_Handler.currentOPA_C;
		
		
		clarkeTrasform(&iABC_handler, &ialphaBeta_handler);
		

		
		parkTrasform(&ialphaBeta_handler, &iDQ_handler, &SC_handler);
		
		D_Controller.Ref = IF_output_Handler.id_ref;
		
		if(lft_Flag >= 20)
		{
			speedLoop(&speedLoop_Handler, &encoder_Handler,&Speed_Controller);
			lft_Flag = 0;
//			spdCnt++;
		}
		
		if(state_Handler.condition == FOC_STAGE)
		{
			Q_Controller.Ref = Speed_Controller.Out;
		}
		else
		{
			Q_Controller.Ref = IF_output_Handler.iq_ref;
		}
		
		D_Controller.Fdb = iDQ_handler.d;
		Q_Controller.Fdb = iDQ_handler.q;
		
		
		PI_Calc(&D_Controller);
		PI_Calc(&Q_Controller);
		
		complexVectorDecouple(&complex_Handler, &encoder_Handler);
		
		uDQ_handler.d = D_Controller.Out;
		uDQ_handler.q = Q_Controller.Out;
		
//		uDQ_handler.d = D_Controller.Out - complex_Handler.decouple_Q;
//		uDQ_handler.q = Q_Controller.Out + complex_Handler.decouple_D + complex_Handler.BemfCompensation;


		
		invParkTransform(&ualphaBeta_handler, &uDQ_handler, &SC_handler);
		
		SVW_Sector(&ualphaBeta_handler, &FOC_handler,&SVM_handler, 
								regCOMPARE, 0, 0);
	
								
		decoder(&encoder_Handler); 
		encoder_Handler.encRPM = low_pass_filter(encoder_Handler.encRPM, 0.002F);
		
		LL_GPIO_ResetOutputPin(Test_PIN_GPIO_Port, Test_PIN_Pin);

		
		LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_B, regCOMPARE[0]);
		LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_C, regCOMPARE[1]);
		LL_HRTIM_TIM_SetCompare1(HRTIM1, LL_HRTIM_TIMER_E, regCOMPARE[2]);
		
		upLoad();

	}
}

void ADC_Low_Freq_Task(void)
{
	
}



