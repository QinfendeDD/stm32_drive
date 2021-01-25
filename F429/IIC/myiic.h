#ifndef __MYIIC_H
#define __MYIIC_H

//#include "stm32f10x_dac.h"
#include "sys.h"
#include "stm32f4xx.h"

//-----------------------------DAC IIC端口定义--------------------------------		   

//IO方向设置
 
//#define SDA_IN()  {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)8<<28;}
//#define SDA_OUT() {GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=(u32)3<<28;}

//IO操作函数	 
#define IIC_SCL    PEout(6) //SCL
#define IIC_SDA    PEout(5) //SDA	 
#define READ_SDA   PEin(5)  //输入SDA 


//IIC所有操作函数

void I2C_Configuration(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号
uint8_t I2CWriteByte(uint8_t Device,uint8_t Resister,uint8_t Data);
uint8_t I2CReadBytes(uint8_t Device,uint8_t Resister,uint8_t Data[],uint8_t Counts);
uint8_t I2CWriteBytes(uint8_t Device,uint8_t Resister,uint8_t Data[],uint8_t Counts);			
#endif 

							
