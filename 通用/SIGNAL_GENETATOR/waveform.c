#include "dac.h"
#include "sys.h"
#include "delay.h"
#include "math.h"
#include "waveform.h"
#include "lcd.h"
#include "led.h"
#include "beep.h"
#include "touch.h"
#define PI  3.14159
#define DAC_DHR12R1_ADDRESS     0x40007408 //ͨ��1 ���ݸ�ʽ12λ�Ҷ���

u16 noTable[tableSize];
u16 sinTable[tableSize];
u16 squareTable[tableSize];
u16 triTable[tableSize];
u16 sawTable[tableSize];

u16 exsample_sinTable[421];
u16 exsample_sawTable[421];
u16 exsample_squTable[421];
u16 exsample_triTable[421];

u32 PSC=0,ARR=655;//1K
u16 VOLTAGE=4095;

extern u8 welcom_flag;
extern u8 SG_flag;
u8 sg_key=0;

u8 sg_freq_key=0;
u8 sg_amp_key=0;
u8 SG_Freq_flag=0;
u8 SG_Amp_flag=0;

u8 Sin_ON_STA=0;
u8 Saw_ON_STA=0;
u8 Squ_ON_STA=0;
u8 Tri_ON_STA=0;

u16 DAC_Value=4095;
u8 duty_value=50;//ռ�ձȳ�ʼֵ
u8 fre_gewei=0,fre_shiwei=0,fre_baiwei=0,fre_qianwei=1,fre_wanwei=0;
u8 amp_gewei=0,amp_shiwei=0,amp_baiwei=3,amp_qianwei=3;
u32 fre_zong=1000,amp_zong=3300;

u16 i,radius;//i���������� radius������ʵ�֡��ΰ�
u16 keyboard_x,keyboard_y;//����������
static u16 Y_examp_pos1 = 0,Y_examp_pos2 = 0;//������ʵ������

/*******************************************************************************
* �������ƣ�       sg_get_keynum()
* �������ܣ�       �ú��������������� ��������� λ��
* ���������       ��
* ���������       key_touch ����λ��
* ����˵����       
*******************************************************************************/
u8 sg_get_keynum(void)
{
	u8 key_touch = 0;
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)			//������������
	{	
		if(tp_dev.x[0]<485&&tp_dev.x[0]>25&&tp_dev.y[0]<460&&tp_dev.y[0]>360)
		{
			if(tp_dev.x[0]<260&&tp_dev.x[0]>60&&tp_dev.y[0]<390&&tp_dev.y[0]>360)key_touch=1;//duty-
			if(tp_dev.x[0]<460&&tp_dev.x[0]>260&&tp_dev.y[0]<390&&tp_dev.y[0]>360)key_touch=2;//duty+
			if(tp_dev.y[0]<460&&tp_dev.y[0]>400)
			{
				if(tp_dev.x[0]<125&&tp_dev.x[0]>25)key_touch=3;//SIN
				else if(tp_dev.x[0]<245&&tp_dev.x[0]>145)key_touch=4;//SAW
				else if(tp_dev.x[0]<365&&tp_dev.x[0]>265)key_touch=5;//SQU
				else if(tp_dev.x[0]<485&&tp_dev.x[0]>385)key_touch=6;//TRI
			}
		}
		if(tp_dev.x[0]<790&&tp_dev.x[0]>480&&tp_dev.y[0]<230&&tp_dev.y[0]>15)
		{
			if(tp_dev.x[0]<580&&tp_dev.x[0]>480&&tp_dev.y[0]<120&&tp_dev.y[0]>20)key_touch=7;//select_amplitude_adjust
			if(tp_dev.x[0]<580&&tp_dev.x[0]>480&&tp_dev.y[0]<230&&tp_dev.y[0]>130)key_touch=8;//select_frequency_adjust
			if(tp_dev.x[0]<750&&tp_dev.x[0]>630&&tp_dev.y[0]<180&&tp_dev.y[0]>60)key_touch=9;//amp_adjust
			if(tp_dev.x[0]<640&&tp_dev.x[0]>590&&tp_dev.y[0]<65&&tp_dev.y[0]>15)key_touch=10;//amp_back
			if(tp_dev.x[0]<790&&tp_dev.x[0]>740&&tp_dev.y[0]<65&&tp_dev.y[0]>15)key_touch=11;//amp_ok
			if(tp_dev.x[0]<640&&tp_dev.x[0]>590&&tp_dev.y[0]<220&&tp_dev.y[0]>170)key_touch=12;//amp_left
			if(tp_dev.x[0]<790&&tp_dev.x[0]>740&&tp_dev.y[0]<220&&tp_dev.y[0]>170)key_touch=13;//amp_clr
		}
		
		if(tp_dev.x[0]<770&&tp_dev.x[0]>500&&tp_dev.y[0]<470&&tp_dev.y[0]>240)
		{
			if(tp_dev.y[0]<290&&tp_dev.y[0]>240)//��һ��
			{
				if(tp_dev.x[0]<560&&tp_dev.x[0]>500)key_touch=14;//freq_1
				else if(tp_dev.x[0]<630&&tp_dev.x[0]>570)key_touch=15;//freq_2
				else if(tp_dev.x[0]<700&&tp_dev.x[0]>640)key_touch=16;//freq_3
				else if(tp_dev.x[0]<770&&tp_dev.x[0]>710)key_touch=17;//freq_back
			}
			if(tp_dev.y[0]<350&&tp_dev.y[0]>300)//�ڶ���
			{
				if(tp_dev.x[0]<560&&tp_dev.x[0]>500)key_touch=18;//freq_4
				else if(tp_dev.x[0]<630&&tp_dev.x[0]>570)key_touch=19;//freq_5
				else if(tp_dev.x[0]<700&&tp_dev.x[0]>640)key_touch=20;//freq_6
				else if(tp_dev.x[0]<770&&tp_dev.x[0]>710)key_touch=21;//freq_ok
			}
			if(tp_dev.y[0]<410&&tp_dev.y[0]>360)//������
			{
				if(tp_dev.x[0]<560&&tp_dev.x[0]>500)key_touch=22;//freq_7
				else if(tp_dev.x[0]<630&&tp_dev.x[0]>570)key_touch=23;//freq_8
				else if(tp_dev.x[0]<700&&tp_dev.x[0]>640)key_touch=24;//freq_9
				else if(tp_dev.x[0]<770&&tp_dev.x[0]>710)key_touch=25;//out
			}
			if(tp_dev.y[0]<470&&tp_dev.y[0]>420)//������
			{
				if(tp_dev.x[0]<560&&tp_dev.x[0]>500)key_touch=26;//CH1
				else if(tp_dev.x[0]<630&&tp_dev.x[0]>570)key_touch=27;//freq_0
				else if(tp_dev.x[0]<700&&tp_dev.x[0]>640)key_touch=28;//MOV
				else if(tp_dev.x[0]<770&&tp_dev.x[0]>710)key_touch=29;//EXIT
			}
		}
	}
	return key_touch; 
}


