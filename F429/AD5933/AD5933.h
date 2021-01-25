#ifndef _ad5933_h_
#define _ad5933_h_

#include "sys.h"
#include "myiic.h"
#include "delay.h"
#include <math.h>
#define AD5933_ADDR 0X0D		//AD5933 I2C地址
#define AD5933_GAIN (1.549138e-8)		//需要经过校准
//控制寄存器
#define CONTROL_REG_H8 0X80		//控制寄存器高8位地址

#define INIT_WITH_START_FREQ 0X10		//控制：以起始频率初始化
#define START_SCAN 0X20		//控制：开始扫描
#define ADD_FREQ 0X30		//控制：频率步进
#define REPEAT_FREQ 0X40		//控制：重复测量
#define MEASURE_TEMPERATURE 0X90		//控制：测量温度
#define LOW_POWER_MODE 0XA0		//控制：省电模式
#define WATING_MODE 0XB0		//控制：待机模式

#define OUTPUT_2_VPP 0X00		//控制：输出vpp=2v
#define OUTPUT_1_VPP 0X06		//控制：输出vpp=1v
#define OUTPUT_400_mVPP 0X04		//控制：输出vpp=400mv
#define OUTPUT_200_mVPP 0X02		//控制：输出vpp=200mv

#define INPUT_MUL_5 0X00		//控制：输入信号放大5倍
#define INPUT_MUL_1 0X01		//控制：输入信号不放大

#define CONTROL_REG_L8 0X81		//控制寄存器低8位

#define RESET 0X10		//控制：复位

#define USE_EXTERNAL_XTAL 0X08		//控制：使用外部晶振
#define USE_INTERNAL_XTAL 0X00		//控制：使用内部晶振
//起始频率寄存器
#define XTAL_FREQ 800000		//HZ

#define START_FREQ_REG_H8 0X82		//起始频率寄存器高8位地址
#define START_FREQ_REG_M8 0X83		//起始频率寄存器中8位地址
#define START_FREQ_REG_L8 0X84		//起始频率寄存器低8位地址

#define START_FREQ(X) (uint32_t)((X*4.0/XTAL_FREQ)*134217728)		//设置起始频率
//步进频率寄存器
#define FREQ_STEP_REG_H8 0X85		//步进频率寄存器高8位地址
#define FREQ_STEP_REG_M8 0X86		//步进频率寄存器中8位地址
#define FREQ_STEP_REG_L8 0X87		//步进频率寄存器低8位地址

#define STEP_FREQ(X) (uint32_t)((X*4.0/XTAL_FREQ)*134217728)		//设置步进频率
//步进次数寄存器
#define FREQ_STEP_NUMBER_REG_H8 0X88		//步进次数寄存器高8位地址
#define FREQ_STEP_NUMBER_REG_L8 0X89		//步进次数寄存器低8位地址

#define FREQ_STEP_NUMBER(X) X		//设置频率步进次数，X要小于511
//建立时间周期寄存器
#define STABLE_TIMES_CYCLE_REG_H8 0X8A		//建立时间周期寄存器高8位地址
#define STABLE_TIMES_CYCLE_REG_L8 0X8B		//建立时间周期寄存器低8位地址

#define STABLE_TIMES_CYCLE_MUL_1 0x0000		//建立时间周期乘1		
#define STABLE_TIMES_CYCLE_MUL_2 0X0200		//建立时间周期乘2
#define STABLE_TIMES_CYCLE_MUL_4 0X0600		//建立时间周期乘4

#define STABLE_TIMES_CYCLE(X) X		//建立时间周期，X要小于511
//状态寄存器
#define STATUS_REG 0X8F		//状态寄存器地址		

#define FINISH_MEASURE_TEMPERATURE 0X01		//已完成温度测量
#define FINISH_DFT 0X02		//已完成DFT
#define FINISH_SCAN 0X04		//已完成扫描

//温度寄存器
#define TEMPERATURE_REG_H8 0X92
#define TEMPERATURE_REG_L8 0X93
//实部寄存器
#define REAL_DATA_REG_H8 0X94
#define REAL_DATA_REG_L8 0X95

//虚部寄存器
#define IMAGINARY_DATA_REG_H8 0X96
#define IMAGINARY_DATA_REG_L8 0X97


double FullMeasure(void);
double ContinueMeasure(void);		//连续的测量
double GetAD5933R(double Amp);
double GetAD5933Gain(double KnownR,double Amp);
double ContinueMeasure2(void);		//连续的测量，不确定可用性
uint8_t AD5933ReadData(uint16_t *RealPart,uint16_t *ImagPart);
#endif

