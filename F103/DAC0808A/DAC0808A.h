#ifndef __DAC0808_H
#define	__DAC0808_H

#include "stm32f10x.h"
#include "delay.h"


void KEY_Config(void);
void Write_DAC0808_Data(unsigned char data);
void DAC0808_Data_Config(void);

#endif /* __EXTI_H */
