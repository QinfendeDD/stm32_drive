#include "MQ2.h"
#include "adc_1.h"
#include "stdio.h"

u16 adc1_ch0_value;
float mq2_percent=0.0;
int smogClass = 0;

void MQ2_Init(void)
{
	Adc_1_Init(); 
}

void Get_MQ2_Percent(void)
{
	adc1_ch0_value = Get_Adc_Average(ADC_Channel_0,10); //得到采集的平均值(最终值)
	//mq2_percent = adc1_ch0_value/4095.0*100;//得到百分比
	mq2_percent = (float)adc1_ch0_value*(3.3/4096); //转换为电压值
	mq2_percent = 2940*mq2_percent + 300; //转换为浓度值
	//printf("MQ2: %4.1f\r\n",mq2_percent);
	if(mq2_percent>6000) smogClass = 1;  //一级
	else if(mq2_percent>5500 && mq2_percent<=6000) smogClass = 2;//二级
	else if(mq2_percent>5100 && mq2_percent<=5500) smogClass = 3;//三级
	else if(mq2_percent>2000 && mq2_percent<=5100) smogClass = 4;//四级
	else smogClass = 0;
}

