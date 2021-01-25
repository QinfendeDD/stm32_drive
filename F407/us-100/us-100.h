#ifndef __US_100_H
#define __US_100_H			  	 
#include "sys.h"
#include "delay.h"

#define trig  PEout(0) //
#define echo  PEout(1) //

void Ultrasonic_INIT(void);//初始化
void Timer2_INIT(void);//以下是定时器2初始化设置
u32 Ultrasonic(void);//以下为超声波测量的数据
#endif  
	 
