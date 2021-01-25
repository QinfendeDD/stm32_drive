/***
	***************************************************************************************
	*	@file  	lcd.c
	*	@brief   使用STM32F29本身的控制器驱动液晶屏，代码移植于官方 STM32F429I_DISCOVERY 
	*				实验板的例程，并作出相应的修改优化使之更加实用易用
   ***************************************************************************************
   *  @description
	*	
	*	以下数据的测试条件为：分辨率800*480，FPC排线长度为20cm。所谓最高驱动频率是指显示正常，并且在清屏
	*	的时候无花屏的现象，使用更高的驱动频率可能会在清屏时造成短暂的花屏。只开单层显示和使用更短的排线
	*	可以允许使用更高的驱动频率，相关参数在lcd_rgb.h里进行设置。
	*	1.只开单层显示下，16位色模式最高驱动频率70M
	*	2.只开单层显示下，24位色模式最高驱动频率52M
	*	3.只开单层显示下，32位色模式最高驱动频率38M
	*	4.开双层显示下，RGB565和ARGB1555模式下，最高驱动频率40M
	*	5.开双层显示下，RGB565和ARGB8888模式下，最高驱动频率25M
	*  5.开双层显示下，RGB888和ARGB1555模式下，最高驱动频率36M
	*	5.开双层显示下，RGB888和ARGB8888模式下，最高驱动频率22M
	*	
	*	其他说明：
	*	1.STM32F29的透明度是用在两个显示层之间
	*	2.在分辨率为800*480时，驱动频率在33M左右就可以达到60帧的刷新率，一般使用这个频率就足够了
	*	3.开层显示下，前景层应设置为带透明色的颜色模式，不然就无法看到背景层，推荐使用RGB565+ARGB8888的搭配
	*	4.屏幕在刚下载或者刚上电的时候有轻微的闪烁属于正常现象，片刻就会消失，使用越低的驱动频率闪烁越明显  
	*	5.在lcd_rgb.h里定义了一些常用的颜色，高8位为透明色，只在ARGB8888和ARGB1555模式下有效，ARGB1555只有
	*	  在高8位为0时才会显示透明。
	******************************************************************************************
***/

#include "lcd_7.h"

static pFONT *LCD_Fonts;

