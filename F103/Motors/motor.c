#include "motor.h"

/************PWM占空比1,2,3,4通道值************/
uint16_t Channel1Pulse=0;
uint16_t Channel2Pulse=0;
uint16_t Channel3Pulse=0; 
uint16_t Channel4Pulse=0;

GPIO_InitTypeDef  GPIO_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
TIM_OCInitTypeDef  TIM_OCInitStructure; 
TIM_BDTRInitTypeDef TIM_BDTRInitStructure; 
/*************************************************************************
 函数: void Motor_Init(void)
 描述: TIM1高级定时器PWM输出基本配置
 参数: 无
 返回: 无
 备注: 
 版本:
*************************************************************************/
void Motor_Init(void)
{ 
	uint16_t TimerPeriod = 0; 
	
	/*DRV8833驱动端口配置---nSLEEP端口配置*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
  

/***********电机驱动PWM信号配置---TIM1高级定时器PWM输出基本配*************/
	/* Private function prototypes -----------------------------------------------*/ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
	/* Compute the value to be set in ARR register to generate signal frequency at 17.57 Khz */
	TimerPeriod = (SystemCoreClock / 10000) - 1; 	   /*PWM输出平率为1K*/
	/* Compute CCR1 value to generate a duty cycle at 50% for channel 1 */ 
	/*          计算周期 时间的方法  
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //设置用来作为TIM 时钟频率除数的预分频值
	那么TIM时钟就为72M  1/72MHZ=0.01388...us           这就是 系统时间
	TimerPeriod = (SystemCoreClock / 17570 ) - 1;          //计算中断周期值
	(72MHz/17570) -1=4096.89..   中断周期值
	4096.89*0.01388=56.8US  这就是实际PWM中断周期时间   
	1/56.8US=17.6KHZ   1除以除以周期时间 就是频率 大约17.6KHz左右。
	各个通道值计算结果。。 
	Channel1Pulse = 5*（4096-1）/10 =2047       2047*0.01388=28.4US
	Channel2Pulse =  25 * (4096 - 1)) / 100=1023   1023*0.01388=14.19US
	Channel3Pulse =  125 *(4096 - 1)) / 1000=511   511*0.01388=7.09US
	*/
	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 9;                        /*设置用来作为TIM 时钟频率除数的预分频值*/ 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     /*计数器模式  向上计数模式 */ 
	TIM_TimeBaseStructure.TIM_Period = TimerPeriod;        
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    /*时钟分割   采样分频 */
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                /*设置 周期 计数值*/ 
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	/* Channel 1, 2,3 and 4 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;                /*脉冲宽度调制模式2*/ 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    /*使能输出比较状态*/
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;  /*使能互补输出状态*/ 
	TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;                   /*脉冲值*/ 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;        /*输出比较极性低*/
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;       /*互补 输出极性高*/ 
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;       /*MOE=0 设置 TIM1输出比较空闲状态*/ 
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;    /*MOE=0 重置 TIM1输出比较空闲状态*/ 
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);                         /*设定好的参数 初始化TIM*/   
	TIM_OCInitStructure.TIM_Pulse = Channel2Pulse; 
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = Channel3Pulse; 
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	/*设置刹车特性  死区时间  锁电平 OSSI OSSR 状态 AOE(自动输出使能)*/ 
	/* Automatic Output enable, Break, dead time and lock configuration*/
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;       /*设置在运行模式下 非工作状态选项 OSSR*/ 
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;       /*设置在运行模式下 非工作状态选项 OSSI*/ 
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;            /*使用锁电平1*/
	TIM_BDTRInitStructure.TIM_DeadTime = 22;                          /*指定了输出和打开 关闭状态之间的延时 约 1US*/  
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;               /*使能刹车*/ 
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High; /*刹车输入管脚极性高*/
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;/*自动输出功能使能*/
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);                      /*初始化设定好的参数*/
	/* TIM1 counter enable */
	TIM_Cmd(TIM1, ENABLE);
	/* Main Output Enable */
	TIM_CtrlPWMOutputs(TIM1, ENABLE); 
	
  
