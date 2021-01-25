#include "myiic.h"
#include "delay.h"

void I2C_Configuration(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_AHB1Periph_GPIOE, ENABLE );	//ʹ��GPIOBʱ��   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIO_SetBits(GPIOE,GPIO_Pin_6|GPIO_Pin_5); 	//PB8,PB9 �����
}
//SDA����Ϊ����
void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_AHB1Periph_GPIOE, ENABLE );	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

//SDA����Ϊ���
void SDA_OUT(void)
{ 
	  GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_AHB1Periph_GPIOE, ENABLE );	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������_OD ; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}
uint8_t I2CReadBytes(uint8_t Device,uint8_t Resister,uint8_t Data[],uint8_t Counts)
{
	uint8_t i;
	Device<<=1;
	Device&=0xFE;
	IIC_Start();
	IIC_Send_Byte(Device);
	if(IIC_Wait_Ack())
		return 1;
	IIC_Send_Byte(Resister);
	if(IIC_Wait_Ack())
		return 1;
	Device|=0x01;
	IIC_Start();
	IIC_Send_Byte(Device);
	if(IIC_Wait_Ack())
		return 1;
	for(i=0;i<Counts;i++){
		Data[i]=IIC_Read_Byte(1);
		if(i==Counts-1)
			IIC_NAck();
		else
			IIC_Ack();
	}
	IIC_Stop();
	return 0;
}
uint8_t I2CWriteByte(uint8_t Device,uint8_t Resister,uint8_t Data)
{
//	uint8_t i;
	Device<<=1;
	Device&=0xFE;
	IIC_Start();
	IIC_Send_Byte(Device);
	if(IIC_Wait_Ack())
		return 1;
	IIC_Send_Byte(Resister);
	if(IIC_Wait_Ack())
		return 1;
	IIC_Send_Byte(Data);
	if(IIC_Wait_Ack())
		return 1;
	IIC_Stop();
	return 0;
}
uint8_t I2CWriteBytes(uint8_t Device,uint8_t Resister,uint8_t Data[],uint8_t Counts)
{
	uint8_t i;
	Device<<=1;
	Device&=0xFE;
	IIC_Start();
	IIC_Send_Byte(Device);
	if(IIC_Wait_Ack())
		return 1;
	IIC_Send_Byte(Resister);
	if(IIC_Wait_Ack())
		return 1;
	for(i=0;i<Counts;i++){
		IIC_Send_Byte(Data[i]);
		if(IIC_Wait_Ack())
			return 1;
	}
	IIC_Stop();
	return 0;
}
