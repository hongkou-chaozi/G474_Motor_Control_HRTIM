#include "PI.h"

PIController_obj   D_Controller;
PIController_obj   Q_Controller;
PIController_obj   Speed_Controller;

void PI_Initialize(void)
{
	D_Controller.Ki = iGainCurrent;
	D_Controller.Kp = pGainCurrent;
	D_Controller.OutMax = currentMaxOutput;
	D_Controller.OutMin = currentMinOutput;
	
	Q_Controller.Ki = iGainCurrent;
	Q_Controller.Kp = pGainCurrent;
	Q_Controller.OutMax = currentMaxOutput;
	Q_Controller.OutMin = currentMinOutput;
	
	Speed_Controller.Ki = iGainSpd;
	Speed_Controller.Kp = pGainSpd;
	Speed_Controller.OutMax = spdMaxOutput;
	Speed_Controller.OutMin = spdMinOutput;
}

void currentControllerReset(void)
{
	
}


void PI_Calc(PIController_obj * PI_Handler)
{
	PI_Handler->Err = PI_Handler->Ref - PI_Handler->Fdb;
	PI_Handler->Out = PI_Handler->Kp * PI_Handler->Err + Ts * PI_Handler->integrator;
	
	if(PI_Handler->Out > PI_Handler->OutMax)
	{
		PI_Handler->Out = PI_Handler->OutMax;
		PI_Handler->deadZone -= PI_Handler->OutMax;
	}
	else if(PI_Handler->Out < PI_Handler->OutMin)
	{
		PI_Handler->Out = PI_Handler->OutMin;
		PI_Handler->deadZone -= PI_Handler->OutMin;
	}
	else
	{
		PI_Handler->deadZone = 0.0F;
	}
	
	float integratorGain = PI_Handler->Ki * PI_Handler->Err;
	
	int tmp0 = PI_Handler->deadZone > 0.0F ? 1 : -1;
	int tmp1 = integratorGain > 0.0F ? 1 : -1;
	
	if((PI_Handler->deadZone != 0.0F) && (tmp0 == tmp1))
	{
		integratorGain = 0;
	}
	PI_Handler->integrator += integratorGain;
}


