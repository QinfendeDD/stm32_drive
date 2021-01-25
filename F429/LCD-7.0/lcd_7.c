/***
	***************************************************************************************
	*	@file  	lcd.c
	*	@brief   ʹ��STM32F29����Ŀ���������Һ������������ֲ�ڹٷ� STM32F429I_DISCOVERY 
	*				ʵ�������̣���������Ӧ���޸��Ż�ʹ֮����ʵ������
   ***************************************************************************************
   *  @description
	*	
	*	�������ݵĲ�������Ϊ���ֱ���800*480��FPC���߳���Ϊ20cm����ν�������Ƶ����ָ��ʾ����������������
	*	��ʱ���޻���������ʹ�ø��ߵ�����Ƶ�ʿ��ܻ�������ʱ��ɶ��ݵĻ�����ֻ��������ʾ��ʹ�ø��̵�����
	*	��������ʹ�ø��ߵ�����Ƶ�ʣ���ز�����lcd_rgb.h��������á�
	*	1.ֻ��������ʾ�£�16λɫģʽ�������Ƶ��70M
	*	2.ֻ��������ʾ�£�24λɫģʽ�������Ƶ��52M
	*	3.ֻ��������ʾ�£�32λɫģʽ�������Ƶ��38M
	*	4.��˫����ʾ�£�RGB565��ARGB1555ģʽ�£��������Ƶ��40M
	*	5.��˫����ʾ�£�RGB565��ARGB8888ģʽ�£��������Ƶ��25M
	*  5.��˫����ʾ�£�RGB888��ARGB1555ģʽ�£��������Ƶ��36M
	*	5.��˫����ʾ�£�RGB888��ARGB8888ģʽ�£��������Ƶ��22M
	*	
	*	����˵����
	*	1.STM32F29��͸����������������ʾ��֮��
	*	2.�ڷֱ���Ϊ800*480ʱ������Ƶ����33M���ҾͿ��Դﵽ60֡��ˢ���ʣ�һ��ʹ�����Ƶ�ʾ��㹻��
	*	3.������ʾ�£�ǰ����Ӧ����Ϊ��͸��ɫ����ɫģʽ����Ȼ���޷����������㣬�Ƽ�ʹ��RGB565+ARGB8888�Ĵ���
	*	4.��Ļ�ڸ����ػ��߸��ϵ��ʱ������΢����˸������������Ƭ�̾ͻ���ʧ��ʹ��Խ�͵�����Ƶ����˸Խ����  
	*	5.��lcd_rgb.h�ﶨ����һЩ���õ���ɫ����8λΪ͸��ɫ��ֻ��ARGB8888��ARGB1555ģʽ����Ч��ARGB1555ֻ��
	*	  �ڸ�8λΪ0ʱ�Ż���ʾ͸����
	******************************************************************************************
***/

#include "lcd_7.h"

static pFONT *LCD_Fonts;

