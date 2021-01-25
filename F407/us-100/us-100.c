#include "us-100.h"

u32 count = 0;
u32 Distance = 0;
//��ʼ��
void Ultrasonic_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//Trig
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//_PP;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;//Echo
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//_FLOATING;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
//	trig=0;
	GPIO_ResetBits(GPIOE, GPIO_Pin_0);//Ԥ������Trig����
}

//�����Ƕ�ʱ��2��ʼ������
//TIM1��TIM8~TIM11��ʱ��ΪAPB2ʱ�ӵ�������168M��TIM2~TIM7��TIM12~TIM14��ʱ��ΪAPB1��ʱ�ӵ�������84M��
void Timer2_INIT(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
  TIM_TimeBaseInitStructure.TIM_Prescaler = 840-1;//��ʱ����Ƶ
  TIM_TimeBaseInitStructure.TIM_Period = 499999;//�Զ���װ��ֵ
  TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
  TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
  TIM_ClearFlag(TIM2, TIM_FLAG_Update); //����ʱ��3�����ж�
}
//����Ϊ����������������
u32 Ultrasonic(void)
{
	uint32_t Distance = 0;
	GPIO_SetBits(GPIOE, GPIO_Pin_0);
	delay_us(20);
	GPIO_ResetBits(GPIOE, GPIO_Pin_0);//����10us������//����
	TIM2->CNT = 0;
	while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1) == 0);
	TIM_Cmd(TIM2, ENABLE);
	while(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1) == 1);
	TIM_Cmd(TIM2, DISABLE);
	count = TIM2->CNT;
	//(�ߵ�ƽʱ��*340m/s)/2
	Distance =(count*17);
	return Distance;
//	delay_ms(500);
}
