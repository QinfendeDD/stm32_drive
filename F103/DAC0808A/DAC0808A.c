
#include "DAC0808A.h"

void DAC0808_Data_Config(void)
{		
	unsigned char temp;
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED的外设时钟*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 

		/*选择要控制的GPIOB引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;   

		/*设置引脚速率为50MHz */   
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIOB0*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
	
		temp= GPIOB->ODR &0xff;
	
	  GPIOB->ODR = (0x00<<8) |temp;
		/* 关闭所有led灯	*/
//		GPIO_SetBits(GPIOB, GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);	
}


void KEY_Config(void)
{		
		/*定义一个GPIO_InitTypeDef类型的结构体*/
		GPIO_InitTypeDef GPIO_InitStructure;

		/*开启LED的外设时钟*/
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 

		/*选择要控制的GPIOB引脚*/															   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;	

		/*设置引脚模式为通用推挽输出*/
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //GPIO_Mode_Out_OD;   

		/*设置引脚速率为50MHz */   
		//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 

		/*调用库函数，初始化GPIOB0*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);	
		/* 关闭所有led灯	*/
//		GPIO_SetBits(GPIOB, GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);	
}

void Write_DAC0808_Data(unsigned char data)
{
	unsigned char temp;
	temp= GPIOB->ODR &0xff;
	GPIOB->ODR = (data<<8) | temp;
}

