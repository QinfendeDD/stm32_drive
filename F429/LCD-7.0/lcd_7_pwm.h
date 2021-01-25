#ifndef _LCD_7_PWM_H
#define	_LCD_7_PWM_H

#include "stm32f4xx.h"

/*---------------------- ���� ------------------------*/

#define	LCD_PWM_PORT			GPIOD
#define	LCD_PWM_CLK				RCC_AHB1Periph_GPIOD
#define  LCD_PWM_PIN				GPIO_Pin_13
#define  LCD_PWM_AF           GPIO_AF_TIM4
#define  LCD_PWM_PinSource    GPIO_PinSource13

#define	LCD_TIM_CLK				RCC_APB1Periph_TIM4
#define  LCD_TIM  	   		TIM4

/*-------------------- �������� ----------------------*/

void LCD_PWMinit(u8 pulse);		 //PWM��ʼ��
void LCD_PwmSetPulse (u8 pulse);	 //����ռ�ձ�
	
#endif 

