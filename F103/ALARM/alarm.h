#ifndef __ALARM_H_
#define __ALARM_H_
#include "sys.h"

#define ALARM_RCC_BEEP             RCC_APB2Periph_GPIOC
#define ALARM_GPIO_BEEP            GPIOC
#define ALARM_GPIO_Pin_BEEP        GPIO_Pin_1	

#define AlarmBeep             PCout(1)  //PC1

void Alarm_Init(void);
void Alarm_Start(int xs);
void Alarm_Stop(void);

#endif
