#ifndef __COMMON_H
#define __COMMON_H
#include "sys.h"
//#include "bitband.h"



#define LED1		BIT_ADDR(GPIOB_ODR_Addr,0)
#define LED1_READ	BIT_ADDR(GPIOB_IDR_Addr,0)

#define LED2		BIT_ADDR(GPIOB_ODR_Addr,1)
#define LED2_READ	BIT_ADDR(GPIOB_IDR_Addr,1)

#define KEY			BIT_ADDR(GPIOB_IDR_Addr,2)



extern u8 USART_RxDat;       //��������
extern u8 USART_RxFlag;      //���ձ�־λ
void Delay_Init(void);       //��ʱ��ʼ������
void Delay_ms(u16 ms);       //��ʱX���뺯��
void Delay_us(u32 us);       //��ʱX΢���
void USART_Config(USART_TypeDef* USARTx,u32 Baud);  //���ڳ�ʼ��
u8 UartSendData(USART_TypeDef* USARTx,u8 ch);
void UartSendString(USART_TypeDef* USARTx,char *str);
#endif




