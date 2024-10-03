#ifndef __PI
#define __PI


#define  currentMaxOutput      (10.0F)
#define  currentMinOutput      (-10.0F)
#define  pGainCurrent          2.5F
#define  iGainCurrent          0.8F

#define  spdMaxOutput      (2.0F)
#define  spdMinOutput      (-2.0F)
#define  pGainSpd          0.01F
#define  iGainSpd          0.006F

#define   Ts                    0.00005F

typedef struct 
{  float  Ref;   			// Input: Reference input
	 float  Fdb;   			// Input: Feedback input
   float  Out;   			// Output: PID output
	 float  deadZone;
	 float  integrator;
	
	 float  Kp;				  // Parameter: Proportional gain  
	 float  Ki;			    // Parameter: Integral gain       

	 float  OutMax;		    // Parameter: Maximum output    
	 float  OutMin;	    	// Parameter: Minimum output   
	
	 float  Err;				  // Variable: Error

	
}PIController_obj;

void PI_Calc(PIController_obj * PI_Handler);
void PI_Initialize(void);

extern PIController_obj   D_Controller;
extern PIController_obj   Q_Controller;
extern PIController_obj   Speed_Controller;





#endif /* PI */