/*******************************************************************************
* �������ƣ�       No_Generation()
* �������ܣ�       ����DAC��ֵΪ0 �Ӷ�ʹ��û�в������
* ���������       ��
* ���������       ��
* ����˵����       
*******************************************************************************/
void No_Generation(void)
{
	u16 n;
	for(n=0;n<tableSize;n++)
	{
		noTable[n]= 0;
	}
}

/*******************************************************************************
* �������ƣ�       sin_example_Generation()
* �������ܣ�       �õ�ʾ��SIN�Ĳ�������
* ���������       ��
* ���������       ��
* ����˵����       ��
*******************************************************************************/
void sin_example_Generation(void)
{
	u16 n;
	for(n=0;n<421;n++)
	{
		exsample_sinTable[n]=	(sin(2*3.14159*n/140)+1)*100+70;	
	}	
}
/*******************************************************************************
* �������ƣ�       square_example_Generation()
* �������ܣ�       �õ�ʾ��SQU�Ĳ�������
* ���������       ��
* ���������       ��
* ����˵����       ��
*******************************************************************************/
void square_example_Generation(void)
{
	u16 n;
	u16 temp;
	temp = ( 140 * duty_value ) / 100;
	for(n=0;n<=140;n++)
	{
		if(n < temp)
		{
			exsample_squTable[n] = 70;	
			exsample_squTable[n+140] = 70;	
			exsample_squTable[n+280] = 70;	
		}
		else
		{
			exsample_squTable[n] = 270;	
			exsample_squTable[n+140] = 270;		
			exsample_squTable[n+280] = 270;		
		}
	}	
}
/*******************************************************************************
* �������ƣ�       tri_example_Generation()
* �������ܣ�       �õ�ʾ��TRI�Ĳ�������
* ���������       ��
* ���������       ��
* ����˵����       ��
*******************************************************************************/
void tri_example_Generation(void)
{
	u16 n;
	for(n=0;n<=140;n++)
	{
		if(n<35)
		{
			exsample_triTable[n] = (1190 - 20 * n) / 7;		
			exsample_triTable[n+140] = (1190 - 20 * n) / 7;	
			exsample_triTable[n+280] = (1190 - 20 * n) / 7;					
		}
		else if(n>=35&&n<=105)
		{
			exsample_triTable[n] = (20 * n -210) / 7;		
			exsample_triTable[n+140] = (20 * n -210) / 7;		
			exsample_triTable[n+280] = (20 * n -210) / 7;		
		}
		else 
		{
			exsample_triTable[n] = (3990 - 20 * n) / 7;		
			exsample_triTable[n+140] = (3990 - 20 * n) / 7;	
			exsample_triTable[n+280] = (3990 - 20 * n) / 7;	
		}
	}	
}

/*******************************************************************************
* �������ƣ�       saw_example_Generation()
* �������ܣ�       �õ�ʾ��SAW�Ĳ�������
* ���������       ��
* ���������       ��
* ����˵����       ��
*******************************************************************************/
void saw_example_Generation(void)
{
	u16 n;
	for(n=0;n<=140;n++)
	{
		if(n<=70)
		{
			exsample_sawTable[n] = (1190 - 10 * n) / 7;		
			exsample_sawTable[n+140] = (1190 - 10 * n) / 7;		
			exsample_sawTable[n+280] = (1190 - 10 * n) / 7;					
		}
		else 
		{
			exsample_sawTable[n] = (2590 - 10 * n) / 7;		
			exsample_sawTable[n+140] = (2590 - 10 * n) / 7;		
			exsample_sawTable[n+280] = (2590 - 10 * n) / 7;					
		}
	}	
}
/*******************************************************************************
* �������ƣ�       sin_Generation()
* �������ܣ�       �õ�SIN�Ĳ�������
* ���������       sin_amp
* ���������       ��
* ����˵����       ��
*******************************************************************************/
void sin_Generation(u16 sin_amp)
{
	u16 n;
	for(n=0;n<tableSize;n++)
	{
		sinTable[n] = ((sin(2 * PI * n / tableSize) + 1) * sin_amp) / 2;	
	}	
}

/*******************************************************************************
* �������ƣ�       square_Generation()
* �������ܣ�       �õ�SQU�Ĳ�������
* ���������       square_amp
* ���������       ��
* ����˵����       ��
*******************************************************************************/
void square_Generation(u16 square_amp)
{
	u16 n;
	u16 temp;
	temp = (u16)( tableSize * duty_value ) / 100;
	for(n=0;n<tableSize;n++)
	{	
		if(n< temp)
			squareTable[n] = square_amp;	
		else
			squareTable[n] = 0;		
	}	
}

/*******************************************************************************
* �������ƣ�       tri_Generation()
* �������ܣ�       �õ�TRI�Ĳ�������
* ���������       tri_amp
* ���������       ��
* ����˵����       ��
*******************************************************************************/
void tri_Generation(u16 tri_amp)
{
	u16 n;
	for(n=0;n<tableSize;n++)
	{
		if(n <= 31)
			triTable[n] = (tri_amp*(n + 31)) / 62;	
		else if(n > 31 && n < 95)
			triTable[n] = (( 95  - n ) * tri_amp) /  64;	
		else 
			triTable[n] = (tri_amp * (n - 95 )) / 64;	
	}	
}

