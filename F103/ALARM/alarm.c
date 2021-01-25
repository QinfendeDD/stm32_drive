#include "alarm.h"
#include "delay.h"
#include "timer_3.h"

/**********************************************************************************
** 函 数 名: Alarm_Init
** 功能描述: 声光报警IO初始化
** 输    入: 报警时间
** 输    出: 无
** 返 回 值: 无
** 日    期: 2019/5/13
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
** 函 数 名: Alarm_Start
** 功能描述: 烟雾报警
** 输    入: 报警时间
** 输    出: 无
** 返 回 值: 无
** 日    期: 2019/5/13
***********************************************************************************/
void Alarm_Start(int xs)
{

}


/**********************************************************************************
** 函 数 名: Alarm_Stop
** 功能描述: 烟雾报警
** 输    入: 报警时间
** 输    出: 无
** 返 回 值: 无
** 日    期: 2019/5/13
***********************************************************************************/
void Alarm_Stop(void)
{

}
