#include "cordic_calc.h"

  /** Calc sinCos using hardware CORDIC
   *  Input: angle in radians (range -pi : pi)
   *  Output: sin, cos (range -1 : 1)
   *  Average execution time: ~98 CPU cycles
   **/
void cordic_sincos_calc(volatile float angle, volatile float *s, volatile float *c)
{
  if (LL_CORDIC_GetFunction(CORDIC) != LL_CORDIC_FUNCTION_SINE)
  {
    /* Force CORDIC reset */
    LL_AHB1_GRP1_ForceReset(LL_AHB1_GRP1_PERIPH_CORDIC);
    /* Release CORDIC reset */
    LL_AHB1_GRP1_ReleaseReset(LL_AHB1_GRP1_PERIPH_CORDIC);

    LL_CORDIC_Config(CORDIC, LL_CORDIC_FUNCTION_SINE, /* cosine function */
                     LL_CORDIC_PRECISION_6CYCLES,     /* max precision for q1.31 cosine */
                     LL_CORDIC_SCALE_0,               /* no scale */
                     LL_CORDIC_NBWRITE_1,             /* One input data: angle. Second input data (modulus) is 1 after cordic reset */
                     LL_CORDIC_NBREAD_2,              /* Two output data: sine, then cosine */
                     LL_CORDIC_INSIZE_32BITS,         /* q1.31 format for input data */
                     LL_CORDIC_OUTSIZE_32BITS);       /* q1.31 format for output data */
  }
//	if(angle > MF_PI)
//	{
//		angle -= M_2PI;
//	}
	
	if(angle > 0.5F)
	{
		angle -= 1.0F;
	}
		

//  LL_CORDIC_WriteData(CORDIC, _IQ31(angle * ONE_BY_PI)); // cordic input data format q31, angle range (1 : -1)
	LL_CORDIC_WriteData(CORDIC, _IQ31(angle * 2));
  *s = _IQ31toF_FAST((int32_t)LL_CORDIC_ReadData(CORDIC));
  *c = _IQ31toF_FAST((int32_t)LL_CORDIC_ReadData(CORDIC));
}

void truncateAngleTo2Pi( float * const angle)
{
    if(*angle >= M_2PI )
    {
        *angle = 0;
    }
    else if( 0.0F > *angle )
    {
        *angle = M_2PI;
    }
    else
    {
       /* Do nothing */
    }
}

void truncateAngleToOne( float * const angle)
{
    if(*angle >= 1.0F )
    {
        *angle = 0;
    }
    else if( 0.0F > *angle )
    {
        *angle = 1.0F;
    }
    else
    {
       /* Do nothing */
    }
}


float low_pass_filter(float input, float ALPHA) 
{
	static float prev_input;
  float output;
    
  output = ALPHA * input + (1 - ALPHA) * prev_input;
    
  prev_input = output;
    
  return output;
}




