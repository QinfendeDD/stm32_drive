#include "us-100.h"

u32 count = 0;
u32 Distance = 0;
//初始化
void Ultrasonic_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//Trig
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//_PP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//Echo
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//_FLOATING;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
//	trig=0;
	GPIO_ResetBits(GPIOE, GPIO_Pin_0);//预先拉低Trig引脚
}

//以下是定时器2初始化设置
//TIM1、TIM8~TIM11的时钟为APB2时钟的两倍即168M，TIM2~TIM7、TIM12~TIM14的时钟为APB1的时钟的两倍即84M。
void Timer2_INIT(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
  TIM_TimeBaseInitStructure.TIM_Prescaler = 840-1;//定时器分频
  TIM_TimeBaseInitStructure.TIM_Period = 499999;//自动重装载值
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);//初始化TIM3
	
  TIM_ClearFlag(TIM2, TIM_FLAG_Update); //允许定时器3更新中断
}
//以下为超声波测量的数据
u32 Ultrasonic(void)
{
	uint32_t Distance = 0;
	GPIO_SetBits(GPIOE, GPIO_Pin_0);
	delay_us(20);
	GPIO_ResetBits(GPIOE, GPIO_Pin_0);//发出10us的脉冲//拉低
	TIM2->CNT = 0;
	while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1) == 0);
	TIM_Cmd(TIM2, ENABLE);
	while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1) == 1);
	TIM_Cmd(TIM2, DISABLE);
	count = TIM2->CNT;
	//(高电平时间*340m/s)/2
	Distance =(count*17);
	return Distance;
//	delay_ms(500);
}