//LCD相关参数结构体
struct	
{
	u32 Color;  //LCD当前画笔颜色
	u32 BackColor;	//背景色
	u32 LayerMemoryAdd;	//层显存地址
	u8  Layer; //当前层
	u8  Direction;	//显示方向
	u8  BufferFlag;  //缓冲层使用标志
	u8  ShowNum_Mode;
}LCD;

      							
// 函数：IO口初始化
//
void LCD_GPIO_Config(void)
{ 
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_AHB1PeriphClockCmd( LCD_GPIO_CLK,ENABLE); 	
						                          
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;

	// LCD 颜色 R 引脚配置 
//	GPIO_PinAFConfig(LTDC_R0_PORT, LTDC_R0_PINSOURCE, GPIO_AF_LTDC);	//AT070TN83没有低2位的数据
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

  
  // LCD 颜色 G 引脚配置 
  
//	GPIO_PinAFConfig(LTDC_G0_PORT, LTDC_G0_PINSOURCE, GPIO_AF_LTDC);  //AT070TN83没有低2位的数据
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

  
  // LCD 颜色 B 引脚配置 
  
//	GPIO_PinAFConfig(LTDC_B0_PORT, LTDC_B0_PINSOURCE, GPIO_AF_LTDC);  //AT070TN83没有低2位的数据
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

  
  //控制线
	GPIO_PinAFConfig(LTDC_CLK_PORT,   LTDC_CLK_PINSOURCE,  GPIO_AF_LTDC); 
	GPIO_PinAFConfig(LTDC_HSYNC_PORT, LTDC_HSYNC_PINSOURCE,GPIO_AF_LTDC);  
	GPIO_PinAFConfig(LTDC_VSYNC_PORT, LTDC_VSYNC_PINSOURCE,GPIO_AF_LTDC);  
	GPIO_PinAFConfig(LTDC_DE_PORT,    LTDC_DE_PINSOURCE,   GPIO_AF_LTDC);
    
	GPIO_InitStruct.GPIO_Pin = LTDC_CLK_PIN;		//驱动时钟
	GPIO_Init(LTDC_CLK_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_HSYNC_PIN;	//水平同步
	GPIO_Init(LTDC_HSYNC_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_VSYNC_PIN;	//垂直同步
	GPIO_Init(LTDC_VSYNC_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = LTDC_DE_PIN;		//数据使能
	GPIO_Init(LTDC_DE_PORT, &GPIO_InitStruct);
			
	//背光
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Pin = LTDC_Black_PIN; 
	GPIO_Init(LTDC_Black_PORT, &GPIO_InitStruct);

	GPIO_SetBits(LTDC_Black_PORT,LTDC_Black_PIN);
	
	//AT070TN83控制线
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15; 			//连到83的第38脚
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = AT070TN83_MODE_PIN; 		//MODE脚
	GPIO_Init(AT070TN83_MODE_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = AT070TN83_LR_PIN; 		//水平扫描方向
	GPIO_Init(AT070TN83_LR_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = AT070TN83_UD_PIN; 		//垂直扫描方向
	GPIO_Init(AT070TN83_UD_PORT, &GPIO_InitStruct);

	GPIO_SetBits(AT070TN83_MODE_PORT,AT070TN83_MODE_PIN);		//MODE拉高，使用DE模式
	GPIO_SetBits(AT070TN83_LR_PORT,AT070TN83_LR_PIN);			//从左到右扫描
	GPIO_ResetBits(AT070TN83_UD_PORT,AT070TN83_UD_PIN);		//从上到下扫描
	   
	GPIO_ResetBits(GPIOA,GPIO_Pin_15);				//38脚要输出低电平，不然83会进入自检模式
	//把未用到的数据脚拉低
	GPIO_ResetBits(LTDC_R0_PORT,LTDC_R0_PIN);		
	GPIO_ResetBits(LTDC_R1_PORT,LTDC_R1_PIN);	
	GPIO_ResetBits(LTDC_G0_PORT,LTDC_G0_PIN);	
	GPIO_ResetBits(LTDC_G1_PORT,LTDC_G1_PIN);		
	GPIO_ResetBits(LTDC_B0_PORT,LTDC_B0_PIN);	
	GPIO_ResetBits(LTDC_B1_PORT,LTDC_B1_PIN);		
}


// 函数：LCD层设置初始化
//	
void LCD_LayerInit(void)
{
	LTDC_Layer_InitTypeDef LTDC_Layer_InitStruct; 

	LTDC_Layer_InitStruct.LTDC_HorizontalStart = HBP + 1;
	LTDC_Layer_InitStruct.LTDC_HorizontalStop = (LCD_Width + HBP);
	LTDC_Layer_InitStruct.LTDC_VerticalStart =  VBP + 1;
	LTDC_Layer_InitStruct.LTDC_VerticalStop = (LCD_Height + VBP);
	LTDC_Layer_InitStruct.LTDC_PixelFormat = ColorMode_0;	//像素格式设置
	LTDC_Layer_InitStruct.LTDC_ConstantAlpha = 255; 
	LTDC_Layer_InitStruct.LTDC_DefaultColorBlue = 0; 	//  默认的颜色       
	LTDC_Layer_InitStruct.LTDC_DefaultColorGreen = 0;       
	LTDC_Layer_InitStruct.LTDC_DefaultColorRed = 0;         
	LTDC_Layer_InitStruct.LTDC_DefaultColorAlpha = 0;
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_CA;    
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_CA;
	LTDC_Layer_InitStruct.LTDC_CFBLineNumber  = LCD_Height; //显示区域的行数
	LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_MemoryAdd; //第一层的起始地址
	
	if( ColorMode_0 == LCD_RGB565 || ColorMode_0 == LCD_ARGB1555 ) //判断颜色格式
	{
		LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_Width * 2) + 3);	 //每行的像素占的总字节数
		LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_Width * 2); 				 //行间距，某像素的起始处到下一行的起始处的增量
	}
	else //这里ARGB8888和RGB888使用相同的计算方式
	{	
		LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_Width * 4) + 3);	//每行的像素占的总字节数
		LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_Width * 4);  			   //行间距，某像素的起始处到下一行的起始处的增量
	}
	LTDC_LayerInit(LTDC_Layer1, &LTDC_Layer_InitStruct);	//初始化层1
 	LTDC_LayerCmd(LTDC_Layer1, ENABLE);  //使能层1

