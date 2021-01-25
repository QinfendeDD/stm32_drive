#include "MCP4725.h"
#include "delay.h"
//#include "stm32f10x_i2c.h"

//I2C����

//��ʼ��IIC
void I2C_Configuration_dac(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_AHB1Periph_GPIOC, ENABLE );	//ʹ��GPIOBʱ��   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_9); 	//PB8,PB9 �����
}
//SDA����Ϊ����
void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_AHB1Periph_GPIOC, ENABLE );	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//SDA����Ϊ���
void SDA_OUT(void)
{ 
	  GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_AHB1Periph_GPIOC, ENABLE );	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������_OD ; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
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

void MCP4725_WriteData_Voltage(u16 Vout)   //��ѹ��λmV
{
  u8 temp;
	u16 Dn;
	Dn = ( 4096 * Vout) / 3.3;
	temp = (0x0F00 & Dn) >> 8;  //12λ����
	IIC_Start();
	IIC_Send_Byte(0XC2);      //����Ѱַ����������1100�� ��ַλA2��A1��A0Ϊ 0 �� 0 �� 1��-> 1100 0010
	IIC_Wait_Ack();
	IIC_Send_Byte(temp); 	
	IIC_Wait_Ack();
	IIC_Send_Byte(Dn);        //����8λ�͵�DAC�Ĵ���
	IIC_Wait_Ack();
	IIC_Stop();//����һ��ֹͣ����  	
	delay_ms(10);	
}
void MCP4725_WriteData_Digital(u16 data)   //12λ������
{
  u8 data_H=0,data_L=0;
	data_H = ( 0x0F00 & data) >> 8;
	data_L = 0X00FF & data ;
	IIC_Start();
	IIC_Send_Byte(0XC0);      //����Ѱַ����������1100�� ��ַλA2��A1��A0Ϊ 0 �� 0 �� 0��-> 1100 0000
	IIC_Wait_Ack();	 
	IIC_Send_Byte(data_H); 	
	IIC_Wait_Ack();	 
	IIC_Send_Byte(data_L);
	IIC_Wait_Ack();	
	IIC_Stop();//����һ��ֹͣ����  	
	delay_ms(10);	
}