//LCD��ز����ṹ��
struct	
{
	u32 Color;  //LCD��ǰ������ɫ
	u32 BackColor;	//����ɫ
	u32 LayerMemoryAdd;	//���Դ��ַ
	u8  Layer; //��ǰ��
	u8  Direction;	//��ʾ����
	u8  BufferFlag;  //�����ʹ�ñ�־
	u8  ShowNum_Mode;
}LCD;

      							
// ������IO�ڳ�ʼ��
//
void LCD_GPIO_Config(void)
{ 
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd( LCD_GPIO_CLK,ENABLE); 	
						                          
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

	// LCD ��ɫ R �������� 
//	GPIO_PinAFConfig(LTDC_R0_PORT, LTDC_R0_PINSOURCE, GPIO_AF_LTDC);	//AT070TN83û�е�2λ������
//	GPIO_PinAFConfig(LTDC_R1_PORT, LTDC_R1_PINSOURCE, GPIO_AF_LTDC);	
	GPIO_PinAFConfig(LTDC_R2_PORT, LTDC_R2_PINSOURCE, GPIO_AF_LTDC);	
	GPIO_PinAFConfig(LTDC_R3_PORT, LTDC_R3_PINSOURCE, GPIO_AF_LTDC);	
	GPIO_PinAFConfig(LTDC_R4_PORT, LTDC_R4_PINSOURCE, GPIO_AF_LTDC);	
	GPIO_PinAFConfig(LTDC_R5_PORT, LTDC_R5_PINSOURCE, GPIO_AF_LTDC);	
	GPIO_PinAFConfig(LTDC_R6_PORT, LTDC_R6_PINSOURCE, GPIO_AF_LTDC);
	GPIO_PinAFConfig(LTDC_R7_PORT, LTDC_R7_PINSOURCE, GPIO_AF_LTDC);	

	GPIO_InitStruct.GPIO_Pin = LTDC_R0_PIN;	
	GPIO_Init(LTDC_R0_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_R1_PIN;
	GPIO_Init(LTDC_R1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_R2_PIN;
	GPIO_Init(LTDC_R2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_R3_PIN;
	GPIO_Init(LTDC_R3_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_R4_PIN;
	GPIO_Init(LTDC_R4_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_R5_PIN;
	GPIO_Init(LTDC_R5_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_R6_PIN;
	GPIO_Init(LTDC_R6_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_R7_PIN;
	GPIO_Init(LTDC_R7_PORT, &GPIO_InitStruct);

  
  // LCD ��ɫ G �������� 
  
//	GPIO_PinAFConfig(LTDC_G0_PORT, LTDC_G0_PINSOURCE, GPIO_AF_LTDC);  //AT070TN83û�е�2λ������
//	GPIO_PinAFConfig(LTDC_G1_PORT, LTDC_G1_PINSOURCE, GPIO_AF_LTDC); 
	GPIO_PinAFConfig(LTDC_G2_PORT, LTDC_G2_PINSOURCE, GPIO_AF_LTDC); 
	GPIO_PinAFConfig(LTDC_G3_PORT, LTDC_G3_PINSOURCE, GPIO_AF_LTDC);  
	GPIO_PinAFConfig(LTDC_G4_PORT, LTDC_G4_PINSOURCE, GPIO_AF_LTDC);  
	GPIO_PinAFConfig(LTDC_G5_PORT, LTDC_G5_PINSOURCE, GPIO_AF_LTDC);  
	GPIO_PinAFConfig(LTDC_G6_PORT, LTDC_G6_PINSOURCE, GPIO_AF_LTDC);  
	GPIO_PinAFConfig(LTDC_G7_PORT, LTDC_G7_PINSOURCE, GPIO_AF_LTDC);  
  
	GPIO_InitStruct.GPIO_Pin = LTDC_G0_PIN;
	GPIO_Init(LTDC_G0_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_G1_PIN;
	GPIO_Init(LTDC_G1_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_G2_PIN;
	GPIO_Init(LTDC_G2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_G3_PIN;
	GPIO_Init(LTDC_G3_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_G4_PIN;
	GPIO_Init(LTDC_G4_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_G5_PIN;
	GPIO_Init(LTDC_G5_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_G6_PIN;
	GPIO_Init(LTDC_G6_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_G7_PIN;
	GPIO_Init(LTDC_G7_PORT, &GPIO_InitStruct);

  
  // LCD ��ɫ B �������� 
  
//	GPIO_PinAFConfig(LTDC_B0_PORT, LTDC_B0_PINSOURCE, GPIO_AF_LTDC);  //AT070TN83û�е�2λ������
//	GPIO_PinAFConfig(LTDC_B1_PORT, LTDC_B1_PINSOURCE, GPIO_AF_LTDC); 	
	GPIO_PinAFConfig(LTDC_B2_PORT, LTDC_B2_PINSOURCE, GPIO_AF_LTDC);  
	GPIO_PinAFConfig(LTDC_B3_PORT, LTDC_B3_PINSOURCE, GPIO_AF_LTDC);
	GPIO_PinAFConfig(LTDC_B4_PORT, LTDC_B4_PINSOURCE, GPIO_AF_LTDC);  
	GPIO_PinAFConfig(LTDC_B5_PORT, LTDC_B5_PINSOURCE, GPIO_AF_LTDC);  
	GPIO_PinAFConfig(LTDC_B6_PORT, LTDC_B6_PINSOURCE, GPIO_AF_LTDC);  
	GPIO_PinAFConfig(LTDC_B7_PORT, LTDC_B7_PINSOURCE, GPIO_AF_LTDC);  

	GPIO_InitStruct.GPIO_Pin = LTDC_B0_PIN;
	GPIO_Init(LTDC_B0_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = LTDC_B1_PIN;
	GPIO_Init(LTDC_B1_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = LTDC_B2_PIN;
	GPIO_Init(LTDC_B2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_B3_PIN;
	GPIO_Init(LTDC_B3_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_B4_PIN;
	GPIO_Init(LTDC_B4_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_B5_PIN;
	GPIO_Init(LTDC_B5_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_B6_PIN;
	GPIO_Init(LTDC_B6_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_B7_PIN;
	GPIO_Init(LTDC_B7_PORT, &GPIO_InitStruct);

  
  //������
	GPIO_PinAFConfig(LTDC_CLK_PORT,   LTDC_CLK_PINSOURCE,  GPIO_AF_LTDC); 
	GPIO_PinAFConfig(LTDC_HSYNC_PORT, LTDC_HSYNC_PINSOURCE,GPIO_AF_LTDC);  
	GPIO_PinAFConfig(LTDC_VSYNC_PORT, LTDC_VSYNC_PINSOURCE,GPIO_AF_LTDC);  
	GPIO_PinAFConfig(LTDC_DE_PORT,    LTDC_DE_PINSOURCE,   GPIO_AF_LTDC);
    
	GPIO_InitStruct.GPIO_Pin = LTDC_CLK_PIN;		//����ʱ��
	GPIO_Init(LTDC_CLK_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_HSYNC_PIN;	//ˮƽͬ��
	GPIO_Init(LTDC_HSYNC_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_VSYNC_PIN;	//��ֱͬ��
	GPIO_Init(LTDC_VSYNC_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_DE_PIN;		//����ʹ��
	GPIO_Init(LTDC_DE_PORT, &GPIO_InitStruct);
			
	//����
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = LTDC_Black_PIN; 
	GPIO_Init(LTDC_Black_PORT, &GPIO_InitStruct);

	GPIO_SetBits(LTDC_Black_PORT,LTDC_Black_PIN);
	
	//AT070TN83������
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15; 			//����83�ĵ�38��
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = AT070TN83_MODE_PIN; 		//MODE��
	GPIO_Init(AT070TN83_MODE_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = AT070TN83_LR_PIN; 		//ˮƽɨ�跽��
	GPIO_Init(AT070TN83_LR_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = AT070TN83_UD_PIN; 		//��ֱɨ�跽��
	GPIO_Init(AT070TN83_UD_PORT, &GPIO_InitStruct);

	GPIO_SetBits(AT070TN83_MODE_PORT,AT070TN83_MODE_PIN);		//MODE���ߣ�ʹ��DEģʽ
	GPIO_SetBits(AT070TN83_LR_PORT,AT070TN83_LR_PIN);			//������ɨ��
	GPIO_ResetBits(AT070TN83_UD_PORT,AT070TN83_UD_PIN);		//���ϵ���ɨ��
	   
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);				//38��Ҫ����͵�ƽ����Ȼ83������Լ�ģʽ
	//��δ�õ������ݽ�����
	GPIO_ResetBits(LTDC_R0_PORT,LTDC_R0_PIN);		
	GPIO_ResetBits(LTDC_R1_PORT,LTDC_R1_PIN);	
	GPIO_ResetBits(LTDC_G0_PORT,LTDC_G0_PIN);	
	GPIO_ResetBits(LTDC_G1_PORT,LTDC_G1_PIN);		
	GPIO_ResetBits(LTDC_B0_PORT,LTDC_B0_PIN);	
	GPIO_ResetBits(LTDC_B1_PORT,LTDC_B1_PIN);		
}


// ������LCD�����ó�ʼ��
//	
void LCD_LayerInit(void)
{
	LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct; 

	LTDC_Layer_InitStruct.LTDC_HorizontalStart = HBP + 1;
	LTDC_Layer_InitStruct.LTDC_HorizontalStop = (LCD_Width + HBP);
	LTDC_Layer_InitStruct.LTDC_VerticalStart =  VBP + 1;
	LTDC_Layer_InitStruct.LTDC_VerticalStop = (LCD_Height + VBP);
	LTDC_Layer_InitStruct.LTDC_PixelFormat = ColorMode_0;	//���ظ�ʽ����
	LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255; 
	LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0; 	//  Ĭ�ϵ���ɫ       
	LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;       
	LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;         
	LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;
	LTDC_Layer_InitStruct.LTDC_CFBLineNumber  = LCD_Height; //��ʾ���������
	LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_MemoryAdd; //��һ�����ʼ��ַ
	
	if( ColorMode_0 == LCD_RGB565 || ColorMode_0 == LCD_ARGB1555 ) //�ж���ɫ��ʽ
	{
		LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_Width * 2) + 3);	 //ÿ�е�����ռ�����ֽ���
		LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_Width * 2); 				 //�м�࣬ĳ���ص���ʼ������һ�е���ʼ��������
	}
	else //����ARGB8888��RGB888ʹ����ͬ�ļ��㷽ʽ
	{	
		LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_Width * 4) + 3);	//ÿ�е�����ռ�����ֽ���
		LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_Width * 4);  			   //�м�࣬ĳ���ص���ʼ������һ�е���ʼ��������
	}
	LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);	//��ʼ����1
 	LTDC_LayerCmd(LTDC_Layer1, ENABLE);  //ʹ�ܲ�1

#if ( LCD_NUM_LAYERS == 2 )	//��������˫��ʱ

	LTDC_Layer_InitStruct.LTDC_PixelFormat = ColorMode_1;		//���ظ�ʽ����
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;  
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;
	LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_MemoryAdd + LCD_MemoryAdd_OFFSET; //��2����ʼ��ַ

	if( ColorMode_1 == LCD_RGB565 || ColorMode_1 == LCD_ARGB1555 ) //�ж���ɫ��ʽ
	{
		LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_Width * 2) + 3);	//ÿ�е�����ռ�����ֽ���
		LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_Width * 2);  				//�м�࣬ĳ���ص���ʼ������һ�е���ʼ��������
	}
	else //����ARGB8888��RGB888ʹ����ͬ�ļ��㷽ʽ
	{	
		LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_Width * 4) + 3);	//ÿ�е�����ռ�����ֽ���
		LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_Width * 4); 			 //�м�࣬ĳ���ص���ʼ������һ�е���ʼ��������
	}
	LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);	//��ʼ����2
	LTDC_LayerCmd(LTDC_Layer2, ENABLE);  //ʹ�ܲ�2
#endif

	LTDC_ReloadConfig(LTDC_IMReload); //�����������
	LTDC_DitherCmd(ENABLE);		//ʹ����ɫ������24λɫ����򿪣������޷��ﵽ24λɫ��Ч��
}

// ������LCD���ó�ʼ��
//	
void LCD_Init(void)
{ 
	u16 LCD_PLLSAIN = 0;		//���ڱ�Ƶ��PLLSAIN��������ȡ��ΧΪ50~432
	u8  LCD_PLLSAIR = 3;		//���ڷ�Ƶ��PLLSAIR��������ȡ��ΧΪ2~7
	u8  LCD_CLKDIV	 = 2;		//LCDʱ�ӷ�Ƶ������Ĭ������Ϊ2��Ƶ
	
	LTDC_InitTypeDef  LTDC_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE); 	//ʹ��LTDCʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE); //ʹ��DMA2D

	LCD_GPIO_Config();  //��ʼ��LCD����
	SDRAM_Init(); 		  //��ʼ��SDRAM

	LCD_PLLSAIN = LCD_CLK * LCD_PLLSAIR * LCD_CLKDIV;	//������Ҫʹ�õ�LCDʱ�Ӽ���PLLSAIN��������ȡ��ΧΪ50~432
	RCC_PLLSAIConfig(LCD_PLLSAIN,7,LCD_PLLSAIR);  //ʱ������
//	RCC_PLLSAIConfig(320,7,2);  
	RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div2);	 //LCDʱ�ӷ�Ƶ���ã�Ҫ��LCD_CLKDIV��Ӧ
	RCC_PLLSAICmd(ENABLE);	//ʹ��PLLSAIʱ��
	while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET);	//�ȴ�ʱ���������
	
	LTDC_InitStruct.LTDC_HSPolarity = LTDC_HSPolarity_AL;     
	LTDC_InitStruct.LTDC_VSPolarity = LTDC_VSPolarity_AL;     
	LTDC_InitStruct.LTDC_DEPolarity = LTDC_DEPolarity_AL;     
	LTDC_InitStruct.LTDC_PCPolarity = LTDC_PCPolarity_IPC;
              
	LTDC_InitStruct.LTDC_BackgroundRedValue = 0;            
	LTDC_InitStruct.LTDC_BackgroundGreenValue = 0;          
	LTDC_InitStruct.LTDC_BackgroundBlueValue = 0;   

	LTDC_InitStruct.LTDC_HorizontalSync =HSW;
	LTDC_InitStruct.LTDC_VerticalSync = VSW;
	LTDC_InitStruct.LTDC_AccumulatedHBP =HBP;
	LTDC_InitStruct.LTDC_AccumulatedVBP = VBP;
	LTDC_InitStruct.LTDC_AccumulatedActiveW = LCD_Width+HBP;
	LTDC_InitStruct.LTDC_AccumulatedActiveH = LCD_Height+VBP;
	LTDC_InitStruct.LTDC_TotalWidth =LCD_Width + HBP + HFP; 
	LTDC_InitStruct.LTDC_TotalHeigh =LCD_Height + VBP + VFP;

	LTDC_Init(&LTDC_InitStruct);	//��ʼ��LCD������

	LCD_LayerInit();	//���ʼ��
	LTDC_Cmd(ENABLE);	//ʹ��LCD������
	
	LCD_DisplayMode(Mode_H); //������ʾ
	LCD_SetFont(&Font32);  //����Ĭ������	
	LCD_ShowNumMode(Fill_Space);	//������ʾĬ�����ո�
	
	LCD_SetLayer(0);  
	LCD_SetBackColor(LCD_BLACK); //���ñ���ɫ
	LCD_SetColor(LCD_WHITE);	//���û�����ɫ
	LCD_Clear(); 	//������ˢ����ɫ

#if LCD_NUM_LAYERS == 2	 //��������˵ڶ���
	LCD_SetLayer(1); 
	LCD_SetBackColor(LCD_BLACK); //���ñ���ɫ
	LCD_SetColor(LCD_WHITE);	//���û�����ɫ
	LCD_Clear(); //������ˢ����ɫ
#endif
}  
//
//	������������
//	������layer - Ҫ���õĲ������ڴ˶�Ϊ0�Ǳ����㣬1��ǰ����
//
void LCD_SetLayer(u8 layer)
{
#if LCD_NUM_LAYERS == 2		//�������˫��
	if (layer == 0)
	{
		LCD.LayerMemoryAdd = LCD_MemoryAdd; 	//���ò�0���Դ��ַ
	}
	else if(layer == 1)
	{
		LCD.LayerMemoryAdd = LCD_MemoryAdd + LCD_MemoryAdd_OFFSET;	//���ò�1���Դ��ַ
	}
	LCD.Layer = layer;	//��¼��ǰ���ڵĲ�
#else
	LCD.LayerMemoryAdd = LCD_MemoryAdd;	//������ʾ��״̬�£�ֻ���ò�0�ĵ�ַ
	LCD.Layer = 0;		 //����Ĭ��Ϊ0
#endif

	LCD.BufferFlag = 0;	//�����ʹ�ñ��
}  

// ������������ɫɫ
// ˵��������ɫ������ʾ�ַ������㻭�ߡ�ͼ�ε���ɫ
//
void LCD_SetColor(u32 Color)
{
	u32 Display_ColorMode = 0;		//��ǰ�����ɫ��ʽ
	u16 Alpha_Value = 0, Red_Value = 0, Green_Value = 0, Blue_Value = 0; //������ɫͨ����ֵ

#if(LCD_NUM_LAYERS == 2)		//�������˫��
	if(LCD.Layer == 0)  			
		Display_ColorMode = ColorMode_0;		//��ȡ��0����ɫ��ʽ
	else
		Display_ColorMode = ColorMode_1;		//��ȡ��1����ɫ��ʽ
#else
	Display_ColorMode = ColorMode_0;	//������ʾ��Ĭ��Ϊ��0����ɫ��ʽ
#endif

	if( Display_ColorMode == LCD_RGB565	)	//��32λɫת��Ϊ16λɫ
	{
		Red_Value   = (u16)((Color&0x00F80000)>>8);
		Green_Value = (u16)((Color&0x0000FC00)>>5);
		Blue_Value  = (u16)((Color&0x000000F8)>>3);
		LCD.Color = (u16)(Red_Value | Green_Value | Blue_Value);		
	}
	else if( Display_ColorMode == LCD_ARGB1555 )	//��32λɫת��ΪARGB1555��ɫ
	{
		if( (Color & 0xFF000000) == 0 )	//�ж��Ƿ�ʹ��͸��ɫ
			Alpha_Value = 0x0000;
		else
			Alpha_Value = 0x8000;

		Red_Value   = (u16)((Color&0x00F80000)>>9);	
		Green_Value = (u16)((Color&0x0000F800)>>6);
		Blue_Value  = (u16)((Color&0x000000F8)>>3);
		LCD.Color = (u16)(Alpha_Value | Red_Value | Green_Value | Blue_Value);	
	}
	else
		LCD.Color = Color;	//24λɫ��32λɫ����Ҫת��
}

// ���������ñ���ɫ
// ˵���������������ʾ�ı�����ɫ������LCD����������ɫ
//
void LCD_SetBackColor(u32 Color)
{
	u32 Display_ColorMode = 0;		//��ǰ�����ɫ��ʽ
	u16 Alpha_Value = 0, Red_Value = 0, Green_Value = 0, Blue_Value = 0;  //������ɫͨ����ֵ

#if(LCD_NUM_LAYERS == 2)	//�������˫��
	if(LCD.Layer == 0)  
		Display_ColorMode = ColorMode_0;	//��ȡ��0����ɫ��ʽ
	else
		Display_ColorMode = ColorMode_1;	//��ȡ��1����ɫ��ʽ
#else
	Display_ColorMode = ColorMode_0;	//������ʾ��Ĭ��Ϊ��0����ɫ��ʽ
#endif

	if( Display_ColorMode == LCD_RGB565	)	//��32λɫת��Ϊ16λɫ
	{
		Red_Value   = (u16)((Color&0x00F80000)>>8);
		Green_Value = (u16)((Color&0x0000FC00)>>5);
		Blue_Value  = (u16)((Color&0x000000F8)>>3);
		LCD.BackColor = (u16)(Red_Value | Green_Value | Blue_Value);	
	}
	else if( Display_ColorMode == LCD_ARGB1555 )	//��32λɫת��ΪARGB1555��ɫ
	{
		if( (Color & 0xFF000000) == 0 )	//�ж��Ƿ�ʹ��͸��ɫ
			Alpha_Value = 0x0000;
		else
			Alpha_Value = 0x8000;

		Red_Value   = (u16)((Color&0x00F80000)>>9);
		Green_Value = (u16)((Color&0x0000F800)>>6);
		Blue_Value  = (u16)((Color&0x000000F8)>>3);
		LCD.BackColor = (u16)(Alpha_Value | Red_Value | Green_Value | Blue_Value);	
	}
	else	
		LCD.BackColor = Color;	//24λɫ��32λɫ����Ҫת��
	
}

// ��������������
// ����������ѡ��Font32/Font24/Font20/Font16/Font12
//
void LCD_SetFont(pFONT *fonts)
{
  LCD_Fonts = fonts;
}

// ������������ʾ����
//	������direction - ��ʾ�ķ���Mode_H ˮƽ��Mode_V ��ֱ
//
void LCD_DisplayMode(u8 direction)
{
	LCD.Direction = direction;
}
//
// ��������������M 
// ˵������LCD����ȫ������Ϊ LCD.BackColor ����ɫ��ʹ��DMA2Dʵ��
//
void LCD_Clear(void)
{
	DMA2D_InitTypeDef   DMA2D_InitStruct;
	u32 Display_Add = 0;			//��ǰ����Դ��ַ
	u8  Display_ColorMode = 0;	//��ǰ�����ɫ��ʽ
	u8  Red_Value = 0, Green_Value = 0, Blue_Value = 0, Alpha_Value = 0;	//������ɫͨ����ֵ

	/****************** ���ݱ�־λ�л��Դ��ַ***********************/
	
	if(LCD.BufferFlag == 0)
		Display_Add = LCD.LayerMemoryAdd;	
	else
		Display_Add = LCD_Buffer;
	
	/*************** ѡ����Ӧ�����ɫģʽ���Դ��ַ *****************/
	
#if(LCD_NUM_LAYERS == 2)
	if(LCD.Layer == 0)  //���ݲ�ͬ�Ĳ�ѡ��ͬ����ɫ��ʽ
	{
		Display_ColorMode = ColorMode_0;
		LTDC_Layer1->CFBAR = Display_Add;		
	}
	else
	{
		Display_ColorMode = ColorMode_1;
		LTDC_Layer2->CFBAR = Display_Add;
	}
#else
	Display_ColorMode = ColorMode_0;
	LTDC_Layer1->CFBAR = Display_Add;		
#endif
	
	/************* ���ݲ�ͬ����ɫģʽ���������ɫͨ����ֵ ***************/
	
	if( Display_ColorMode == LCD_RGB565	)	//��ȡRGB565��RGBֵ
	{   
		Red_Value   = (u16)(0xF800 & LCD.BackColor) >> 11;
		Green_Value = (u16)(0x07E0 & LCD.BackColor) >> 5;
		Blue_Value  = (u16)(0x001F & LCD.BackColor);

	}
	else if( Display_ColorMode == LCD_ARGB1555	) //��ȡARGB1555��RGBֵ
	{   
		if ( (LCD.BackColor & 0x8000) == 0 )	//�ж��Ƿ���͸��ɫ
			Alpha_Value = 0x00;
		else
			Alpha_Value = 0xFF;

		Red_Value   = (u16)(0x7C00 & LCD.BackColor) >> 10;
		Green_Value = (u16)(0x03E0 & LCD.BackColor) >> 5;
		Blue_Value  = (u16)(0x001F & LCD.BackColor);
	}
	else		//24λɫ��32λɫ����Ҫת��
	{
		Blue_Value  = LCD.BackColor;
		Green_Value = LCD.BackColor >> 8;	
		Red_Value   = LCD.BackColor >> 16;   
		Alpha_Value = LCD.BackColor >> 24; 
	}
	
	/********************** ��ʼ��DMA2D **************************/
	
	DMA2D_DeInit();  
	DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;      
	DMA2D_InitStruct.DMA2D_CMode = Display_ColorMode;   
	DMA2D_InitStruct.DMA2D_OutputRed   = Red_Value;   
	DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;   
	DMA2D_InitStruct.DMA2D_OutputBlue  = Blue_Value;     
	DMA2D_InitStruct.DMA2D_OutputAlpha = Alpha_Value;  
	DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Display_Add;                                                                        
	DMA2D_InitStruct.DMA2D_OutputOffset = 0;                
	DMA2D_InitStruct.DMA2D_NumberOfLine = LCD_Height;            
	DMA2D_InitStruct.DMA2D_PixelPerLine = LCD_Width; 
	DMA2D_Init(&DMA2D_InitStruct);  

	DMA2D_StartTransfer();  
	while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET);

	LTDC_ReloadConfig(LTDC_VBReload); //�����������
	LCD.BufferFlag =~LCD.BufferFlag;	 //�л��Դ��ַ��־
}


//	����������
//	������x-ˮƽ���꣬y-��ֱ���꣬color-Ҫ���Ƶ���ɫ
//
void LCD_DrawPoint(u16 x,u16 y,u32 color)
{
	u32 Display_Add = 0;			//��ǰ����Դ��ַ
	u32 Display_ColorMode = 0;	//��ǰ�����ɫ��ʽ

	/*************** ���ݱ�־λ�л��Դ��ַ ****************/	
	
	if(LCD.BufferFlag == 0)	
		Display_Add = LCD_Buffer;	
	else
		Display_Add = LCD.LayerMemoryAdd;	

	/**************** ѡ����Ӧ�����ɫģʽ **************/
	
#if(LCD_NUM_LAYERS == 2)
		if(LCD.Layer == 0)  //���ݲ�ͬ�Ĳ�ѡ��ͬ����ɫ��ʽ
			Display_ColorMode = ColorMode_0;
		else
			Display_ColorMode = ColorMode_1;
#else
		Display_ColorMode = ColorMode_0;
#endif
	
	/****************** 32λɫģʽ ***********************/
		
	if( Display_ColorMode == LCD_ARGB8888 ) 
	{
		if (LCD.Direction == Mode_H) //ˮƽ����
		{
			*(__IO u32*)( Display_Add + 4*(x + y*LCD_Width) ) = color ; 	
		}
		else if(LCD.Direction == Mode_V)	//��ֱ����
		{
			*(__IO u32*)( Display_Add + 4*((LCD_Height - x - 1)*LCD_Width + y) ) = color ;
		}
	}
	
	/****************** 24λɫģʽ ***********************/	
	
	else if ( Display_ColorMode == LCD_RGB888 )
	{
		if (LCD.Direction == Mode_H) //ˮƽ����
		{
			*(__IO u16*)( Display_Add + 3*(x + y*LCD_Width) ) = color ; 
			*(__IO  u8*)( Display_Add + 3*(x + y*LCD_Width) + 2 ) = color>>16 ; 	
		}
		else if(LCD.Direction == Mode_V)	//��ֱ����
		{
			*(__IO u16*)( Display_Add + 3*((LCD_Height - x - 1)*LCD_Width + y) ) = color ; 
			*(__IO  u8*)( Display_Add + 3*((LCD_Height - x - 1)*LCD_Width + y) +2) = color>>16 ; 	
		}	
	}
	
	/****************** 16λɫģʽ ***********************/	
	else		
	{
		if (LCD.Direction == Mode_H) //ˮƽ����
		{
			*(__IO u16*)( Display_Add + 2*(x + y*LCD_Width) ) = color ; 	
		}
		else if(LCD.Direction == Mode_V)	//��ֱ����
		{
			*(__IO u16*)( Display_Add + 2*((LCD_Height - x - 1)*LCD_Width + y) ) = color ;
		}	
	}
}  

//	����������
//	������x-ˮƽ���꣬y-��ֱ����
//	���أ���ȡ����ɫ
// ˵������ʹ��16��24λɫģʽʱ������������ɫ������16λ��24λ��
//
u32 LCD_ReadPoint(u16 x,u16 y)
{
	u32 color = 0;
	u32 Display_Add = 0;		//��ǰ����Դ��ַ
	u32 Display_ColorMode = 0;	//��ǰ�����ɫ��ʽ

	/*************** ���ݱ�־λ�л��Դ��ַ ****************/	

	if(LCD.BufferFlag == 0)	
		Display_Add = LCD_Buffer;	
	else
		Display_Add = LCD.LayerMemoryAdd;	

	/**************** ѡ����Ӧ�����ɫģʽ **************/

#if(LCD_NUM_LAYERS == 2)
	if(LCD.Layer == 0)  //���ݲ�ͬ�Ĳ�ѡ��ͬ����ɫ��ʽ
		Display_ColorMode = ColorMode_0;
	else
		Display_ColorMode = ColorMode_1;
#else
	Display_ColorMode = ColorMode_0;
#endif

	/****************** 32λɫģʽ ***********************/
	if( Display_ColorMode == LCD_ARGB8888 ) 
	{
		if (LCD.Direction == Mode_H) //ˮƽ����
		{
			color = *(__IO u32*)( Display_Add + 4*(x + y*LCD_Width) ); 	
		}
		else if(LCD.Direction == Mode_V)	//��ֱ����
		{
			color = *(__IO u32*)( Display_Add + 4*((LCD_Height - x - 1)*LCD_Width + y) );
		}
	}
	/****************** 24λɫģʽ ***********************/	
	else if ( Display_ColorMode == LCD_RGB888 )
	{
		if (LCD.Direction == Mode_H) //ˮƽ����
		{
			color = *(__IO u32*)( Display_Add + 3*(x + y*LCD_Width) ) &0x00ffffff; 	
		}
		else if(LCD.Direction == Mode_V)	//��ֱ����
		{
			color = *(__IO u32*)( Display_Add + 3*((LCD_Height - x - 1)*LCD_Width + y) ) &0x00ffffff; 	
		}	
	}
	/****************** 16λɫģʽ ***********************/	
	else		
	{
		if (LCD.Direction == Mode_H) //ˮƽ����
		{
			color = *(__IO u16*)( Display_Add + 2*(x + y*LCD_Width) ); 	
		}
		else if(LCD.Direction == Mode_V)	//��ֱ����
		{
			color = *(__IO u16*)( Display_Add + 2*((LCD_Height - x - 1)*LCD_Width + y) );
		}	
	}
	return color;
}  

//	��������ʾ�����ַ�
//	������x-ˮƽ���꣬y-��ֱ���꣬c-�ַ��ĵ�ַ
//
void LCD_DisplayChar(u16 x, u16 y,u8 add)
{
	u8  index = 0, counter = 0;
   u8   disChar;	//�洢�ַ��ĵ�ַ
	u16  Xaddress = x; //ˮƽ����
	
	add = add - 32; 
	for(index = 0; index < LCD_Fonts->Sizes; index++)
	{
		disChar = LCD_Fonts->table[add*LCD_Fonts->Sizes + index]; //��ȡ�ַ���ģֵ
		for(counter = 0; counter < 8; counter++)
		{ 
			if(disChar & 0x01)	
			{		
				LCD_DrawPoint(Xaddress,y,LCD.Color);	//��ǰģֵ��Ϊ0ʱ��ʹ�û���ɫ���
			}
			else		
			{		
				LCD_DrawPoint(Xaddress,y,LCD.BackColor);	//����ʹ�ñ���ɫ���Ƶ�
			}
			disChar >>= 1;
			Xaddress++;  //ˮƽ�����Լ�
			
			if( (Xaddress - x)==LCD_Fonts->Width ) //���ˮƽ����ﵽ���ַ���ȣ����˳���ǰѭ��
			{													//������һ�еĻ���
				Xaddress = x;
				y++;
				break;
			}
		}	
	}
}

//	��������ʾ�ַ���
//	������x-ˮƽ���꣬y-��ֱ���꣬p-�ַ����ĵ�ַ
//
void LCD_DisplayString( u16 x, u16 y, u8 *p) 
{  
	while ((x < LCD_Width) && (*p != 0))	//�ж���ʾ�����Ƿ񳬳���ʾ�������ַ��Ƿ�Ϊ���ַ�
	{
		 LCD_DisplayChar( x,y,*p);
		 x += LCD_Fonts->Width; //��ʾ��һ���ַ�
		 p++;	//ȡ��һ���ַ���ַ
	}
}
// ������ȡ�����ĵ�Nλ���������ڲ�������Խ�����ʾ
//	������number - Ŀ��������size - Ŀ�����ĳ���
//	���أ�ȡ���ĵ�Nλ����
//
u8 GetNumber(u32 num,u8 size)
{
	u32 numPow = 1;
	u8  vaule;
	
	while(size>0)
	{
		numPow *=10;
		size--;
	}
	vaule = (num/numPow)%10;
	return vaule;	
}

// ����������������ʾ��ʽ
//	������mode - ���ò�0���ǲ��ո�Fill_Space ���ո�Fill_Zero �����
//
void LCD_ShowNumMode(u8 mode)
{
	LCD.ShowNum_Mode = mode;
}

//	��������ʾ����
//	������x-ˮƽ���꣬y-��ֱ����
//    	number - Ҫ��ʾ������,0��4294967295֮��
//
void  LCD_DisplayNumber( u16 x, u16 y, u32 number, u8 len) 
{  
	u8 i,value;
	u8 zero_Flag = 0;

	for(i=0;i<len;i++)
	{
		value = GetNumber(number,len-i-1);	//��ȡ��ǰ�����ĵ�Nλ��
		if( zero_Flag==0 && (i<len-1) )	//�ж�ǰ��Ϊ0�Ĳ���
		{
			if(value == 0)
			{
				if(LCD.ShowNum_Mode == 0)	
					LCD_DisplayChar( x+i*LCD_Fonts->Width, y,48);	//���0
				else
					LCD_DisplayChar( x+i*LCD_Fonts->Width, y,32);	//���ո�
				continue;
			}
			else
				zero_Flag = 1;	//��������һ����0��ʱ��1
		}									
		LCD_DisplayChar( x+i*LCD_Fonts->Width, y, value+48 );	//����ֵ���������ʾ
	}
}
//��ʾ�з�����
void  LCD_DisplayNumberyoufuhao( u16 x, u16 y, u32 number,u16 num, u8 len) 
{  
	u8 i,value,j=1;
	u8 zero_Flag = 0;
	if(number>=num)
		{
			number-=num;
			for(i=0;i<len;i++)
			{
				value = GetNumber(number,len-i-1);	//��ȡ��ǰ�����ĵ�Nλ��
				if( zero_Flag==0 && (i<len-1) )	//�ж�ǰ��Ϊ0�Ĳ���
				{
				  if(value == 0)
					{
						if(LCD.ShowNum_Mode == 0)	
							LCD_DisplayChar( x+i*LCD_Fonts->Width, y,48);	//���0
						else
							LCD_DisplayChar( x+i*LCD_Fonts->Width, y,32);	//���ո�
						continue;
					}
					else
				    zero_Flag = 1;	//��������һ����0��ʱ��1
				}	
				if(j==1)
				{
					LCD_DisplayChar(x+(i-1)*LCD_Fonts->Width, y,45);
					j=0;
				}
				LCD_DisplayChar( x+i*LCD_Fonts->Width, y, value+48 );	//����ֵ���������ʾ
			}
		}
	else
		LCD_DisplayNumber( x, y,number,len);
}
#define ABS(X)  ((X) > 0 ? (X) : -(X))    

//	����������
//	������x1��y1Ϊ������꣬x2��y2Ϊ�յ�����
//
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0, 
	yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0, 
	curpixel = 0;

	deltax = ABS(x2 - x1);        /* The difference between the x's */
	deltay = ABS(y2 - y1);        /* The difference between the y's */
	x = x1;                       /* Start x off at the first pixel */
	y = y1;                       /* Start y off at the first pixel */

	if (x2 >= x1)                 /* The x-values are increasing */
	{
	 xinc1 = 1;
	 xinc2 = 1;
	}
	else                          /* The x-values are decreasing */
	{
	 xinc1 = -1;
	 xinc2 = -1;
	}

	if (y2 >= y1)                 /* The y-values are increasing */
	{
	 yinc1 = 1;
	 yinc2 = 1;
	}
	else                          /* The y-values are decreasing */
	{
	 yinc1 = -1;
	 yinc2 = -1;
	}

	if (deltax >= deltay)         /* There is at least one x-value for every y-value */
	{
	 xinc1 = 0;                  /* Don't change the x when numerator >= denominator */
	 yinc2 = 0;                  /* Don't change the y for every iteration */
	 den = deltax;
	 num = deltax / 2;
	 numadd = deltay;
	 numpixels = deltax;         /* There are more x-values than y-values */
	}
	else                          /* There is at least one y-value for every x-value */
	{
	 xinc2 = 0;                  /* Don't change the x for every iteration */
	 yinc1 = 0;                  /* Don't change the y when numerator >= denominator */
	 den = deltay;
	 num = deltay / 2;
	 numadd = deltax;
	 numpixels = deltay;         /* There are more y-values than x-values */
	}
	for (curpixel = 0; curpixel <= numpixels; curpixel++)
	{
	 LCD_DrawPoint(x,y,LCD.Color);             /* Draw the current pixel */
	 num += numadd;              /* Increase the numerator by the top of the fraction */
	 if (num >= den)             /* Check if numerator >= denominator */
	 {
		num -= den;               /* Calculate the new numerator value */
		x += xinc1;               /* Change the x as appropriate */
		y += yinc1;               /* Change the y as appropriate */
	 }
	 x += xinc2;                 /* Change the x as appropriate */
	 y += yinc2;                 /* Change the y as appropriate */
	}

}
//	������������
//	������x��yΪ������꣬width-���εĳ��ȣ�height-���
//
void LCD_DrawRect(u16 x, u16 y, u16 width, u16 height)
{
	/* draw horizontal lines */
	LCD_DrawLine(x, y, x+width, y);
	LCD_DrawLine(x, y+height, x+width, y+height);

	/* draw vertical lines */
	LCD_DrawLine(x, y, x, y+height);
	LCD_DrawLine(x+width, y, x+width, y+height);
}
//	��������Բ
//	������x��yΪ������꣬r-�뾶
//	˵�����뾶���ܴ���xy�����ֵ
//
void LCD_DrawCircle(u16 x, u16 y, u16 r)
{
	int Xadd = -r, Yadd = 0, err = 2-2*r, e2;
	do {   

		LCD_DrawPoint(x-Xadd,y+Yadd,LCD.Color);
		LCD_DrawPoint(x+Xadd,y+Yadd,LCD.Color);
		LCD_DrawPoint(x+Xadd,y-Yadd,LCD.Color);
		LCD_DrawPoint(x-Xadd,y-Yadd,LCD.Color);
		
		e2 = err;
		if (e2 <= Yadd) {
			err += ++Yadd*2+1;
			if (-Xadd == Yadd && e2 <= Xadd) e2 = 0;
		}
		if (e2 > Xadd) err += ++Xadd*2+1;
    }
    while (Xadd <= 0);
    
}
//	����������Բ
//	������x��yΪ������꣬r1��r2�ֱ�Ϊˮƽ�ʹ�ֱ����ĳ���
//
void LCD_DrawEllipse(int x, int y, int r1, int r2)
{
  int Xadd = -r1, Yadd = 0, err = 2-2*r1, e2;
  float K = 0, rad1 = 0, rad2 = 0;
   
  rad1 = r1;
  rad2 = r2;
  
  if (r1 > r2)
  { 
    do {
      K = (float)(rad1/rad2);
		 
		LCD_DrawPoint(x-Xadd,y+(u16)(Yadd/K),LCD.Color);
		LCD_DrawPoint(x+Xadd,y+(u16)(Yadd/K),LCD.Color);
		LCD_DrawPoint(x+Xadd,y-(u16)(Yadd/K),LCD.Color);
		LCD_DrawPoint(x-Xadd,y-(u16)(Yadd/K),LCD.Color);     
		 
      e2 = err;
      if (e2 <= Yadd) {
        err += ++Yadd*2+1;
        if (-Xadd == Yadd && e2 <= Xadd) e2 = 0;
      }
      if (e2 > Xadd) err += ++Xadd*2+1;
    }
    while (Xadd <= 0);
  }
  else
  {
    Yadd = -r2; 
    Xadd = 0;
    do { 
      K = (float)(rad2/rad1);

		LCD_DrawPoint(x-(u16)(Xadd/K),y+Yadd,LCD.Color);
		LCD_DrawPoint(x+(u16)(Xadd/K),y+Yadd,LCD.Color);
		LCD_DrawPoint(x+(u16)(Xadd/K),y-Yadd,LCD.Color);
		LCD_DrawPoint(x-(u16)(Xadd/K),y-Yadd,LCD.Color);  
		 
      e2 = err;
      if (e2 <= Xadd) {
        err += ++Xadd*3+1;
        if (-Yadd == Xadd && e2 <= Yadd) e2 = 0;
      }
      if (e2 > Yadd) err += ++Yadd*3+1;     
    }
    while (Yadd <= 0);
  }
}

//	���������ʵ�ľ���
//	������x��yΪ������꣬width - ���ȣ�height - ���
//
void LCD_FillRect(u16 x, u16 y, u16 width, u16 height)
{
	DMA2D_InitTypeDef   DMA2D_InitStruct;
	u32 Display_Add = 0;					//��ǰ����Դ��ַ
	u8 Display_ColorMode = 0;			//��ǰ�����ɫ��ʽ
	u8  Display_BytesPerPixel = 0;	//��ǰ���ÿ�����ص��ֽ���
	u8 Red_Value = 0, Green_Value = 0, Blue_Value = 0,Alpha_Value = 0;	//����ɫͨ����ֵ

	/****************** ���ݱ�־λ�л��Դ��ַ***********************/
	
	if(LCD.BufferFlag == 0)	
		Display_Add = LCD_Buffer;
	else
		Display_Add = LCD.LayerMemoryAdd;

	/*************** ѡ����Ӧ�����ɫģʽ�����ص��ֽ��� *****************/
	
#if(LCD_NUM_LAYERS == 2)
	if(LCD.Layer == 0)  //���ݲ�ͬ�Ĳ�ѡ��ͬ������
	{
		Display_ColorMode = ColorMode_0;
		Display_BytesPerPixel = BytesPerPixel_0;
	}
	else
	{
		Display_ColorMode = ColorMode_1;
		Display_BytesPerPixel = BytesPerPixel_1;
	}
#else
	Display_ColorMode = ColorMode_0;
	Display_BytesPerPixel = BytesPerPixel_0;
#endif

	/************* ���ݲ�ͬ����ɫģʽ���������ɫͨ����ֵ ***************/
	
	if( Display_ColorMode == LCD_RGB565	)	//��ȡRGB565��RGBֵ
	{   
		Red_Value   = (u16)(0xF800 & LCD.Color) >> 11;
		Green_Value = (u16)(0x07E0 & LCD.Color) >> 5;
		Blue_Value  = (u16)(0x001F & LCD.Color);

	}
	else if( Display_ColorMode == LCD_ARGB1555	) //��ȡARGB1555��RGBֵ
	{   
		if ( (LCD.Color & 0x8000) == 0 )	//�ж��Ƿ�ʹ��͸��ɫ
			Alpha_Value = 0x00;
		else
			Alpha_Value = 0xFF;

		Red_Value   = (u16)(0x7C00 & LCD.Color) >> 10;
		Green_Value = (u16)(0x03E0 & LCD.Color) >> 5;
		Blue_Value  = (u16)(0x001F & LCD.Color);
	}
	else
	{
		Blue_Value  = LCD.Color;
		Green_Value = LCD.Color >> 8;	
		Red_Value   = LCD.Color >> 16;   
		Alpha_Value = LCD.Color >> 24; 
	}
	
	/********************** ��ʼ��DMA2D **************************/	
	
	DMA2D_DeInit();
	DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;       
	DMA2D_InitStruct.DMA2D_CMode = Display_ColorMode;      
	DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;      
	DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;     
	DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;                
	DMA2D_InitStruct.DMA2D_OutputAlpha = Alpha_Value;                   

	if(LCD.Direction == Mode_H)  //�������
	{
		DMA2D_InitStruct.DMA2D_OutputOffset = (LCD_Width - width);                
		DMA2D_InitStruct.DMA2D_NumberOfLine = height;            
		DMA2D_InitStruct.DMA2D_PixelPerLine = width;
		DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Display_Add + Display_BytesPerPixel*(LCD_Width * y + x);
	}
	else	//�������
	{
		DMA2D_InitStruct.DMA2D_OutputOffset = (LCD_Width - height);                
		DMA2D_InitStruct.DMA2D_NumberOfLine = width;            
		DMA2D_InitStruct.DMA2D_PixelPerLine = height;			
		DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Display_Add + Display_BytesPerPixel*((LCD_Height - x - 1 - width)*LCD_Width + y) ;
	}	
	DMA2D_Init(&DMA2D_InitStruct); 

	DMA2D_StartTransfer();	//��ʼ����
	while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET);	//�ȴ��������
}

//	���������ʵ��Բ
//	������x��yΪ������꣬r - �뾶
// ˵�����뾶Ҫ����y����
//
void LCD_FillCircle(u16 x, u16 y, u16 r)
{
  int32_t  D;    /* Decision Variable */ 
  uint32_t  CurX;/* Current X Value */
  uint32_t  CurY;/* Current Y Value */ 
  
  D = 3 - (r << 1);
  
  CurX = 0;
  CurY = r;
  
  while (CurX <= CurY)
  {
    if(CurY > 0) 
    { 
		LCD_DrawLine(x - CurX, y - CurY,x - CurX,y - CurY + 2*CurY);
		LCD_DrawLine(x + CurX, y - CurY,x + CurX,y - CurY + 2*CurY); 
    }
    
    if(CurX > 0) 
    {
		LCD_DrawLine(x - CurY, y - CurX,x - CurY,y - CurX + 2*CurX);
		LCD_DrawLine(x + CurY, y - CurX,x + CurY,y - CurX + 2*CurX); 		 
    }
    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }
  
  LCD_DrawCircle(x, y, r);  
}
