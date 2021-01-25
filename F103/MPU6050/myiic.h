#ifndef __MYIIC_H
#define __MYIIC_H

#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//IIC ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/6
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
   	   		   
//IO��������
//#define SDA_IN_6050()  {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=0<<9*2;}	//PB9����ģʽ
//#define SDA_OUT_6050() {GPIOB->MODER&=~(3<<(9*2));GPIOB->MODER|=1<<9*2;} //PB9���ģʽ
//IO��������	 
#define IIC_SCL_6050    PAout(1) //SCL
#define IIC_SDA_6050    PAout(2) //SDA	 
#define READ_SDA_6050   PAin(2)  //����SDA 

//IIC���в�������
void IIC_Init_6050(void);                //��ʼ��IIC��IO��				 
void IIC_Start_6050(void);				//����IIC��ʼ�ź�
void IIC_Stop_6050(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte_6050(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte_6050(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack_6050(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack_6050(void);					//IIC����ACK�ź�
void IIC_NAck_6050(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte_6050(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte_6050(u8 daddr,u8 addr);	  
#endif
















