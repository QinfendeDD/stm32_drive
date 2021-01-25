#ifndef __ADC_H_
#define __ADC_H_	

#include "sys.h"


void Adc_1_Init(void);
u16  Get_Adc(u8 ch); 
u16 Get_Adc_Average(u8 ch,u8 times); 
 
#endif 