/*******************************初始电机状态设置************************************/	
	//关闭电机驱动芯片休眠状态
	nSLEEP_Motor(OFF);
	//初始电机速度为0
	Speed_Motor(0,0);
}
/*************************************************************************
 函数: void nSLEEP_Motor(unsigned char i)
 描述: DRV8833 nSLEEP使能
 参数: i  : ON--关闭，OFF--开启
 返回: 无
 备注: 
 版本：
*************************************************************************/
void nSLEEP_Motor(unsigned char i)
{
	if(i == 1)
	{
		GPIO_SetBits(GPIOD,GPIO_Pin_2);
	}
	else if(i == 0)
	{
		GPIO_ResetBits(GPIOD,GPIO_Pin_2);
	}
	else
	{
		;
	}
}
	
/*************************************************************************
 函数: void Speed_Motor(int Speed_Motor_Left_Value,int Speed_Motor_Right_Value)
 描述: 电流电机调速
 参数: Speed_Motor_Left_Value，Speed_Motor_Right_Value
 返回: 无
 备注: Speed_Motor_Left_Value输入范围（-100~100）
       Speed_Motor_Right_Value输入范围（-100~100）
 版本:
*************************************************************************/
void Speed_Motor(int Speed_Motor_Left_Value,int Speed_Motor_Right_Value)
{
  int Speed_Motor_Left = 0;
	int Speed_Motor_Right = 0;
	Channel1Pulse = 0;
	Channel2Pulse = 0;
	Channel3Pulse = 0; 
	Channel4Pulse = 0;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	Speed_Motor_Left  = Speed_Motor_Right_Value * 100;
	Speed_Motor_Right = Speed_Motor_Left_Value  * 100;
	
	/************DRV8833驱动端口************/
	/*
			PA8 ---AIN1	 PA9 ---AIN2
			PA10---BIN1	 PA11---BIN2
	*/
/***********DRV8833四组电机控制信号在对应模式下控制电机正反装配置时钟选择*****************
    通过下面条件语句判断电机运行状态（正、反。。。），在相应的状态下配置相应信号IO端口********/
	if((Speed_Motor_Left > 0) &&(Speed_Motor_Right > 0))		  //正向
	{
		/*
		 PA8->PWM,PA9->0
		 PA10->PWM,PA11->0
		 */
		/******LEFT  PA8->PWM,PA9->0******/	
		Channel1Pulse = Speed_Motor_Left;
		Channel3Pulse = Speed_Motor_Right;
		/************PA8->PWM************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/************PA9->0************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_9);

	   /******RIGHT  PA10->PWM,PA11->0******/	
	   /*************PA10->PWM**************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/************PA11->0************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	}
	else if((Speed_Motor_Left > 0) &&(Speed_Motor_Right < 0))
	{
		/*
		 PA8->PWM,PA9->0
		 PA10->0,PA11->PWM
		 */
		Channel1Pulse = Speed_Motor_Left;
		Channel4Pulse = - (Speed_Motor_Right);
		/************PA8->PWM,PA9->0************/		
		/****************PA8->PWM***************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/****************PA9->0****************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_9);
	   /************RIGHT  PA10->0,PA11->PWM************/	
	   /******************PA10->PWM*********************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/**********************PA11->0******************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_10);
	}
	else if((Speed_Motor_Left > 0) &&(Speed_Motor_Right == 0))
	{
		/*
		 PA8->PWM,PA9->0
		 PA10->1,PA11->1
		*/
		Channel1Pulse = Speed_Motor_Left;
		/************PA8->PWM,PA9->0************/		
		/***************PA8->PWM****************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/****************PA9->0****************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_9);
	   /********RIGHT  PA10->1,PA11->1********/	
	   /*****************PA10->1**************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/****************PA11->1**************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_SetBits(GPIOA,GPIO_Pin_10);
		GPIO_SetBits(GPIOA,GPIO_Pin_11);	
	}
	else if((Speed_Motor_Left < 0) &&(Speed_Motor_Right > 0))	  //反向
	{
			/*
		 PA8->0,PA9->PWM
		 PA10->PWM,PA11->0
		 */
		/************LEFT  PA8->0,PA9->PWM************/	
		Channel2Pulse = -(Speed_Motor_Left);
		Channel3Pulse = Speed_Motor_Right;
		/*******************PA8->0******************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/*****************PA9->PWM*****************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);

	   /**********RIGHT  PA10->PWM,PA11->0*********/	
	   /***************PA10->PWM******************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/****************PA11->0*****************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_11);	
	}
	else if((Speed_Motor_Left < 0) &&(Speed_Motor_Right < 0))
	{
		/*
		 PA8->0,PA9->PWM
		 PA10->0,PA11->PWM
		 */
		/************LEFT  PA8->0,PA9->PWM************/	
		Channel2Pulse = -(Speed_Motor_Left);
		Channel4Pulse = -(Speed_Motor_Right);
		/****************PA9->PWM*********************/ 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/****************PA8->0**********************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);

	   /***********RIGHT  PA10->0,PA11->PWM***********/	
	   /*******************PA11->PWM******************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/******************PA10->0*******************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_10);
	}
	else if((Speed_Motor_Left < 0) &&(Speed_Motor_Right == 0))
	{
		/*
		 PA8->0,PA9->PWM
		 PA10->1,PA11->1
		 */
		/************LEFT  PA8->0,PA9->PWM************/	
		Channel2Pulse = -(Speed_Motor_Left);
		/*******************PA9->PWM*****************/ 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/******************PA8->0*******************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);

	   /************RIGHT  PA10->1,PA11->1************/	
	   /*******************PA11->1********************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/********************PA10->1*****************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_SetBits(GPIOA,GPIO_Pin_10);
		GPIO_SetBits(GPIOA,GPIO_Pin_11);
	}
	else if((Speed_Motor_Left == 0) &&(Speed_Motor_Right > 0))
	{
		  	/*
		 PA8->1,PA9->1
		 PA10->PWM,PA11->0
		 */
		/************LEFT  PA8->0,PA9->PWM************/	
		Channel3Pulse = Speed_Motor_Right;
		/********************PA9->1*******************/ 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/*******************PA8->1********************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
		GPIO_SetBits(GPIOA,GPIO_Pin_9);

	   /************RIGHT  PA10->PWM,PA11->0************/	
	   /*******************PA10->PWM********************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/*******************PA11->1*********************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	}
	else if((Speed_Motor_Left == 0) &&(Speed_Motor_Right < 0))
	{
		/*
		 PA8->1,PA9->1
		 PA10->0,PA11->PWM
		 */
		/************LEFT  PA8->1,PA9->1************/	
		Channel4Pulse = -(Speed_Motor_Right);
		/*****************PA9->1*******************/ 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		/******************PA8->1*****************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
		GPIO_SetBits(GPIOA,GPIO_Pin_9);

	   /************RIGHT  PA10->0,PA11->PWM************/	
	   /********************PA10->0*********************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA,GPIO_Pin_10);
		/*******************PA11->PWM******************/
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode =GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);		
	}
	else if((Speed_Motor_Left == 0) &&(Speed_Motor_Right == 0))
	{
		/*
		 PA8->1,PA9->1
		 PA10->,PA11->1
		 */
		/************LEFT  PA8->1,PA9->1************/	
		/*******************PA9->1******************/ 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);	
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
		GPIO_SetBits(GPIOA,GPIO_Pin_9);
		GPIO_SetBits(GPIOA,GPIO_Pin_10);
		GPIO_SetBits(GPIOA,GPIO_Pin_11);
	}

/**************在这四组信号配置完毕后，对电机运行速度进行配置************************
	         即电机运行速度（PWM占空比）的配置******************************************/	
	TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;         /*脉冲 值*/ 
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);               /*设定好的参数 初始化TIM*/   
	TIM_OCInitStructure.TIM_Pulse = Channel2Pulse; 
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = Channel3Pulse; 
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
}
 


 

