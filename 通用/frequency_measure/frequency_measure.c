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
///*����ı������PWMƵ�ʷ�Χ���ɽ�TIMʱ��Ƶ������Ƶ�������ı�arr��psc��������������*/
//void TIM3_CH1_CH2_Cap_Init(u32 arr,u16 psc)				//Ƶ�ʲ���
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseSttructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	TIM_ICInitTypeDef TIM_ICInitStructure;
//	
//	TIM_DeInit(TIM3);
//	  /* TIM3 clock enable */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);	//TIM3ʹ��
//	/* GPIOA clock enable */
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE);//GPIOA GPIOB ʱ�ӿ���

//	/* TIM3 channel 2 pin (PA.07) configuration */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;                               //GPIO����
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
//	GPIO_Init(GPIOB,&GPIO_InitStructure);
//	
//	 /* Enable the TIM3 global Interrupt NVIC���� */
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	
//	TIM_TimeBaseSttructure.TIM_Prescaler = psc;  //��ʱ����Ƶ
//	TIM_TimeBaseSttructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
//	TIM_TimeBaseSttructure.TIM_Period  = arr;//�Զ���װ��ֵ
//	TIM_TimeBaseSttructure.TIM_ClockDivision  = TIM_CKD_DIV1;
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseSttructure);		//��ʼ��TIM3�����벶�����
//	
//	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1 ;//ѡ�������IC1ӳ�䵽TI1��
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//�����ز���
//	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//ӳ�䵽TI1��//�ܽ���Ĵ�����Ӧ��ϵ
//	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1; /*��������Ԥ��Ƶ������Ƶ�������ڶ��ٸ�����������һ�β���
//	������ź�Ƶ��û�б䣬��õ�����Ҳ����䡣����ѡ��4��Ƶ����ÿ�ĸ��������ڲ���һ�β��������������źű仯��Ƶ���������
//	�����Լ�������������жϵĴ���*/
//	
//	TIM_ICInitStructure.TIM_ICFilter = 0x00;//ICF=0000  ���������˲��� ���˲�
//	/*	�˲����ã������������������϶������ȶ�0x0~0xF*/
//	
//  TIM_ICInit(TIM3,&TIM_ICInitStructure);

//  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;//CC1S=01 ѡ�������IC2sӳ�䵽TI1��
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;//�½��ز���
//	TIM_ICInit(TIM3,&TIM_ICInitStructure);
//	
//	TIM_ITConfig(TIM3,TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2,ENABLE);//�����жϸ��£�����CC1IE�����ж�
//	TIM_Cmd(TIM3,ENABLE);//����TIM3
//	
//	
//	
//}

//void frequency_measure_start_stop(u8 num)
//{
////	switch (num)
////	{
////		case 0:
////			TIM_Cmd(TIM3,DISABLE);//�رն�ʱ��
////		break;
////		
////		case 1:
////			TIM_Cmd(TIM3,ENABLE);//������ʱ��
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
//TIM_ClearITPendingBit(TIM3, TIM_IT_Update|TIM_IT_CC1|TIM_IT_CC2); //���TIM���жϴ�����λ

//IC2Value = TIM_GetCapture2(TIM3); //��ȡIC2����Ĵ�����ֵ����ΪPWM���ڵļ���ֵ
//if (IC2Value != 0)
//{
//DutyCycle = (TIM_GetCapture1(TIM3) * 100) / IC2Value; //��ȡIC1����Ĵ�����ֵ��������ռ�ձ�

//Frequency = 72000000 / IC2Value; //����PWMƵ��
//}
//else
//{
//DutyCycle = 0;
//Frequency = 0;
//}
//}
