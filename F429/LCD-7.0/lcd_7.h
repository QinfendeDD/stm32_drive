#ifndef __LCD_7_H
#define	__LCD_7_H

#include "stm32f4xx.h"
#include "sdram.h"
#include "lcd_7_fonts.h"
#include "usart.h"
#include "delay.h"

#define 	LCD_CLK  		25		//����LCD����ʱ�ӣ�����Ϊ�˷��������ֵӦ��10-70֮�䣬��λΪM
#define 	LCD_NUM_LAYERS  2		//������ʾ�Ĳ�����429������������ʾ

#define	ColorMode_0   LCD_RGB565   		//�����0����ɫ��ʽ
//#define	ColorMode_0   LCD_ARGB1555   
//#define	ColorMode_0   LCD_RGB888  
//#define	ColorMode_0   LCD_ARGB8888   

#if  LCD_NUM_LAYERS == 2						
//	#define	ColorMode_1   LCD_RGB565   	//�����1����ɫ��ʽ
//	#define	ColorMode_1   LCD_ARGB1555   
// #define	ColorMode_1   LCD_RGB888   
	#define	ColorMode_1   LCD_ARGB8888   
#endif

#define	Mode_H	0		//LCDˮƽ��ʾ
#define	Mode_V	1		//LCD������ʾ
#define  Fill_Zero  0	//���0
#define  Fill_Space 1	//���ո�

#define	LCD_ARGB1555    3   //������ɫ��ʽ
#define	LCD_RGB565      2   
#define	LCD_RGB888      1   
#define	LCD_ARGB8888    0  

/*-------------------------- ������ɫ ------------------------------*/

#define 	LCD_WHITE       0xffFFFFFF
#define 	LCD_BLACK       0xff000000
                           
#define 	LCD_BLUE        0xff0000FF
#define 	LCD_GREEN       0xff00FF00
#define 	LCD_RED         0xffFF0000
#define 	LCD_CYAN        0xff00FFFF
#define 	LCD_MAGENTA     0xffFF00FF
#define 	LCD_YELLOW      0xffFFFF00
#define 	LCD_GREY        0xff2C2C2C
                           
#define 	LIGHT_BLUE      0xff8080FF
#define 	LIGHT_GREEN     0xff80FF80
#define 	LIGHT_RED       0xffFF8080
#define 	LIGHT_CYAN      0xff80FFFF
#define 	LIGHT_MAGENTA   0xffFF80FF
#define 	LIGHT_YELLOW    0xffFFFF80
#define 	LIGHT_GREY      0xffA3A3A3
                           
#define 	DARK_BLUE       0xff000080
#define 	DARK_GREEN      0xff008000
#define 	DARK_RED        0xff800000
#define 	DARK_CYAN       0xff008080
#define 	DARK_MAGENTA    0xff800080
#define 	DARK_YELLOW     0xff808000
#define 	DARK_GREY       0xff404040

/*-------------------------- �������� --------------------------------*/
	
void  LCD_Init(void);  //LCD��ʼ��
void  LCD_Clear(void); //����

void  LCD_SetLayer(u8 Layerx); 					//���ò�
void  LCD_SetColor(u32 Color); 				   //���û�����ɫ
void  LCD_SetBackColor(u32 Color);  			//���ñ�����ɫ
void  LCD_SetFont(pFONT *fonts);					//��������
void 	LCD_DisplayMode(u8 direction);  			//������ʾ����

void 	LCD_DisplayChar(u16 x, u16 y,u8 add);							//��ʾ�����ַ�
void 	LCD_DisplayString( u16 x, u16 y, u8 *p);	 					//��ʾ�ַ���
void  LCD_ShowNumMode(u8 mode);
void  LCD_DisplayNumber( u16 x, u16 y, u32 number, u8 len) ;   //��ʾʮ������
void  LCD_DisplayNumberyoufuhao( u16 x, u16 y, u32 number,u16 num, u8 len); //��ʾ�з���ʮ������
void  LCD_DrawPoint(u16 x,u16 y,u32 color);   				//����
u32 	LCD_ReadPoint(u16 x,u16 y);								//����
void  LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);		//����
void  LCD_DrawRect(u16 x, u16 y, u16 width, u16 height);	//������
void  LCD_DrawCircle(u16 x, u16 y, u16 r);					//��Բ
void  LCD_DrawEllipse(int x, int y, int r1, int r2);		//����Բ

void  LCD_FillRect(u16 x, u16 y, u16 width, u16 height);		//������
void  LCD_FillCircle(u16 x, u16 y, u16 r);						//���Բ


