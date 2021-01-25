/**
  * @attention
  * 采用高级定时器TIM1，输出带有死区时间的PWM波形，占空比不能改变，要求：死区时间为1us，占空比为50%，CH1，CH2之间的相位差为2ms，周期为8ms
  * 实验平台:STM32F103ZET6开发板 
  *
  ******************************************************************************
  */ 

#include "pwm.h"
#include "led.h"

void TIM_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
	TIM_OCInitTypeDef  TIM_OCInitStructure;  
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;  
	
    /* IO管脚定义设置 */  
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB , ENABLE);  //使能GPIO外设时钟使能
  
    /* TIM1 clock enable */  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);// 
  
    /*  管脚设配置*/  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //复用满负载输出为3.3V 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  

	/* TIM时钟配置*/	
	TIM_TimeBaseStructure.TIM_Prescaler =71;		//预分频
	TIM_TimeBaseStructure.TIM_Period =3999;	//0~65535,周期8ms
	TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision =0;	//采样分频
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;	//重复预分频，更新PWM占空比
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);		//	初始化时钟
	
	
	/* PWM配置*/
	TIM_OCInitStructure.TIM_OCMode =TIM_OCMode_Toggle;	//输出比较模式，只在于比较寄存器CCR值相同时电平才翻转（周期结束时也不翻转），否则不翻转。
	/*	
			PWM模式1－	在向上计数时，一旦TIMx_CNT<TIMx_CCR1时通道1为有效电平，否则为无效电平；
									在向下计数时，一旦TIMx_CNT>TIMx_CCR1时通道1为无效电平(OC1REF=0)，否则为有效电平(OC1REF=1)。
			PWM模式2－ 	在向上计数时，一旦TIMx_CNT<TIMx_CCR1时通道1为无效电平，否则为有效电平；
									在向下计数时，一旦TIMx_CNT>TIMx_CCR1时通道1为有效电平，否则为无效电平。
	*/
	TIM_OCInitStructure.TIM_OutputState =TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;	
	TIM_OCInitStructure.TIM_Pulse =10;			// 此处直接赋值设置CCR值

	/*互补配置*/
	TIM_OCInitStructure.TIM_OutputNState =TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity =TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState =TIM_OCIdleState_Reset;		//刹车之后的输出状态
	TIM_OCInitStructure.TIM_OCNIdleState =TIM_OCNIdleState_Reset;	//刹车之后的互补输出状态
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_Pulse = 2010;
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);	
	
	/* 死区配置*/
	TIM_BDTRInitStructure.TIM_OSSRState =TIM_OSSRState_Enable;		//运行模式下输出
	TIM_BDTRInitStructure.TIM_OSSIState =TIM_OSSIState_Enable;		//空闲模式下输出
	TIM_BDTRInitStructure.TIM_LOCKLevel =TIM_LOCKLevel_OFF;			//锁定级别
	TIM_BDTRInitStructure.TIM_DeadTime =0x01;						//设置死区时间 0-0xff；1us
	/*		
							
		例如  10us   应该在第四区域
		死区时间 =1/72*1000*(32+DTG)*16=10us;		(72Mhz)
		即：   13.89*(32+DTG)*16 =10000;
		DTG =12.99  ~  13；
		DTG[4:0]=13    对应16进制    D
		因为是第四区域 DTG[7:5]=111   对应16进制 E
		所以DTG = DTG[7:5]  +DTG[4:0] =0x0ED;
		即TIM_BDTRInitStructure.TIM_DeadTime =0x0ED;
	*/
	
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;  //刹车使能关闭
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;  //刹车输出极性，即刹车控制引脚PB12接GND时，PWM停止输出  
	TIM_BDTRInitStructure.TIM_AutomaticOutput =TIM_AutomaticOutput_Enable; //自动输出使能
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
	
	/*  刹车控制引脚为TIM1_BKIN pin(PB.12)，将PB12接GND，channel和其互补通道，都变为刹车后的电平，具体为0还是1，要看之前互补配置时 对下列2个参数的设置： 
			.TIM_OCIdleState = TIM_OCIdleState_Reset;   //刹车之后，PWM通道变为0 
			.TIM_OCNIdleState = TIM_OCNIdleState_Reset; //刹车之后，PWM互补通道变为0        
			注意：如果没必要，还是不要开启刹车功能，因为会对PWM产生影响，特别是当PB12悬空时，波形将会有很大的波动。 
			这里不打开刹车功能，即.TIM_Break = TIM_Break_Disable; 
	*/  

	/*开启PWM输出*/
	TIM_CtrlPWMOutputs(TIM1,ENABLE);	//PWM输出开启
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1预装载使能
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1预装载使能
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器  影子寄存器 启用
	
	TIM_Cmd(TIM1,ENABLE);
	

}