#if ( LCD_NUM_LAYERS == 2 )	//当定义了双层时

	LTDC_Layer_InitStruct.LTDC_PixelFormat = ColorMode_1;		//像素格式设置
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_1 = LTDC_BlendingFactor1_PAxCA;  
	LTDC_Layer_InitStruct.LTDC_BlendingFactor_2 = LTDC_BlendingFactor2_PAxCA;
	LTDC_Layer_InitStruct.LTDC_CFBStartAdress = LCD_MemoryAdd + LCD_MemoryAdd_OFFSET; //层2的起始地址

	if( ColorMode_1 == LCD_RGB565 || ColorMode_1 == LCD_ARGB1555 ) //判断颜色格式
	{
		LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_Width * 2) + 3);	//每行的像素占的总字节数
		LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_Width * 2);  				//行间距，某像素的起始处到下一行的起始处的增量
	}
	else //这里ARGB8888和RGB888使用相同的计算方式
	{	
		LTDC_Layer_InitStruct.LTDC_CFBLineLength = ((LCD_Width * 4) + 3);	//每行的像素占的总字节数
		LTDC_Layer_InitStruct.LTDC_CFBPitch = (LCD_Width * 4); 			 //行间距，某像素的起始处到下一行的起始处的增量
	}
	LTDC_LayerInit(LTDC_Layer2, &LTDC_Layer_InitStruct);	//初始化层2
	LTDC_LayerCmd(LTDC_Layer2, ENABLE);  //使能层2
#endif

	LTDC_ReloadConfig(LTDC_IMReload); //重新载入参数
	LTDC_DitherCmd(ENABLE);		//使能颜色抖动，24位色必须打开，否则无法达到24位色的效果
}

// 函数：LCD配置初始化
//	
void LCD_Init(void)
{ 
	u16 LCD_PLLSAIN = 0;		//用于倍频的PLLSAIN参数，可取范围为50~432
	u8  LCD_PLLSAIR = 3;		//用于分频的PLLSAIR参数，可取范围为2~7
	u8  LCD_CLKDIV	 = 2;		//LCD时钟分频参数，默认设置为2分频
	
	LTDC_InitTypeDef  LTDC_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_LTDC, ENABLE); 	//使能LTDC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2D, ENABLE); //使能DMA2D

	LCD_GPIO_Config();  //初始化LCD引脚
	SDRAM_Init(); 		  //初始化SDRAM

	LCD_PLLSAIN = LCD_CLK * LCD_PLLSAIR * LCD_CLKDIV;	//根据需要使用的LCD时钟计算PLLSAIN参数，可取范围为50~432
	RCC_PLLSAIConfig(LCD_PLLSAIN,7,LCD_PLLSAIR);  //时钟配置
//	RCC_PLLSAIConfig(320,7,2);  
	RCC_LTDCCLKDivConfig(RCC_PLLSAIDivR_Div2);	 //LCD时钟分频设置，要和LCD_CLKDIV对应
	RCC_PLLSAICmd(ENABLE);	//使能PLLSAI时钟
	while(RCC_GetFlagStatus(RCC_FLAG_PLLSAIRDY) == RESET);	//等待时钟配置完成
	
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

	LTDC_Init(&LTDC_InitStruct);	//初始化LCD控制器

	LCD_LayerInit();	//层初始化
	LTDC_Cmd(ENABLE);	//使能LCD控制器
	
	LCD_DisplayMode(Mode_H); //横屏显示
	LCD_SetFont(&Font32);  //设置默认字体	
	LCD_ShowNumMode(Fill_Space);	//数字显示默认填充空格
	
	LCD_SetLayer(0);  
	LCD_SetBackColor(LCD_BLACK); //设置背景色
	LCD_SetColor(LCD_WHITE);	//设置画笔颜色
	LCD_Clear(); 	//清屏，刷背景色

#if LCD_NUM_LAYERS == 2	 //如果定义了第二层
	LCD_SetLayer(1); 
	LCD_SetBackColor(LCD_BLACK); //设置背景色
	LCD_SetColor(LCD_WHITE);	//设置画笔颜色
	LCD_Clear(); //清屏，刷背景色
