#include "led.h"
#include "digital_multimeter.h"
#include "touch.h"
#include "lcd.h"
#include "delay.h"
#include "adc.h"
#include "timer.h"

extern u8 welcom_flag;
extern u8 DM_flag;//OSC_flag为1 数字示波器才可以进行循环
extern u8  TIM9CH1_CAPTURE_STA;		//输入捕获状态		    				
extern u16	TIM9CH1_CAPTURE_VAL;	//输入捕获值  

u8 dm_key=0;
u8 Voltage_flag = 0;
u8 Current_flag = 0;
u8 Cap_flag = 0;
u8 Res_flag = 0;
u8 Inductor_flag = 0;

/*******************************************************************************
* 函数名称：       DM_Init()
* 函数功能：       数字万用表初始化程序
* 输入参数：       无
* 输出参数：       无
* 函数说明：       配置CD4052
*******************************************************************************/
void DM_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOB时钟

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化
	
	GPIO_SetBits(GPIOB,GPIO_Pin_2 | GPIO_Pin_3);//GPIOA2,3 选择X3 不进行任何测量
}
/*******************************************************************************
* 函数名称：       DM_Adc_Init()
* 函数功能：       初始化数字万用表用的ADC3
* 输入参数：       无
* 输出参数：       无
* 函数说明：       无
*******************************************************************************/
void DM_Adc_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOA时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //使能ADC3时钟

  //先初始化ADC3通道6 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//PF8 通道6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//不带上下拉
  GPIO_Init(GPIOF, &GPIO_InitStructure);//初始化  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//复位结束	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//初始化
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC3, &ADC_InitStructure);//ADC初始化
	
 
	ADC_Cmd(ADC3, ENABLE);//开启AD转换器	
}
/*******************************************************************************
* 函数名称：       DM_Get_Adc()
* 函数功能：       得到ADC3通道ch采样转换后的值
* 输入参数：       ch：通道
* 输出参数：       无
* 函数说明：       无
*******************************************************************************/
u16 DM_Get_Adc(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC3, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADC3);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC3);	//返回最近一次ADC1规则组的转换结果
}
/*******************************************************************************
* 函数名称：       DM_GetAdc_Average()
* 函数功能：       将ADC3通道ch的值取times次平均
* 输入参数：       ch：通道 times：次数
* 输出参数：       无
* 函数说明：       无
*******************************************************************************/
u16 DM_GetAdc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=DM_Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
}
/*******************************************************************************
* 函数名称：       dm_get_keynum()
* 函数功能：       得到万用表界面下触摸区域的位置
* 输入参数：       无
* 输出参数：       key_touch 触摸位置
* 函数说明：       
*******************************************************************************/
u8 dm_get_keynum(void)
{
	u8 key_touch=0;
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
	{	
		if(tp_dev.x[0]<80&&tp_dev.y[0]<60)key_touch=1;//EXIT
		if(tp_dev.x[0]<500&&tp_dev.x[0]>300&&tp_dev.y[0]<340&&tp_dev.y[0]>140)key_touch=2;//电压测量
		if(tp_dev.y[0]<220&&tp_dev.y[0]>20)
		{
			if(tp_dev.x[0]<300&&tp_dev.x[0]>100)key_touch=3;//电流测量
			else if(tp_dev.x[0]<700&&tp_dev.x[0]>500)key_touch=4;//电容测量
		}
		if(tp_dev.y[0]<460&&tp_dev.y[0]>260)
		{
			if(tp_dev.x[0]<300&&tp_dev.x[0]>100)key_touch=5;//电阻测量
			else if(tp_dev.x[0]<700&&tp_dev.x[0]>500)key_touch=6;//电感测量
		}
	}
	return key_touch; 
}


