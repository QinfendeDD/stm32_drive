#include "iic2.h"
#include "delay.h"
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
 
//初始化IIC
void IIC_Init2(void)
{					     
// 	RCC->APB2ENR|=1<<3;//先使能外设IO PORTB时钟 							 
//	GPIOB->MODER&=0XFFFF00FF;//PB10/11 推挽输出
//	GPIOB->MODER|=0X00003300;	   
//	GPIOB->ODR|=3<<10;     //PB10,11 输出高
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化	
}
//SDA设置为输入
void SDA_IN2(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE );	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//_FLOATING; 
//	  GPIO_InitStructure.GPIO_OType = GPIO_OType_FLOATING;// 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//SDA设置为输出
void SDA_OUT2(void)
{ 
	  GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_AHB1Periph_GPIOB, ENABLE );	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//_OD 
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;// 复用开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//产生IIC起始信号
void IIC_Start2(void)
{
	SDA_OUT2();     //sda线输出
	IIC_SDA2=1;	  	  
	IIC_SCL2=1;
	delay_us(4);
 	IIC_SDA2=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL2=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop2(void)
{
	SDA_OUT2();//sda线输出
	IIC_SCL2=0;
	IIC_SDA2=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL2=1; 
	IIC_SDA2=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack2(void)
{
	u8 ucErrTime=0;
	SDA_IN2();      //SDA设置为输入  
	IIC_SDA2=1;delay_us(1);	   
	IIC_SCL2=1;delay_us(1);	 
	while(READ_SDA2)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop2();
			return 1;
		}
	}
	IIC_SCL2=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack2(void)
{
	IIC_SCL2=0;
	SDA_OUT2();
	IIC_SDA2=0;
	delay_us(2);
	IIC_SCL2=1;
	delay_us(2);
	IIC_SCL2=0;
}
//不产生ACK应答		    
void IIC_NAck2(void)
{
	IIC_SCL2=0;
	SDA_OUT2();
	IIC_SDA2=1;
	delay_us(2);
	IIC_SCL2=1;
	delay_us(2);
	IIC_SCL2=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte2(u8 txd)
{                        
    u8 t;   
	SDA_OUT2(); 	    
    IIC_SCL2=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA2=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL2=1;
		delay_us(2); 
		IIC_SCL2=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte2(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN2();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL2=0; 
        delay_us(2);
		IIC_SCL2=1;
        receive<<=1;
        if(READ_SDA2)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck2();//发送nACK
    else
        IIC_Ack2(); //发送ACK   
    return receive;
}



























