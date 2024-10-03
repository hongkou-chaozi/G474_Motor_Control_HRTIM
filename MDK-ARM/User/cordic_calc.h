#ifndef __CORDIC
#define __CORDIC

#include "cordic.h"
#include <math.h>
#include "encoder.h"


#define SQRT3    (1.732F)
#define ONE_BY_SQRT3 (0.57735F)			 // 1 / sqrt(3)
#define ONE_BY_SQRT2 (0.7071F)			 // 1 / sqrt(2)
#define TWO_BY_SQRT3 (2.0F * 0.577350F)	 // (1 / sqrt(3)) * 2
#define SQRT3_BY_2 (0.8660254F)				 // sqrt(3) / 2
#define SQRT2 (1.414213562F)					 // sqrt(2)
#define COS_30_DEG (0.8660254F)
#define SIN_30_DEG (0.5F)
#define COS_MINUS_30_DEG (0.86602540F)
#define SIN_MINUS_30_DEG (-0.5F)
#define RADS2RPM 9.54929658F
#define RPM2RADS (1.0F / RADS2RPM)
#define ONE_BY_THREE  (0.3333F)
#define TWO_BY_THREE  (0.6667F)
#define One_By_Sixty  (0.016667F)

#define MF_PI 3.1416F
#define fre_PI MF_PI * 2.0F * 0.00005F
#define M_2PI (MF_PI * 2.0F)
#define M_PI2 (MF_PI * 0.5F)
#define M_PI4 (MF_PI * 0.25F)
#define M_PI3 (MF_PI / 3.0F)
#define PI_BY_30 (MF_PI / 30.0F)
#define ONE_BY_PI (0.31831F)
#define ONE_BY_2PI (0.159155F)

#define _IQ(A) (int32_t)((A) * 16777216.0F)
#define _IQ31(A) (int32_t)((A) * 2147483648.0F)
#define _IQmpy(A, B) (int32_t)(((int64_t)(A) * (B)) >> 24)
#define _IQtoF(A) ((float)(A) * (1.0F / 16777216.0F))
#define _IQ31toF(A) ((float)(A) * (1.0F / 2147483648.F))
#define _IQ31toF_FAST(A) ((float)(A) * (0.0000000004656612873F))


void cordic_sincos_calc(volatile float angle, volatile float *s, volatile float *c);
void truncateAngleTo2Pi( float * const angle);
void truncateAngleToOne( float * const angle);
float low_pass_filter(float input, float ALPHA);


#endif /* CORDIC_CALC */
