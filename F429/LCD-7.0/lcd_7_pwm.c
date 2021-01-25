/***
	***************************************************************************
	*	@file  	lcd_pwm.c
	*	@brief   定时器初始化，配置pwm,作为LCD的背光调节,若PWM背光引脚改变，除了需要修改相关的宏外，
	*				还需要把下面三个函数改成相应通道的函数 ：
	*				TIM_OC2Init();  
	*				TIM_OC2PreloadConfig();  
	*				TIM_SetCompare2(); 
   ******************************************************************************
   *  
	* 
	* 
	* 
	***************************************************************************
***/
#include "lcd_7_pwm.h"

static u16 LCD_PwmPeriod = 500;  //定时器重载值
static u32 LCD_PwmPrescaler = 90 - 1;  //定时器分频值

// 函数：TIM4 PWM CH2 IO口初始化
//
void  LCD_PWM_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//IO口时钟配置
	RCC_AHB1PeriphClockCmd (LCD_PWM_CLK, ENABLE); 

	//IO引脚复用
	GPIO_PinAFConfig(LCD_PWM_PORT,LCD_PWM_PinSource,LCD_PWM_AF);
	
	//IO配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //速度

	//初始化 TIM4_CH2 引脚
	GPIO_InitStructure.GPIO_Pin = LCD_PWM_PIN;	 
	GPIO_Init(LCD_PWM_PORT, &GPIO_InitStructure);	
													   
}

// 函数：LCD PWM 占空比设置
// 参数：pulse - 占空比，范围 0-100
//			
void  LCD_PwmSetPulse (u8 pulse)
{
	u16 compareValue ; 
	
	compareValue = pulse * LCD_PwmPeriod / 100; //根据占空比设置比较值
	TIM_SetCompare2(LCD_TIM,compareValue); 
}
// 函数：LCD PWM 初始化
//	参数：	pulse - LCD背光占空比
// 说明：LCD背光的PWM频率固定为10KHz
//
void	LCD_PWMinit(u8 pulse)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	LCD_PWM_GPIO (); //初始化IO口
	RCC_APB1PeriphClockCmd(LCD_TIM_CLK,ENABLE);  ///使能时钟
	
	//定时器基本设置
	TIM_TimeBaseInitStructure.TIM_Period = LCD_PwmPeriod; 	//重载值
	TIM_TimeBaseInitStructure.TIM_Prescaler= LCD_PwmPrescaler;  //分频系数
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

	TIM_TimeBaseInit(LCD_TIM,&TIM_TimeBaseInitStructure);//初始化

	//PWM输出配置
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //使能比较输出
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // 小于跳变值输出高电平
	
	TIM_OC2Init(LCD_TIM, &TIM_OCInitStructure);  //初始化定时器比较输出通道 2
	TIM_OC2PreloadConfig(LCD_TIM, TIM_OCPreload_Enable);  //自动重载比较输出通道 2 的值
	
	TIM_ARRPreloadConfig(LCD_TIM,ENABLE);//使能自动重载
  
	TIM_Cmd(TIM4,ENABLE); //使能定时器	
	
	LCD_PwmSetPulse (pulse); //设置占空比
}

