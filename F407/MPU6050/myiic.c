#include "myiic.h"
#include "delay.h"
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

//��ʼ��IIC
void IIC_Init_6050(void)
{			
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOBʱ��

  //GPIOB8,B9��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��
}
//SDA����Ϊ����
void SDA_IN_6050(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE );	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//_FLOATING; 
//	  GPIO_InitStructure.GPIO_OType = GPIO_OType_FLOATING;// 
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//SDA����Ϊ���
void SDA_OUT_6050(void)
{ 
	  GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_AHB1Periph_GPIOC, ENABLE );	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//_OD 
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;// ���ÿ�©���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//����IIC��ʼ�ź�
void IIC_Start_6050(void)
{
	SDA_OUT_6050();     //sda�����
	IIC_SDA_6050=1;	  	  
	IIC_SCL_6050=1;
	delay_us(4);
 	IIC_SDA_6050=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_6050=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop_6050(void)
{
	SDA_OUT_6050();//sda�����
	IIC_SCL_6050=0;
	IIC_SDA_6050=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_6050=1; 
	IIC_SDA_6050=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack_6050(void)
{
	u8 ucErrTime=0;
	SDA_IN_6050();      //SDA����Ϊ����  
	IIC_SDA_6050=1;delay_us(1);	   
	IIC_SCL_6050=1;delay_us(1);	 
	while(READ_SDA_6050)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop_6050();
			return 1;
		}
	}
	IIC_SCL_6050=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack_6050(void)
{
	IIC_SCL_6050=0;
	SDA_OUT_6050();
	IIC_SDA_6050=0;
	delay_us(2);
	IIC_SCL_6050=1;
	delay_us(2);
	IIC_SCL_6050=0;
}
//������ACKӦ��		    
void IIC_NAck_6050(void)
{
	IIC_SCL_6050=0;
	SDA_OUT_6050();
	IIC_SDA_6050=1;
	delay_us(2);
	IIC_SCL_6050=1;
	delay_us(2);
	IIC_SCL_6050=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte_6050(u8 txd)
{                        
    u8 t;   
	SDA_OUT_6050(); 	    
    IIC_SCL_6050=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA_6050=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL_6050=1;
		delay_us(2); 
		IIC_SCL_6050=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte_6050(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN_6050();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL_6050=0; 
        delay_us(2);
		IIC_SCL_6050=1;
        receive<<=1;
        if(READ_SDA_6050)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck_6050();//����nACK
    else
        IIC_Ack_6050(); //����ACK   
    return receive;
}



























