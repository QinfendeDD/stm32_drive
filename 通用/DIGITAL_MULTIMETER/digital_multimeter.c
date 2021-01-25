#include "led.h"
#include "digital_multimeter.h"
#include "touch.h"
#include "lcd.h"
#include "delay.h"
#include "adc.h"
#include "timer.h"

extern u8 welcom_flag;
extern u8 DM_flag;//OSC_flagΪ1 ����ʾ�����ſ��Խ���ѭ��
extern u8  TIM9CH1_CAPTURE_STA;		//���벶��״̬		    				
extern u16	TIM9CH1_CAPTURE_VAL;	//���벶��ֵ  

u8 dm_key=0;
u8 Voltage_flag = 0;
u8 Current_flag = 0;
u8 Cap_flag = 0;
u8 Res_flag = 0;
u8 Inductor_flag = 0;

/*******************************************************************************
* �������ƣ�       DM_Init()
* �������ܣ�       �������ñ��ʼ������
* ���������       ��
* ���������       ��
* ����˵����       ����CD4052
*******************************************************************************/
void DM_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);//ʹ��GPIOBʱ��

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//����
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��
	
	GPIO_SetBits(GPIOB,GPIO_Pin_2 | GPIO_Pin_3);//GPIOA2,3 ѡ��X3 �������κβ���
}
/*******************************************************************************
* �������ƣ�       DM_Adc_Init()
* �������ܣ�       ��ʼ���������ñ��õ�ADC3
* ���������       ��
* ���������       ��
* ����˵����       ��
*******************************************************************************/
void DM_Adc_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);//ʹ��GPIOAʱ��
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //ʹ��ADC3ʱ��

  //�ȳ�ʼ��ADC3ͨ��6 IO��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//PF8 ͨ��6
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
  GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��  
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//��λ����	 
 
	
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMAʧ��
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//��ɨ��ģʽ	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//�ر�����ת��
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
  ADC_Init(ADC3, &ADC_InitStructure);//ADC��ʼ��
	
 
	ADC_Cmd(ADC3, ENABLE);//����ADת����	
}
/*******************************************************************************
* �������ƣ�       DM_Get_Adc()
* �������ܣ�       �õ�ADC3ͨ��ch����ת�����ֵ
* ���������       ch��ͨ��
* ���������       ��
* ����˵����       ��
*******************************************************************************/
u16 DM_Get_Adc(u8 ch)   
{
	  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC3, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADCͨ��,480������,��߲���ʱ�������߾�ȷ��			    
  
	ADC_SoftwareStartConv(ADC3);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC3);	//�������һ��ADC1�������ת�����
}
/*******************************************************************************
* �������ƣ�       DM_GetAdc_Average()
* �������ܣ�       ��ADC3ͨ��ch��ֵȡtimes��ƽ��
* ���������       ch��ͨ�� times������
* ���������       ��
* ����˵����       ��
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
* �������ƣ�       dm_get_keynum()
* �������ܣ�       �õ����ñ�����´��������λ��
* ���������       ��
* ���������       key_touch ����λ��
* ����˵����       
*******************************************************************************/
u8 dm_get_keynum(void)
{
	u8 key_touch=0;
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)			//������������
	{	
		if(tp_dev.x[0]<80&&tp_dev.y[0]<60)key_touch=1;//EXIT
		if(tp_dev.x[0]<500&&tp_dev.x[0]>300&&tp_dev.y[0]<340&&tp_dev.y[0]>140)key_touch=2;//��ѹ����
		if(tp_dev.y[0]<220&&tp_dev.y[0]>20)
		{
			if(tp_dev.x[0]<300&&tp_dev.x[0]>100)key_touch=3;//��������
			else if(tp_dev.x[0]<700&&tp_dev.x[0]>500)key_touch=4;//���ݲ���
		}
		if(tp_dev.y[0]<460&&tp_dev.y[0]>260)
		{
			if(tp_dev.x[0]<300&&tp_dev.x[0]>100)key_touch=5;//�������
			else if(tp_dev.x[0]<700&&tp_dev.x[0]>500)key_touch=6;//��в���
		}
	}
	return key_touch; 
}