/*******************************************************************************
* 函数名称：       DM_BackGround()
* 函数功能：       数字万用表背景制作
* 输入参数：       无
* 输出参数：       无
* 函数说明：       无
*******************************************************************************/
void DM_BackGround(void)
{
	u16 radius;
	LCD_Fill(0,0,80,60,LGRAY);
	LCD_ShowString(16,18,800,480,24,"EXIT");//电压
	for(radius=0;radius<=100;radius++)
	{
		POINT_COLOR=LGRAY; 
		LCD_Draw_Circle(400,240,radius);
		POINT_COLOR=BLUE; 
		LCD_Draw_Circle(200,120,radius);
		POINT_COLOR=GREEN; 
		LCD_Draw_Circle(200,360,radius);
		POINT_COLOR=YELLOW; 
		LCD_Draw_Circle(600,120,radius);
		POINT_COLOR=RED; 
		LCD_Draw_Circle(600,360,radius);
	}
	LCD_ShowString(364,228,800,480,24,"0.000V");//电压
	LCD_ShowString(152,108,800,480,24,"00.000mA");//电流
	LCD_ShowString(152,348,800,480,24,"0000000R");//电阻
	LCD_ShowString(540,108,800,480,24,"000.00nF");//电容
	LCD_ShowString(558,348,800,480,24,"00000uH");//电感
}
/*******************************************************************************
* 函数名称：       DM_Run()
* 函数功能：       功能选择函数
* 输入参数：       无
* 输出参数：       无
* 函数说明：       进入数字万用表后，首先进入该函数，通过相应的按钮可选择对应的功能
*******************************************************************************/
void DM_Run(void)
{
	DM_BackGround();
	while(DM_flag==1)
	{
		LED4 =! LED4;
		dm_key = dm_get_keynum();
		switch(dm_key)
		{
			case 1:
			{
				welcom_flag=1;
				DM_flag=0;
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 0;
				LCD_Clear(WHITE);
				Welcome_Background();
				break;
			}
			case 2:
			{
				Voltage_flag = 1;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 0;
				Voltage_Measure();
				break;
			}
			case 3:
			{
				Voltage_flag = 0;
				Current_flag = 1;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 0;
				I_Measure();
				break;
			}
			case 4:
			{
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 1;
				Res_flag = 0;
				Inductor_flag = 0;
				PA2_B = 0;
				PA3_A = 1;
				Cap_Measure();
				break;
			}
			case 5:
			{
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 1;
				Inductor_flag = 0;
				PA2_B = 0;
				PA3_A = 0;
				Res_Measure();
				break;
			}
			case 6:
			{
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 1;
				PA2_B = 1;
				PA3_A = 0;
				Indutor_Measure();
				break;
			}
		}
	}
}
/*******************************************************************************
* 函数名称：       Voltage_Measure()
* 函数功能：       电压测量显示
* 输入参数：       无
* 输出参数：       无
* 函数说明：       实时测量
*******************************************************************************/
void Voltage_Measure(void)
{	
	u16 adcx;
	float temp;
	while(Voltage_flag == 1)
	{
		LED4 =! LED4;
		dm_key = dm_get_keynum();
		adcx=DM_GetAdc_Average(ADC_Channel_6,20);//获取通道5的转换值，20次取平均
		temp=(float)adcx*(3.3/4096);          //获取计算后的带小数的实际电压值，比如3.1111
		adcx=temp;                            //赋值整数部分给adcx变量，因为adcx为u16整形
		LCD_ShowNum(364,228,adcx,1,24);
		LCD_ShowChar(376,228,46,24,0);//显示小数点
		temp-=adcx;                           //把已经显示的整数部分去掉，留下小数部分，比如3.1111-3=0.1111
		temp*=1000;                           //小数部分乘以1000，例如：0.1111就转换为111.1，相当于保留三位小数。
		LCD_ShowxNum(388,228,temp,3,24,0X80);	
		switch(dm_key)
		{
			case 1:
			{
				welcom_flag=1;
				DM_flag=0;
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 0;
				LCD_Clear(WHITE);
				Welcome_Background();
				break;
			}
			case 3:
			{
				Voltage_flag = 0;
				Current_flag = 1;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 0;
				I_Measure();
				break;
			}
			case 4:
			{
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 1;
				Res_flag = 0;
				Inductor_flag = 0;
				PA2_B = 0;
				PA3_A = 1;
				Cap_Measure();
				break;
			}
			case 5:
			{
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 1;
				Inductor_flag = 0;
				PA2_B = 0;
				PA3_A = 0;
				Res_Measure();
				break;
			}
			case 6:
			{
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 1;
				PA2_B = 1;
				PA3_A = 0;
				Indutor_Measure();
				break;
			}
		}
	}
}
/*******************************************************************************
* 函数名称：       I_Measure()
* 函数功能：       电流测量显示
* 输入参数：       无
* 输出参数：       无
* 函数说明：       实时测量
*******************************************************************************/
void I_Measure(void)
{
	u16 adcx;
	float temp;
	while(Current_flag == 1)
	{
		LED4 =! LED4;
		dm_key = dm_get_keynum();
		adcx=DM_GetAdc_Average(ADC_Channel_6,20);//获取通道5的转换值，20次取平均
		temp=(float)adcx*(11/4096);          //单位mV
		//temp =(float) temp / 300;//得到电流值 单位mA
		//显示部分
		adcx=temp;                            //赋值整数部分给adcx变量，因为adcx为u16整形
		LCD_ShowxNum(152,108,adcx,2,24,0);
		LCD_ShowChar(176,108,46,24,0);//显示小数点
		temp-=adcx;                           //把已经显示的整数部分去掉，留下小数部分，比如3.1111-3=0.1111
		temp*=1000;                           //小数部分乘以1000，例如：0.1111就转换为111.1，相当于保留3位小数。
		LCD_ShowxNum(188,108,temp,3,24,0X80);
		switch(dm_key)
		{
			case 1:
			{
				welcom_flag=1;
				DM_flag=0;
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 0;
				LCD_Clear(WHITE);
				Welcome_Background();
				break;
			}
			case 2:
			{
				Voltage_flag = 1;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 0;
				Voltage_Measure();
				break;
			}
			case 4:
			{
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 1;
				Res_flag = 0;
				Inductor_flag = 0;
				PA2_B = 0;
				PA3_A = 1;
				Cap_Measure();
				break;
			}
			case 5:
			{
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 1;
				Inductor_flag = 0;
				PA2_B = 0;
				PA3_A = 0;
				Res_Measure();
				break;
			}
			case 6:
			{
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 1;
				PA2_B = 1;
				PA3_A = 0;
				Indutor_Measure();
				break;
			}
		}
	}
}
/*******************************************************************************
* 函数名称：       Cap_Measure()
* 函数功能：       电容测量显示
* 输入参数：       无
* 输出参数：       无
* 函数说明：       实时测量
*******************************************************************************/
void Cap_Measure(void)
{
	long long temp=0;  //理论可测值 10pf~470nf
	float cap_value;
	u16 cap_temp;
	while(Cap_flag == 1)
	{
		LED4 =! LED4;
		dm_key = dm_get_keynum();
	 	if(TIM9CH1_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
		{
			delay_ms(100);
			temp=TIM9CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFF;		 		         //溢出时间总和
			temp+=TIM9CH1_CAPTURE_VAL;		   //得到总的高电平时间
			cap_value = (float) temp / 130 ;
			cap_temp = cap_value ;
			LCD_ShowxNum(540,108,cap_temp,3,24,0);
			LCD_ShowChar(576,108,46,24,0);//显示小数点
			cap_value = cap_value - cap_temp;
			cap_temp = cap_value * 100;
			LCD_ShowxNum(588,108,cap_temp,2,24,0x80);
			TIM9CH1_CAPTURE_STA=0;			     //开启下一次捕获
		}
		switch(dm_key)
		{
			case 1:
			{
				welcom_flag=1;
				DM_flag=0;
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 0;
				LCD_Clear(WHITE);
				Welcome_Background();
				break;
			}
			case 2:
			{
				Voltage_flag = 1;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 0;
				Voltage_Measure();
				break;
			}
			case 3:
			{
				Voltage_flag = 0;
				Current_flag = 1;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 0;
				I_Measure();
				break;
			}
			case 5:
			{
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 1;
				Inductor_flag = 0;
				PA2_B = 0;
				PA3_A = 0;
				Res_Measure();
				break;
			}
			case 6:
			{
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 1;
				PA2_B = 1;
				PA3_A = 0;
				Indutor_Measure();
				break;
			}
		}
	}
}
/*******************************************************************************
* 函数名称：       Res_Measure()
* 函数功能：       电阻测量显示
* 输入参数：       无
* 输出参数：       无
* 函数说明：       实时测量
*******************************************************************************/
void Res_Measure(void)
{
	long long temp=0;  //理论可测值 MΩ
	u32 res_value;
	while(Res_flag == 1)
	{
		delay_ms(100);
		LED4 =! LED4;
		dm_key = dm_get_keynum();
	 	if(TIM9CH1_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
		{
			temp=TIM9CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFF;		 		         //溢出时间总和
			temp+=TIM9CH1_CAPTURE_VAL;		   //得到总的高电平时间
			res_value =  (7.3* temp) - 10500;
			if(res_value <+ 1000)res_value = res_value;
			else if(res_value <= 10000)res_value = res_value + 30;
			else if(res_value < 100000)res_value = res_value + 750;
			else if(res_value < 1000000)res_value = res_value + 3600;
			else if(res_value < 2000000)res_value = res_value + 35000;
			else res_value = res_value;
			LCD_ShowxNum(152,348,res_value,7,24,0);
			TIM9CH1_CAPTURE_STA=0;			     //开启下一次捕获
		}
		switch(dm_key)
		{
			case 1:
			{
				welcom_flag=1;
				DM_flag=0;
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 0;
				LCD_Clear(WHITE);
				Welcome_Background();
				break;
			}
			case 2:
			{
				Voltage_flag = 1;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 0;
				Voltage_Measure();
				break;
			}
			case 3:
			{
				Voltage_flag = 0;
				Current_flag = 1;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 0;
				I_Measure();
				break;
			}
			case 4:
			{
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 1;
				Res_flag = 0;
				Inductor_flag = 0;
				PA2_B = 0;
				PA3_A = 1;
				Cap_Measure();
				break;
			}
			case 6:
			{
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 1;
				PA2_B = 1;
				PA3_A = 0;
				Indutor_Measure();
				break;
			}
		}
	}
}
/*******************************************************************************
* 函数名称：       Indutor_Measure()
* 函数功能：       电感测量显示
* 输入参数：       无
* 输出参数：       无
* 函数说明：       实时测量
*******************************************************************************/
void Indutor_Measure(void)
{
	long long temp=0;  //理论可测值 MΩ
	float inductor_value;

	while(Inductor_flag == 1)
	{
		delay_ms(150);
		LED4 =! LED4;
		dm_key = dm_get_keynum();
		if(TIM9CH1_CAPTURE_STA&0X80)        //成功捕获到了一次高电平
		{
			temp=TIM9CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFF;		 		         //溢出时间总和
			temp+=TIM9CH1_CAPTURE_VAL;		   //得到总的高电平时间
			if(temp < 32)
			inductor_value = 4.3029 * temp * temp -67.968 * temp + 326.7;
			else
			inductor_value = 0.2077 * temp * temp -34.57 * temp + 4193.6;
			LCD_ShowxNum(558,348,inductor_value,5,24,0);
			TIM9CH1_CAPTURE_STA=0;			     //开启下一次捕获
		}		
		switch(dm_key)
		{
			case 1:
			{
				welcom_flag=1;
				DM_flag=0;
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 0;
				LCD_Clear(WHITE);
				Welcome_Background();
				break;
			}
			case 2:
			{
				Voltage_flag = 1;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 0;
				Voltage_Measure();
				break;
			}
			case 3:
			{
				Voltage_flag = 0;
				Current_flag = 1;
				Cap_flag = 0;
				Res_flag = 0;
				Inductor_flag = 0;
				I_Measure();
				break;
			}
			case 4:
			{
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 1;
				Res_flag = 0;
				Inductor_flag = 0;
				PA2_B = 0;
				PA3_A = 1;
				Cap_Measure();
				break;
			}
			case 5:
			{
				Voltage_flag = 0;
				Current_flag = 0;
				Cap_flag = 0;
				Res_flag = 1;
				Inductor_flag = 0;
				PA2_B = 0;
				PA3_A = 0;
				Res_Measure();
				break;
			}
		}
	}
}

