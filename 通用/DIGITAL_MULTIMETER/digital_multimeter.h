#ifndef __DIGITAL_MULTIMETER_H
#define __DIGITAL_MULTIMETER_H
#include "sys.h"

#define PA2_B PAout(2)	// PA2_B
#define PA3_A PAout(3)	// PA3_A

void DM_Init(void);//≥ı ºªØ		

void DM_Adc_Init(void);
u16 DM_Get_Adc(u8 ch);
u16 DM_GetAdc_Average(u8 ch,u8 times);

void DM_BackGround(void);
void DM_Run(void);
u8 dm_get_keynum(void);
void Voltage_Measure(void);
void I_Measure(void);
void Cap_Measure(void);
void Res_Measure(void);
void Indutor_Measure(void);

#endif
