#include "oscilloscope.h"
#include "delay.h"
#include "led.h"
#include "touch.h"
#include "adc.h"
#include "lcd.h"

u8 sample_time = 5;//����ʱ�������ñ���
u8 Sample_Time = ADC_SampleTime_84Cycles;
u16 multiple =100 , Yinit =160;
u16 freq_base = 185 ;//�ǲ���Ƶ�ʵ�������� ȡֵ��ADC_SampleTime_xCycles�й�
u16 amp_grid, time_grid;
u16 Position_Freq,position_freq;


extern u8 welcom_flag;
extern u8 OSC_flag;//OSC_flagΪ1 ����ʾ�����ſ��Խ���ѭ��
u8 OSC_ON_FLAG=0;//����ʾ��������������ѭ��
u16 buff[700];//�洢ADC���ɼ����Ĳ�������


/*******************************************************************************
* �������ƣ�       osc_get_keynum()
* �������ܣ�       �õ�ʾ���������´��������λ��
* ���������       ��
* ���������       key_touch ����λ��
* ����˵����       
*******************************************************************************/
u8 osc_get_keynum(void)
{
	u8 key_touch=0;
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)			//������������
	{	
		if(tp_dev.y[0]<479&&tp_dev.y[0]>421)//OFF ON EXIT
		{
			if(tp_dev.x[0]<70)key_touch=1;//OFF	
			else if(tp_dev.x[0]<140&&tp_dev.x[0]>70)key_touch=2;//ON
			else if(tp_dev.x[0]<620&&tp_dev.x[0]>520)key_touch=3;//EXIT
		}
		if(tp_dev.x[0]<720&&tp_dev.x[0]>620)//���ȵ���
		{
			if(tp_dev.y[0]<55&&tp_dev.y[0]>20)key_touch=4;//Expend_Y
			else if(tp_dev.y[0]<100&&tp_dev.y[0]>65)key_touch=5;//Shrink_Y
		}
		if(tp_dev.y[0]<210&&tp_dev.y[0]>130)//ˮƽ�����
		{
			if(tp_dev.x[0]<705&&tp_dev.x[0]>670)key_touch=6;//Expend_X
			else if(tp_dev.x[0]<750&&tp_dev.x[0]>715)key_touch=7;//Shrink_X
		}
		if(tp_dev.x[0]<750&&tp_dev.x[0]>670)//λ�õ��� ����ƽ��
		{
			if(tp_dev.y[0]<275&&tp_dev.y[0]>240)key_touch=8;//Position_UP
			else if(tp_dev.y[0]<320&&tp_dev.y[0]>285)key_touch=9;//Position_DOWN
		}
	}
	return key_touch; 
}
/*******************************************************************************
* �������ƣ�       Lcd_DrawNetwork()
* �������ܣ�       ��ʾ������ʾ��������
* ���������       ��
* ���������       ��
* ����˵����       
*******************************************************************************/
void Lcd_DrawNetwork(void)
{
	u16 index_y = 0;
	u16 index_x = 0;	
	
    //���е�	
	for(index_x = 60;index_x < 610;index_x += 50)
	{
		for(index_y = 20;index_y < 410;index_y += 10)
		{
			LCD_Fast_DrawPoint(index_x,index_y,WHITE);	
		}
	}
	//���е�
	for(index_y = 60;index_y < 410;index_y += 50)
	{
		for(index_x = 20;index_x < 610;index_x += 10)
		{
			LCD_Fast_DrawPoint(index_x,index_y,WHITE);	
		}
	}
}
/*******************************************************************************
* �������ƣ�       clear_point()
* �������ܣ�       �������Ҫ�����ε���һ�е�����
* ���������       u16 num
* ���������       ��
* ����˵����       
*******************************************************************************/
void clear_point(u16 num)
{
	u16 index_clear_lie = 0; 
	POINT_COLOR = BLACK ;
	for(index_clear_lie = 10;index_clear_lie < 410;index_clear_lie++)
	{		
		LCD_DrawPoint(num,index_clear_lie );
	}
	if(!((num-10)%50))//�ж�hang�Ƿ�Ϊ50�ı��� ���е�
	{
		for(index_clear_lie = 20;index_clear_lie < 410;index_clear_lie += 10)
		{		
			LCD_Fast_DrawPoint(num ,index_clear_lie,WHITE );
		}
	}
	if(!((num-10)%10))//�ж�hang�Ƿ�Ϊ10�ı��� ���е�
	{
		for(index_clear_lie = 60;index_clear_lie <410;index_clear_lie += 50)
		{		
			LCD_Fast_DrawPoint(num ,index_clear_lie,WHITE );
		}
	}	
	POINT_COLOR = YELLOW;	
}
/*******************************************************************************
* �������ƣ�       DrawOscillogram()
* �������ܣ�       ������ͼ ������ֵ Ƶ��
* ���������       u16 buff �ɼ����Ĳ��ε�ѹ����ֵ u8 sample_cycles ����ADC����������
* ���������       ��
* ����˵����       
*******************************************************************************/
void DrawOscillogram(u16 *buff,u8 sample_cycles)//������ͼ
{
	//�����������õı���
	u8 temp;//ADC��ʾʱ ��
	u16 i = 0,j=10,n=0;//i��n��������forѭ�� j��������������ת����
	u32 max_data=buff[1];
	u32 min_data=0;//��Ϊû�и�ѹģ�� ����ͨ���������ֵ *2 �ó� ���ֵ
	float Vpp=0;//���ֵ

	//����Ƶ�����ñ���
	u16 freq_temp,freq_a=1,freq_xian;//freq_temp��������һ�������� ������ռ����Ļ�ĵ�����freq_a�������� ���ξ��任������ݣ�freq_xian������ʾƵ��ʱ��
	u16 freq_j=0,freq[2];//������¼����������λ��
	u16 freq_buff_a[610];//freq_buff_a��¼�任��ķ���
	float Freq;//Ƶ��ֵ
	
	static u16 Ypos1 = 210,Ypos2 = 0;//������ʱ��
	
	//��ȡ�ɼ����Ĳ��ε����� ADֵ
	for(i = 1;i < 600;i++)
	{
		buff[i] = Get_Adc(ADC_Channel_5,sample_cycles);
	}
	//������ȡ����ѹֵ����Ӧ��������
	for(n = 1;n<600;n++)
	{
		if(buff[n] > max_data)
		{
			max_data = buff[n];
		}				
	} 
	//�����β������в��α任
	POINT_COLOR = YELLOW;
	for(i = 1;i < 600;i++)
	{
		j = i + 10;
		clear_point(j);	
		Ypos2 = ( buff[i] * multiple )/max_data + Yinit ;//ͨ������� ���Խ��������ֵ�Ĳ��� ��������ʾ ţ������������ʾ����ϵͳ�������
		//���α任 ���Ⲩ�θ�Ϊ����
		if(Ypos2 >= 210)
			freq_buff_a[freq_a++] = 260;
		else
			freq_buff_a[freq_a++] = 160;
		if(Ypos2>=10&&Ypos2<=410)
		LCD_DrawLine (j ,Ypos1 , j+1,Ypos2);//���Ʋ���ͼ
		Ypos1 = Ypos2 ;

	}
	//���ֵ��ʾ 
	POINT_COLOR = BLACK;	
	Vpp = (float)(max_data - min_data);//�ó���ֵ��Ӧ��������
	Vpp = (float) Vpp*(3.3/4096);//����Ϊģ����
	amp_grid = (Vpp * 1000) / multiple;
	temp = Vpp;//ȡ�����ֵ����������
	LCD_ShowNum(353,442,temp,1,16);//��ʾС����
	LCD_ShowChar(361,442,46,16,0);
	Vpp-=temp;//ȡ��С������
	Vpp*=1000;
	LCD_ShowxNum(369,442,Vpp,3,16,0X80);
	LCD_ShowChar(393,442,86,16,0);//��ʾ"V"
	
	LCD_ShowNum(727,54,amp_grid,4,12);
	//����Ƶ����
	for(i=1;i<600;i++)
	{
//		if(freq_buff_a[i]==160&&flag==0)//���ڵ͵�ƽ	
//			if(freq_buff_a[i+1]==260)//��������ֵ������ڸߵ�ƽ ����һ��������
		if(freq_buff_a[i]!=freq_buff_a[i+1])
				freq[freq_j++]=i;//��¼���������ص�����Ļ�϶�Ӧ��λ��	
		if(freq_j==2)break;//�õ����㹻������ ����ѭ�� ����ռ��̫����Դ
	}
	freq_temp = 2 * ( freq[1] - freq[0] );//�ó�����һ������������Ļ����ռ�ĵ���
	Freq = (float) (( freq_base * 1000 )/ freq_temp) ;//����Ƶ�� ��һ���׼ֵ ���� �� Ƶ�ʳɷ��ȵĹ�ϵ
	//��ʾ ԭ��ͬ���ֵ
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
* �������ƣ�       Osc_Welcome()
* �������ܣ�       ʾ������ӭ����
* ���������       ��
* ���������       ��
* ����˵����       ���뻶ӭ����� ����whileѭ�� �ȴ���������һ��ָ��ĵ���
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
	
	//��ʾ�����߿����ò�����ʾ����ɫ   
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
	//���ȵ��ڰ�ť
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
	//����ʱ����� ��ˮƽ���ڰ�ť
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
	//λ�õ�����ť �ڷ��ȵ��ڰ�ť�� ��ֱ�������220
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
	//ʾ�����Լ췽������ ͨ��PWM����һ������
	POINT_COLOR=BLACK;
	LCD_DrawLine(795,385,795,405);
	LCD_DrawLine(755,405,795,405);
	LCD_DrawLine(755,405,755,425);	
	LCD_DrawLine(755,425,795,425);
	LCD_DrawLine(795,425,795,445);
	LCD_ShowString(757,453,40,24,24,"PF9");
  //ʾ����ʹ��ע������	
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
* �������ƣ�       Osc_ON()
* �������ܣ�       ʾ�������� ��ʼ����
* ���������       ��
* ���������       ��
* ����˵����       �ú�������ʵ���� ���� �������� ˮƽ���� ����λ�������ƶ�
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
* �������ƣ�       Osc_Expend_Y()
* �������ܣ�       ����Ŵ�
* ���������       ��
* ���������       ��
* ����˵����       ÿ����һ�� ����Ŵ�ť multiple ���100 ���൱����ʾ������ռ�ĸ���������һ��
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
* �������ƣ�       Osc_Shrink_Y()
* �������ܣ�       ������С
* ���������       ��
* ���������       ��
* ����˵����       ÿ����һ�� ����Ŵ�ť multiple ���100 ���൱����ʾ������ռ�ĸ�������Сһ��
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
* �������ƣ�       Osc_Expend_X()
* �������ܣ�       ˮƽ�Ŵ�
* ���������       ��
* ���������       ��
* ����˵����       ͨ������ˮƽ���ηŴ�ť �� sample_time���м�1 Ȼ��ͨ��switch���ѡ����Ӧ�� ������
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
* �������ƣ�       Osc_Shrink_X()
* �������ܣ�       ˮƽ��С
* ���������       ��
* ���������       ��
* ����˵����       ͨ������ˮƽ������С��ť �� sample_time���м�1 Ȼ��ͨ��switch���ѡ����Ӧ�� ������
									���൱����߲���ʱ��
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
* �������ƣ�       Osc_UP()
* �������ܣ�       ��������
* ���������       ��
* ���������       ��
* ����˵����       YinitΪ������ʱ��ƫ��ֵ
*******************************************************************************/
void Osc_UP(void)
{
	if(Yinit > 30)
		Yinit -= 25;
	else
		Yinit = Yinit;
}
/*******************************************************************************
* �������ƣ�       Osc_Down()
* �������ܣ�       ��������
* ���������       ��
* ���������       ��
* ����˵����       YinitΪ������ʱ��ƫ��ֵ
*******************************************************************************/
void Osc_Down(void)
{
	if(Yinit < 290)
		Yinit += 25;
	else
		Yinit = Yinit;
}
/*******************************************************************************
* �������ƣ�       TIM5_PWM_Init()
* �������ܣ�       ���PWM��
* ���������       arr���Զ��ؼ���ֵ psc��Ԥ��Ƶϵ��
* ���������       ��
* ����˵����       ��PWM�����ö�������Դ���� ��Ϊʾ�����Լ��ź� ���źŵ�Ƶ�ʡ���ֵ
									 ��ռ�ձ�ͨ������ɵ�
*******************************************************************************/
void TIM5_PWM_Init(u32 arr,u32 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//ʹ��PORTAʱ��	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); //GPIOA0����Ϊ��ʱ��5
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;           //GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //��ʼ��PA0
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//��ʼ����ʱ��5
	
	//��ʼ��TIM5 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(TIM5, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM5 OC1

	TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);  //ʹ��TIM5��CCR1�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM5,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIM5
	
	TIM_SetCompare1(TIM5,250);	//�޸ıȽ�ֵ���޸�ռ�ձ�										  
}  
