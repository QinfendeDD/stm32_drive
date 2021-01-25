#ifndef _ad5933_h_
#define _ad5933_h_

#include "sys.h"
#include "myiic.h"
#include "delay.h"
#include <math.h>
#define AD5933_ADDR 0X0D		//AD5933 I2C��ַ
#define AD5933_GAIN (1.549138e-8)		//��Ҫ����У׼
//���ƼĴ���
#define CONTROL_REG_H8 0X80		//���ƼĴ�����8λ��ַ

#define INIT_WITH_START_FREQ 0X10		//���ƣ�����ʼƵ�ʳ�ʼ��
#define START_SCAN 0X20		//���ƣ���ʼɨ��
#define ADD_FREQ 0X30		//���ƣ�Ƶ�ʲ���
#define REPEAT_FREQ 0X40		//���ƣ��ظ�����
#define MEASURE_TEMPERATURE 0X90		//���ƣ������¶�
#define LOW_POWER_MODE 0XA0		//���ƣ�ʡ��ģʽ
#define WATING_MODE 0XB0		//���ƣ�����ģʽ

#define OUTPUT_2_VPP 0X00		//���ƣ����vpp=2v
#define OUTPUT_1_VPP 0X06		//���ƣ����vpp=1v
#define OUTPUT_400_mVPP 0X04		//���ƣ����vpp=400mv
#define OUTPUT_200_mVPP 0X02		//���ƣ����vpp=200mv

#define INPUT_MUL_5 0X00		//���ƣ������źŷŴ�5��
#define INPUT_MUL_1 0X01		//���ƣ������źŲ��Ŵ�

#define CONTROL_REG_L8 0X81		//���ƼĴ�����8λ

#define RESET 0X10		//���ƣ���λ

#define USE_EXTERNAL_XTAL 0X08		//���ƣ�ʹ���ⲿ����
#define USE_INTERNAL_XTAL 0X00		//���ƣ�ʹ���ڲ�����
//��ʼƵ�ʼĴ���
#define XTAL_FREQ 800000		//HZ

#define START_FREQ_REG_H8 0X82		//��ʼƵ�ʼĴ�����8λ��ַ
#define START_FREQ_REG_M8 0X83		//��ʼƵ�ʼĴ�����8λ��ַ
#define START_FREQ_REG_L8 0X84		//��ʼƵ�ʼĴ�����8λ��ַ

#define START_FREQ(X) (uint32_t)((X*4.0/XTAL_FREQ)*134217728)		//������ʼƵ��
//����Ƶ�ʼĴ���
#define FREQ_STEP_REG_H8 0X85		//����Ƶ�ʼĴ�����8λ��ַ
#define FREQ_STEP_REG_M8 0X86		//����Ƶ�ʼĴ�����8λ��ַ
#define FREQ_STEP_REG_L8 0X87		//����Ƶ�ʼĴ�����8λ��ַ

#define STEP_FREQ(X) (uint32_t)((X*4.0/XTAL_FREQ)*134217728)		//���ò���Ƶ��
//���������Ĵ���
#define FREQ_STEP_NUMBER_REG_H8 0X88		//���������Ĵ�����8λ��ַ
#define FREQ_STEP_NUMBER_REG_L8 0X89		//���������Ĵ�����8λ��ַ

#define FREQ_STEP_NUMBER(X) X		//����Ƶ�ʲ���������XҪС��511
//����ʱ�����ڼĴ���
#define STABLE_TIMES_CYCLE_REG_H8 0X8A		//����ʱ�����ڼĴ�����8λ��ַ
#define STABLE_TIMES_CYCLE_REG_L8 0X8B		//����ʱ�����ڼĴ�����8λ��ַ

#define STABLE_TIMES_CYCLE_MUL_1 0x0000		//����ʱ�����ڳ�1		
#define STABLE_TIMES_CYCLE_MUL_2 0X0200		//����ʱ�����ڳ�2
#define STABLE_TIMES_CYCLE_MUL_4 0X0600		//����ʱ�����ڳ�4

#define STABLE_TIMES_CYCLE(X) X		//����ʱ�����ڣ�XҪС��511
//״̬�Ĵ���
#define STATUS_REG 0X8F		//״̬�Ĵ�����ַ		

#define FINISH_MEASURE_TEMPERATURE 0X01		//������¶Ȳ���
#define FINISH_DFT 0X02		//�����DFT
#define FINISH_SCAN 0X04		//�����ɨ��

//�¶ȼĴ���
#define TEMPERATURE_REG_H8 0X92
#define TEMPERATURE_REG_L8 0X93
//ʵ���Ĵ���
#define REAL_DATA_REG_H8 0X94
#define REAL_DATA_REG_L8 0X95

//�鲿�Ĵ���
#define IMAGINARY_DATA_REG_H8 0X96
#define IMAGINARY_DATA_REG_L8 0X97


double FullMeasure(void);
double ContinueMeasure(void);		//�����Ĳ���
double GetAD5933R(double Amp);
double GetAD5933Gain(double KnownR,double Amp);
double ContinueMeasure2(void);		//�����Ĳ�������ȷ��������
uint8_t AD5933ReadData(uint16_t *RealPart,uint16_t *ImagPart);
#endif

