#ifndef __MYIIC_H
#define __MYIIC_H

//#include "stm32f10x_dac.h"
#include "sys.h"
#include "stm32f4xx.h"

//-----------------------------DAC IIC�˿ڶ���--------------------------------		   

//IO��������
 
//#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
//#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO��������	 
#define IIC_SCL    PEout(6) //SCL
#define IIC_SDA    PEout(5) //SDA	 
#define READ_SDA   PEin(5)  //����SDA 


//IIC���в�������

void I2C_Configuration(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�
uint8_t I2CWriteByte(uint8_t Device,uint8_t Resister,uint8_t Data);
uint8_t I2CReadBytes(uint8_t Device,uint8_t Resister,uint8_t Data[],uint8_t Counts);
uint8_t I2CWriteBytes(uint8_t Device,uint8_t Resister,uint8_t Data[],uint8_t Counts);			
#endif 

							