#endif
}  
//
//	函数：层设置
//	参数：layer - 要设置的层数，在此定为0是背景层，1是前景层
//
void LCD_SetLayer(u8 layer)
{
#if LCD_NUM_LAYERS == 2		//如果开了双层
	if (layer == 0)
	{
		LCD.LayerMemoryAdd = LCD_MemoryAdd; 	//设置层0的显存地址
	}
	else if(layer == 1)
	{
		LCD.LayerMemoryAdd = LCD_MemoryAdd + LCD_MemoryAdd_OFFSET;	//设置层1的显存地址
	}
	LCD.Layer = layer;	//记录当前所在的层
#else
	LCD.LayerMemoryAdd = LCD_MemoryAdd;	//单层显示的状态下，只设置层0的地址
	LCD.Layer = 0;		 //层标记默认为0
#endif

	LCD.BufferFlag = 0;	//缓冲层使用标记
}  

// 函数：设置颜色色
// 说明：此颜色包括显示字符、画点画线、图形等颜色
//
void LCD_SetColor(u32 Color)
{
	u32 Display_ColorMode = 0;		//当前层的颜色格式
	u16 Alpha_Value = 0, Red_Value = 0, Green_Value = 0, Blue_Value = 0; //各个颜色通道的值

#if(LCD_NUM_LAYERS == 2)		//如果开了双层
	if(LCD.Layer == 0)  			
		Display_ColorMode = ColorMode_0;		//获取层0的颜色格式
	else
		Display_ColorMode = ColorMode_1;		//获取层1的颜色格式
#else
	Display_ColorMode = ColorMode_0;	//单层显示下默认为层0的颜色格式
#endif

	if( Display_ColorMode == LCD_RGB565	)	//将32位色转换为16位色
	{
		Red_Value   = (u16)((Color&0x00F80000)>>8);
		Green_Value = (u16)((Color&0x0000FC00)>>5);
		Blue_Value  = (u16)((Color&0x000000F8)>>3);
		LCD.Color = (u16)(Red_Value | Green_Value | Blue_Value);		
	}
	else if( Display_ColorMode == LCD_ARGB1555 )	//将32位色转换为ARGB1555颜色
	{
		if( (Color & 0xFF000000) == 0 )	//判断是否使用透明色
			Alpha_Value = 0x0000;
		else
			Alpha_Value = 0x8000;

		Red_Value   = (u16)((Color&0x00F80000)>>9);	
		Green_Value = (u16)((Color&0x0000F800)>>6);
		Blue_Value  = (u16)((Color&0x000000F8)>>3);
		LCD.Color = (u16)(Alpha_Value | Red_Value | Green_Value | Blue_Value);	
	}
	else
		LCD.Color = Color;	//24位色或32位色不需要转换
}

// 函数：设置背景色
// 说明：用于字体等显示的背景颜色，而非LCD整个面板的颜色
//
void LCD_SetBackColor(u32 Color)
{
	u32 Display_ColorMode = 0;		//当前层的颜色格式
	u16 Alpha_Value = 0, Red_Value = 0, Green_Value = 0, Blue_Value = 0;  //各个颜色通道的值

#if(LCD_NUM_LAYERS == 2)	//如果开了双层
	if(LCD.Layer == 0)  
		Display_ColorMode = ColorMode_0;	//获取层0的颜色格式
	else
		Display_ColorMode = ColorMode_1;	//获取层1的颜色格式
#else
	Display_ColorMode = ColorMode_0;	//单层显示下默认为层0的颜色格式
#endif

	if( Display_ColorMode == LCD_RGB565	)	//将32位色转换为16位色
	{
		Red_Value   = (u16)((Color&0x00F80000)>>8);
		Green_Value = (u16)((Color&0x0000FC00)>>5);
		Blue_Value  = (u16)((Color&0x000000F8)>>3);
		LCD.BackColor = (u16)(Red_Value | Green_Value | Blue_Value);	
	}
	else if( Display_ColorMode == LCD_ARGB1555 )	//将32位色转换为ARGB1555颜色
	{
		if( (Color & 0xFF000000) == 0 )	//判断是否使用透明色
			Alpha_Value = 0x0000;
		else
			Alpha_Value = 0x8000;

		Red_Value   = (u16)((Color&0x00F80000)>>9);
		Green_Value = (u16)((Color&0x0000F800)>>6);
		Blue_Value  = (u16)((Color&0x000000F8)>>3);
		LCD.BackColor = (u16)(Alpha_Value | Red_Value | Green_Value | Blue_Value);	
	}
	else	
		LCD.BackColor = Color;	//24位色或32位色不需要转换
	
}

