#ifndef _PID_DEF
#define _PID_DEF

#include "stm32f10x.h"





void Xianfu_Pwm(int *PWM_value);
void PWM_abs(int *number);
int Position_PID(int Feedback_value,int User_Target);
int Increment_PI(int Feedback_value,int User_Target);
#endif


