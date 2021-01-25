#include "hc05.h"
#include "stdio.h"
#include "ad9220.h"
#include "TSL1401.h"

uint8_t Receive=0;
void Usart3Init(void)
{
	USART_InitTypeDef UsartInit;
	NVIC_InitTypeDef NVICUsart3;
	GPIO_InitTypeDef GPIOInit;
	USART_DeInit(USART3);		
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	GPIOInit.GPIO_Mode=GPIO_Mode_AF_PP;	
	GPIOInit.GPIO_Pin=GPIO_Pin_10;
	GPIOInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInit);
	
	GPIOInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;	
	GPIOInit.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIOInit);
	
	UsartInit.USART_BaudRate=115200;		
	UsartInit.USART_WordLength=USART_WordLength_8b;		
	UsartInit.USART_StopBits=USART_StopBits_1;		
	UsartInit.USART_Parity=USART_Parity_No;		
	UsartInit.USART_HardwareFlowControl=USART_HardwareFlowControl_None;		
	UsartInit.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;		
	USART_Init(USART3,&UsartInit);	
	
	NVICUsart3.NVIC_IRQChannel=USART3_IRQn;		
	NVICUsart3.NVIC_IRQChannelPreemptionPriority=1;	
	NVICUsart3.NVIC_IRQChannelSubPriority=0;
	NVICUsart3.NVIC_IRQChannelCmd=ENABLE;		
	NVIC_Init(&NVICUsart3);		
	USART_ClearFlag(USART3,USART_FLAG_TC);		
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);		
	USART_Cmd(USART3,ENABLE);		
}
void SentData(uint8_t Data)		
{
	USART_SendData(USART3,Data);
	while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
}
void SentDatas(uint8_t *Data)		
{
	uint8_t i;
	for(i=0;Data[i]!='\0';i++)
		SentData(Data[i]);
}

void USART3_IRQHandler()		
{
	static uint8_t RXData;
	if (USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET){
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);		
		RXData=USART_ReceiveData(USART3);
		Receive=1;
	}
}

