#ifndef __US_100_H
#define __US_100_H			  	 
#include "sys.h"
#include "delay.h"

#define trig  PEout(0) //
#define echo  PEout(1) //

void Ultrasonic_INIT(void);//��ʼ��
void Timer2_INIT(void);//�����Ƕ�ʱ��2��ʼ������
u32 Ultrasonic(void);//����Ϊ����������������
#endif  
	 
