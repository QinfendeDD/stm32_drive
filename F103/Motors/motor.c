#include "motor.h"

/************PWMռ�ձ�1,2,3,4ͨ��ֵ************/
uint16_t Channel1Pulse=0;
uint16_t Channel2Pulse=0;
uint16_t Channel3Pulse=0; 
uint16_t Channel4Pulse=0;

GPIO_InitTypeDef  GPIO_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 
TIM_OCInitTypeDef  TIM_OCInitStructure; 
TIM_BDTRInitTypeDef TIM_BDTRInitStructure; 
/*************************************************************************
 ����: void Motor_Init(void)
 ����: TIM1�߼���ʱ��PWM�����������
 ����: ��
 ����: ��
 ��ע: 
 �汾:
*************************************************************************/
void Motor_Init(void)
{ 
	uint16_t TimerPeriod = 0; 
	
	/*DRV8833�����˿�����---nSLEEP�˿�����*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
  

/***********�������PWM�ź�����---TIM1�߼���ʱ��PWM���������*************/
	/* Private function prototypes -----------------------------------------------*/ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
	/* Compute the value to be set in ARR register to generate signal frequency at 17.57 Khz */
	TimerPeriod = (SystemCoreClock / 10000) - 1; 	   /*PWM���ƽ��Ϊ1K*/
	/* Compute CCR1 value to generate a duty cycle at 50% for channel 1 */ 
	/*          �������� ʱ��ķ���  
	TIM_TimeBaseStructure.TIM_Prescaler = 0; //����������ΪTIM ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	��ôTIMʱ�Ӿ�Ϊ72M  1/72MHZ=0.01388...us           ����� ϵͳʱ��
	TimerPeriod = (SystemCoreClock / 17570 ) - 1;          //�����ж�����ֵ
	(72MHz/17570) -1=4096.89..   �ж�����ֵ
	4096.89*0.01388=56.8US  �����ʵ��PWM�ж�����ʱ��   
	1/56.8US=17.6KHZ   1���Գ�������ʱ�� ����Ƶ�� ��Լ17.6KHz���ҡ�
	����ͨ��ֵ���������� 
	Channel1Pulse = 5*��4096-1��/10 =2047       2047*0.01388=28.4US
	Channel2Pulse =  25 * (4096 - 1)) / 100=1023   1023*0.01388=14.19US
	Channel3Pulse =  125 *(4096 - 1)) / 1000=511   511*0.01388=7.09US
	*/
	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 9;                        /*����������ΪTIM ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ*/ 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     /*������ģʽ  ���ϼ���ģʽ */ 
	TIM_TimeBaseStructure.TIM_Period = TimerPeriod;        
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;                    /*ʱ�ӷָ�   ������Ƶ */
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                /*���� ���� ����ֵ*/ 
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	/* Channel 1, 2,3 and 4 Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;                /*�����ȵ���ģʽ2*/ 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    /*ʹ������Ƚ�״̬*/
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;  /*ʹ�ܻ������״̬*/ 
	TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;                   /*����ֵ*/ 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;        /*����Ƚϼ��Ե�*/
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;       /*���� ������Ը�*/ 
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;       /*MOE=0 ���� TIM1����ȽϿ���״̬*/ 
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;    /*MOE=0 ���� TIM1����ȽϿ���״̬*/ 
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);                         /*�趨�õĲ��� ��ʼ��TIM*/   
	TIM_OCInitStructure.TIM_Pulse = Channel2Pulse; 
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = Channel3Pulse; 
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	/*����ɲ������  ����ʱ��  ����ƽ OSSI OSSR ״̬ AOE(�Զ����ʹ��)*/ 
	/* Automatic Output enable, Break, dead time and lock configuration*/
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;       /*����������ģʽ�� �ǹ���״̬ѡ�� OSSR*/ 
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;       /*����������ģʽ�� �ǹ���״̬ѡ�� OSSI*/ 
	TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;            /*ʹ������ƽ1*/
	TIM_BDTRInitStructure.TIM_DeadTime = 22;                          /*ָ��������ʹ� �ر�״̬֮�����ʱ Լ 1US*/  
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Enable;               /*ʹ��ɲ��*/ 
	TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High; /*ɲ������ܽż��Ը�*/
	TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;/*�Զ��������ʹ��*/
	TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);                      /*��ʼ���趨�õĲ���*/
	/* TIM1 counter enable */
	TIM_Cmd(TIM1, ENABLE);
	/* Main Output Enable */
	TIM_CtrlPWMOutputs(TIM1, ENABLE); 
	
  
/*******************************��ʼ���״̬����************************************/	
	//�رյ������оƬ����״̬
	nSLEEP_Motor(OFF);
	//��ʼ����ٶ�Ϊ0
	Speed_Motor(0,0);
}
/*************************************************************************
 ����: void nSLEEP_Motor(unsigned char i)
 ����: DRV8833 nSLEEPʹ��
 ����: i  : ON--�رգ�OFF--����
 ����: ��
 ��ע: 
 �汾��
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
 ����: void Speed_Motor(int Speed_Motor_Left_Value,int Speed_Motor_Right_Value)
 ����: �����������
 ����: Speed_Motor_Left_Value��Speed_Motor_Right_Value
 ����: ��
 ��ע: Speed_Motor_Left_Value���뷶Χ��-100~100��
       Speed_Motor_Right_Value���뷶Χ��-100~100��
 �汾:
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
	
	/************DRV8833�����˿�************/
	/*
			PA8 ---AIN1	 PA9 ---AIN2
			PA10---BIN1	 PA11---BIN2
	*/
/***********DRV8833�����������ź��ڶ�Ӧģʽ�¿��Ƶ������װ����ʱ��ѡ��*****************
    ͨ��������������жϵ������״̬����������������������Ӧ��״̬��������Ӧ�ź�IO�˿�********/
	if((Speed_Motor_Left > 0) &&(Speed_Motor_Right > 0))		  //����
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
	else if((Speed_Motor_Left < 0) &&(Speed_Motor_Right > 0))	  //����
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

/**************���������ź�������Ϻ󣬶Ե�������ٶȽ�������************************
	         ����������ٶȣ�PWMռ�ձȣ�������******************************************/	
	TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;         /*���� ֵ*/ 
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);               /*�趨�õĲ��� ��ʼ��TIM*/   
	TIM_OCInitStructure.TIM_Pulse = Channel2Pulse; 
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = Channel3Pulse; 
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_Pulse = Channel4Pulse;
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
}
 


 

