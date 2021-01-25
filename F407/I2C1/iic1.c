#include "iic1.h"
#include "delay.h"
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
 
//��ʼ��IIC
void IIC_Init1(void)
{					     
// 	RCC->APB2ENR|=1<<3;//��ʹ������IO PORTBʱ�� 							 
//	GPIOB->MODER&=0XFFFF00FF;//PB10/11 �������
//	GPIOB->MODER|=0X00003300;	   
//	GPIOB->ODR|=3<<10;     //PB10,11 �����
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
//	GPIO_Init(GPIOG, &GPIO_InitStructure);
}
//SDA����Ϊ����
void SDA_IN1(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE );	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//_FLOATING; 
//	  GPIO_InitStructure.GPIO_OType = GPIO_OType_FLOATING;// 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//SDA����Ϊ���
void SDA_OUT1(void)
{ 
	  GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_AHB1Periph_GPIOB, ENABLE );	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//_OD 
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;// ���ÿ�©���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//����IIC��ʼ�ź�
void IIC_Start1(void)
{
	SDA_OUT1();     //sda�����
	IIC_SDA1=1;	  	  
	IIC_SCL1=1;
	delay_us(4);
 	IIC_SDA1=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL1=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop1(void)
{
	SDA_OUT1();//sda�����
	IIC_SCL1=0;
	IIC_SDA1=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL1=1; 
	IIC_SDA1=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack1(void)
{
	u8 ucErrTime=0;
	SDA_IN1();      //SDA����Ϊ����  
	IIC_SDA1=1;delay_us(1);	   
	IIC_SCL1=1;delay_us(1);	 
	while(READ_SDA1)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop1();
			return 1;
		}
	}
	IIC_SCL1=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack1(void)
{
	IIC_SCL1=0;
	SDA_OUT1();
	IIC_SDA1=0;
	delay_us(2);
	IIC_SCL1=1;
	delay_us(2);
	IIC_SCL1=0;
}
//������ACKӦ��		    
void IIC_NAck1(void)
{
	IIC_SCL1=0;
	SDA_OUT1();
	IIC_SDA1=1;
	delay_us(2);
	IIC_SCL1=1;
	delay_us(2);
	IIC_SCL1=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte1(u8 txd)
{                        
    u8 t;   
	SDA_OUT1(); 	    
    IIC_SCL1=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA1=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL1=1;
		delay_us(2); 
		IIC_SCL1=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte1(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN1();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL1=0; 
        delay_us(2);
		IIC_SCL1=1;
        receive<<=1;
        if(READ_SDA1)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck1();//����nACK
    else
        IIC_Ack1(); //����ACK   
    return receive;
}



























