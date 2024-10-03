#include "FOC.h"

Vector_obj   complex_Handler;
SpeedLoop_obj   speedLoop_Handler;
float speedRef = 0;

void currentLoopStep()
{
	
}

void complexVectorDecouple(Vector_obj * complex, Encoder_obj * encoder)
{
	static float integrator1;
	static float integrator2;
	
  if (integrator1 > Saturation) 
	{
    complex->decouple_D = Saturation;
  } 
	else if (integrator1 < -Saturation) 
	{
    complex->decouple_D = -Saturation;
  } 
	else 
	{
    complex->decouple_D = integrator1;
  }
	
	if (integrator2 > Saturation) 
	{
    complex->decouple_Q = Saturation;
  } 
	else if (integrator2 < -Saturation) 
	{
    complex->decouple_Q = -Saturation;
  } 
	else 
	{
    complex->decouple_Q = integrator2;
  }
	
	integrator1 += encoder->encRPM * Pn * PI_BY_30 * D_Controller.Err * intCoeff * Ts;
	integrator2 += encoder->encRPM * Pn * PI_BY_30 * Q_Controller.Err * intCoeff * Ts;
	
	complex->BemfCompensation = encoder->encRPM * Pn * PI_BY_30 * flux;
		
}

void speedLoop(SpeedLoop_obj * spd_Handler, Encoder_obj * encoder,PIController_obj * PI_Handler)
{
	PI_Handler->Fdb = encoder->encRPM;
	PI_Handler->Ref = speedRef;
	PI_Calc(PI_Handler);
}
