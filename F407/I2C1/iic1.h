#ifndef __IIC1_H
#define __IIC1_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//IIC驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

   	   		   
//IO方向设置
//#define SDA_IN1()  {GPIOB->MODER&=0XFFFF0FFF;GPIOB->MODER|=8<<12;}
//#define SDA_OUT1() {GPIOB->MODER&=0XFFFF0FFF;GPIOB->MODER|=3<<12;}
//#define SDA_IN1() {GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=0<<7*2;} 
//#define SDA_OUT1() {GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=1<<7*2;}  
//IO操作函数	 
#define IIC_SCL1    PBout(10) //SCL
#define IIC_SDA1    PBout(11) //SDA
#define READ_SDA1   PBin(11)  //输入SDA 

//IIC所有操作函数
void IIC_Init1(void);                //初始化IIC的IO口				 
void IIC_Start1(void);				//发送IIC开始信号
void IIC_Stop1(void);	  			//发送IIC停止信号
void IIC_Send_Byte1(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte1(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack1(void); 				//IIC等待ACK信号
void IIC_Ack1(void);					//IIC发送ACK信号
void IIC_NAck1(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte1(u8 daddr,u8 addr,u8 data);
u8 IIC_Read_One_Byte1(u8 daddr,u8 addr);	  
#endif
















