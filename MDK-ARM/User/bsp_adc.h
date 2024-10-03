#ifndef _BSP_ADC
#define _BSP_ADC

#include "main.h"
#include "upload.h"
#include "mc_lib.h"

#define One_By_HalfADC  0.00048828125F // (1 / 2048)
#define One_By_MaxADC  0.000244141F // (1 / 4096)

#define  OP_GAIN          20.0F
#define  Shunt_Resistor   0.005F
#define  ConvertFactor_OPA    (3.3F * One_By_MaxADC / OP_GAIN / Shunt_Resistor) * 1.02F
#define  ConvertFactor_INA    (3.3F * One_By_MaxADC / OP_GAIN / Shunt_Resistor) * 1.1F

extern int offsetFlag;

typedef struct _adcData_obj_
{
	int32_t Zero_Offset_Count;
	
	int32_t OPA_A_SUM;
	int32_t OPA_B_SUM;
	int32_t OPA_C_SUM;
	int32_t INA_A_SUM;
	int32_t INA_B_SUM;
	int32_t INA_C_SUM;
	
	int32_t OPA_A_Zero_Offset;
	int32_t OPA_B_Zero_Offset;
	int32_t OPA_C_Zero_Offset;
	int32_t INA_A_Zero_Offset;
	int32_t INA_B_Zero_Offset;
	int32_t INA_C_Zero_Offset;
	
	int32_t phCur_OPA_A;
  int32_t phCur_OPA_B;
  int32_t phCur_OPA_C;
	
	int32_t phCur_INA_A;
  int32_t phCur_INA_B;
  int32_t phCur_INA_C;
	
  uint32_t volt_U;
  uint32_t volt_V;
  uint32_t volt_W;
  uint32_t Volt_Bus;
	
	uint32_t volt_Adjust;
} adcData_obj;

typedef struct _adcConv_obj_
{
	
	float currentINA_A;
  float currentINA_B;
  float currentINA_C;
	
	float currentOPA_A;
  float currentOPA_B;
  float currentOPA_C;
	
  float voltage_U;
  float voltage_V;
  float voltage_W;
  float voltage_Bus;
	
	float voltage_Adjust;
} adcConv_obj;

extern adcData_obj     adcRawData_Handler;
extern adcConv_obj     adcConvData_Handler;

void ADC_Zero_Offset(adcData_obj * rawADC);
void getPhaseCurrent(adcData_obj * rawADC, adcConv_obj * convCurrent);
void currentReconstrution(SVM_obj * SVMhandler, adcData_obj * rawADC, adcConv_obj * convCurrent);

#endif

