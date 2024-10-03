#include "mc_Lib.h"
#include "PI.h"

//默认使用cordic计算角度的正余弦值，取消注释则采用arm_math库计算正余弦值
//#define ARM_MATH_SIN_COS

SVM_obj                         SVM_handler;
FOC_obj                         FOC_handler;
alphaBeta_obj                   ialphaBeta_handler;
DQ_obj                          iDQ_handler;
alphaBeta_obj                   ualphaBeta_handler;
DQ_obj                          uDQ_handler;
ABC_obj                         iABC_handler;
ABC_obj                         uABC_handler;
SC_obj                          SC_handler;
PIController_obj                current_PI_handler;
PIController_obj                speed_PI_handler;
SMO_obj                         SMO_handler;

void clarkeTrasform(ABC_obj * abc, alphaBeta_obj * alphaBeta)
{
	alphaBeta->alpha = abc->a;
	alphaBeta->beta = (abc->a * ONE_BY_SQRT3) + (abc->b * 2.0F * ONE_BY_SQRT3);
//	alphaBeta->alpha = TWO_BY_THREE * abc->a - ONE_BY_THREE * abc->b - ONE_BY_THREE * abc->c;
//	alphaBeta->beta = ONE_BY_SQRT3 * abc->b - ONE_BY_SQRT3 * abc->c;
}

void parkTrasform(alphaBeta_obj * alphaBeta, DQ_obj * dqframe, SC_obj * scLookup)
{
	dqframe->d = alphaBeta->alpha * scLookup->Cos + alphaBeta->beta * scLookup->Sin;
	dqframe->q = -alphaBeta->alpha * scLookup->Sin + alphaBeta->beta * scLookup->Cos;
}

void invClarkeTrasform(ABC_obj * abc, alphaBeta_obj * alphaBeta)
{
	abc->a = alphaBeta->alpha;
	abc->b = -0.5F * alphaBeta->alpha + SQRT3_BY_2 * alphaBeta->beta;
	abc->c = -abc->a - abc->b;
}

void invParkTransform(alphaBeta_obj * alphaBeta, DQ_obj * dqframe, SC_obj * scLookup)
{
    alphaBeta->alpha =  dqframe->d * scLookup->Cos - dqframe->q * scLookup->Sin;
    alphaBeta->beta  =  dqframe->d * scLookup->Sin + dqframe->q * scLookup->Cos; 
}

void freGen(SC_obj * scLookup, float freq)
{
	scLookup->Angle += fre_PI * freq;

  float VF_Theta = scLookup->Angle;

  if (VF_Theta > M_2PI) {
    VF_Theta -= M_2PI;
  } else if (VF_Theta < 0.0F) {
    VF_Theta += M_2PI;
  }
	
	scLookup->Angle = VF_Theta;
	
#ifdef ARM_MATH_SIN_COS
	
  scLookup->Sin = arm_sin_f32(VF_Theta);
  scLookup->Cos = arm_cos_f32(VF_Theta);

#else
	
	cordic_sincos_calc(VF_Theta, &scLookup->Sin, &scLookup->Cos);

#endif
}

void SVM_MinMax(ABC_obj * abc, DQ_obj * dq, alphaBeta_obj * alphaBeta, SC_obj * scLookup, FOC_obj * focHanlder ,
						 SVM_obj * SVMhandler, float * regCCR)
{
//  invParkTransform(alphaBeta, dq, scLookup);
//  invClarkeTrasform(abc, alphaBeta);

  float maxV;
  float minV;
  float zeroSeqComp;
	
  float cmpTmp[3] = {abc->a, abc->b, abc->c};
	
  maxV = cmpTmp[0];
  minV = cmpTmp[0];
		
	  /* 找出三个电流值中的最大值和最小值 */
  for (uint8_t k = 1; k < 3; k++) 
  {
    maxV = fmaxf(maxV, cmpTmp[k]);
    minV = fminf(minV, cmpTmp[k]);
  }
	
	 /* 计算零序分量 */
  zeroSeqComp = (maxV + minV) * -0.5F;
	
  regCCR[0] = (-(zeroSeqComp + cmpTmp[0]) / focHanlder->Vbus + 0.5F) * SVMhandler->PWMtimerCounts;
  regCCR[1] = (-(zeroSeqComp + cmpTmp[1]) / focHanlder->Vbus + 0.5F) * SVMhandler->PWMtimerCounts;
  regCCR[2] = (-(zeroSeqComp + cmpTmp[2]) / focHanlder->Vbus + 0.5F) * SVMhandler->PWMtimerCounts;
}