/*******************************************************************************
* �������ƣ�       saw_Generation()
* �������ܣ�       �õ�SAW�Ĳ�������
* ���������       saw_amp
* ���������       ��
* ����˵����       ��
*******************************************************************************/
void saw_Generation(u16 saw_amp)
{
	u16 n;
	for(n=0;n<tableSize;n++)
	{
		if(n < 63)
			sawTable[n] = (saw_amp * (n + 63)) / 126;	
		else if(n > 63 && n < 127)
			sawTable[n] = (saw_amp * (n - 63)) / 126 ;	
		
		sawTable[63] = 0;
		sawTable[127] = saw_amp / 2;
	}	
}
/*******************************************************************************
* �������ƣ�       TIM6_Configuration()
* �������ܣ�       ���ö�ʱ��6
* ���������       pscԤ��Ƶϵ�� arr�Զ���װ��ֵ
* ���������       ��
*******************************************************************************/
void TIM6_Configuration(u16 psc,u16 arr)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);	

 	TIM_PrescalerConfig(TIM6,psc, TIM_PSCReloadMode_Update);
  TIM_SetAutoreload(TIM6,arr);
  TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
	TIM_Cmd(TIM6, ENABLE);
}
/*******************************************************************************
* �������ƣ�       DacGPIO_Configuration()
* �������ܣ�       ���ú���������������� PA4 DAC1
* ���������       ��
* ���������       ��
* ����˵����       ����Ƶ�� 84M/((psc+1)*(arr+1))
*******************************************************************************/
void DacGPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//ʱ��ʹ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//ѡ��4������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ�����뷽ʽ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��PA4
}
/*******************************************************************************
* �������ƣ�       DAC_DMA_Configuration()
* �������ܣ�       DACͨ��1���� DMA������5ͨ��7����
* ���������       u32 cmar ������������ʼ��ַ
* ���������       ��
*******************************************************************************/
void DAC_DMA_Configuration(u32 cmar)
{
	DAC_InitTypeDef  DAC_InitStructure;	//DAC��ʼ���ṹ��

	DMA_InitTypeDef DMA_InitStructure;//DMA��ʼ���ṹ��

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);//ʹ��DMA����
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//ʹ��DAC
	
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;//ѡ��DAC������ʽ ѡ�� ��ʱ��6����
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;//DAC�޲��β���
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;//�ر�DAC������� ����Ҫ��������Ȼ���ر� ��������� ������
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);//��ʼ��DACͨ��1
	//����DMA1 Stream 5 channel 7 ����DAC1
	DMA_DeInit(DMA1_Stream5);//��DMA1��������5��ΪĬ��ֵ
	DMA_InitStructure.DMA_Channel = DMA_Channel_7; //ͨ��7
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1_ADDRESS;//DMA�������ַ (uint32_t)&DAC->DHR12R1
	DMA_InitStructure.DMA_Memory0BaseAddr = cmar;//DMA�ڴ����ַ
	DMA_InitStructure.DMA_BufferSize = tableSize;//DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;//���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//����������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//DMAͨ�� xӵ�и����ȼ� 
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���   
	
	DMA_Cmd(DMA1_Stream5, ENABLE);//ʹ��DMA
	
	DAC_Cmd(DAC_Channel_1, ENABLE);//ʹ��DAC1
	
	DAC_DMACmd(DAC_Channel_1, ENABLE);//ʹ��DACͨ��1��DMA
} 
/*******************************************************************************
* �������ƣ�       Get_PSCARR()
* �������ܣ�       �õ�Ԥ��Ƶϵ��PSC
* ���������       u32 Frequency
* ���������       u32 psc Ԥ��Ƶϵ��
* ����˵����       DAC����Ĳ��ε�Ƶ��Ϊ��
									���õĶ�ʱ���Ĵ���Ƶ��/DMA�Ļ������=������ε�Ƶ��
									���㷨��Ҫ��ͨ������PSC�������Ӧ��Ƶ��
*******************************************************************************/
u32 Get_PSCARR(u32 Frequency)
{
	u32 psc=0;float temp;u8 small;
	if(Frequency>=5&&Frequency<40000)
	{
		temp=163840/Frequency;
		psc=(u32)temp;
		small=temp-psc;
		small=small*10;
		if(small>=5)
			psc=psc+1;
	}
	else if(Frequency>=100&&Frequency<1000)
	{
		temp=163800/Frequency;
		psc=(u32)temp;
		small=temp-psc;
		small=small*10;
		if(small>=5)
			psc=psc+1;
	}
	else if(Frequency>=1000&&Frequency<10000)
	{
		temp=165000/Frequency;
		psc=(u32)temp;
		small=temp-psc;
		small=small*10;
		if(small>=5)
			psc=psc+1;
	}
	else if(Frequency>=10000&&Frequency<=40000)
	{
		temp=153750/Frequency;
		psc=(u32)temp;
		small=temp-psc;
		small=small*10;
		if(small>=5)
			psc=psc+1;
	}
	return psc;
}
/*******************************************************************************
* �������ƣ�       Get_PSCARR()
* �������ܣ�       �õ�Ԥ��Ƶϵ��PSC
* ���������       u32 Frequency
* ���������       u32 psc Ԥ��Ƶϵ��
* ����˵����       DAC����Ĳ��ε�Ƶ��Ϊ�����õĶ�ʱ���Ĵ���Ƶ��/DMA�Ļ������=������ε�Ƶ��
									���㷨��Ҫ��ͨ������ARR�������Ӧ�Ķ�Ƶ�� ��ʵ֤������Ч������ 
									��������ܹ����Ч������ӵĺ� ���Գ���һ��
*******************************************************************************/
void Get_TIM_PSCARR(u32 Frequency)
{
	u32 uiTIMxCLK;
	uiTIMxCLK = SystemCoreClock/2;
	if(Frequency<100)
	{
		PSC=10000-1;
		ARR=(uiTIMxCLK/10000)/Frequency -1 ;
	}
	else if(Frequency<3000)
	{
		PSC=100-1;
		ARR=(uiTIMxCLK/100)/Frequency -1 ;
	}
	else
	{
		PSC=0;//����Ҫ��Ƶ ��Ƶ��Ϊ1
		ARR= uiTIMxCLK / Frequency -1 ;
	}
}
/*******************************************************************************
* �������ƣ�       Get_VOLTAGE()
* �������ܣ�       �õ���ӦDAC��ֵ
* ���������       u16 vol ����ĵ�ѹֵ
* ���������       u16 temp ��Ӧ��DAC��ֵ
* ����˵����       ����ͨ��1�����ѹ  vol:0~3300,����0~3.3V ���֮ǰ�Ƚ����ж�
										ʹ�þ��ȸ���
*******************************************************************************/
u16 Get_VOLTAGE(u16 vol)
{
	u16 error_adjust;
	double temp=vol;
	temp = ( temp * 4095)/ 3300;
	error_adjust = temp;
	error_adjust = (temp - error_adjust) * 10;
	if(error_adjust < 5)
		temp = temp ;
	else 
		temp = temp + 1;
	return (u16)temp;
}
/*******************************************************************************
* �������ƣ�       SG_Welcome()
* �������ܣ�       �źŷ�������ӭ����
* ���������       ��
* ���������       ��
* ����˵����       �ڴ˽��� �ȴ�������Ӧ�Ĳ���
*******************************************************************************/
void SG_Welcome(void)
{
	u16 duty_temp=260;//ռ�ձȽ�������ʼֵ �ܹ�ռ400��
	{
	square_example_Generation();
	sin_example_Generation();//����ʾ������
	tri_example_Generation();
	saw_example_Generation();
	//������������߿�
	LCD_Fill(0,0,9,479,DARKBLUE);
	LCD_Fill(791,0,799,479,DARKBLUE);
	LCD_Fill(10,0,790,9,DARKBLUE);
	LCD_Fill(10,471,791,479,DARKBLUE);
	LCD_Fill(10,10,460,300,BLACK);//��������������ʾ����
	LCD_Fill(10,301,461,310,DARKBLUE);//�����±߿�
	LCD_Fill(461,10,470,310,DARKBLUE);//�����б߿�
	//���������ز�����ʾ
	POINT_COLOR=RED;
	LCD_ShowString(31,17,800,480,16,"DAC:0000");
	LCD_ShowxNum(63,17,DAC_Value,4,16,0);
	LCD_ShowString(126,17,800,480,16,"AMP:0000mV");
	LCD_ShowxNum(158,17,amp_zong,4,16,0);
	LCD_ShowString(237,17,800,480,16,"Duty:50%");
	LCD_ShowxNum(277,17,duty_value,2,16,0);
	LCD_ShowString(332,17,800,480,16,"Freq:00000Hz");
	LCD_ShowxNum(372,17,fre_zong,5,16,0);
	//����ռ�ձȵ��� ���ֻ���ڲ���Ϊ����ʱ������ ��סdutyָʾ����߿��Լ�Сռ�ձ� �ұ߿�������ռ�ձ� ����Ϊ1
	POINT_COLOR=BLACK;
	LCD_ShowString(60,318,800,480,24,"Duty:2% ~ 98%");
	LCD_DrawRectangle(59,360,461,390);
	LCD_Fill(60,362,duty_temp,388,LIGHTBLUE);
	//����ѡ��ť �����ֲ���
	POINT_COLOR=BLACK;
	LCD_Fill(25,400,125,460,LIGHTBLUE);//sin
	LCD_ShowString(57,418,800,480,24,"SIN");
	LCD_Fill(145,400,245,460,LIGHTBLUE);//saw
	LCD_ShowString(177,418,800,480,24,"SAW");
	LCD_Fill(265,400,365,460,LIGHTBLUE);//squ
	LCD_ShowString(297,418,800,480,24,"SQU");
	LCD_Fill(385,400,485,460,LIGHTBLUE);//tri
	LCD_ShowString(417,418,800,480,24,"TRI");
	//��ֵ������ʾ����
	LCD_DrawRectangle(480,20,580,120);
	LCD_ShowString(512,23,800,480,24,"AMP");
	LCD_ShowString(498,57,800,480,16,"0~3300mV");
	LCD_ShowxNum(506,88,amp_zong,4,24,0);
	//Ƶ����ʾ���ô���
	LCD_DrawRectangle(480,130,580,230);
	LCD_ShowString(506,133,800,480,24,"Freq");
	LCD_ShowString(494,167,800,480,16,"0~30000Hz");
	LCD_ShowxNum(502,198,fre_zong,5,24,0);
	//��ֵ������ť����
	POINT_COLOR=GRAY;
	for(radius=0;radius<=60;radius++)//��ֵ���ڰ�ť
	LCD_Draw_Circle(690,120,radius);
	POINT_COLOR=BLACK;
	//λ��ѡ��ť
	LCD_Fill(590,170,640,220,LGRAY);//����
	LCD_DrawLine(600,195,630,175);
	LCD_DrawLine(600,195,630,215);
	LCD_DrawLine(630,175,630,215);
	LCD_Fill(740,170,790,220,LGRAY);//clear
	LCD_ShowString(747,183,800,480,24,"CLR");
	LCD_Fill(590,15,640,65,LGRAY);//���� ���ȱ���Ϊԭ����ֵ
	LCD_ShowString(600,32,800,480,16,"BACK");
	LCD_Fill(740,15,790,65,LGRAY);//ȷ�ϰ�ť
	LCD_ShowString(753,28,800,480,24,"OK");
	//Ƶ�����ü��̰�ť
	POINT_COLOR=BLACK;
	for(keyboard_y=240;keyboard_y<=420;keyboard_y=keyboard_y+60)
		for(keyboard_x=500;keyboard_x<=770;keyboard_x=keyboard_x+70)
			LCD_Fill(keyboard_x,keyboard_y,keyboard_x+60,keyboard_y+50,LGRAYBLUE);
	LCD_ShowNum(524,253,1,1,24);
	LCD_ShowNum(594,253,2,1,24);
	LCD_ShowNum(664,253,3,1,24);
	LCD_ShowString(716,253,800,480,24,"BACK");//���� Ƶ�ʱ���Ϊԭ����ֵ
	LCD_ShowNum(524,313,4,1,24);
	LCD_ShowNum(594,313,5,1,24);
	LCD_ShowNum(664,313,6,1,24);
	LCD_ShowString(728,313,800,480,24,"OK");//ȷ�ϰ�ť
	LCD_ShowNum(524,373,7,1,24);
	LCD_ShowNum(594,373,8,1,24);
	LCD_ShowNum(664,373,9,1,24);
	LCD_ShowString(722,373,800,480,24,"OUT");//�����ť
	LCD_ShowString(512,433,800,480,24,"CH1");
	LCD_ShowNum(594,433,0,1,24);
	LCD_ShowString(652,433,800,480,24,"MOV");
	LCD_ShowString(716,433,800,480,24,"EXIT");
	}
	while(SG_flag==1)
	{
		LED3 =! LED3;
		sg_key = sg_get_keynum();
		switch(sg_key)
		{
			case 1://duty-
			{
				if(Squ_ON_STA)
				{
					--duty_value;
					if(duty_value < 2)duty_value=2;
					duty_temp = 60 + 4 * duty_value ;
					POINT_COLOR=RED;
					LCD_ShowxNum(277,17,duty_value,2,16,0);
					LCD_Fill(60,362,460,388,WHITE);
					LCD_Fill(60,362,duty_temp,388,LIGHTBLUE);
					square_example_Generation();
					LCD_Fill(10,40,460,300,BLACK);//��������������ʾ����
					POINT_COLOR=YELLOW;
					Y_examp_pos1=170;
					for(i=0;i<=420;i++)
					{
						Y_examp_pos2 = exsample_squTable[i];//����ʾ������
						LCD_DrawLine(i+20,Y_examp_pos1,i+21,Y_examp_pos2);
						Y_examp_pos1 = Y_examp_pos2;
					}
					square_Generation(VOLTAGE);
				}
				break;
			}
			case 2://duty+
			{
				if(Squ_ON_STA)
				{
					++duty_value;
					if(duty_value > 98)duty_value=98;
					duty_temp = 60 + 4 * duty_value ;
					POINT_COLOR=RED;
					LCD_ShowxNum(277,17,duty_value,2,16,0);
					LCD_Fill(60,362,460,388,WHITE);
					LCD_Fill(60,362,duty_temp,388,LIGHTBLUE);
					square_example_Generation();
					LCD_Fill(10,40,460,300,BLACK);//��������������ʾ����
					POINT_COLOR=YELLOW;
					Y_examp_pos1=170;
					for(i=0;i<=420;i++)
					{
						Y_examp_pos2 = exsample_squTable[i];//����ʾ������
						LCD_DrawLine(i+20,Y_examp_pos1,i+21,Y_examp_pos2);
						Y_examp_pos1 = Y_examp_pos2;
					}
					square_Generation(VOLTAGE);
				}
				break;
			}
			case 3://SIN_ON
			{
				Sin_ON_STA=1;
				Saw_ON_STA=0;
				Squ_ON_STA=0;
				Tri_ON_STA=0;
				LCD_Fill(10,40,460,300,BLACK);//��������������ʾ����
				POINT_COLOR=YELLOW;
				Y_examp_pos1=170;
				for(i=0;i<=420;i++)
				{
					Y_examp_pos2 = exsample_sinTable[i];//����ʾ������
					LCD_DrawLine(i+20,Y_examp_pos1,i+21,Y_examp_pos2);
					Y_examp_pos1 = Y_examp_pos2;
				}
				break;
			}
			case 4://SAW_ON
			{
				Sin_ON_STA=0;
				Saw_ON_STA=1;
				Squ_ON_STA=0;
				Tri_ON_STA=0;
				LCD_Fill(10,40,460,300,BLACK);//��������������ʾ����
				POINT_COLOR=YELLOW;
				Y_examp_pos1=170;
				for(i=0;i<=420;i++)
				{
					Y_examp_pos2 = exsample_sawTable[i];//���ʾ������
					LCD_DrawLine(i+20,Y_examp_pos1,i+21,Y_examp_pos2);
					Y_examp_pos1 = Y_examp_pos2;
				}
				break;
			}
			case 5://SQU_ON
			{
				Sin_ON_STA=0;
				Saw_ON_STA=0;
				Squ_ON_STA=1;
				Tri_ON_STA=0;
				LCD_Fill(10,40,460,300,BLACK);//��������������ʾ����
				POINT_COLOR=YELLOW;
				Y_examp_pos1=170;
				for(i=0;i<=420;i++)
				{
					Y_examp_pos2 = exsample_squTable[i];//����ʾ������
					LCD_DrawLine(i+20,Y_examp_pos1,i+21,Y_examp_pos2);
					Y_examp_pos1 = Y_examp_pos2;
				}
				break;
			}
			case 6://TRI_ON
			{
				Sin_ON_STA=0;
				Saw_ON_STA=0;
				Squ_ON_STA=0;
				Tri_ON_STA=1;
				LCD_Fill(10,40,460,300,BLACK);//��������������ʾ����
				POINT_COLOR=YELLOW;
				Y_examp_pos1=170;
				for(i=0;i<=420;i++)
				{
					Y_examp_pos2 = exsample_triTable[i];//����ʾ������
					LCD_DrawLine(i+20,Y_examp_pos1,i+21,Y_examp_pos2);
					Y_examp_pos1 = Y_examp_pos2;
				}
				break;
			}
			case 7://select_amplitude_adjust
			{
				SG_Freq_flag=0;
				SG_Amp_flag=1;
				SG_Amp_Set();
				break;
			}
			case 8://select_frequency_adjust
			{
				SG_Freq_flag=1;
				SG_Amp_flag=0;
				SG_Freq_Set();
				break;
			}
			case 25://out
			{
				TIM6_Configuration(PSC,ARR);
				if(Sin_ON_STA==1)
				{
					sin_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)sinTable);
				}
				if(Saw_ON_STA==1)
				{
					saw_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)sawTable);
				}
				if(Squ_ON_STA==1)
				{
					square_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)squareTable);
				}
				if(Tri_ON_STA==1)
				{
					tri_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)triTable);
				}
				break;
			}
			case 26://CH1 ��Ϊ��CH2ͨ�� �� �رղ���ʹ��
			{
				Sin_ON_STA=0;
				Saw_ON_STA=0;
				Squ_ON_STA=0;
				Tri_ON_STA=0;
				LCD_Fill(10,40,460,300,BLACK);//��������������ʾ����
				DAC_DMA_Configuration((u32)noTable);
				break;
			}
			case 29://EXIT
			{
				welcom_flag=1;
				SG_flag=0;
				LCD_Clear(WHITE);
				Welcome_Background();
				break;
			}
			default:break;
		}
	}
}
/*******************************************************************************
* �������ƣ�       SG_Amp_Set()
* �������ܣ�       �����ź��������
* ���������       ��
* ���������       ��
* ����˵����       �ı�VOLTAGE��ֵ
*******************************************************************************/
void SG_Amp_Set(void)
{
	u8 left=1;//left��ʾ���� 1 ��λ 2 ʮλ 3��λ 4 ǧλ
	POINT_COLOR=BLACK;
	LCD_ShowNum(506,88,amp_qianwei,1,24);
	LCD_ShowNum(518,88,amp_baiwei,1,24);
	LCD_ShowNum(530,88,amp_shiwei,1,24);
	POINT_COLOR=RED;
	LCD_ShowNum(542,88,amp_gewei,1,24);
	while(SG_Amp_flag==1)
	{
		sg_key = sg_get_keynum();
		switch(sg_key)
		{
			case 3://SIN_ON
			{
				Sin_ON_STA=1;
				Saw_ON_STA=0;
				Squ_ON_STA=0;
				Tri_ON_STA=0;
				LCD_Fill(10,40,460,300,BLACK);//��������������ʾ����
				POINT_COLOR=YELLOW;
				Y_examp_pos1=170;
				for(i=0;i<=420;i++)
				{
					Y_examp_pos2 = exsample_sinTable[i];//����ʾ������
					LCD_DrawLine(i+20,Y_examp_pos1,i+21,Y_examp_pos2);
					Y_examp_pos1 = Y_examp_pos2;
				}
				break;
			}
			case 4://SAW_ON
			{
				Sin_ON_STA=0;
				Saw_ON_STA=1;
				Squ_ON_STA=0;
				Tri_ON_STA=0;
				LCD_Fill(10,40,460,300,BLACK);//��������������ʾ����
				POINT_COLOR=YELLOW;
				Y_examp_pos1=170;
				for(i=0;i<=420;i++)
				{
					Y_examp_pos2 = exsample_sawTable[i];//���ʾ������
					LCD_DrawLine(i+20,Y_examp_pos1,i+21,Y_examp_pos2);
					Y_examp_pos1 = Y_examp_pos2;
				}
				break;
			}
			case 5://SQU_ON
			{
				Sin_ON_STA=0;
				Saw_ON_STA=0;
				Squ_ON_STA=1;
				Tri_ON_STA=0;
				LCD_Fill(10,40,460,300,BLACK);//��������������ʾ����
				POINT_COLOR=YELLOW;
				Y_examp_pos1=170;
				for(i=0;i<=420;i++)
				{
					Y_examp_pos2 = exsample_squTable[i];//����ʾ������
					LCD_DrawLine(i+20,Y_examp_pos1,i+21,Y_examp_pos2);
					Y_examp_pos1 = Y_examp_pos2;
				}
				break;
			}
			case 6://TRI_ON
			{
				Sin_ON_STA=0;
				Saw_ON_STA=0;
				Squ_ON_STA=0;
				Tri_ON_STA=1;
				LCD_Fill(10,40,460,300,BLACK);//��������������ʾ����
				POINT_COLOR=YELLOW;
				Y_examp_pos1=170;
				for(i=0;i<=420;i++)
				{
					Y_examp_pos2 = exsample_triTable[i];//����ʾ������
					LCD_DrawLine(i+20,Y_examp_pos1,i+21,Y_examp_pos2);
					Y_examp_pos1 = Y_examp_pos2;
				}
				break;
			}
			case 9://amp_adjust
			{
				POINT_COLOR=RED;
				switch(left)
				{
					case 1:
					{
						amp_gewei++;
						if(amp_gewei==10)
							amp_gewei=0;
						LCD_ShowNum(542,88,amp_gewei,1,24);
						break;
					}
					case 2:
					{
						amp_shiwei++;
						if(amp_shiwei==10)
							amp_shiwei=0;
						LCD_ShowNum(530,88,amp_shiwei,1,24);
						break;
					}
					case 3:
					{
						amp_baiwei++;
						if(amp_baiwei==10)
							amp_baiwei=0;
						LCD_ShowNum(518,88,amp_baiwei,1,24);
						break;
					}
					case 4:
					{
						amp_qianwei++;
						if(amp_qianwei==10)
							amp_qianwei=0;
						LCD_ShowNum(506,88,amp_qianwei,1,24);
						break;
					}
					default : break;
				}
				break;
				}
			case 10://amp_back
			{
				SG_Amp_flag=0;
				amp_qianwei=amp_zong/1000;
				amp_baiwei=amp_zong%1000/100;
				amp_shiwei=amp_zong%100/10;
				amp_gewei=amp_zong%10;
				amp_zong=1000*amp_qianwei+100*amp_baiwei+10*amp_shiwei+amp_gewei;
				POINT_COLOR=BLACK;
				LCD_ShowxNum(506,88,amp_zong,4,24,0);
				
				break;
			}
			case 11://amp_ok
			{
				SG_Amp_flag=0;
				amp_zong=1000*amp_qianwei+100*amp_baiwei+10*amp_shiwei+amp_gewei;
				if(amp_zong>3300)
				{
					amp_qianwei=3;
					amp_baiwei=3;
					amp_shiwei=0;
					amp_gewei=0;
					BEEP=1;
					delay_ms(1000);
					BEEP=0;
					amp_zong = 3300;
				}
				VOLTAGE = Get_VOLTAGE(amp_zong);
				DAC_Value = VOLTAGE ;
				LCD_ShowxNum(158,17,amp_zong,4,16,0);
				LCD_ShowxNum(63,17,DAC_Value,4,16,0);
				POINT_COLOR=BLACK;
				LCD_ShowxNum(506,88,amp_zong,4,24,0);
				TIM6_Configuration(PSC,ARR);
				if(Sin_ON_STA==1)
				{
					sin_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)sinTable);
				}
				if(Saw_ON_STA==1)
				{
					saw_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)sawTable);
				}
				if(Squ_ON_STA==1)
				{
					square_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)squareTable);
				}
				if(Tri_ON_STA==1)
				{
					tri_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)triTable);
				}
				break;
			}
			case 12://amp_left
			{
				++left;
				if(left==5)left=1;
				switch(left)
				{
					case 1:
					{
						POINT_COLOR=BLACK;
						LCD_ShowNum(506,88,amp_qianwei,1,24);
						LCD_ShowNum(518,88,amp_baiwei,1,24);
						LCD_ShowNum(530,88,amp_shiwei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(542,88,amp_gewei,1,24);
						break;
					}
					case 2:
					{
						POINT_COLOR=BLACK;
						LCD_ShowNum(506,88,amp_qianwei,1,24);
						LCD_ShowNum(518,88,amp_baiwei,1,24);
						LCD_ShowNum(542,88,amp_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(530,88,amp_shiwei,1,24);
						break;
					}
					case 3:
					{
						POINT_COLOR=BLACK;
						LCD_ShowNum(506,88,amp_qianwei,1,24);
						LCD_ShowNum(530,88,amp_shiwei,1,24);
						LCD_ShowNum(542,88,amp_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(518,88,amp_baiwei,1,24);
						break;
					}
					case 4:
					{
						POINT_COLOR=BLACK;
						LCD_ShowNum(518,88,amp_baiwei,1,24);
						LCD_ShowNum(530,88,amp_shiwei,1,24);
						LCD_ShowNum(542,88,amp_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(506,88,amp_qianwei,1,24);
						break;
					}
					default:break;
				}
				break;
			}
			case 13://amp_clr
			{
				POINT_COLOR=RED;
				switch(left)
				{
					case 1:
					{
						amp_gewei=0;
						LCD_ShowNum(542,88,amp_gewei,1,24);
						break;
					}
					case 2:
					{
						amp_shiwei=0;
						LCD_ShowNum(530,88,amp_shiwei,1,24);
						break;
					}
					case 3:
					{
						amp_baiwei=0;
						LCD_ShowNum(518,88,amp_baiwei,1,24);
						break;
					}
					case 4:
					{
						amp_qianwei=0;
						LCD_ShowNum(506,88,amp_qianwei,1,24);
						break;
					}
					default : break;
				}
				break;
			}
			case 25://out
			{
				TIM6_Configuration(PSC,ARR);
				if(Sin_ON_STA==1)
				{
					sin_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)sinTable);
				}
				if(Saw_ON_STA==1)
				{
					saw_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)sawTable);
				}
				if(Squ_ON_STA==1)
				{
					square_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)squareTable);
				}
				if(Tri_ON_STA==1)
				{
					tri_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)triTable);
				}
				break;
			}
			case 29://EXIT
			{
				welcom_flag=1;
				SG_Amp_flag=0;
				SG_flag=0;
				LCD_Clear(WHITE);
				Welcome_Background();
				break;
			}
			default:break;
		}
	}
}
/*******************************************************************************
* �������ƣ�       SG_Freq_Set()
* �������ܣ�       �����ź����Ƶ��
* ���������       ��
* ���������       ��
* ����˵����       �ı�PSC ARR��ֵ
*******************************************************************************/
void SG_Freq_Set(void)
{
	u8 left=1;//left��ʾ���� 1 ��λ 2 ʮλ 3��λ 4 ǧλ 5 ��λ
	LCD_ShowxNum(502,198,fre_zong,5,24,0);
	POINT_COLOR=BLACK;
	LCD_ShowNum(502,198,fre_wanwei,1,24);
	LCD_ShowNum(514,198,fre_qianwei,1,24);
	LCD_ShowNum(526,198,fre_baiwei,1,24);
	LCD_ShowNum(538,198,fre_shiwei,1,24);
	POINT_COLOR=RED;
	LCD_ShowNum(550,198,fre_gewei,1,24);
	while(SG_Freq_flag==1)
	{
		sg_key = sg_get_keynum();
		switch(sg_key)
		{
			case 3://SIN_ON
			{
				Sin_ON_STA=1;
				Saw_ON_STA=0;
				Squ_ON_STA=0;
				Tri_ON_STA=0;
				LCD_Fill(10,40,460,300,BLACK);//��������������ʾ����
				POINT_COLOR=YELLOW;
				Y_examp_pos1=170;
				for(i=0;i<=420;i++)
				{
					Y_examp_pos2 = exsample_sinTable[i];//����ʾ������
					LCD_DrawLine(i+20,Y_examp_pos1,i+21,Y_examp_pos2);
					Y_examp_pos1 = Y_examp_pos2;
				}
				break;
			}
			case 4://SAW_ON
			{
				Sin_ON_STA=0;
				Saw_ON_STA=1;
				Squ_ON_STA=0;
				Tri_ON_STA=0;
				LCD_Fill(10,40,460,300,BLACK);//��������������ʾ����
				POINT_COLOR=YELLOW;
				Y_examp_pos1=170;
				for(i=0;i<=420;i++)
				{
					Y_examp_pos2 = exsample_sawTable[i];//���ʾ������
					LCD_DrawLine(i+20,Y_examp_pos1,i+21,Y_examp_pos2);
					Y_examp_pos1 = Y_examp_pos2;
				}
				break;
			}
			case 5://SQU_ON
			{
				Sin_ON_STA=0;
				Saw_ON_STA=0;
				Squ_ON_STA=1;
				Tri_ON_STA=0;
				LCD_Fill(10,40,460,300,BLACK);//��������������ʾ����
				POINT_COLOR=YELLOW;
				Y_examp_pos1=170;
				for(i=0;i<=420;i++)
				{
					Y_examp_pos2 = exsample_squTable[i];//����ʾ������
					LCD_DrawLine(i+20,Y_examp_pos1,i+21,Y_examp_pos2);
					Y_examp_pos1 = Y_examp_pos2;
				}
				break;
			}
			case 6://TRI_ON
			{
				Sin_ON_STA=0;
				Saw_ON_STA=0;
				Squ_ON_STA=0;
				Tri_ON_STA=1;
				LCD_Fill(10,40,460,300,BLACK);//��������������ʾ����
				POINT_COLOR=YELLOW;
				Y_examp_pos1=170;
				for(i=0;i<=420;i++)
				{
					Y_examp_pos2 = exsample_triTable[i];//����ʾ������
					LCD_DrawLine(i+20,Y_examp_pos1,i+21,Y_examp_pos2);
					Y_examp_pos1 = Y_examp_pos2;
				}
				break;
			}
			case 14://freq_1
			{
				switch(left)
				{
					case 1:
					{
						fre_gewei=1;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(550,198,fre_gewei,1,24);
						break;
					}
					case 2:
					{
						fre_shiwei=1;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						break;
					}
					case 3:
					{
						fre_baiwei=1;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						break;
					}
					case 4:
					{
						fre_qianwei=1;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						break;
					}
					case 5:
					{
						fre_wanwei=1;
						POINT_COLOR=BLACK;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						break;
					}
					default : break;
				}
				break;
			}
			case 15://freq_2
			{
				switch(left)
				{
					case 1:
					{
						fre_gewei=2;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(550,198,fre_gewei,1,24);
						break;
					}
					case 2:
					{
						fre_shiwei=2;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						break;
					}
					case 3:
					{
						fre_baiwei=2;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						break;
					}
					case 4:
					{
						fre_qianwei=2;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						break;
					}
					case 5:
					{
						fre_wanwei=2;
						POINT_COLOR=BLACK;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						break;
					}
					default : break;
				}
				break;
			}
			case 16://freq_3
			{
				switch(left)
				{
					case 1:
					{
						fre_gewei=3;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(550,198,fre_gewei,1,24);
						break;
					}
					case 2:
					{
						fre_shiwei=3;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						break;
					}
					case 3:
					{
						fre_baiwei=3;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						break;
					}
					case 4:
					{
						fre_qianwei=3;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						break;
					}
					case 5:
					{
						fre_wanwei=3;
						POINT_COLOR=BLACK;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						break;
					}
					default : break;
				}
				break;
			}
			case 17://freq_back
			{
				SG_Freq_flag=0;
				fre_wanwei=fre_zong/10000;
				fre_qianwei=fre_zong%10000/1000;
				fre_baiwei=fre_zong%1000/100;
				fre_shiwei=fre_zong%100/10;
				fre_gewei=fre_zong%10;
				fre_zong=10000*fre_wanwei+1000*fre_qianwei+100*fre_baiwei+10*fre_shiwei+fre_gewei;
				POINT_COLOR=BLACK;
				LCD_ShowxNum(502,198,fre_zong,5,24,0);
				break;
			}
			case 18://freq_4
			{
				switch(left)
				{
					case 1:
					{
						fre_gewei=4;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(550,198,fre_gewei,1,24);
						break;
					}
					case 2:
					{
						fre_shiwei=4;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						break;
					}
					case 3:
					{
						fre_baiwei=4;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						break;
					}
					case 4:
					{
						fre_qianwei=4;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						break;
					}
					case 5:
					{
						fre_wanwei=4;
						POINT_COLOR=BLACK;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						break;
					}
					default : break;
				}
				break;
			}
			case 19://freq_5
			{
				switch(left)
				{
					case 1:
					{
						fre_gewei=5;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(550,198,fre_gewei,1,24);
						break;
					}
					case 2:
					{
						fre_shiwei=5;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						break;
					}
					case 3:
					{
						fre_baiwei=5;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						break;
					}
					case 4:
					{
						fre_qianwei=5;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						break;
					}
					case 5:
					{
						fre_wanwei=5;
						POINT_COLOR=BLACK;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						break;
					}
					default : break;
				}
				break;
			}
			case 20://freq_6
			{
				switch(left)
				{
					case 1:
					{
						fre_gewei=6;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(550,198,fre_gewei,1,24);
						break;
					}
					case 2:
					{
						fre_shiwei=6;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						break;
					}
					case 3:
					{
						fre_baiwei=6;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						break;
					}
					case 4:
					{
						fre_qianwei=6;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						break;
					}
					case 5:
					{
						fre_wanwei=6;
						POINT_COLOR=BLACK;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						break;
					}
					default : break;
				}
				break;
			}
			case 21://freq_ok
			{
					SG_Freq_flag=0;
					fre_zong=10000*fre_wanwei+1000*fre_qianwei+100*fre_baiwei+10*fre_shiwei+fre_gewei;
//					if(fre_zong<1)
//					{
//						fre_zong = 5;
//						fre_gewei=5;
//						BEEP=1;
//						delay_ms(500);
//						BEEP=0;
//					}
//					else
						if(fre_zong>0&&fre_zong<=80000)
					{
						fre_zong = fre_zong;
					}
					else 
					{
						fre_zong = 80000;
						fre_wanwei=8;
						fre_qianwei=0;
						fre_baiwei=0;
						fre_shiwei=0;
						fre_gewei=0;
						BEEP=1;
						delay_ms(1000);
						BEEP=0;
					}
				//PSC = Get_PSCARR(fre_zong);
				Get_TIM_PSCARR(fre_zong * 128);
					
				LCD_ShowxNum(372,17,fre_zong,5,16,0);
				POINT_COLOR=BLACK;
				LCD_ShowxNum(502,198,fre_zong,5,24,0);
				TIM6_Configuration(PSC,ARR);
				if(Sin_ON_STA==1)
				{
					sin_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)sinTable);
				}
				if(Saw_ON_STA==1)
				{
					saw_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)sawTable);
				}
				if(Squ_ON_STA==1)
				{
					square_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)squareTable);
				}
				if(Tri_ON_STA==1)
				{
					tri_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)triTable);
				}
				break;
			}
			case 22://freq_7
			{
				switch(left)
				{
					case 1:
					{
						fre_gewei=7;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(550,198,fre_gewei,1,24);
						break;
					}
					case 2:
					{
						fre_shiwei=7;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						break;
					}
					case 3:
					{
						fre_baiwei=7;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						break;
					}
					case 4:
					{
						fre_qianwei=7;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						break;
					}
					case 5:
					{
						fre_wanwei=7;
						POINT_COLOR=BLACK;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						break;
					}
					default : break;
				}
				break;
			}
			case 23://freq_8
			{
				switch(left)
				{
					case 1:
					{
						fre_gewei=8;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(550,198,fre_gewei,1,24);
						break;
					}
					case 2:
					{
						fre_shiwei=8;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						break;
					}
					case 3:
					{
						fre_baiwei=8;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						break;
					}
					case 4:
					{
						fre_qianwei=8;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						break;
					}
					case 5:
					{
						fre_wanwei=8;
						POINT_COLOR=BLACK;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						break;
					}
					default : break;
				}
				break;
			}
			case 24://freq_9
			{
				switch(left)
				{
					case 1:
					{
						fre_gewei=9;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(550,198,fre_gewei,1,24);
						break;
					}
					case 2:
					{
						fre_shiwei=9;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						break;
					}
					case 3:
					{
						fre_baiwei=9;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						break;
					}
					case 4:
					{
						fre_qianwei=9;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						break;
					}
					case 5:
					{
						fre_wanwei=9;
						POINT_COLOR=BLACK;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						break;
					}
					default : break;
				}
				break;
			}
			case 25://out
			{
				TIM6_Configuration(PSC,ARR);
				if(Sin_ON_STA==1)
				{
					sin_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)sinTable);
				}
				if(Saw_ON_STA==1)
				{
					saw_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)sawTable);
				}
				if(Squ_ON_STA==1)
				{
					square_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)squareTable);
				}
				if(Tri_ON_STA==1)
				{
					tri_Generation(VOLTAGE);
					DAC_DMA_Configuration((u32)triTable);
				}
				break;
			}
			case 27://freq_0
			{
				switch(left)
				{
					case 1:
					{
						fre_gewei=0;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(550,198,fre_gewei,1,24);
						break;
					}
					case 2:
					{
						fre_shiwei=0;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						break;
					}
					case 3:
					{
						fre_baiwei=0;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						break;
					}
					case 4:
					{
						fre_qianwei=0;
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						break;
					}
					case 5:
					{
						fre_wanwei=0;
						POINT_COLOR=BLACK;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						break;
					}
					default : break;
				}
				break;
			}
			case 28:
			{
				++left;
				if(left==6)left=1;
				switch(left)
				{
					case 1:
					{
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(550,198,fre_gewei,1,24);
						break;
					}
					case 2:
					{
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						break;
					}
					case 3:
					{
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						break;
					}
					case 4:
					{
						POINT_COLOR=BLACK;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						break;
					}
					case 5:
					{
						POINT_COLOR=BLACK;
						LCD_ShowNum(514,198,fre_qianwei,1,24);
						LCD_ShowNum(526,198,fre_baiwei,1,24);
						LCD_ShowNum(538,198,fre_shiwei,1,24);
						LCD_ShowNum(550,198,fre_gewei,1,24);
						POINT_COLOR=RED;
						LCD_ShowNum(502,198,fre_wanwei,1,24);
						break;
					}
					default : break;
				}
				break;
			}
			case 29://EXIT
			{
				welcom_flag=1;
				SG_Freq_flag=0;
				SG_flag=0;
				LCD_Clear(WHITE);
				Welcome_Background();
				break;
			}
			default:break;
		}
	}
}

