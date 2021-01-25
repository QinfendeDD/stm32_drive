#ifndef __RELAY_H
#define __RELAY_H		
/*************************************************************************************
************************ͷ�ļ�********************************************************
**************************************************************************************/
#include "sys.h"
#include "delay.h"
#include "stm32f4xx.h"
/*---------------------- LED���ú� ------------------------*/

#define RELAY1_PIN             GPIO_Pin_3        		  // RELAY1 ����      
#define RELAY1_PORT            GPIOD                  // RELAY1 GPIO�˿�     
#define RELAY1_CLK             RCC_AHB1Periph_GPIOD	  // RELAY1 GPIO�˿�ʱ��

#define RELAY2_PIN             GPIO_Pin_4        		  // RELAY1 ����      
#define RELAY2_PORT            GPIOD                  // RELAY1 GPIO�˿�     
#define RELAY2_CLK             RCC_AHB1Periph_GPIOD	  // RELAY1 GPIO�˿�ʱ��

#define RELAY3_PIN             GPIO_Pin_5        		  // RELAY1 ����      
#define RELAY3_PORT            GPIOD                  // RELAY1 GPIO�˿�     
#define RELAY3_CLK             RCC_AHB1Periph_GPIOD	  // RELAY1 GPIO�˿�ʱ��

#define RELAY4_PIN             GPIO_Pin_6        		  // RELAY1 ����      
#define RELAY4_PORT            GPIOD                  // RELAY1 GPIO�˿�     
#define RELAY4_CLK             RCC_AHB1Periph_GPIOD	  // RELAY1 GPIO�˿�ʱ��

#define RELAY5_PIN             GPIO_Pin_7        		  // RELAY1 ����      
#define RELAY5_PORT            GPIOD                  // RELAY1 GPIO�˿�     
#define RELAY5_CLK             RCC_AHB1Periph_GPIOD	  // RELAY1 GPIO�˿�ʱ��

/*---------------------- LED���ƺ� ------------------------*/
					
#define RELAY1_ON 	  GPIO_ResetBits(RELAY1_PORT,RELAY1_PIN);	// ����͵�ƽ����RELAY1	
#define RELAY1_OFF 	  GPIO_SetBits(RELAY1_PORT,RELAY1_PIN);		// ����ߵ�ƽ���ر�RELAY1	

#define RELAY2_ON 	  GPIO_ResetBits(RELAY2_PORT,RELAY2_PIN);	// ����͵�ƽ����RELAY1	
#define RELAY2_OFF 	  GPIO_SetBits(RELAY2_PORT,RELAY2_PIN);		// ����ߵ�ƽ���ر�RELAY1

#define RELAY3_ON 	  GPIO_ResetBits(RELAY3_PORT,RELAY3_PIN);	// ����͵�ƽ����RELAY1	
#define RELAY3_OFF 	  GPIO_SetBits(RELAY3_PORT,RELAY3_PIN);		// ����ߵ�ƽ���ر�RELAY1

#define RELAY4_ON 	  GPIO_ResetBits(RELAY4_PORT,RELAY4_PIN);	// ����͵�ƽ����RELAY1	
#define RELAY4_OFF 	  GPIO_SetBits(RELAY4_PORT,RELAY4_PIN);		// ����ߵ�ƽ���ر�RELAY1

#define RELAY5_ON 	  GPIO_ResetBits(RELAY5_PORT,RELAY5_PIN);	// ����͵�ƽ����RELAY1	
#define RELAY5_OFF 	  GPIO_SetBits(RELAY5_PORT,RELAY5_PIN);		// ����ߵ�ƽ���ر�RELAY1

/*---------------------- �������� ----------------------------*/
void s11(void);
void s22(void);
/************************************************************************************
********************************��������*********************************************
*************************************************************************************/
void Relay_Init(void);

#endif  
