#include "PWM.h"

void SetPWMFreq()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE); 	//IO口时钟配置

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;  		// 复用模式 	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  	// 推挽
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		// 上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //	速度等级
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	 
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);								// 初始化 TIM2_CH1 引脚
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2);		// 设置复用
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//定时器基本设置
	TIM_TimeBaseInitStructure.TIM_Period 			= 10-1; 								// 重载值
	TIM_TimeBaseInitStructure.TIM_Prescaler		    = 9 - 1;  // 分频系数
	TIM_TimeBaseInitStructure.TIM_CounterMode		= TIM_CounterMode_Up; 				// 向上计数
	TIM_TimeBaseInitStructure.TIM_ClockDivision	    = TIM_CKD_DIV1; 						// 数字滤波采样时钟分频设置
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);					// 初始化TIM2基本设置

	//PWM输出配置
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_PWM1;			// PWM模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//	使能比较输出
	TIM_OCInitStructure.TIM_OCPolarity 	= TIM_OCPolarity_High;		// 小于跳变值输出高电平
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  			            //初始化定时器比较输出通道 1
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);               //自动重载比较输出通道 1 的值
	
	TIM_ARRPreloadConfig(TIM2,ENABLE);	//	使能自动重载
	TIM_Cmd(TIM2,ENABLE); 
	
	TIM_SetCompare1(TIM2,5);
}
