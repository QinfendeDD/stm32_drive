#include "FIRE.h"

u8  fireSta = 0;  //����״̬

void FIRE_Init(void)
{
//��ʼ��PC10Ϊ��������		    
//�л���ʱ����ߵ�ƽ
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ��GPIOCʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//��������ģʽ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIO	
}

void FIRE_getSTA(void)
{
	if(Fire_Sta)
	{
		fireSta = 0;
	}
	else
	{
		fireSta = 1;
	}
}