// 函数：设置字体
// 参数：字体选择，Font32/Font24/Font20/Font16/Font12
//
void LCD_SetFont(pFONT *fonts)
{
  LCD_Fonts = fonts;
}

// 函数：设置显示方向
//	参数：direction - 显示的方向，Mode_H 水平，Mode_V 竖直
//
void LCD_DisplayMode(u8 direction)
{
	LCD.Direction = direction;
}
//
// 函数：清屏函数M 
// 说明：将LCD背景全部设置为 LCD.BackColor 的颜色，使用DMA2D实现
//
void LCD_Clear(void)
{
	DMA2D_InitTypeDef   DMA2D_InitStruct;
	u32 Display_Add = 0;			//当前层的显存地址
	u8  Display_ColorMode = 0;	//当前层的颜色格式
	u8  Red_Value = 0, Green_Value = 0, Blue_Value = 0, Alpha_Value = 0;	//各个颜色通道的值

	/****************** 根据标志位切换显存地址***********************/
	
	if(LCD.BufferFlag == 0)
		Display_Add = LCD.LayerMemoryAdd;	
	else
		Display_Add = LCD_Buffer;
	
	/*************** 选择相应层的颜色模式和显存地址 *****************/
	
#if(LCD_NUM_LAYERS == 2)
	if(LCD.Layer == 0)  //根据不同的层选择不同的颜色格式
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
	
	/************* 根据不同的颜色模式来换算各颜色通道的值 ***************/
	
	if( Display_ColorMode == LCD_RGB565	)	//提取RGB565的RGB值
	{   
		Red_Value   = (u16)(0xF800 & LCD.BackColor) >> 11;
		Green_Value = (u16)(0x07E0 & LCD.BackColor) >> 5;
		Blue_Value  = (u16)(0x001F & LCD.BackColor);

	}
	else if( Display_ColorMode == LCD_ARGB1555	) //提取ARGB1555的RGB值
	{   
		if ( (LCD.BackColor & 0x8000) == 0 )	//判断是否有透明色
			Alpha_Value = 0x00;
		else
			Alpha_Value = 0xFF;

		Red_Value   = (u16)(0x7C00 & LCD.BackColor) >> 10;
		Green_Value = (u16)(0x03E0 & LCD.BackColor) >> 5;
		Blue_Value  = (u16)(0x001F & LCD.BackColor);
	}
	else		//24位色和32位色不需要转换
	{
		Blue_Value  = LCD.BackColor;
		Green_Value = LCD.BackColor >> 8;	
		Red_Value   = LCD.BackColor >> 16;   
		Alpha_Value = LCD.BackColor >> 24; 
	}
	
	/********************** 初始化DMA2D **************************/
	
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

	LTDC_ReloadConfig(LTDC_VBReload); //重新载入参数
	LCD.BufferFlag =~LCD.BufferFlag;	 //切换显存地址标志
}


