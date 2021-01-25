/**
  * @attention
  * ���ø߼���ʱ��TIM1�������������ʱ���PWM���Σ�Ҫ������ʱ��Ϊ1us��ռ�ձ�Ϊ50%��CH1��CH2֮�����λ��Ϊ2ms������Ϊ8ms
  * ʵ��ƽ̨:STM32F103ZET6������ 
  *
  ******************************************************************************
  */ 
#include "pwm.h"
#include "led.h"

extern char Upflag;//�ñ�������Ϊ��ʶ�ô��ж�Ϊ�����жϻ��������жϣ��Դ������ò�ͬ��CCRֵ������֤ռ�ձ���ͬ
                   
				   
void TIM_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;  
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 
	
    /* �����ܽŶ������� */  
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
  
    /* TIM1 clock enable */  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  
    /*  �ܽ�������*/  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //�������������Ϊ3.3V 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;//����PWM�ܽ�����
    GPIO_Init(GPIOB, &GPIO_InitStructure);  

	/* TIMʱ������*/	
	TIM_TimeBaseStructure.TIM_Prescaler =71; //Ԥ��Ƶ
	TIM_TimeBaseStructure.TIM_Period =3999;	//0~65535,����Ϊ8ms������Ϊ���ϼ���ģʽ��������(1/72x1000000)x?x2=1/20000
	TIM_TimeBaseStructure.TIM_CounterMode =TIM_CounterMode_CenterAligned1;
	TIM_TimeBaseStructure.TIM_ClockDivision =0;	//������Ƶ
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;	//�ظ�Ԥ��Ƶ������PWMռ�ձ�
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);		//	��ʼ��ʱ��
		
	/* PWM����*/
	TIM_OCInitStructure.TIM_OCMode =TIM_OCMode_PWM2;	//����Ƚ�ģʽ��PWMģʽ1��2����Toggle
	/*	
	PWMģʽ1�������ϼ���ʱ��һ��TIMx_CNT<TIMx_CCR1ʱͨ��1Ϊ��Ч��ƽ������Ϊ��Ч��ƽ��
			  �����¼���ʱ��һ��TIMx_CNT>TIMx_CCR1ʱͨ��1Ϊ��Ч��ƽ(OC1REF=0)������Ϊ��Ч��ƽ(OC1REF=1)��
	PWMģʽ2�������ϼ���ʱ��һ��TIMx_CNT<TIMx_CCR1ʱͨ��1Ϊ��Ч��ƽ������Ϊ��Ч��ƽ��
			  �����¼���ʱ��һ��TIMx_CNT>TIMx_CCR1ʱͨ��1Ϊ��Ч��ƽ������Ϊ��Ч��ƽ��
	*/
	TIM_OCInitStructure.TIM_OutputState =TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;	
	TIM_OCInitStructure.TIM_Pulse =10;	// �˴����θ�ֵ����CCRֵ
	/*��������*/
	TIM_OCInitStructure.TIM_OutputNState =TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCNPolarity =TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState =TIM_OCIdleState_Reset;		//ɲ��֮������״̬
	TIM_OCInitStructure.TIM_OCNIdleState =TIM_OCNIdleState_Reset;	//ɲ��֮��Ļ������״̬
	TIM_OC1Init(TIM1,&TIM_OCInitStructure);
	
	TIM_OCInitStructure.TIM_Pulse = 2010;//����2ms��1/1000000x��=2/1000��x=2000
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
		
	/* ��������*/
	TIM_BDTRInitStructure.TIM_OSSRState =TIM_OSSRState_Enable;	//����ģʽ�����
	TIM_BDTRInitStructure.TIM_OSSIState =TIM_OSSIState_Enable;	//����ģʽ�����
	TIM_BDTRInitStructure.TIM_LOCKLevel =TIM_LOCKLevel_OFF;		//��������
	TIM_BDTRInitStructure.TIM_DeadTime =0x01;					//��������ʱ��0-0xff��1us
	/*									
	   ����  10us   Ӧ���ڵ�������
	   ����ʱ�� =1/72*1000*(32+DTG)*16=10us;		(72Mhz)
	   ����13.89*(32+DTG)*16 =10000;
	   DTG =12.99  ~  13��
	   DTG[4:0]=13    ��Ӧ16����    D
	   ��Ϊ�ǵ������� DTG[7:5]=111   ��Ӧ16���� E
	   ����DTG = DTG[7:5]  +DTG[4:0] =0x0ED;
	   ��TIM_BDTRInitStructure.TIM_DeadTime =0x0ED;
	   
	   ����ʱ��deadtime�ļ��㷽����
       UTG[7:0]:����������װ�ã���Щλ�����˲��뻥�����֮�����������ʱ�䣬����DT��ʾ�����ʱ�䣺
       DTG[7:0] = 0xx => DT = DTG[7:0]�� Tdtg , Tdtg = TDTS;����һ���䣩
       DTG[7:0] = 10x => DT = (64 +  DTG[5:0])�� Tdtg , Tdtg = 2��TDTS;���ڶ����䣩
       DTG[7:0] = 110 => DT = (32 +  DTG[4:0])�� Tdtg , Tdtg = 8��TDTS;���������䣩
       DTG[7:0] = 111 => DT = (32 +  DTG[4:0])�� Tdtg , Tdtg = 16��TDTS;���������䣩     
	   ���ӣ���TDTS =  13.89ns(72MHz),���ܵ�����ʱ��Ϊ��
       0��1764ns��������13.89ns��
       1777.9ns��3528.88ns��
       3555.84ns��7000.56ns��
       7111.68ns��14001.12ns��
       ��� deadtime=1us����������0��1764ns�����Թ�ʽѡ��& amp;nbsp;DTG[7:0] = 0xx => DT = DTG[7:0]�� Tdtg , Tdtg = TDTS ;
       1us = x 13.89ns��x=72��DTG[7:0]=0x72���������õڶ�����ʽ�õ���30��DTG[5��0]=0b11110������DTG��ǰ��λ���� DTG[7��0]=0b1011110=0x5E��
	*/	
	TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;			//ɲ��ʹ�ܹر�
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_Low;        //ɲ��������ԣ���ɲ����������PB12��GNDʱ��PWMֹͣ���  
	/*  ɲ����������ΪTIM1_BKIN pin(PB.12)����PB12��GND��channel���以��ͨ��������Ϊɲ����ĵ�ƽ������Ϊ0����1��Ҫ��֮ǰ��������ʱ������2�����������ã� 
		.TIM_OCIdleState = TIM_OCIdleState_Reset;   //ɲ��֮��PWMͨ����Ϊ0 
		.TIM_OCNIdleState = TIM_OCNIdleState_Reset; //ɲ��֮��PWM����ͨ����Ϊ0        
		ע�⣺���û��Ҫ�����ǲ�Ҫ����ɲ�����ܣ���Ϊ���PWM����Ӱ�죬�ر��ǵ�PB12����ʱ�����ν����кܴ�Ĳ����� 
		���ﲻ��ɲ�����ܣ���.TIM_Break = TIM_Break_Disable; 
	*/  

	TIM_BDTRInitStructure.TIM_AutomaticOutput =TIM_AutomaticOutput_Enable;
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM1�ж�,��������ж�


	/*����PWM���*/
	TIM_CtrlPWMOutputs(TIM1,ENABLE);	//PWM�������
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //CH2Ԥװ��ʹ��
	
	TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���  Ӱ�ӼĴ��� ����
		
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;  //TIM1�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM1,ENABLE);		
}

//��ʱ��1�жϷ�������������жϡ������ж�������װ��ȽϼĴ���CCRֵ
void TIM1_UP_IRQHandler(void)   //TIM1�ж�
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)  //���TIM1�����жϷ������
		{
		  TIM_ClearITPendingBit(TIM1, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		 			
	      if(Upflag==0)//�����ж�
          {            //���磺Upflag��ʼ��Ϊ0����������������ARRֵʱ����ʱ��������һ���ж�Ϊ�����жϣ�ִ�д˴����룬����Upflag��1
		      Upflag=1;
 		      TIM_SetCompare1(TIM1,3990);
 		      TIM_SetCompare2(TIM1,1990);	
          }

          else if(Upflag==1)//�����ж�
          {                 //���磺��ʱ�����������жϣ�����֮ǰUpflag��1��ִ�д˴����룬����Upflag��0
		      Upflag=0;		 
 		      TIM_SetCompare1(TIM1,10);
 		      TIM_SetCompare2(TIM1,2010);
		
  }	
		
		}
}



