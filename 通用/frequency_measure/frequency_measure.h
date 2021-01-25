#ifndef FREQUENCY_MEASURE_H
#define FREQUENCY_MEASURE_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "stm32f10x_rcc.h"
#include "mymath.h"
#define sample_time 64  //һ�����ڲ���64��






extern	 u16 rising;
extern	 u16 falling;
extern	 u16 rising_last;
extern   float frequency;

typedef struct
{
	float AC_voltage;
	float DC_voltage;
	float AC_current;
	float DC_current;
	float frequency;
	float phase;
	float T;
}measure_data;

extern u8 sample_done;			//������ɱ�־λ
extern float  AC_sample[130];	//AC�����㴢��

extern float frequency;
extern measure_data measure ;


void TIM3_CH1_CH2_Cap_Init(u32 arr,u16 psc);
void AC_Vlotage_Measure(float *sample_data);
void frequency_measure_start_stop(u8 num);
void AC_sample_time_set(void);

#endif