//	函数：画点
//	参数：x-水平坐标，y-垂直坐标，color-要绘制的颜色
//
void LCD_DrawPoint(u16 x,u16 y,u32 color)
{
	u32 Display_Add = 0;			//当前层的显存地址
	u32 Display_ColorMode = 0;	//当前层的颜色格式

	/*************** 根据标志位切换显存地址 ****************/	
	
	if(LCD.BufferFlag == 0)	
		Display_Add = LCD_Buffer;	
	else
		Display_Add = LCD.LayerMemoryAdd;	

	/**************** 选择相应层的颜色模式 **************/
	
#if(LCD_NUM_LAYERS == 2)
		if(LCD.Layer == 0)  //根据不同的层选择不同的颜色格式
			Display_ColorMode = ColorMode_0;
		else
			Display_ColorMode = ColorMode_1;
#else
		Display_ColorMode = ColorMode_0;
#endif
	
	/****************** 32位色模式 ***********************/
		
	if( Display_ColorMode == LCD_ARGB8888 ) 
	{
		if (LCD.Direction == Mode_H) //水平方向
		{
			*(__IO u32*)( Display_Add + 4*(x + y*LCD_Width) ) = color ; 	
		}
		else if(LCD.Direction == Mode_V)	//垂直方向
		{
			*(__IO u32*)( Display_Add + 4*((LCD_Height - x - 1)*LCD_Width + y) ) = color ;
		}
	}
	
	/****************** 24位色模式 ***********************/	
	
	else if ( Display_ColorMode == LCD_RGB888 )
	{
		if (LCD.Direction == Mode_H) //水平方向
		{
			*(__IO u16*)( Display_Add + 3*(x + y*LCD_Width) ) = color ; 
			*(__IO  u8*)( Display_Add + 3*(x + y*LCD_Width) + 2 ) = color>>16 ; 	
		}
		else if(LCD.Direction == Mode_V)	//垂直方向
		{
			*(__IO u16*)( Display_Add + 3*((LCD_Height - x - 1)*LCD_Width + y) ) = color ; 
			*(__IO  u8*)( Display_Add + 3*((LCD_Height - x - 1)*LCD_Width + y) +2) = color>>16 ; 	
		}	
	}
	
	/****************** 16位色模式 ***********************/	
	else		
	{
		if (LCD.Direction == Mode_H) //水平方向
		{
			*(__IO u16*)( Display_Add + 2*(x + y*LCD_Width) ) = color ; 	
		}
		else if(LCD.Direction == Mode_V)	//垂直方向
		{
			*(__IO u16*)( Display_Add + 2*((LCD_Height - x - 1)*LCD_Width + y) ) = color ;
		}	
	}
}  

//	函数：读点
//	参数：x-水平坐标，y-垂直坐标
//	返回：读取的颜色
// 说明：在使用16或24位色模式时，读出来的颜色数据是16位或24位的
//
u32 LCD_ReadPoint(u16 x,u16 y)
{
	u32 color = 0;
	u32 Display_Add = 0;		//当前层的显存地址
	u32 Display_ColorMode = 0;	//当前层的颜色格式

	/*************** 根据标志位切换显存地址 ****************/	

	if(LCD.BufferFlag == 0)	
		Display_Add = LCD_Buffer;	
	else
		Display_Add = LCD.LayerMemoryAdd;	

	/**************** 选择相应层的颜色模式 **************/

#if(LCD_NUM_LAYERS == 2)
	if(LCD.Layer == 0)  //根据不同的层选择不同的颜色格式
		Display_ColorMode = ColorMode_0;
	else
		Display_ColorMode = ColorMode_1;
#else
	Display_ColorMode = ColorMode_0;
#endif

	/****************** 32位色模式 ***********************/
	if( Display_ColorMode == LCD_ARGB8888 ) 
	{
		if (LCD.Direction == Mode_H) //水平方向
		{
			color = *(__IO u32*)( Display_Add + 4*(x + y*LCD_Width) ); 	
		}
		else if(LCD.Direction == Mode_V)	//垂直方向
		{
			color = *(__IO u32*)( Display_Add + 4*((LCD_Height - x - 1)*LCD_Width + y) );
		}
	}
	/****************** 24位色模式 ***********************/	
	else if ( Display_ColorMode == LCD_RGB888 )
	{
		if (LCD.Direction == Mode_H) //水平方向
		{
			color = *(__IO u32*)( Display_Add + 3*(x + y*LCD_Width) ) &0x00ffffff; 	
		}
		else if(LCD.Direction == Mode_V)	//垂直方向
		{
			color = *(__IO u32*)( Display_Add + 3*((LCD_Height - x - 1)*LCD_Width + y) ) &0x00ffffff; 	
		}	
	}
	/****************** 16位色模式 ***********************/	
	else		
	{
		if (LCD.Direction == Mode_H) //水平方向
		{
			color = *(__IO u16*)( Display_Add + 2*(x + y*LCD_Width) ); 	
		}
		else if(LCD.Direction == Mode_V)	//垂直方向
		{
			color = *(__IO u16*)( Display_Add + 2*((LCD_Height - x - 1)*LCD_Width + y) );
		}	
	}
	return color;
}  

