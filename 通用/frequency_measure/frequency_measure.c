//#include "frequency_measure.h"
//#include "mymath.h"
//#include "math.h"

//measure_data measure ;
//u16 rising;
//u16 falling;
//u16 rising_last;
//float frequency,IC2Value,DutyCycle,Frequency;
//float  AC_sample[130];
//u8 sample_done;

///*TIM2_CH1->PA5*/
///*TIM2_CH2->PB3*/
///*若想改变测量的PWM频率范围，可将TIM时钟频率做分频处理，即改变arr和psc这两个函数参数*/
//void TIM3_CH1_CH2_Cap_Init(u32 arr,u16 psc)				//频率捕获
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseSttructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	TIM_ICInitTypeDef TIM_ICInitStructure;
//	
//	TIM_DeInit(TIM3);
//	  /* TIM3 clock enable */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	//TIM3使能
//	/* GPIOA clock enable */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//GPIOA GPIOB 时钟开启

//	/* TIM3 channel 2 pin (PA.07) configuration */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                               //GPIO配置
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	
//	 /* Enable the TIM3 global Interrupt NVIC配置 */
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	
//	TIM_TimeBaseSttructure.TIM_Prescaler = psc;  //定时器分频
//	TIM_TimeBaseSttructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
//	TIM_TimeBaseSttructure.TIM_Period  = arr;//自动重装初值
//	TIM_TimeBaseSttructure.TIM_ClockDivision  = TIM_CKD_DIV1;
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseSttructure);		//初始化TIM3的输入捕获参数
//	
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1 ;//选择输入端IC1映射到TI1上
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//上升沿捕获
//	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//映射到TI1上//管脚与寄存器对应关系
//	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; /*配置输入预分频，不分频，控制在多少个输入周期做一次捕获
//	输入的信号频率没有变，测得的周期也不会变。比如选择4分频，则每四个输入周期才做一次捕获，这样在输入信号变化不频繁的情况下
//	，可以减少软件被不断中断的次数*/
//	
//	TIM_ICInitStructure.TIM_ICFilter = 0x00;//ICF=0000  配置输入滤波器 不滤波
//	/*	滤波设置，经历几个周期跳变认定波形稳定0x0~0xF*/
//	
//  TIM_ICInit(TIM3,&TIM_ICInitStructure);

//  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;//CC1S=01 选择输入端IC2s映射到TI1上
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;//下降沿捕获
//	TIM_ICInit(TIM3,&TIM_ICInitStructure);
//	
//	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2,ENABLE);//允许中断更新，允许CC1IE捕获中断
//	TIM_Cmd(TIM3,ENABLE);//启动TIM3
//	
//	
//	
//}

//void frequency_measure_start_stop(u8 num)
//{
////	switch (num)
////	{
////		case 0:
////			TIM_Cmd(TIM3,DISABLE);//关闭定时器
////		break;
////		
////		case 1:
////			TIM_Cmd(TIM3,ENABLE);//开启定时器
////		break;
////		
////		default:break;
////	}
//		measure.frequency = safe_div(1000000.0f,rising,0);
//		measure.T = rising/1000000.0f;
//}

//void AC_Vlotage_Measure(float *sample_data)
//{
//	float h;
//	float F;
//	float U1 = 0;
//	float U2 = 0;
//	u8 i;
//	float *p;
//	p = sample_data+64;
//	frequency_measure_start_stop(1);
//	AC_sample_time_set();
//	while(!sample_done);
//		h = measure.T/sample_time;
//		for(i=1;i<64;i+=2)
//		{
//			U1 += pow(*(p+i),2);
//			
//			U2 += pow(*(p+i+1),2);
//		}
//// 		AC_sample[64];
//		F = h/3*((4*U1+2*U2)-pow((*(p+64)),2)+ pow(* p,2));
//		measure.AC_voltage = sqrt(F/measure.T);
//		sample_done = 0;
//		U1 = U2 = 0;
//		
//}


//	

//void AC_sample_time_set(void)
//{
//	float t;
//	u16 n;
//	t = measure.T/64.0f;
//	n=(u16)(t*1000000);
//	if(n>200)
//	{
//		TIM7->ARR = n;
//	}
//	else
//	{
//		TIM7->ARR = 200;
//	}
//}

//void TIM3_IRQHandler(void)
//{
//TIM_ClearITPendingBit(TIM3, TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2); //清除TIM的中断待处理位

//IC2Value = TIM_GetCapture2(TIM3); //读取IC2捕获寄存器的值，即为PWM周期的计数值
//if (IC2Value != 0)
//{
//DutyCycle = (TIM_GetCapture1(TIM3) * 100) / IC2Value; //读取IC1捕获寄存器的值，并计算占空比

//Frequency = 72000000 / IC2Value; //计算PWM频率
//}
//else
//{
//DutyCycle = 0;
//Frequency = 0;
//}
//}
