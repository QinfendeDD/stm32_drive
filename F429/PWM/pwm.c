#include "PWM.h"

void SetPWMFreq()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd (RCC_AHB1Periph_GPIOA, ENABLE); 	//IO��ʱ������

	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;  		// ����ģʽ 	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  	// ����
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;		// ����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //	�ٶȵȼ�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	 
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);								// ��ʼ�� TIM2_CH1 ����
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_TIM2);		// ���ø���
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	//��ʱ����������
	TIM_TimeBaseInitStructure.TIM_Period 			= 10-1; 								// ����ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler		    = 9 - 1;  // ��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_CounterMode		= TIM_CounterMode_Up; 				// ���ϼ���
	TIM_TimeBaseInitStructure.TIM_ClockDivision	    = TIM_CKD_DIV1; 						// �����˲�����ʱ�ӷ�Ƶ����
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);					// ��ʼ��TIM2��������

	//PWM�������
	TIM_OCInitStructure.TIM_OCMode 		= TIM_OCMode_PWM1;			// PWMģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//	ʹ�ܱȽ����
	TIM_OCInitStructure.TIM_OCPolarity 	= TIM_OCPolarity_High;		// С������ֵ����ߵ�ƽ
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  			            //��ʼ����ʱ���Ƚ����ͨ�� 1
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);               //�Զ����رȽ����ͨ�� 1 ��ֵ
	
	TIM_ARRPreloadConfig(TIM2,ENABLE);	//	ʹ���Զ�����
	TIM_Cmd(TIM2,ENABLE); 
	
	TIM_SetCompare1(TIM2,5);
}