//	函数：显示单个字符
//	参数：x-水平坐标，y-垂直坐标，c-字符的地址
//
void LCD_DisplayChar(u16 x, u16 y,u8 add)
{
	u8  index = 0, counter = 0;
   u8   disChar;	//存储字符的地址
	u16  Xaddress = x; //水平坐标
	
	add = add - 32; 
	for(index = 0; index < LCD_Fonts->Sizes; index++)
	{
		disChar = LCD_Fonts->table[add*LCD_Fonts->Sizes + index]; //获取字符的模值
		for(counter = 0; counter < 8; counter++)
		{ 
			if(disChar & 0x01)	
			{		
				LCD_DrawPoint(Xaddress,y,LCD.Color);	//当前模值不为0时，使用画笔色绘点
			}
			else		
			{		
				LCD_DrawPoint(Xaddress,y,LCD.BackColor);	//否则使用背景色绘制点
			}
			disChar >>= 1;
			Xaddress++;  //水平坐标自加
			
			if( (Xaddress - x)==LCD_Fonts->Width ) //如果水平坐标达到了字符宽度，则退出当前循环
			{													//进入下一行的绘制
				Xaddress = x;
				y++;
				break;
			}
		}	
	}
}

//	函数：显示字符串
//	参数：x-水平坐标，y-垂直坐标，p-字符串的地址
//
void LCD_DisplayString( u16 x, u16 y, u8 *p) 
{  
	while ((x < LCD_Width) && (*p != 0))	//判断显示坐标是否超出显示区域并且字符是否为空字符
	{
		 LCD_DisplayChar( x,y,*p);
		 x += LCD_Fonts->Width; //显示下一个字符
		 p++;	//取下一个字符地址
	}
}
// 函数：取整数的第N位的数，用于拆分整数以进行显示
//	参数：number - 目标整数，size - 目标数的长度
//	返回：取出的第N位的数
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

// 函数：设置数字显示方式
//	参数：mode - 设置补0还是补空格，Fill_Space 填充空格，Fill_Zero 填充零
//
void LCD_ShowNumMode(u8 mode)
{
	LCD.ShowNum_Mode = mode;
}

//	函数：显示数字
//	参数：x-水平坐标，y-垂直坐标
//    	number - 要显示的数字,0到4294967295之间
//
void  LCD_DisplayNumber( u16 x, u16 y, u32 number, u8 len) 
{  
	u8 i,value;
	u8 zero_Flag = 0;

	for(i=0;i<len;i++)
	{
		value = GetNumber(number,len-i-1);	//获取当前整数的第N位数
		if( zero_Flag==0 && (i<len-1) )	//判断前面为0的部分
		{
			if(value == 0)
			{
				if(LCD.ShowNum_Mode == 0)	
					LCD_DisplayChar( x+i*LCD_Fonts->Width, y,48);	//填充0
				else
					LCD_DisplayChar( x+i*LCD_Fonts->Width, y,32);	//填充空格
				continue;
			}
			else
				zero_Flag = 1;	//当遇到第一个非0数时置1
		}									
		LCD_DisplayChar( x+i*LCD_Fonts->Width, y, value+48 );	//将拆分的数进行显示
	}
}
//显示有符号数
void  LCD_DisplayNumberyoufuhao( u16 x, u16 y, u32 number,u16 num, u8 len) 
{  
	u8 i,value,j=1;
	u8 zero_Flag = 0;
	if(number>=num)
		{
			number-=num;
			for(i=0;i<len;i++)
			{
				value = GetNumber(number,len-i-1);	//获取当前整数的第N位数
				if( zero_Flag==0 && (i<len-1) )	//判断前面为0的部分
				{
				  if(value == 0)
					{
						if(LCD.ShowNum_Mode == 0)	
							LCD_DisplayChar( x+i*LCD_Fonts->Width, y,48);	//填充0
						else
							LCD_DisplayChar( x+i*LCD_Fonts->Width, y,32);	//填充空格
						continue;
					}
					else
				    zero_Flag = 1;	//当遇到第一个非0数时置1
				}	
				if(j==1)
				{
					LCD_DisplayChar(x+(i-1)*LCD_Fonts->Width, y,45);
					j=0;
				}
				LCD_DisplayChar( x+i*LCD_Fonts->Width, y, value+48 );	//将拆分的数进行显示
			}
		}
	else
		LCD_DisplayNumber( x, y,number,len);
}
#define ABS(X)  ((X) > 0 ? (X) : -(X))    

