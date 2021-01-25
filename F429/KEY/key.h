#ifndef __KEY_H
#define __KEY_H	 
//#include "sys.h" 
#include "stm32f4xx.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//按键输入驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

/*下面的方式是通过直接操作库函数方式读取IO*/
/*#define KEY0 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4) //PE4
#define KEY1 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3)	//PE3 
#define KEY2 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_2) //PE2
#define KEY3   	GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_5)	//PA0
#define KEY4   	GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6)	//PA0
#define KEY5   	GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_1)	//PA0
*/
/*下面方式是通过位带操作方式读取IO*/

#define KEY0 		PFin(4)   //PE4
#define KEY1 		PFin(3)		//PE3 
#define KEY2 		PFin(2)		//P32
#define KEY3 	  PFin(5)		//PA0
#define KEY4 		PFin(6)		//P32
#define KEY5 	  PFin(7)		//PA0

//#define KEY0_PRES 	1
//#define KEY1_PRES	  2
//#define KEY2_PRES	  3
//#define KEY3_PRES   4
//#define KEY4_PRES   5
//#define KEY5_PRES   6

void KEY_Init(void);	//IO初始化
u8 KEY_Scan(u8);  		//按键扫描函数	

#endif
