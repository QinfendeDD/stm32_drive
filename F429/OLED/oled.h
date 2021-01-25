#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//OLED ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/4
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	  
#define SDA_IN_OLED() {GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=0<<7*2;} 
#define SDA_OUT_OLED() {GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=1<<7*2;}  
#define READ_SDA_OLED PCin(7)
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
#define Max_Column	130
#define RES_OLED PGout(8)

#define IIC_SCL_OLED 	PCout(6)//D0
#define IIC_SDA_OLED 	PCout(7)//D1
void OLED_Init_iic(void);//IO����
void IIC_Start_OLED(void);//iic��ʼ�ź�
void IIC_Stop_OLED(void);//iic�����ź�
void IIC_WriteByte_OLED(u8 data);
void WriteCmd_OLED(u8 command);//iicд����
void WriteDat_OLED(u8 data);//iicд����
void OLED_Fill(unsigned char fill_Data);//���ȫ��,������Ļ�ǰ�ɫ��
void OLED_DrawBMP(u8 x0,u8 y0,u8 x1,unsigned char y1,u8 BMP[]);//��ʾͼƬ
void OLED_Init(void);//OLED��ʼ��
void res_OLED(void);//��λ�ź�
void OLED_CLS(void);//����
void OLED_Set_Pos(unsigned char x, unsigned char y);//��������
void OLED_Display_On(void);//����OLED��ʾ
void OLED_Display_Off(void);//�ر�OLED��ʾ 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);//��ʾ�ַ�
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);//��ʾ2λ����
void OLED_ShowString(u8 x,u8 y,u8 chr[],u8 Char_Size);//��ʾ�ַ���
void OLED_ShowCHinese(u8 x,u8 y,u8 no);//��ʾ����
void OLED_ShowSZ(unsigned char x, unsigned char y, unsigned char Nn);//��ʾ�ַ�
#endif  
	 



