/*
*********************************************************************************************************
*
* 作    者 ：李晨洋
*	模块名称 : ADF4351 驱动模块
*	文件名称 : ADF4351.c
*	版    本 : V1.0
*	说    明 : 推荐该ADF4351模块代码与STMF407使用
*			 
*********************************************************************************************************
*/
#include "ADF4351.h"
#include "delay.h"


void ADF_Output_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void ADF_Input_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
}

//void delay (int length)
//{
//	while (length >0)
//    	length--;
//}

void WriteToADF4351(u8 count, u8 *buf)
{
	u8 ValueToWrite = 0;
	u8 i = 0;
	u8 j = 0;
	
//	ADF_Output_GPIOInit();
	
	ADF4351_CE = 1;
	delay_us(1);
	ADF4351_CLK = 0;
	ADF4351_LE = 0;
	delay_us(1);
	
	for(i = count; i>0; i--)
	{
		ValueToWrite = *(buf+i-1);
		for(j=0; j<8; j++)
		{
			if(0x80 == (ValueToWrite & 0x80))
			{
				ADF4351_OUTPUT_DATA = 1;
			}
			else
			{
				ADF4351_OUTPUT_DATA = 0;
			}
			delay_us(1);
			ADF4351_CLK = 1;
			delay_us(1);
			ValueToWrite <<= 1;
			ADF4351_CLK = 0;	
		}
	}
	ADF4351_OUTPUT_DATA = 0;
	delay_us(1);
	ADF4351_LE = 1;
	delay_us(1);
	ADF4351_LE = 0;
}


void ReadToADF4351(u8 count, u8 *buf)
{
	u8 i = 0;
	u8 j = 0;
	u8 iTemp = 0;
	u8 RotateData = 0;
	
	ADF_Input_GPIOInit();
	ADF4351_CE = 1;
	delay_us(1);
	ADF4351_CLK = 0;
	ADF4351_LE = 0;
	delay_us(1);
	
	for(i = count; i>0; i--)
	{
		for(j = 0; j<8; j++)
		{
			RotateData <<=1;
			delay_us(1);
			iTemp = ADF4351_INPUT_DATA;
			ADF4351_CLK = 1;
			if(0x01 == (iTemp&0x01))
			{
				RotateData |= 1;
			}
			delay_us(1);
			ADF4351_CLK = 0;
		}
		*(buf+i-1) = RotateData;
	}
	delay_us(1);
	ADF4351_LE = 1;
	delay_us(1);
	ADF4351_LE = 0;
}


void ADF4351Init(void)
{
	u8 buf[4] = {0,0,0,0};
	
	ADF_Output_GPIOInit();
	
	buf[3] = 0x00;				//此处配置固定输出35M
	buf[2] = 0x58;
	buf[1] = 0x00;				//编写通讯寄存器0x00580005来控制进程
 	buf[0] = 0x05;				//写入寄存器5设置数字锁检测器
	WriteToADF4351(4,buf);		

	buf[3] = 0x00;
	buf[2] = 0xec;				//(DB23=1)信号直接取自 VCO;(DB22-20:4H) RF分频器为16;(DB19-12:50H)R为80
	buf[1] = 0x80;				//(DB11=0)VCO powerd up;
 	buf[0] = 0x3C;				//(DB11=0)启用VCO电源(DB5=1)RF输出;(DB4-3=3H)输出功率电平上升5d;
	WriteToADF4351(4,buf);		

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x04;				//(DB14-3:96H)时钟分配器的值是150。
 	buf[0] = 0xB3;
	WriteToADF4351(4,buf);	

	buf[3] = 0x00;
	buf[2] = 0x01;				//(DB14-3:96H)时钟分配器值为150。
	buf[1] = 0x0E;				//(DB8=0)启用分数n数字锁检测;
 	buf[0] = 0x42;				////(DB12-9:7H)设置Icp 2.50 mA;
	WriteToADF4351(4,buf);		//(DB23-14:1H)R计数器为1

	buf[3] = 0x00;
	buf[2] = 0x00;
	buf[1] = 0x80;			   //(DB14-3:6H)MOD计数器为6;
 	buf[0] = 0x29;			   //(DB26-15:6H)相位字为1，相位不同步
	WriteToADF4351(4,buf); //也没有使用伪优化函数
							           //(DB27=1)预估值为8/9

	buf[3] = 0x00;
	buf[2] = 0x2c;
	buf[1] = 0x80;
 	buf[0] = 0x18;				//(DB14-3:0H)裂缝值为0;
	WriteToADF4351(4,buf);		//(DB30-15:140H)INT值为320;
}
