#ifndef __OSCILLOSCOPE_H
#define __OSCILLOSCOPE_H
#include "sys.h"


void clear_point(u16 num);//������ʾ����ǰ��	
void Lcd_DrawNetwork(void);//������
void DrawOscillogram(u16* buf,u8 sample_cycles);//������ͼ	
void DrawOscillogram_Position(u16* buf,u8 sample_cycles);//������ͼ	���������ƶ�ʱ��
float get_vpp(u16 *buf);//��ȡ���ֵ
float get_freq(u16 *freq_buff);//��ȡƵ��ֵ
void TIM5_PWM_Init(u32 arr,u32 psc);


u8 osc_get_keynum(void);

void Osc_Welcome(void);               //���ñ��� OFF  fun63
void Osc_ON(void);                //��ON          fun64
void Osc_Expend_Y(void);          //�Ŵ�          fun65
void Osc_Shrink_Y(void);          //��С          fun66
void Osc_Expend_X(void);          //����չ��      fun67
void Osc_Shrink_X(void);          //������С      fun68
void Osc_UP(void);                //��������      fun69
void Osc_Down(void);              //��������      fun70



#endif

