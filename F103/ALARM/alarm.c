#include "alarm.h"
#include "delay.h"
#include "timer_3.h"

/**********************************************************************************
** �� �� ��: Alarm_Init
** ��������: ���ⱨ��IO��ʼ��
** ��    ��: ����ʱ��
** ��    ��: ��
** �� �� ֵ: ��
** ��    ��: 2019/5/13
***********************************************************************************/
void Alarm_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(ALARM_RCC_BEEP, ENABLE);	 
	
	GPIO_InitStructure.GPIO_Pin = ALARM_GPIO_Pin_BEEP;	//PC1			 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(ALARM_GPIO_BEEP, &GPIO_InitStructure);	
	GPIO_ResetBits(ALARM_GPIO_BEEP,ALARM_GPIO_Pin_BEEP);						    

}


/**********************************************************************************
** �� �� ��: Alarm_Start
** ��������: ������
** ��    ��: ����ʱ��
** ��    ��: ��
** �� �� ֵ: ��
** ��    ��: 2019/5/13
***********************************************************************************/
void Alarm_Start(int xs)
{

}


/**********************************************************************************
** �� �� ��: Alarm_Stop
** ��������: ������
** ��    ��: ����ʱ��
** ��    ��: ��
** �� �� ֵ: ��
** ��    ��: 2019/5/13
***********************************************************************************/
void Alarm_Stop(void)
{

}