/*-------------------------LCD��ز����궨��----------------------------*/

#define HBP  46
#define VBP  23
#define HSW  1000
#define VSW  600
#define HFP  200
#define VFP  22

#define LCD_Width     800				//LCD�����س���
#define LCD_Height    480				//LCD�����ؿ��
#define LCD_Pixels    800*480 		//�ֱ���
#define LCD_MemoryAdd   0xD0000000 	//�Դ����ʼ��ַ  


#if ( ColorMode_0 == LCD_RGB565 || ColorMode_0 == LCD_ARGB1555 )
	#define BytesPerPixel_0		2		//16λɫģʽÿ������ռ2�ֽ�
#elif ColorMode_0 == LCD_RGB888
	#define BytesPerPixel_0		3		//24λɫģʽÿ������ռ3�ֽ�
#else
	#define BytesPerPixel_0		4		//32λɫģʽÿ������ռ4�ֽ�
#endif	

#if LCD_NUM_LAYERS == 2

	#if ( ColorMode_1 == LCD_RGB565 || ColorMode_1 == LCD_ARGB1555 )
		#define BytesPerPixel_1		2	//16λɫģʽÿ������ռ2�ֽ�
	#elif ColorMode_1 == LCD_RGB888	
		#define BytesPerPixel_1		3	//24λɫģʽÿ������ռ3�ֽ�
	#else	
		#define BytesPerPixel_1		4	//32λɫģʽÿ������ռ4�ֽ�
	#endif	

	#define LCD_MemoryAdd_OFFSET   LCD_Pixels * BytesPerPixel_0 	 //�ڶ�����Դ��ƫ�Ƶ�ַ 
	#define LCD_Buffer	LCD_MemoryAdd + LCD_MemoryAdd_OFFSET + LCD_Pixels * BytesPerPixel_1	//��˫��ʱ����������ʼ��ַ
#else
	#define LCD_Buffer	LCD_MemoryAdd + LCD_Pixels * BytesPerPixel_0	//ֻ������ʱ����������ʼ��ַ
#endif

/*--------------------------���Ŷ���--------------------------------*/

//IO��ʱ��
#define LCD_GPIO_CLK	 RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD \
							 | RCC_AHB1Periph_GPIOJ | RCC_AHB1Periph_GPIOK | RCC_AHB1Periph_GPIOI

//��ɫ������
#define LTDC_R0_PORT        	GPIOI
#define LTDC_R0_PIN         	GPIO_Pin_15
#define LTDC_R0_PINSOURCE     GPIO_PinSource15

#define LTDC_R1_PORT        	GPIOJ
#define LTDC_R1_PIN         	GPIO_Pin_0
#define LTDC_R1_PINSOURCE     GPIO_PinSource0

#define LTDC_R2_PORT        	GPIOJ
#define LTDC_R2_PIN         	GPIO_Pin_1
#define LTDC_R2_PINSOURCE     GPIO_PinSource1

#define LTDC_R3_PORT        	GPIOJ
#define LTDC_R3_PIN         	GPIO_Pin_2
#define LTDC_R3_PINSOURCE     GPIO_PinSource2

#define LTDC_R4_PORT        	GPIOJ
#define LTDC_R4_PIN         	GPIO_Pin_3
#define LTDC_R4_PINSOURCE     GPIO_PinSource3

#define LTDC_R5_PORT        	GPIOJ
#define LTDC_R5_PIN         	GPIO_Pin_4
#define LTDC_R5_PINSOURCE     GPIO_PinSource4

#define LTDC_R6_PORT        	GPIOJ
#define LTDC_R6_PIN         	GPIO_Pin_5
#define LTDC_R6_PINSOURCE     GPIO_PinSource5

#define LTDC_R7_PORT        	GPIOJ
#define LTDC_R7_PIN         	GPIO_Pin_6
#define LTDC_R7_PINSOURCE     GPIO_PinSource6

//��ɫ������
#define LTDC_G0_PORT        	GPIOJ
#define LTDC_G0_PIN         	GPIO_Pin_7
#define LTDC_G0_PINSOURCE     GPIO_PinSource7

#define LTDC_G1_PORT        	GPIOJ
#define LTDC_G1_PIN         	GPIO_Pin_8
#define LTDC_G1_PINSOURCE     GPIO_PinSource8

#define LTDC_G2_PORT        	GPIOJ
#define LTDC_G2_PIN         	GPIO_Pin_9
#define LTDC_G2_PINSOURCE     GPIO_PinSource9

