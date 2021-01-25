#ifndef __OSCILLOSCOPE_H
#define __OSCILLOSCOPE_H
#include "sys.h"


void clear_point(u16 num);//更新显示屏当前列	
void Lcd_DrawNetwork(void);//画网格
void DrawOscillogram(u16* buf,u8 sample_cycles);//画波形图	
void DrawOscillogram_Position(u16* buf,u8 sample_cycles);//画波形图	——上下移动时用
float get_vpp(u16 *buf);//获取峰峰值
float get_freq(u16 *freq_buff);//获取频率值
void TIM5_PWM_Init(u32 arr,u32 psc);


u8 osc_get_keynum(void);

void Osc_Welcome(void);               //设置背景 OFF  fun63
void Osc_ON(void);                //开ON          fun64
void Osc_Expend_Y(void);          //放大          fun65
void Osc_Shrink_Y(void);          //缩小          fun66
void Osc_Expend_X(void);          //波形展宽      fun67
void Osc_Shrink_X(void);          //波形缩小      fun68
void Osc_UP(void);                //波形上移      fun69
void Osc_Down(void);              //波形下移      fun70



#endif