//	函数：画线
//	参数：x1、y1为起点坐标，x2、y2为终点坐标
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
//	函数：画矩形
//	参数：x、y为起点坐标，width-矩形的长度，height-宽度
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
//	函数：画圆
//	参数：x、y为起点坐标，r-半径
//	说明：半径不能大于xy坐标的值
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
//	函数：画椭圆
//	参数：x、y为起点坐标，r1、r2分别为水平和垂直半轴的长度
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

//	函数：填充实心矩形
//	参数：x、y为起点坐标，width - 长度，height - 宽度
//
void LCD_FillRect(u16 x, u16 y, u16 width, u16 height)
{
	DMA2D_InitTypeDef   DMA2D_InitStruct;
	u32 Display_Add = 0;					//当前层的显存地址
	u8 Display_ColorMode = 0;			//当前层的颜色格式
	u8  Display_BytesPerPixel = 0;	//当前层的每个像素的字节数
	u8 Red_Value = 0, Green_Value = 0, Blue_Value = 0,Alpha_Value = 0;	//各颜色通道的值

	/****************** 根据标志位切换显存地址***********************/
	
	if(LCD.BufferFlag == 0)	
		Display_Add = LCD_Buffer;
	else
		Display_Add = LCD.LayerMemoryAdd;

	/*************** 选择相应层的颜色模式和像素点字节数 *****************/
	
#if(LCD_NUM_LAYERS == 2)
	if(LCD.Layer == 0)  //根据不同的层选择不同的配置
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

	/************* 根据不同的颜色模式来换算各颜色通道的值 ***************/
	
	if( Display_ColorMode == LCD_RGB565	)	//提取RGB565的RGB值
	{   
		Red_Value   = (u16)(0xF800 & LCD.Color) >> 11;
		Green_Value = (u16)(0x07E0 & LCD.Color) >> 5;
		Blue_Value  = (u16)(0x001F & LCD.Color);

	}
	else if( Display_ColorMode == LCD_ARGB1555	) //提取ARGB1555的RGB值
	{   
		if ( (LCD.Color & 0x8000) == 0 )	//判断是否使用透明色
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
	
	/********************** 初始化DMA2D **************************/	
	
	DMA2D_DeInit();
	DMA2D_InitStruct.DMA2D_Mode = DMA2D_R2M;       
	DMA2D_InitStruct.DMA2D_CMode = Display_ColorMode;      
	DMA2D_InitStruct.DMA2D_OutputGreen = Green_Value;      
	DMA2D_InitStruct.DMA2D_OutputBlue = Blue_Value;     
	DMA2D_InitStruct.DMA2D_OutputRed = Red_Value;                
	DMA2D_InitStruct.DMA2D_OutputAlpha = Alpha_Value;                   

	if(LCD.Direction == Mode_H)  //横屏填充
	{
		DMA2D_InitStruct.DMA2D_OutputOffset = (LCD_Width - width);                
		DMA2D_InitStruct.DMA2D_NumberOfLine = height;            
		DMA2D_InitStruct.DMA2D_PixelPerLine = width;
		DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Display_Add + Display_BytesPerPixel*(LCD_Width * y + x);
	}
	else	//竖屏填充
	{
		DMA2D_InitStruct.DMA2D_OutputOffset = (LCD_Width - height);                
		DMA2D_InitStruct.DMA2D_NumberOfLine = width;            
		DMA2D_InitStruct.DMA2D_PixelPerLine = height;			
		DMA2D_InitStruct.DMA2D_OutputMemoryAdd = Display_Add + Display_BytesPerPixel*((LCD_Height - x - 1 - width)*LCD_Width + y) ;
	}	
	DMA2D_Init(&DMA2D_InitStruct); 

	DMA2D_StartTransfer();	//开始传输
	while(DMA2D_GetFlagStatus(DMA2D_FLAG_TC) == RESET);	//等待传输完成
}

//	函数：填充实心圆
//	参数：x、y为起点坐标，r - 半径
// 说明：半径要大于y坐标
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
