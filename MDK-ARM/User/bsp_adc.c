#include "bsp_adc.h"

adcData_obj     adcRawData_Handler;
adcConv_obj     adcConvData_Handler;


	
void ADC_Zero_Offset(adcData_obj * rawADC)
{
	  if(rawADC->Zero_Offset_Count <= 300)
    {
			rawADC->INA_A_SUM += ADC1->JDR1;
			rawADC->INA_B_SUM += ADC1->JDR2;
			rawADC->INA_C_SUM += ADC1->JDR3;
      rawADC->OPA_A_SUM += ADC2->JDR1;
			rawADC->OPA_B_SUM += ADC2->JDR2;
			rawADC->OPA_C_SUM += ADC2->JDR3;

      rawADC->Zero_Offset_Count++;
			
      if(rawADC->Zero_Offset_Count == 300)
      {
				rawADC->INA_A_Zero_Offset = rawADC->INA_A_SUM / 300;
				rawADC->INA_B_Zero_Offset = rawADC->INA_B_SUM / 300;
				rawADC->INA_C_Zero_Offset = rawADC->INA_C_SUM / 300;
				rawADC->OPA_A_Zero_Offset = (rawADC->OPA_A_SUM / 300) + 12;
				rawADC->OPA_B_Zero_Offset = (rawADC->OPA_B_SUM / 300) + 12;
				rawADC->OPA_C_Zero_Offset = (rawADC->OPA_C_SUM / 300) + 12;

				rawADC->INA_A_SUM = 0;
				rawADC->INA_B_SUM = 0;
				rawADC->INA_C_SUM = 0;
				rawADC->OPA_A_SUM = 0;
				rawADC->OPA_B_SUM = 0;
				rawADC->OPA_C_SUM = 0;
				
        rawADC->Zero_Offset_Count = 0;
				offsetFlag = 0;
      }
    }
}

void getPhaseCurrent(adcData_obj * rawADC, adcConv_obj * convCurrent)
{
	rawADC->phCur_OPA_A = ADC2->JDR1;
	rawADC->phCur_OPA_B = ADC2->JDR2;
	rawADC->phCur_OPA_C = ADC2->JDR3;
	rawADC->phCur_INA_A = ADC1->JDR1;
	rawADC->phCur_INA_B = ADC1->JDR2;
	rawADC->phCur_INA_C = ADC1->JDR3;

	convCurrent->currentOPA_A = (rawADC->phCur_OPA_A - rawADC->OPA_A_Zero_Offset) * ConvertFactor_OPA;
	convCurrent->currentOPA_B = (rawADC->phCur_OPA_B - rawADC->OPA_B_Zero_Offset) * ConvertFactor_OPA;
	convCurrent->currentOPA_C = (rawADC->phCur_OPA_C - rawADC->OPA_C_Zero_Offset) * ConvertFactor_OPA;
	
	convCurrent->currentINA_A = (rawADC->phCur_INA_A - rawADC->INA_A_Zero_Offset) * ConvertFactor_INA;
	convCurrent->currentINA_B = (rawADC->phCur_INA_B - rawADC->INA_B_Zero_Offset) * ConvertFactor_INA;
	convCurrent->currentINA_C = (rawADC->phCur_INA_C - rawADC->INA_C_Zero_Offset) * ConvertFactor_INA;
}

void currentReconstrution(SVM_obj * SVMhandler, adcData_obj * rawADC, adcConv_obj * convCurrent)
{
	if(SVMhandler->N == 3 || SVMhandler->N == 2)
	{
		rawADC->phCur_OPA_B = ADC2->JDR2;
		rawADC->phCur_OPA_C = ADC2->JDR3;
		
		convCurrent->currentOPA_B = (rawADC->phCur_OPA_B - rawADC->OPA_B_Zero_Offset) * ConvertFactor_OPA;
	  convCurrent->currentOPA_C = (rawADC->phCur_OPA_C - rawADC->OPA_C_Zero_Offset) * ConvertFactor_OPA;
		
		convCurrent->currentOPA_A = -convCurrent->currentOPA_B - convCurrent->currentOPA_C;
	}
	
	else if(SVMhandler->N == 1 || SVMhandler->N == 5)
	{
		rawADC->phCur_OPA_A = ADC2->JDR1;
		rawADC->phCur_OPA_C = ADC2->JDR3;
		
		convCurrent->currentOPA_A = (rawADC->phCur_OPA_A - rawADC->OPA_B_Zero_Offset) * ConvertFactor_OPA;
	  convCurrent->currentOPA_C = (rawADC->phCur_OPA_C - rawADC->OPA_C_Zero_Offset) * ConvertFactor_OPA;
		
		convCurrent->currentOPA_B = -convCurrent->currentOPA_A - convCurrent->currentOPA_C;
	}
	
	else if(SVMhandler->N == 4 || SVMhandler->N == 6)
	{
		rawADC->phCur_OPA_A = ADC2->JDR1;
		rawADC->phCur_OPA_B = ADC2->JDR2;
		
		convCurrent->currentOPA_A = (rawADC->phCur_OPA_A - rawADC->OPA_B_Zero_Offset) * ConvertFactor_OPA;
	  convCurrent->currentOPA_B = (rawADC->phCur_OPA_B - rawADC->OPA_B_Zero_Offset) * ConvertFactor_OPA;
		
		convCurrent->currentOPA_C = -convCurrent->currentOPA_B - convCurrent->currentOPA_A;
	}
}


