#include "MCP4725.h"
#include "delay.h"
//#include "stm32f10x_i2c.h"

//I2C配置

//初始化IIC
void I2C_Configuration_dac(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_AHB1Periph_GPIOC, ENABLE );	//使能GPIOB时钟   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;   //推挽输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_9); 	//PB8,PB9 输出高
}
//SDA设置为输入
void SDA_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_AHB1Periph_GPIOC, ENABLE );	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//SDA设置为输出
void SDA_OUT(void)
{ 
	  GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_AHB1Periph_GPIOC, ENABLE );	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出_OD ; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
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
	IIC_SCL=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
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
//不产生ACK应答		    
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
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT(); 	    
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2); 
		IIC_SCL=0;	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
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
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}

void MCP4725_WriteData_Voltage(u16 Vout)   //电压单位mV
{
  u8 temp;
	u16 Dn;
	Dn = ( 4096 * Vout) / 3.3;
	temp = (0x0F00 & Dn) >> 8;  //12位数据
	IIC_Start();
	IIC_Send_Byte(0XC2);      //器件寻址，器件代吗：1100； 地址位A2，A1，A0为 0 ， 0 ， 1；-> 1100 0010
	IIC_Wait_Ack();
	IIC_Send_Byte(temp); 	
	IIC_Wait_Ack();
	IIC_Send_Byte(Dn);        //将低8位送到DAC寄存器
	IIC_Wait_Ack();
	IIC_Stop();//产生一个停止条件  	
	delay_ms(10);	
}
void MCP4725_WriteData_Digital(u16 data)   //12位数字量
{
  u8 data_H=0,data_L=0;
	data_H = ( 0x0F00 & data) >> 8;
	data_L = 0X00FF & data ;
	IIC_Start();
	IIC_Send_Byte(0XC0);      //器件寻址，器件代吗：1100； 地址位A2，A1，A0为 0 ， 0 ， 0；-> 1100 0000
	IIC_Wait_Ack();	 
	IIC_Send_Byte(data_H); 	
	IIC_Wait_Ack();	 
	IIC_Send_Byte(data_L);
	IIC_Wait_Ack();	
	IIC_Stop();//产生一个停止条件  	
	delay_ms(10);	
}