/*******************************************************************************
* �������ƣ�       DM_BackGround()
* �������ܣ�       �������ñ�������
* ���������       ��
* ���������       ��
* ����˵����       ��
*******************************************************************************/
void DM_BackGround(void)
{
	u16 radius;
	LCD_Fill(0,0,80,60,LGRAY);
	LCD_ShowString(16,18,800,480,24,"EXIT");//��ѹ
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
	LCD_ShowString(364,228,800,480,24,"0.000V");//��ѹ
	LCD_ShowString(152,108,800,480,24,"00.000mA");//����
	LCD_ShowString(152,348,800,480,24,"0000000R");//����
	LCD_ShowString(540,108,800,480,24,"000.00nF");//����
	LCD_ShowString(558,348,800,480,24,"00000uH");//���
}
/*******************************************************************************
* �������ƣ�       DM_Run()
* �������ܣ�       ����ѡ����
* ���������       ��
* ���������       ��
* ����˵����       �����������ñ�����Ƚ���ú�����ͨ����Ӧ�İ�ť��ѡ���Ӧ�Ĺ���
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
* �������ƣ�       Voltage_Measure()
* �������ܣ�       ��ѹ������ʾ
* ���������       ��
* ���������       ��
* ����˵����       ʵʱ����
*******************************************************************************/
void Voltage_Measure(void)
{	
	u16 adcx;
	float temp;
	while(Voltage_flag == 1)
	{
		LED4 =! LED4;
		dm_key = dm_get_keynum();
		adcx=DM_GetAdc_Average(ADC_Channel_6,20);//��ȡͨ��5��ת��ֵ��20��ȡƽ��
		temp=(float)adcx*(3.3/4096);          //��ȡ�����Ĵ�С����ʵ�ʵ�ѹֵ������3.1111
		adcx=temp;                            //��ֵ�������ָ�adcx��������ΪadcxΪu16����
		LCD_ShowNum(364,228,adcx,1,24);
		LCD_ShowChar(376,228,46,24,0);//��ʾС����
		temp-=adcx;                           //���Ѿ���ʾ����������ȥ��������С�����֣�����3.1111-3=0.1111
		temp*=1000;                           //С�����ֳ���1000�����磺0.1111��ת��Ϊ111.1���൱�ڱ�����λС����
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
* �������ƣ�       I_Measure()
* �������ܣ�       ����������ʾ
* ���������       ��
* ���������       ��
* ����˵����       ʵʱ����
*******************************************************************************/
void I_Measure(void)
{
	u16 adcx;
	float temp;
	while(Current_flag == 1)
	{
		LED4 =! LED4;
		dm_key = dm_get_keynum();
		adcx=DM_GetAdc_Average(ADC_Channel_6,20);//��ȡͨ��5��ת��ֵ��20��ȡƽ��
		temp=(float)adcx*(11/4096);          //��λmV
		//temp =(float) temp / 300;//�õ�����ֵ ��λmA
		//��ʾ����
		adcx=temp;                            //��ֵ�������ָ�adcx��������ΪadcxΪu16����
		LCD_ShowxNum(152,108,adcx,2,24,0);
		LCD_ShowChar(176,108,46,24,0);//��ʾС����
		temp-=adcx;                           //���Ѿ���ʾ����������ȥ��������С�����֣�����3.1111-3=0.1111
		temp*=1000;                           //С�����ֳ���1000�����磺0.1111��ת��Ϊ111.1���൱�ڱ���3λС����
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
* �������ƣ�       Cap_Measure()
* �������ܣ�       ���ݲ�����ʾ
* ���������       ��
* ���������       ��
* ����˵����       ʵʱ����
*******************************************************************************/
void Cap_Measure(void)
{
	long long temp=0;  //���ۿɲ�ֵ 10pf~470nf
	float cap_value;
	u16 cap_temp;
	while(Cap_flag == 1)
	{
		LED4 =! LED4;
		dm_key = dm_get_keynum();
	 	if(TIM9CH1_CAPTURE_STA&0X80)        //�ɹ�������һ�θߵ�ƽ
		{
			delay_ms(100);
			temp=TIM9CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFF;		 		         //���ʱ���ܺ�
			temp+=TIM9CH1_CAPTURE_VAL;		   //�õ��ܵĸߵ�ƽʱ��
			cap_value = (float) temp / 130 ;
			cap_temp = cap_value ;
			LCD_ShowxNum(540,108,cap_temp,3,24,0);
			LCD_ShowChar(576,108,46,24,0);//��ʾС����
			cap_value = cap_value - cap_temp;
			cap_temp = cap_value * 100;
			LCD_ShowxNum(588,108,cap_temp,2,24,0x80);
			TIM9CH1_CAPTURE_STA=0;			     //������һ�β���
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
* �������ƣ�       Res_Measure()
* �������ܣ�       ���������ʾ
* ���������       ��
* ���������       ��
* ����˵����       ʵʱ����
*******************************************************************************/
void Res_Measure(void)
{
	long long temp=0;  //���ۿɲ�ֵ M��
	u32 res_value;
	while(Res_flag == 1)
	{
		delay_ms(100);
		LED4 =! LED4;
		dm_key = dm_get_keynum();
	 	if(TIM9CH1_CAPTURE_STA&0X80)        //�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM9CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFF;		 		         //���ʱ���ܺ�
			temp+=TIM9CH1_CAPTURE_VAL;		   //�õ��ܵĸߵ�ƽʱ��
			res_value =  (7.3* temp) - 10500;
			if(res_value <+ 1000)res_value = res_value;
			else if(res_value <= 10000)res_value = res_value + 30;
			else if(res_value < 100000)res_value = res_value + 750;
			else if(res_value < 1000000)res_value = res_value + 3600;
			else if(res_value < 2000000)res_value = res_value + 35000;
			else res_value = res_value;
			LCD_ShowxNum(152,348,res_value,7,24,0);
			TIM9CH1_CAPTURE_STA=0;			     //������һ�β���
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
* �������ƣ�       Indutor_Measure()
* �������ܣ�       ��в�����ʾ
* ���������       ��
* ���������       ��
* ����˵����       ʵʱ����
*******************************************************************************/
void Indutor_Measure(void)
{
	long long temp=0;  //���ۿɲ�ֵ M��
	float inductor_value;

	while(Inductor_flag == 1)
	{
		delay_ms(150);
		LED4 =! LED4;
		dm_key = dm_get_keynum();
		if(TIM9CH1_CAPTURE_STA&0X80)        //�ɹ�������һ�θߵ�ƽ
		{
			temp=TIM9CH1_CAPTURE_STA&0X3F; 
			temp*=0XFFFF;		 		         //���ʱ���ܺ�
			temp+=TIM9CH1_CAPTURE_VAL;		   //�õ��ܵĸߵ�ƽʱ��
			if(temp < 32)
			inductor_value = 4.3029 * temp * temp -67.968 * temp + 326.7;
			else
			inductor_value = 0.2077 * temp * temp -34.57 * temp + 4193.6;
			LCD_ShowxNum(558,348,inductor_value,5,24,0);
			TIM9CH1_CAPTURE_STA=0;			     //������һ�β���
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

