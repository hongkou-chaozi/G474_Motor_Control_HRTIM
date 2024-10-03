#ifndef _MC_LIB_H
#define _MC_LIB_H


#include "stdint.h"
#include "math.h"
#include "cordic.h"
#include "arm_math.h"
#include "cordic_calc.h"



typedef struct _SVM_obj_
{
    float   PWMtimerCounts;  // PWM Period in PWM Timer Counts
    float   Vr1;        // Normalized Phase A voltage obtained using modified Clarke transform
    float   Vr2;        // Normalized Phase B voltage obtained using modified Clarke transform
    float   Vr3;        // Normalized Phase C voltage obtained using modified Clarke transform
    float	  T1;         // Length of Vector T1
    float   T2;         // Length of Vector T2
    float   Ta;         // Ta = To/2 + T1 + T2
    float   Tb;         // Tb = To/2 + T2
    float   Tc;         // Tc = To/2
    float 	dutyCycle_A;      // Phase A Duty Cycle
    float   dutyCycle_B;      // Phase B Duty Cycle
    float   dutyCycle_C;      // Phase C Duty Cycle
		uint16_t N;
} SVM_obj;

typedef struct _FOC_obj_
{
    float   openLoopAngle;      // Rotor Angle in Radians
    float   Vbus;   // Measured DC Bus voltage in volts
    float   phaseVoltageMax;// Maximum Phase to Neutral Voltage
	  float		VdqNorm;        // Normalized vector sum of D and Q axis voltage
		float		squareMaxPhaseVoltage;    // Square of Maximum Phase Voltage  
		float   squareDVoltage;            // Square of D axis Voltage in Volts
		float		squareQVoltage;            // Square of Q axis Voltage in Volts
//		float		VqRefVoltage;               // Estimated Q axis voltage during Flux Weakening in Volts
} FOC_obj;

typedef struct _alphaBeta_obj_
{
    float   alpha;     // Alpha axis component in 2 Phase Stationary Frame 
    float   beta;      // Beta axis component  in 2 Phase Stationary Frame

} alphaBeta_obj;

typedef struct _DQ_obj_
{
    float   d;     // D axis component in 2 Phase Rotating Frame 
    float   q;      // Q axis component  in 2 Phase Rotating Frame

} DQ_obj;

typedef struct _ABC_obj_
{
    float   a;      // A axis component in 3 Phase Stationary Frame 
    float   b;      // B axis component in 3 Phase Stationary Frame 
    float   c;      // C axis component in 3 Phase Stationary Frame 
} ABC_obj;

typedef struct _SC_obj_
{
    float   Angle; // Angle in radians whose sine/cosine needs to be calculated
    float   Sin;   // Sine(Angle)
    float   Cos;   // Cosine(Angle) 
} SC_obj;


typedef struct _SMO_obj_
{
    float	deltaT;      						// Integration Interval
    float estAngle;    	    					// Estimated Rotor Angle
    float pri_estSpeed;     						// primary speed estimation
    float lastAlphaCurrent;  						// last value for Ialpha
    float lastBetaCurrent;   						// last value for Ibeta
    float difAlphaCur;     						// difference Ialpha
    float difBetaCur;      						// difference Ibeta
	  float	BEMF_alpha;								// BEMF alpha
	  float	BEMF_beta;								// BEMF beta
	  float	BEMF_d;								// BEMF d
	  float	BEMF_q;								// BEMF q
    float	indAlphaVoltage;   						// dI*Ls/dt alpha
	  float	indBetaVoltage;    						// dI*Ls/dt beta
	  float	BEMF_d_fil;        						// BEMF d filtered
	  float	BEMF_q_fil;        						// BEMF q filtered
	  float	dqFilterConstant; 						// filter constant for d-q BEMF
		float estSpeed; 							// Estimated speed 
		float spdFilterConstant; 					// Filter Konstant for Estimated speed 
    float stepLastValpha;  						// Value from last control step Ialpha 
    float stepLastVbeta;   						// Value from last control step Ibeta
		float RhoOffset;            				// estimated rotor angle init offset in radians
    float	Rs;								// Rs value - stator resistance in ohms
		float	LsDt;								// Ls/dt value - stator inductand / dt - variable with speed
   	float	InvKFi;	    					// InvKfi constant value ( InvKfi = Omega/BEMF )
    float KFi;                               // Backemf Constant in V-sec/rad
    float InvKFi_Below_Nominal_Speed;        // 1/(Backemf Constant in V-sec/rad) when electrical speed < Nominal electrical Speed of the motor
    float Ls_DIV_2_PI;                       // Phase Inductance Ls / 2*PI
    float Nominal_Speed;                     // Nominal Electrical Speed of the motor radians/sec
    float Decimate_Nominal_Speed;            // Nominal Electrical Speed/10 of the motor

} SMO_obj;

extern SVM_obj                         SVM_handler;
extern FOC_obj                         FOC_handler;
extern alphaBeta_obj                   ualphaBeta_handler;
extern DQ_obj                          uDQ_handler;
extern alphaBeta_obj                   ialphaBeta_handler;
extern DQ_obj                          iDQ_handler;
extern ABC_obj                         iABC_handler;
extern ABC_obj                         uABC_handler;
extern SC_obj                          SC_handler;
extern SMO_obj                         SMO_handler;

void SVM_MinMax(ABC_obj * abc, DQ_obj * dq, alphaBeta_obj * alphaBeta, SC_obj * scLookup, 
						 FOC_obj * focHanlder ,SVM_obj * SVMhandler, float * regCCR);

void SVW_Sector(alphaBeta_obj * alphaBeta, FOC_obj * focHanlder ,SVM_obj * SVMhandler, 
								float * regCCR, DQ_obj * dq, SC_obj * scLookup);

void freGen(SC_obj * scLookup, float freq);


void clarkeTrasform(ABC_obj * abc, alphaBeta_obj * alphaBeta);
void parkTrasform(alphaBeta_obj * alphaBeta, DQ_obj * dqframe, SC_obj * scLookup);
void invClarkeTrasform(ABC_obj * abc, alphaBeta_obj * alphaBeta);
void invParkTransform(alphaBeta_obj * alphaBeta, DQ_obj * dqframe, SC_obj * scLookup);
void sinCosLookup(SC_obj * sc);




//void SMO_Estimator(mcParam_PLLEstimator *pllestimatorParam,
//                        mcParam_SinCos *scParam, mcParam_FOC *focParam, 
//                        mcParam_ControlRef *ctrlParam, 
//                        mcParam_AlphaBeta *I_alphabetaParam,
//                        mcParam_AlphaBeta *V_alphabetaParam );





#endif // #ifndef _MC_LIB_H
