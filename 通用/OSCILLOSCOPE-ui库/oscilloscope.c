#include "oscilloscope.h"
#include "delay.h"
#include "led.h"
#include "touch.h"
#include "adc.h"
#include "lcd.h"

u8 sample_time = 5;//调节时间轴所用变量
u8 Sample_Time = ADC_SampleTime_84Cycles;
u16 multiple =100 , Yinit =160;
u16 freq_base = 185 ;//是测量频率的灵魂所在 取值与ADC_SampleTime_xCycles有关
u16 amp_grid, time_grid;
u16 Position_Freq,position_freq;


extern u8 welcom_flag;
extern u8 OSC_flag;//OSC_flag为1 数字示波器才可以进行循环
u8 OSC_ON_FLAG=0;//进入示波器开机界面后的循环
u16 buff[700];//存储ADC所采集到的波形数据


/*******************************************************************************
* 函数名称：       osc_get_keynum()
* 函数功能：       得到示波器界面下触摸区域的位置
* 输入参数：       无
* 输出参数：       key_touch 触摸位置
* 函数说明：       
*******************************************************************************/
u8 osc_get_keynum(void)
{
	u8 key_touch=0;
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
	{	
		if(tp_dev.y[0]<479&&tp_dev.y[0]>421)//OFF ON EXIT
		{
			if(tp_dev.x[0]<70)key_touch=1;//OFF	
			else if(tp_dev.x[0]<140&&tp_dev.x[0]>70)key_touch=2;//ON
			else if(tp_dev.x[0]<620&&tp_dev.x[0]>520)key_touch=3;//EXIT
		}
		if(tp_dev.x[0]<720&&tp_dev.x[0]>620)//幅度调节
		{
			if(tp_dev.y[0]<55&&tp_dev.y[0]>20)key_touch=4;//Expend_Y
			else if(tp_dev.y[0]<100&&tp_dev.y[0]>65)key_touch=5;//Shrink_Y
		}
		if(tp_dev.y[0]<210&&tp_dev.y[0]>130)//水平轴调节
		{
			if(tp_dev.x[0]<705&&tp_dev.x[0]>670)key_touch=6;//Expend_X
			else if(tp_dev.x[0]<750&&tp_dev.x[0]>715)key_touch=7;//Shrink_X
		}
		if(tp_dev.x[0]<750&&tp_dev.x[0]>670)//位置调节 上下平移
		{
			if(tp_dev.y[0]<275&&tp_dev.y[0]>240)key_touch=8;//Position_UP
			else if(tp_dev.y[0]<320&&tp_dev.y[0]>285)key_touch=9;//Position_DOWN
		}
	}
	return key_touch; 
}
/*******************************************************************************
* 函数名称：       Lcd_DrawNetwork()
* 函数功能：       画示波器显示背景网格
* 输入参数：       无
* 输出参数：       无
* 函数说明：       
*******************************************************************************/
void Lcd_DrawNetwork(void)
{
	u16 index_y = 0;
	u16 index_x = 0;	
	
    //画列点	
	for(index_x = 60;index_x < 610;index_x += 50)
	{
		for(index_y = 20;index_y < 410;index_y += 10)
		{
			LCD_Fast_DrawPoint(index_x,index_y,WHITE);	
		}
	}
	//画行点
	for(index_y = 60;index_y < 410;index_y += 50)
	{
		for(index_x = 20;index_x < 610;index_x += 10)
		{
			LCD_Fast_DrawPoint(index_x,index_y,WHITE);	
		}
	}
}
/*******************************************************************************
* 函数名称：       clear_point()
* 函数功能：       清除即将要画波形的那一列的数据
* 输入参数：       u16 num
* 输出参数：       无
* 函数说明：       
*******************************************************************************/
void clear_point(u16 num)
{
	u16 index_clear_lie = 0; 
	POINT_COLOR = BLACK ;
	for(index_clear_lie = 10;index_clear_lie < 410;index_clear_lie++)
	{		
		LCD_DrawPoint(num,index_clear_lie );
	}
	if(!((num-10)%50))//判断hang是否为50的倍数 画列点
	{
		for(index_clear_lie = 20;index_clear_lie < 410;index_clear_lie += 10)
		{		
			LCD_Fast_DrawPoint(num ,index_clear_lie,WHITE );
		}
	}
	if(!((num-10)%10))//判断hang是否为10的倍数 画行点
	{
		for(index_clear_lie = 60;index_clear_lie <410;index_clear_lie += 50)
		{		
			LCD_Fast_DrawPoint(num ,index_clear_lie,WHITE );
		}
	}	
	POINT_COLOR = YELLOW;	
}
/*******************************************************************************
* 函数名称：       DrawOscillogram()
* 函数功能：       画波形图 测量幅值 频率
* 输入参数：       u16 buff 采集到的波形电压数字值 u8 sample_cycles 设置ADC采样的速率
* 输出参数：       无
* 函数说明：       
*******************************************************************************/
void DrawOscillogram(u16 *buff,u8 sample_cycles)//画波形图
{
	//测量幅度所用的变量
	u8 temp;//ADC显示时 用
	u16 i = 0,j=10,n=0;//i和n都是用作for循环 j用来波形纵坐标转换用
	u32 max_data=buff[1];
	u32 min_data=0;//因为没有负压模块 所以通过测量最大值 *2 得出 峰峰值
	float Vpp=0;//峰峰值

	//测量频率所用变量
	u16 freq_temp,freq_a=1,freq_xian;//freq_temp用来计算一个周期内 波形所占的屏幕的点数；freq_a用来控制 波形经变换后的数据；freq_xian用来显示频率时用
	u16 freq_j=0,freq[2];//用来记录两个上升沿位置
	u16 freq_buff_a[610];//freq_buff_a记录变换后的方波
	float Freq;//频率值
	
	static u16 Ypos1 = 210,Ypos2 = 0;//画波形时用
	
	//读取采集到的波形的数据 AD值
	for(i = 1;i < 600;i++)
	{
		buff[i] = Get_Adc(ADC_Channel_5,sample_cycles);
	}
	//用来获取最大电压值所对应的数字量
	for(n = 1;n<600;n++)
	{
		if(buff[n] > max_data)
		{
			max_data = buff[n];
		}				
	} 
	//画波形并将进行波形变换
	POINT_COLOR = YELLOW;
	for(i = 1;i < 600;i++)
	{
		j = i + 10;
		clear_point(j);	
		Ypos2 = ( buff[i] * multiple )/max_data + Yinit ;//通过此语句 可以将任意幅度值的波形 都居中显示 牛批！！！整个示波器系统灵魂所在
		//波形变换 任意波形改为方波
		if(Ypos2 >= 210)
			freq_buff_a[freq_a++] = 260;
		else
			freq_buff_a[freq_a++] = 160;
		if(Ypos2>=10&&Ypos2<=410)
		LCD_DrawLine (j ,Ypos1 , j+1,Ypos2);//绘制波形图
		Ypos1 = Ypos2 ;

	}
	//峰峰值显示 
	POINT_COLOR = BLACK;	
	Vpp = (float)(max_data - min_data);//得出峰值对应的数字量
	Vpp = (float) Vpp*(3.3/4096);//换算为模拟量
	amp_grid = (Vpp * 1000) / multiple;
	temp = Vpp;//取出峰峰值的整数部分
	LCD_ShowNum(353,442,temp,1,16);//显示小数点
	LCD_ShowChar(361,442,46,16,0);
	Vpp-=temp;//取出小数部分
	Vpp*=1000;
	LCD_ShowxNum(369,442,Vpp,3,16,0X80);
	LCD_ShowChar(393,442,86,16,0);//显示"V"
	
	LCD_ShowNum(727,54,amp_grid,4,12);
	//测量频率用
	for(i=1;i<600;i++)
	{
//		if(freq_buff_a[i]==160&&flag==0)//处于低电平	
//			if(freq_buff_a[i+1]==260)//接下来的值如果处于高电平 即是一个上升沿
		if(freq_buff_a[i]!=freq_buff_a[i+1])
				freq[freq_j++]=i;//记录两个上升沿的在屏幕上对应的位置	
		if(freq_j==2)break;//得到了足够的数据 跳出循环 避免占用太多资源
	}
	freq_temp = 2 * ( freq[1] - freq[0] );//得出波形一个周期内在屏幕上所占的点数
	Freq = (float) (( freq_base * 1000 )/ freq_temp) ;//计算频率 有一组基准值 点数 与 频率成反比的关系
	//显示 原理同峰峰值
	freq_xian = Freq;
	Position_Freq = Freq ;
	LCD_ShowNum(185,442,freq_xian,5,16);
	LCD_ShowChar(225,442,46,16,0);
	Freq -= freq_xian;
	Freq*=1000;
	position_freq = Freq;
	LCD_ShowxNum(233,442,Freq,3,16,0X80);
	LCD_ShowString(257,442,20,16,16,"Hz");
	
}
/*******************************************************************************
* 函数名称：       Osc_Welcome()
* 函数功能：       示波器欢迎界面
* 输入参数：       无
* 输出参数：       无
* 函数说明：       进入欢迎界面后 进入while循环 等待触摸即下一个指令的到来
*******************************************************************************/
void Osc_Welcome(void)
{
	u8 osc_off_key=0;
	{
	LCD_Clear(LGRAYBLUE);
	LCD_Fill(141,421,519,479,WHITE);
	LCD_Fill(0,421,70,479,RED);//OFF button
	LCD_Fill(71,421,140,479,BROWN);//ON button
	LCD_Fill(520,421,620,479,BROWN);//EXIT button
	LCD_Fill(621,331,680,369,RED);//CH1 button
	LCD_Fill(681,331,730,369,BROWN);//CH2 button
	LCD_Fill(731,331,799,369,LGRAY);//MATH button
	LCD_ShowString(17,438,40,24,24,"OFF");
	LCD_ShowString(93,438,40,24,24,"ON");
	LCD_ShowString(546,438,50,24,24,"EXIT");
	LCD_ShowString(632,338,50,24,24,"CH1");
	LCD_ShowString(692,338,50,24,24,"CH2");
	LCD_ShowString(746,338,50,24,24,"MATH");
	
	LCD_ShowString(150,442,40,16,16,"Fre:0Hz");
	LCD_ShowString(320,442,40,16,16,"VPP:0V");
	
	//画示波器边框及设置波形显示背景色   
	LCD_Fill(0,0,620,420,BLACK);//set background
	LCD_Fill(0,0,10,420,LGRAY);//draw left frame
	LCD_Fill(610,0,620,420,LGRAY);//draw right frame
	LCD_Fill(10,0,610,10,LGRAY);//draw up frame
	LCD_Fill(10,410,610,420,LGRAY);//draw down frame
	POINT_COLOR=BLUE;
	LCD_DrawRectangle(621,0,799,100);
	LCD_DrawRectangle(621,110,799,210);
	LCD_DrawRectangle(621,220,799,320);
	LCD_DrawRectangle(621,330,799,370);
	LCD_DrawRectangle(621,380,799,478);	
	POINT_COLOR=RED;
	LCD_DrawLine(622,19,798,19);
	LCD_DrawLine(622,129,798,129);
	LCD_DrawLine(622,239,798,239);
	LCD_DrawLine(750,381,750,477);
	POINT_COLOR=BLACK;
	LCD_ShowString(650,2,180,16,16,"Vertical_Adjust");
	LCD_ShowString(751,54,80,12,12,"0mv/grid");	
	LCD_ShowString(662,112,180,16,16,"Level_Adjust");	
	LCD_ShowString(621,162,50,16,16,"Expend");	
	LCD_ShowString(751,162,50,16,16,"Shrink");	
	LCD_ShowString(658,222,180,16,16,"Position_Move");	
	//幅度调节按钮
	POINT_COLOR=BLUE;
	LCD_DrawLine(630,30,670,21);
	LCD_DrawLine(670,21,710,30);
	LCD_DrawLine(631,30,709,30);
	LCD_Fill(655,31,685,55,BLUE);
	POINT_COLOR=BLUE;
	LCD_Fill(655,65,685,89,BLUE);
	LCD_DrawLine(631,90,709,90);
	LCD_DrawLine(670,99,710,90);
	LCD_DrawLine(630,90,670,99);
	//采样时间调整 即水平调节按钮
	POINT_COLOR=BLUE;
	LCD_DrawLine(670,170,680,131);
	LCD_DrawLine(670,170,680,209);
	LCD_DrawLine(680,132,680,208);
	LCD_Fill(681,155,705,185,BLUE);
	POINT_COLOR=BLUE;
	LCD_Fill(715,155,739,185,BLUE);
	LCD_DrawLine(740,132,740,208);
	LCD_DrawLine(740,131,750,170);
	LCD_DrawLine(740,209,750,170);
	//位置调整按钮 在幅度调节按钮上 垂直坐标加上220
	POINT_COLOR=BLUE;
	LCD_DrawLine(670,250,710,241);
	LCD_DrawLine(710,241,750,250);
	LCD_DrawLine(671,250,749,250);
	LCD_Fill(695,251,725,275,BLUE);
	POINT_COLOR=BLUE;
	LCD_Fill(695,285,725,309,BLUE);
	LCD_DrawLine(711,310,749,310);
	LCD_DrawLine(710,319,750,310);
	LCD_DrawLine(670,310,710,319);
	//示波器自检方波测试 通过PWM产生一个方波
	POINT_COLOR=BLACK;
	LCD_DrawLine(795,385,795,405);
	LCD_DrawLine(755,405,795,405);
	LCD_DrawLine(755,405,755,425);	
	LCD_DrawLine(755,425,795,425);
	LCD_DrawLine(795,425,795,445);
	LCD_ShowString(757,453,40,24,24,"PF9");
  //示波器使用注意事项	
	LCD_ShowString(638,381,130,24,24,"CH1:PA5");	
	LCD_ShowString(638,406,130,24,24,"CH2:PAX");	
	LCD_ShowString(631,431,130,24,24,"MAXVPP:3V");	
	LCD_ShowString(641,457,130,16,16,"^~(=o*o=)~^");	
	}
	while(OSC_flag==1)
	{
		osc_off_key = osc_get_keynum();
		switch(osc_off_key)
		{
			case 2:
			{
				OSC_ON_FLAG=1;
				OSC_flag=0;
				Osc_ON();
				break;
			}
			case 3:
			{
				welcom_flag=1;
				OSC_flag=0;
				LCD_Clear(WHITE);
				Welcome_Background();
				break;
			}
			default:break;
		}
	}
}
/*******************************************************************************
* 函数名称：       Osc_ON()
* 函数功能：       示波器开机 开始测量
* 输入参数：       无
* 输出参数：       无
* 函数说明：       该函数里面实现了 波形 垂轴缩放 水平缩放 波形位置上下移动
*******************************************************************************/
void Osc_ON(void)
{
	u8 osc_on_key=0;
	
	{
	LCD_Fill(0,421,70,479,BROWN);//off button
	LCD_Fill(71,421,140,479,RED);//on button
	LCD_ShowString(17,438,40,24,24,"OFF");
	LCD_ShowString(93,438,40,24,24,"ON");
	}
	while(OSC_ON_FLAG==1)
	{
		DrawOscillogram(buff,Sample_Time);
		LED2 =! LED2;
		osc_on_key = osc_get_keynum();
		switch(osc_on_key)
		{
			case 1:
			{
				OSC_ON_FLAG=0;
				OSC_flag=1;
				LCD_Clear(WHITE);
				Osc_Welcome();
				break;
			}
			case 3:
			{
				welcom_flag=1;
				OSC_flag=0;
				OSC_ON_FLAG=0;
				LCD_Clear(WHITE);
				Welcome_Background();
				break;
			}
			case 4:
			{
				Osc_Expend_Y();
				break;
			}
			case 5:
			{
				Osc_Shrink_Y();
				break;
			}
			case 6:
			{
				Osc_Expend_X();
				break;
			}
			case 7:
			{
				Osc_Shrink_X();
				break;
			}
			case 8:
			{
				Osc_UP();
				break;
			}
			case 9:
			{
				Osc_Down();
				break;
			}
			default :break;
		}
	}
}
/*******************************************************************************
* 函数名称：       Osc_Expend_Y()
* 函数功能：       垂轴放大
* 输入参数：       无
* 输出参数：       无
* 函数说明：       每按下一次 垂轴放大按钮 multiple 会加100 即相当于显示波形所占的格子数增加一倍
*******************************************************************************/
void Osc_Expend_Y(void)
{
	multiple += 100;
	Yinit -= 50;
	if(!(multiple==100||multiple==200||multiple==300||multiple==400))
	{
		multiple = 100;
		Yinit = 160;
	}	
}
/*******************************************************************************
* 函数名称：       Osc_Shrink_Y()
* 函数功能：       垂轴缩小
* 输入参数：       无
* 输出参数：       无
* 函数说明：       每按下一次 垂轴放大按钮 multiple 会减100 即相当于显示波形所占的格子数减小一倍
*******************************************************************************/
void Osc_Shrink_Y(void)
{
	multiple -= 100;
	Yinit += 50;
	if(!(multiple==100||multiple==200||multiple==300||multiple==400))
	{
		multiple = 100;
		Yinit = 160;
	}	
}
/*******************************************************************************
* 函数名称：       Osc_Expend_X()
* 函数功能：       水平放大
* 输入参数：       无
* 输出参数：       无
* 函数说明：       通过触摸水平波形放大按钮 对 sample_time进行减1 然后通过switch语句选择相应的 采样率
*******************************************************************************/
void Osc_Expend_X(void)
{
	sample_time--;
	switch(sample_time)
	{
		case 1:Sample_Time = ADC_SampleTime_3Cycles;freq_base=600;break;
		case 2:Sample_Time = ADC_SampleTime_15Cycles;freq_base=455;break;
		case 3:Sample_Time = ADC_SampleTime_28Cycles;freq_base=343;break;
		case 4:Sample_Time = ADC_SampleTime_56Cycles;freq_base=246;break;
		case 5:Sample_Time = ADC_SampleTime_84Cycles;freq_base=185;break;
		case 6:Sample_Time = ADC_SampleTime_112Cycles;freq_base=145;break;
		case 7:Sample_Time = ADC_SampleTime_144Cycles;freq_base=120;break;
		case 8:Sample_Time = ADC_SampleTime_480Cycles;freq_base=40;break;
		default:sample_time=5;Sample_Time = ADC_SampleTime_84Cycles;freq_base=185;break;
	}
}
/*******************************************************************************
* 函数名称：       Osc_Shrink_X()
* 函数功能：       水平缩小
* 输入参数：       无
* 输出参数：       无
* 函数说明：       通过触摸水平波形缩小按钮 对 sample_time进行加1 然后通过switch语句选择相应的 采样率
									即相当于提高采样时间
*******************************************************************************/
void Osc_Shrink_X(void)
{
	sample_time++;
	switch(sample_time)
	{
		case 1:Sample_Time = ADC_SampleTime_3Cycles;freq_base=600;break;
		case 2:Sample_Time = ADC_SampleTime_15Cycles;freq_base=455;break;
		case 3:Sample_Time = ADC_SampleTime_28Cycles;freq_base=343;break;
		case 4:Sample_Time = ADC_SampleTime_56Cycles;freq_base=246;break;
		case 5:Sample_Time = ADC_SampleTime_84Cycles;freq_base=185;break;
		case 6:Sample_Time = ADC_SampleTime_112Cycles;freq_base=145;break;
		case 7:Sample_Time = ADC_SampleTime_144Cycles;freq_base=120;break;
		case 8:Sample_Time = ADC_SampleTime_480Cycles;freq_base=40;break;
		default:sample_time=5;Sample_Time = ADC_SampleTime_84Cycles;freq_base=185;break;
	}
}
/*******************************************************************************
* 函数名称：       Osc_UP()
* 函数功能：       垂轴上移
* 输入参数：       无
* 输出参数：       无
* 函数说明：       Yinit为画波形时的偏移值
*******************************************************************************/
void Osc_UP(void)
{
	if(Yinit > 30)
		Yinit -= 25;
	else
		Yinit = Yinit;
}
/*******************************************************************************
* 函数名称：       Osc_Down()
* 函数功能：       垂轴下移
* 输入参数：       无
* 输出参数：       无
* 函数说明：       Yinit为画波形时的偏移值
*******************************************************************************/
void Osc_Down(void)
{
	if(Yinit < 290)
		Yinit += 25;
	else
		Yinit = Yinit;
}
/*******************************************************************************
* 函数名称：       TIM5_PWM_Init()
* 函数功能：       输出PWM波
* 输入参数：       arr：自动重加载值 psc：预分频系数
* 输出参数：       无
* 函数说明：       该PWM波采用独立的资源产生 作为示波器自检信号 其信号的频率、幅值
									 和占空比通过软件可调
*******************************************************************************/
void TIM5_PWM_Init(u32 arr,u32 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //GPIOA0复用为定时器5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA0
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//初始化定时器5
	
	//初始化TIM5 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM5 OC1

	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCR1上的预装载寄存器
 
  TIM_ARRPreloadConfig(TIM5,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM5, ENABLE);  //使能TIM5
	
	TIM_SetCompare1(TIM5,250);	//修改比较值，修改占空比										  
}  
