#ifndef __MOTOR_H
#define __MOTOR_H

#include "sys.h"
void Motor_Init(void);
void nSLEEP_Motor(unsigned char i);
void Speed_Motor(int Speed_Motor_Left_Value,int Speed_Motor_Right_Value);

#endif
