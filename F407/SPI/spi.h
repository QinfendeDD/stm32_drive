#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//#include "spi.h"


//#define CS_RESET GPIO_ResetBits(GPIOA,GPIO_Pin_15)
//#define CS_SET GPIO_SetBits(GPIOA,GPIO_Pin_15)

//#define SCK_AD PAout(5)
//#define MISO_AD PAout(6)
//#define MOSI_AD PAout(7)
//

void SPI1_Init(void);			 //��ʼ��SPI1��
void SPI1_SetSpeed(u8 SpeedSet); //����SPI1�ٶ�   
u8 SPI1_ReadWriteByte(u8 TxData);//SPI1���߶�дһ���ֽ�

#endif