void SVW_Sector(alphaBeta_obj * alphaBeta, FOC_obj * focHanlder ,SVM_obj * SVMhandler, 
								float * regCCR, DQ_obj * dq, SC_obj * scLookup)
{
//	invParkTransform(alphaBeta, dq, scLookup);
	
	float Va = 0;
	float Vb = 0;
	float Vc = 0;
	
	SVMhandler->N = 0;
	
	SVMhandler->Vr1 = alphaBeta->beta;
	SVMhandler->Vr2 = SQRT3_BY_2 * alphaBeta->alpha - 0.5F * alphaBeta->beta;
	SVMhandler->Vr3 = -SQRT3_BY_2 * alphaBeta->alpha - 0.5F * alphaBeta->beta;
	
	if(SVMhandler->Vr1 > 0)
	{
		SVMhandler->N = 1;
	}
	if(SVMhandler->Vr2 > 0)
	{
		SVMhandler->N += 2;
	}
	if(SVMhandler->Vr3 > 0)
	{
		SVMhandler->N += 4;
	}
	
	Va = SQRT3 * alphaBeta->beta;
	Vb = SQRT3_BY_2 * alphaBeta->beta + 1.5F * alphaBeta->alpha;
	Vc = SQRT3_BY_2 * alphaBeta->beta - 1.5F * alphaBeta->alpha;
	
	float temp1 = 1 / focHanlder->Vbus;
	Va *= temp1;
	Vb *= temp1;
	Vc *= temp1;
	
	if (SVMhandler->N == 0)  // Sector 0: this is special case for (Ualpha,Ubeta) = (0,0)
	{
		SVMhandler->Ta = 0.5F;
		SVMhandler->Tb = 0.5F;
		SVMhandler->Tc = 0.5F;
	}
	
	else if (SVMhandler->N == 1)  // Sector 2: t1=Z and t2=Y (abc ---> Tb,Ta,Tc)
	{
		SVMhandler->T1 = Vc;
		SVMhandler->T2 = Vb;
	}
	else if (SVMhandler->N == 2)  // Sector 6: t1=Y and t2=-X (abc ---> Ta,Tc,Tb)
	{
		SVMhandler->T1 = Vb;
		SVMhandler->T2 = -Va;
	}
	else if (SVMhandler->N == 3)  // Sector 1: t1=-Z and t2=X (abc ---> Ta,Tb,Tc)
	{
		SVMhandler->T1 = -Vc;
		SVMhandler->T2 = Va;
	}
	else if (SVMhandler->N == 4)  // Sector 4: t1=-X and t2=Z (abc ---> Tc,Tb,Ta)
	{
		SVMhandler->T1 = -Va;
		SVMhandler->T2 = Vc;
	}
	else if (SVMhandler->N == 5)  // Sector 3: t1=X and t2=-Y (abc ---> Tb,Tc,Ta)
	{
		SVMhandler->T1 = Va;
		SVMhandler->T2 = -Vb;
	}
	else if (SVMhandler->N == 6)  // Sector 5: t1=-Y and t2=-Z (abc ---> Tc,Ta,Tb)
	{
		SVMhandler->T1 = -Vb;
		SVMhandler->T2 = -Vc;
	}
	
	if((SVMhandler->T1 + SVMhandler->T2) > 1.0F)
	{
		float temp2= 1 / (SVMhandler->T1 + SVMhandler->T2);
		SVMhandler->T1 *= temp2;
		SVMhandler->T2 *= temp2;
	}
	
	SVMhandler->dutyCycle_A = 0.5F * (1.0F - SVMhandler->T1 - SVMhandler->T2);
	SVMhandler->dutyCycle_B = SVMhandler->dutyCycle_A + SVMhandler->T2;
	SVMhandler->dutyCycle_C = SVMhandler->dutyCycle_B + SVMhandler->T1;
	
	if (SVMhandler->N == 1) 		 // Sector 2
	{
		SVMhandler->Ta = SVMhandler->dutyCycle_B;
		SVMhandler->Tb = SVMhandler->dutyCycle_C;
		SVMhandler->Tc = SVMhandler->dutyCycle_A;
	}
	else if (SVMhandler->N == 2) 	// Sector 6
	{
		SVMhandler->Ta = SVMhandler->dutyCycle_C;
		SVMhandler->Tb = SVMhandler->dutyCycle_A;
		SVMhandler->Tc = SVMhandler->dutyCycle_B;
	}
	else if (SVMhandler->N == 3)  // Sector 1
	{
		SVMhandler->Ta = SVMhandler->dutyCycle_C;
		SVMhandler->Tb = SVMhandler->dutyCycle_B;
		SVMhandler->Tc = SVMhandler->dutyCycle_A;
	}
	else if (SVMhandler->N == 4) // Sector 4
	{
		SVMhandler->Ta = SVMhandler->dutyCycle_A;
		SVMhandler->Tb = SVMhandler->dutyCycle_B;
		SVMhandler->Tc = SVMhandler->dutyCycle_C;
	}
	else if (SVMhandler->N == 5) // Sector 3
	{
		SVMhandler->Ta = SVMhandler->dutyCycle_A;
		SVMhandler->Tb = SVMhandler->dutyCycle_C;
		SVMhandler->Tc = SVMhandler->dutyCycle_B;
	}
	else if (SVMhandler->N == 6) // Sector 5
	{
		SVMhandler->Ta = SVMhandler->dutyCycle_B;
		SVMhandler->Tb = SVMhandler->dutyCycle_A;
		SVMhandler->Tc = SVMhandler->dutyCycle_C;
	}
	
	regCCR[0] = SVMhandler->Ta * SVMhandler->PWMtimerCounts;
  regCCR[1] = SVMhandler->Tb * SVMhandler->PWMtimerCounts;
  regCCR[2] = SVMhandler->Tc * SVMhandler->PWMtimerCounts;

}













