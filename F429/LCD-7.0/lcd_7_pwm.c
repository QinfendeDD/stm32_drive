/***
	***************************************************************************
	*	@file  	lcd_pwm.c
	*	@brief   ��ʱ����ʼ��������pwm,��ΪLCD�ı������,��PWM�������Ÿı䣬������Ҫ�޸���صĺ��⣬
	*				����Ҫ���������������ĳ���Ӧͨ���ĺ��� ��
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

static u16 LCD_PwmPeriod = 500;  //��ʱ������ֵ
static u32 LCD_PwmPrescaler = 90 - 1;  //��ʱ����Ƶֵ

// ������TIM4 PWM CH2 IO�ڳ�ʼ��
//
void  LCD_PWM_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//IO��ʱ������
	RCC_AHB1PeriphClockCmd (LCD_PWM_CLK, ENABLE); 

	//IO���Ÿ���
	GPIO_PinAFConfig(LCD_PWM_PORT,LCD_PWM_PinSource,LCD_PWM_AF);
	
	//IO����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //�ٶ�

	//��ʼ�� TIM4_CH2 ����
	GPIO_InitStructure.GPIO_Pin = LCD_PWM_PIN;	 
	GPIO_Init(LCD_PWM_PORT, &GPIO_InitStructure);	
													   
}

// ������LCD PWM ռ�ձ�����
// ������pulse - ռ�ձȣ���Χ 0-100
//			
void  LCD_PwmSetPulse (u8 pulse)
{
	u16 compareValue ; 
	
	compareValue = pulse * LCD_PwmPeriod / 100; //����ռ�ձ����ñȽ�ֵ
	TIM_SetCompare2(LCD_TIM,compareValue); 
}
// ������LCD PWM ��ʼ��
//	������	pulse - LCD����ռ�ձ�
// ˵����LCD�����PWMƵ�ʹ̶�Ϊ10KHz
//
void	LCD_PWMinit(u8 pulse)
{

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	LCD_PWM_GPIO (); //��ʼ��IO��
	RCC_APB1PeriphClockCmd(LCD_TIM_CLK,ENABLE);  ///ʹ��ʱ��
	
	//��ʱ����������
	TIM_TimeBaseInitStructure.TIM_Period = LCD_PwmPeriod; 	//����ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler= LCD_PwmPrescaler;  //��Ƶϵ��
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

	TIM_TimeBaseInit(LCD_TIM,&TIM_TimeBaseInitStructure);//��ʼ��

	//PWM�������
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWMģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //ʹ�ܱȽ����
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // С������ֵ����ߵ�ƽ
	
	TIM_OC2Init(LCD_TIM, &TIM_OCInitStructure);  //��ʼ����ʱ���Ƚ����ͨ�� 2
	TIM_OC2PreloadConfig(LCD_TIM, TIM_OCPreload_Enable);  //�Զ����رȽ����ͨ�� 2 ��ֵ
	
	TIM_ARRPreloadConfig(LCD_TIM,ENABLE);//ʹ���Զ�����
  
	TIM_Cmd(TIM4,ENABLE); //ʹ�ܶ�ʱ��	
	
	LCD_PwmSetPulse (pulse); //����ռ�ձ�
}

