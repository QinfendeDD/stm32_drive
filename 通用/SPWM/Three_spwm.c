#include "Three_spwm.h"
#include "sys.h"

u16 DutyFactor1 = 50;
void TIM1_INIT_Config()
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		TIM_TimeBaseInitTypeDef TIM_TimBaseStructure;
		TIM_OCInitTypeDef TIM_OCInitStructure;
		TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
		
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_TIM1 | 
		RCC_APB2Periph_AFIO,ENABLE);
		
		GPIO_PinRemapConfig(GPIO_FullRemap_TIM1 , ENABLE);
		TIM_DeInit(TIM1); 
		TIM_InternalClockConfig(TIM1);
		
		GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|
		GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
		GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
		GPIO_Init(GPIOE,&GPIO_InitStructure);
		
		TIM_TimBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
		TIM_TimBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
		TIM_TimBaseStructure.TIM_Period= 4000;
		TIM_TimBaseStructure.TIM_Prescaler=0;
		TIM_TimBaseStructure.TIM_RepetitionCounter=0;
		TIM_TimeBaseInit(TIM1, &TIM_TimBaseStructure);
		
		TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;
		TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
		TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;
		TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCPolarity=TIM_OCNPolarity_High;
		TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_Pulse=DutyFactor1 *2813 / 100;
		TIM_OC1Init(TIM1,&TIM_OCInitStructure);
    TIM_OC2Init(TIM1,&TIM_OCInitStructure);
		TIM_OC3Init(TIM1,&TIM_OCInitStructure);
		
		TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;
    TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;
    TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF; 
    TIM_BDTRInitStructure.TIM_DeadTime = 36; 	//死区时间
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable; //刹车功能使能
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;	//刹车输入极性
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);  

    //ccr1自动重装载
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  
    TIM_ARRPreloadConfig(TIM1, ENABLE);
		TIM_Cmd(TIM1,ENABLE); 
     //高级定时器才有的主输出使能，不开启不能用;
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

	}

//TIMx PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM1_PWM_Init_config(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;	
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);  //使能GPIOE时钟时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //使能AFIO时钟
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//关闭jtag 开启sw
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);//映射为TIMx
	
	//设置该引脚为复用输出功能,输出TIMx_CHx的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14; //TIMx_CHx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
 
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	  TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;
		TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;
		TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;
		TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCPolarity=TIM_OCNPolarity_High;
		TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse=DutyFactor1 *2813 / 100;;
	//根据TIM_OCInitStruct中指定的参数初始化外设TIMx
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);	
	 TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
   TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
   TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
   TIM_BDTRInitStructure.TIM_DeadTime = 780;//死区时间为 12/SYSTEMCLK (ns)
   TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;//关闭外部break功能
   TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
   TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
   TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
		
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE 主输出使能	
	//TIM_ARRPreloadConfig(TIM1, ENABLE); //使能TIMx在ARR上的预装载寄存器
	TIM_Cmd(TIM1, ENABLE);  //使能TIMx

}


