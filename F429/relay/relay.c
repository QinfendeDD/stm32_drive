
#include "relay.h"
//IO�ڳ�ʼ��
void Relay_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ��
	RCC_AHB1PeriphClockCmd ( RELAY1_CLK, ENABLE); 	//��ʼ��GPIOGʱ��	
	RCC_AHB1PeriphClockCmd ( RELAY2_CLK, ENABLE); 	//��ʼ��GPIODʱ��	
	RCC_AHB1PeriphClockCmd ( RELAY3_CLK, ENABLE); 	//��ʼ��GPIODʱ��	
	RCC_AHB1PeriphClockCmd ( RELAY4_CLK, ENABLE); 	//��ʼ��GPIODʱ��	
	RCC_AHB1PeriphClockCmd ( RELAY5_CLK, ENABLE); 	//��ʼ��GPIODʱ��	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   //���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //�������
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	//����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //�ٶ�ѡ��
	
	//��ʼ�� RELAY1 ����
	GPIO_InitStructure.GPIO_Pin = RELAY1_PIN;	 
	GPIO_Init(RELAY1_PORT, &GPIO_InitStructure);	
	
	//��ʼ�� RELAY2 ����
	GPIO_InitStructure.GPIO_Pin = RELAY2_PIN;	 
	GPIO_Init(RELAY2_PORT, &GPIO_InitStructure);
	
	//��ʼ�� RELAY3 ����
	GPIO_InitStructure.GPIO_Pin = RELAY3_PIN;	 
	GPIO_Init(RELAY3_PORT, &GPIO_InitStructure);
	
	//��ʼ�� RELAY4 ����
	GPIO_InitStructure.GPIO_Pin = RELAY4_PIN;	 
	GPIO_Init(RELAY4_PORT, &GPIO_InitStructure);
	
	//��ʼ�� RELAY5 ����
	GPIO_InitStructure.GPIO_Pin = RELAY5_PIN;	 
	GPIO_Init(RELAY5_PORT, &GPIO_InitStructure);
	
//	GPIO_ResetBits(RELAY1_PORT,RELAY1_PIN);  //PD3����͵�ƽ
	GPIO_SetBits(RELAY2_PORT,RELAY2_PIN);  //PD4����͵�ƽ
//	GPIO_ResetBits(RELAY3_PORT,RELAY3_PIN);  //PD5����͵�ƽ
//	GPIO_ResetBits(RELAY4_PORT,RELAY4_PIN);  //PD6����͵�ƽ
//	GPIO_ResetBits(RELAY5_PORT,RELAY5_PIN);  //PD7����͵�ƽ
	
}
void s11()
{
	GPIO_ResetBits(RELAY1_PORT,RELAY1_PIN);	// ����͵�ƽ����RELAY1
	GPIO_SetBits(RELAY2_PORT,RELAY2_PIN);		// ����ߵ�ƽ���ر�RELAY1
	//RELAY2_ON;
//	RELAY3_OFF;
//	RELAY4_OFF;
}
void s22()
{
	GPIO_SetBits(RELAY1_PORT,RELAY1_PIN);		// ����ߵ�ƽ���ر�RELAY1
	GPIO_ResetBits(RELAY2_PORT,RELAY2_PIN);	// ����͵�ƽ����RELAY1	
//	RELAY3_OFF;
//	RELAY4_OFF;
}