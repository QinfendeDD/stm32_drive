#include "FIRE.h"

u8  fireSta = 0;  //火焰状态

void FIRE_Init(void)
{
//初始化PC10为下拉输入		    
//有火焰时输出高电平
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOC时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//输入下拉模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO	
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
