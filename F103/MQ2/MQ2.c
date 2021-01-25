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
	adc1_ch0_value = Get_Adc_Average(ADC_Channel_0,10); //�õ��ɼ���ƽ��ֵ(����ֵ)
	//mq2_percent = adc1_ch0_value/4095.0*100;//�õ��ٷֱ�
	mq2_percent = (float)adc1_ch0_value*(3.3/4096); //ת��Ϊ��ѹֵ
	mq2_percent = 2940*mq2_percent + 300; //ת��ΪŨ��ֵ
	//printf("MQ2: %4.1f\r\n",mq2_percent);
	if(mq2_percent>6000) smogClass = 1;  //һ��
	else if(mq2_percent>5500 && mq2_percent<=6000) smogClass = 2;//����
	else if(mq2_percent>5100 && mq2_percent<=5500) smogClass = 3;//����
	else if(mq2_percent>2000 && mq2_percent<=5100) smogClass = 4;//�ļ�
	else smogClass = 0;
}

