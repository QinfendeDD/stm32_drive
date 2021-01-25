#ifndef __OLED_H
#define __OLED_H			  	 
#include "sys.h"
#include "stdlib.h"	    
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//OLED 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/4
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	  
#define SDA_IN_OLED() {GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=0<<7*2;} 
#define SDA_OUT_OLED() {GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=1<<7*2;}  
#define READ_SDA_OLED PCin(7)
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define Max_Column	130
#define RES_OLED PGout(8)

#define IIC_SCL_OLED 	PCout(6)//D0
#define IIC_SDA_OLED 	PCout(7)//D1
void OLED_Init_iic(void);//IO配置
void IIC_Start_OLED(void);//iic起始信号
void IIC_Stop_OLED(void);//iic结束信号
void IIC_WriteByte_OLED(u8 data);
void WriteCmd_OLED(u8 command);//iic写命令
void WriteDat_OLED(u8 data);//iic写数据
void OLED_Fill(unsigned char fill_Data);//填充全屏,整个屏幕是白色的
void OLED_DrawBMP(u8 x0,u8 y0,u8 x1,unsigned char y1,u8 BMP[]);//显示图片
void OLED_Init(void);//OLED初始化
void res_OLED(void);//复位信号
void OLED_CLS(void);//清屏
void OLED_Set_Pos(unsigned char x, unsigned char y);//坐标设置
void OLED_Display_On(void);//开启OLED显示
void OLED_Display_Off(void);//关闭OLED显示 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);//显示字符
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);//显示2位数字
void OLED_ShowString(u8 x,u8 y,u8 chr[],u8 Char_Size);//显示字符串
void OLED_ShowCHinese(u8 x,u8 y,u8 no);//显示汉字
void OLED_ShowSZ(unsigned char x, unsigned char y, unsigned char Nn);//显示字符
#endif  
	 



