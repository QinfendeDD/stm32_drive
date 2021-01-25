#ifndef __IIC1_H
#define __IIC1_H
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
#define IIC_SCL1    PBout(10) //SCL
#define IIC_SDA1    PBout(11) //SDA
#define READ_SDA1   PBin(11)  //����SDA 

//IIC���в�������
void IIC_Init1(void);                //��ʼ��IIC��IO��				 
void IIC_Start1(void);				//����IIC��ʼ�ź�
void IIC_Stop1(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte1(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte1(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack1(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack1(void);					//IIC����ACK�ź�
void IIC_NAck1(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte1(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte1(u8 daddr,u8 addr);	  
#endif
















