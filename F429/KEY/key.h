#ifndef __KEY_H
#define __KEY_H	 
//#include "sys.h" 
#include "stm32f4xx.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//����������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

/*����ķ�ʽ��ͨ��ֱ�Ӳ����⺯����ʽ��ȡIO*/
/*#define KEY0 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_4) //PE4
#define KEY1 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3)	//PE3 
#define KEY2 		GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_2) //PE2
#define KEY3   	GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_5)	//PA0
#define KEY4   	GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_6)	//PA0
#define KEY5   	GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_1)	//PA0
*/
/*���淽ʽ��ͨ��λ��������ʽ��ȡIO*/

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

void KEY_Init(void);	//IO��ʼ��
u8 KEY_Scan(u8);  		//����ɨ�躯��	

#endif
