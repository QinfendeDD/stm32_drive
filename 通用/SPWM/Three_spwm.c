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
    TIM_BDTRInitStructure.TIM_DeadTime = 36; 	//����ʱ��
    TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable; //ɲ������ʹ��
    TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;	//ɲ�����뼫��
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);  

    //ccr1�Զ���װ��
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  
    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  
    TIM_ARRPreloadConfig(TIM1, ENABLE);
		TIM_Cmd(TIM1,ENABLE); 
     //�߼���ʱ�����е������ʹ�ܣ�������������;
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

	}

//TIMx PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM1_PWM_Init_config(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;	
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);  //ʹ��GPIOEʱ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); //ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);  //ʹ��AFIOʱ��
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//�ر�jtag ����sw
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM1,ENABLE);//ӳ��ΪTIMx
	
	//���ø�����Ϊ�����������,���TIMx_CHx��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14; //TIMx_CHx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
 
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	  TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Reset;
		TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM2;
		TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;
		TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_OCPolarity=TIM_OCNPolarity_High;
		TIM_OCInitStructure.TIM_OutputNState=TIM_OutputNState_Enable;
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_Pulse=DutyFactor1 *2813 / 100;;
	//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);	
	 TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;
   TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
   TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1;
   TIM_BDTRInitStructure.TIM_DeadTime = 780;//����ʱ��Ϊ 12/SYSTEMCLK (ns)
   TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;//�ر��ⲿbreak����
   TIM_BDTRInitStructure.TIM_BreakPolarity = TIM_BreakPolarity_High;
   TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
   TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);
		
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
	
	TIM_CtrlPWMOutputs(TIM1,ENABLE);	//MOE �����ʹ��	
	//TIM_ARRPreloadConfig(TIM1, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIMx

}