#define LTDC_G3_PORT        	GPIOJ
#define LTDC_G3_PIN         	GPIO_Pin_10
#define LTDC_G3_PINSOURCE     GPIO_PinSource10

#define LTDC_G4_PORT        	GPIOJ
#define LTDC_G4_PIN         	GPIO_Pin_11
#define LTDC_G4_PINSOURCE     GPIO_PinSource11

#define LTDC_G5_PORT        	GPIOK
#define LTDC_G5_PIN         	GPIO_Pin_0
#define LTDC_G5_PINSOURCE     GPIO_PinSource0

#define LTDC_G6_PORT        	GPIOK
#define LTDC_G6_PIN         	GPIO_Pin_1
#define LTDC_G6_PINSOURCE     GPIO_PinSource1

#define LTDC_G7_PORT        	GPIOK
#define LTDC_G7_PIN         	GPIO_Pin_2
#define LTDC_G7_PINSOURCE     GPIO_PinSource2

//��ɫ������
#define LTDC_B0_PORT        	GPIOJ
#define LTDC_B0_PIN         	GPIO_Pin_12
#define LTDC_B0_PINSOURCE     GPIO_PinSource12

#define LTDC_B1_PORT       	GPIOJ
#define LTDC_B1_PIN           GPIO_Pin_13
#define LTDC_B1_PINSOURCE     GPIO_PinSource13

#define LTDC_B2_PORT        	GPIOJ
#define LTDC_B2_PIN         	GPIO_Pin_14
#define LTDC_B2_PINSOURCE     GPIO_PinSource14

#define LTDC_B3_PORT        	GPIOJ
#define LTDC_B3_PIN         	GPIO_Pin_15
#define LTDC_B3_PINSOURCE     GPIO_PinSource15

#define LTDC_B4_PORT        	GPIOK
#define LTDC_B4_PIN         	GPIO_Pin_3
#define LTDC_B4_PINSOURCE     GPIO_PinSource3

#define LTDC_B5_PORT        	GPIOK
#define LTDC_B5_PIN         	GPIO_Pin_4
#define LTDC_B5_PINSOURCE     GPIO_PinSource4

#define LTDC_B6_PORT        	GPIOK
#define LTDC_B6_PIN         	GPIO_Pin_5
#define LTDC_B6_PINSOURCE     GPIO_PinSource5

#define LTDC_B7_PORT        	GPIOK
#define LTDC_B7_PIN         	GPIO_Pin_6
#define LTDC_B7_PINSOURCE     GPIO_PinSource6

//�����ź���
#define LTDC_CLK_PORT         GPIOI					//LCDʱ������
#define LTDC_CLK_PIN          GPIO_Pin_14
#define LTDC_CLK_PINSOURCE    GPIO_PinSource14

#define LTDC_HSYNC_PORT       GPIOI					//��ͬ��
#define LTDC_HSYNC_PIN        GPIO_Pin_12
#define LTDC_HSYNC_PINSOURCE  GPIO_PinSource12

#define LTDC_VSYNC_PORT       GPIOI					//֡ͬ��
#define LTDC_VSYNC_PIN        GPIO_Pin_13
#define LTDC_VSYNC_PINSOURCE  GPIO_PinSource13

#define LTDC_DE_PORT          GPIOK					//����ʹ��
#define LTDC_DE_PIN           GPIO_Pin_7
#define LTDC_DE_PINSOURCE     GPIO_PinSource7
	
#define LTDC_Black_PORT       GPIOD					//����
#define LTDC_Black_PIN        GPIO_Pin_13
  
// AT070TN83���ݽ�
#define AT070TN83_MODE_PORT        	GPIOI						//MODE
#define AT070TN83_MODE_CLK         	RCC_AHB1Periph_GPIOI
#define AT070TN83_MODE_PIN         	GPIO_Pin_11
#define AT070TN83_MODE_PINSOURCE    GPIO_PinSource11

#define AT070TN83_LR_PORT         GPIOC						//ˮƽɨ�跽��
#define AT070TN83_LR_CLK          RCC_AHB1Periph_GPIOC
#define AT070TN83_LR_PIN          GPIO_Pin_13
#define AT070TN83_LR_PINSOURCE    GPIO_PinSource13

#define AT070TN83_UD_PORT         GPIOB						//��ֱɨ�跽��
#define AT070TN83_UD_CLK          RCC_AHB1Periph_GPIOB
#define AT070TN83_UD_PIN          GPIO_Pin_2
#define AT070TN83_UD_PINSOURCE    GPIO_PinSource2

#endif 
