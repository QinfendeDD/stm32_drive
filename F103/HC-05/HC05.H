#ifndef __HC05_H__
#define __HC05_H__

#include "stm32f10x.h"

void Usart3Init(void);
void SentData(uint8_t Data);
void SentDatas(uint8_t *Data);

extern uint8_t Receive;


#endif