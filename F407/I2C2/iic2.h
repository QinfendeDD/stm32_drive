#ifndef __IIC2_H
#define __IIC2_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

   	   		   
//IO��������
//#define SDA_IN1()  {GPIOB->MODER&=0XFFFF0FFF;GPIOB->MODER|=8<<12;}
//#define SDA_OUT1() {GPIOB->MODER&=0XFFFF0FFF;GPIOB->MODER|=3<<12;}
//#define SDA_IN1() {GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=0<<7*2;} 
//#define SDA_OUT1() {GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=1<<7*2;}  
//IO��������	 
#define IIC_SCL2    PBout(12) //SCL
#define IIC_SDA2    PBout(13) //SDA
#define READ_SDA2   PBin(13)  //����SDA 

//IIC���в�������
void IIC_Init2(void);                //��ʼ��IIC��IO��				 
void IIC_Start2(void);				//����IIC��ʼ�ź�
void IIC_Stop2(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte2(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte2(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack2(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack2(void);					//IIC����ACK�ź�
void IIC_NAck2(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte2(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte2(u8 daddr,u8 addr);	  
#endif
















