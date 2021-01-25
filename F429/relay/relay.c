
#include "relay.h"
//IO口初始化
void Relay_Init(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure; //定义结构体
	RCC_AHB1PeriphClockCmd ( RELAY1_CLK, ENABLE); 	//初始化GPIOG时钟	
	RCC_AHB1PeriphClockCmd ( RELAY2_CLK, ENABLE); 	//初始化GPIOD时钟	
	RCC_AHB1PeriphClockCmd ( RELAY3_CLK, ENABLE); 	//初始化GPIOD时钟	
	RCC_AHB1PeriphClockCmd ( RELAY4_CLK, ENABLE); 	//初始化GPIOD时钟	
	RCC_AHB1PeriphClockCmd ( RELAY5_CLK, ENABLE); 	//初始化GPIOD时钟	
	
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;   //输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;  //推挽输出
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;	//上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; //速度选择
	
	//初始化 RELAY1 引脚
	GPIO_InitStructure.GPIO_Pin = RELAY1_PIN;	 
	GPIO_Init(RELAY1_PORT, &GPIO_InitStructure);	
	
	//初始化 RELAY2 引脚
	GPIO_InitStructure.GPIO_Pin = RELAY2_PIN;	 
	GPIO_Init(RELAY2_PORT, &GPIO_InitStructure);
	
	//初始化 RELAY3 引脚
	GPIO_InitStructure.GPIO_Pin = RELAY3_PIN;	 
	GPIO_Init(RELAY3_PORT, &GPIO_InitStructure);
	
	//初始化 RELAY4 引脚
	GPIO_InitStructure.GPIO_Pin = RELAY4_PIN;	 
	GPIO_Init(RELAY4_PORT, &GPIO_InitStructure);
	
	//初始化 RELAY5 引脚
	GPIO_InitStructure.GPIO_Pin = RELAY5_PIN;	 
	GPIO_Init(RELAY5_PORT, &GPIO_InitStructure);
	
//	GPIO_ResetBits(RELAY1_PORT,RELAY1_PIN);  //PD3输出低电平
	GPIO_SetBits(RELAY2_PORT,RELAY2_PIN);  //PD4输出低电平
//	GPIO_ResetBits(RELAY3_PORT,RELAY3_PIN);  //PD5输出低电平
//	GPIO_ResetBits(RELAY4_PORT,RELAY4_PIN);  //PD6输出低电平
//	GPIO_ResetBits(RELAY5_PORT,RELAY5_PIN);  //PD7输出低电平
	
}
void s11()
{
	GPIO_ResetBits(RELAY1_PORT,RELAY1_PIN);	// 输出低电平，打开RELAY1
	GPIO_SetBits(RELAY2_PORT,RELAY2_PIN);		// 输出高电平，关闭RELAY1
	//RELAY2_ON;
//	RELAY3_OFF;
//	RELAY4_OFF;
}
void s22()
{
	GPIO_SetBits(RELAY1_PORT,RELAY1_PIN);		// 输出高电平，关闭RELAY1
	GPIO_ResetBits(RELAY2_PORT,RELAY2_PIN);	// 输出低电平，打开RELAY1	
//	RELAY3_OFF;
//	RELAY4_OFF;
}