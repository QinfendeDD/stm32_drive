#include "MQ7.h"
#include "adc_1.h"
#include "stdio.h"

u16 adc1_ch1_value;
float mq7_percent=0.0;
int coClass = 0;

void MQ7_Init(void)
{
	Adc_1_Init(); 
}

void Get_MQ7_Percent(void)
{
	adc1_ch1_value = Get_Adc_Average(ADC_Channel_1,10); //�õ��ɼ���ƽ��ֵ(����ֵ)
	mq7_percent = (float)adc1_ch1_value*(3.3/4096); //ת��Ϊ��ѹֵ
	mq7_percent = 300*mq7_percent + 10; //ת��ΪŨ��ֵ

	//printf("MQ7: %4.1f\r\n",mq7_percent);
	if(mq7_percent>700 && mq7_percent<=1000) coClass = 1;  //һ��
	else if(mq7_percent>400 && mq7_percent<=700) coClass = 2;//����
	else if(mq7_percent>200 && mq7_percent<=400) coClass = 3;//����
	else if(mq7_percent>150 && mq7_percent<=200) coClass = 4;//�ļ�
	else coClass = 0;
}

