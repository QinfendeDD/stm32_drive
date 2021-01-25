#define  __SPWM
#include "stm32f10x.h"
#include "SPWM.h"
const u32 spwm[100] = {
0x4000,0x4405,0x4805,0x4BFE,0x4FEA,0x53C7,0x578F,0x5B40,0x5ED5,0x624A,
0x659E,0x68CB,0x6BCF,0x6EA7,0x714F,0x73C6,0x7609,0x7815,0x79E8,0x7B81,
0x7CDD,0x7DFC,0x7EDD,0x7F7E,0x7FDF,0x7FFF,0x7FDF,0x7F7E,0x7EDD,0x7DFC,
0x7CDD,0x7B81,0x79E8,0x7815,0x7609,0x73C6,0x714F,0x6EA7,0x6BCF,0x68CB,
0x659E,0x624A,0x5ED5,0x5B40,0x578F,0x53C7,0x4FEA,0x4BFE,0x4805,0x4405,
0x4000,0x3BFB,0x37FB,0x3402,0x3016,0x2C39,0x2871,0x24C0,0x212B,0x1DB6,
0x1A62,0x1735,0x1431,0x1159,0x0EB1,0x0C3A,0x09F7,0x07EB,0x0618,0x047F,
0x0323,0x0204,0x0123,0x0082,0x0021,0x0001,0x0021,0x0082,0x0123,0x0204,
0x0323,0x047F,0x0618,0x07EB,0x09F7,0x0C3A,0x0EB1,0x1159,0x1431,0x1735,
0x1A62,0x1DB6,0x212B,0x24C0,0x2871,0x2C39,0x3016,0x3402,0x37FB,0x3BFB,
};//���������Ϊ1��ȫ��100������,����Ϊ15,����ֵ��СΪ0-32767(2^15)


static u16 i = 0;               //��ʾ���Ҳ�ȡ����

u16 TimerPeriod   = 0;          //�Զ���װ��ֵ
u16 Channel1Pulse = 0;          
u16 Channel2Pulse = 0;

/*****************************************************************
* ����:��ʱ��1����2·������PWM��(Ƶ�� = pfreq / (psc+1))      
*˵��: channel1,  channel2  --> PA.8, PA.9   
*       channel1N, channel2N --> PB.13, PB.14 (����ͨ��)
*       TimerPeriod          --> �Զ���װ������ֵ
*       ChannelxPulse        --> ռ������ֵ
*****************************************************************/

void TIM1_PWM_Init(u16 pfreq ,u16 psc)          //pfreqΪ����Ƶʱ��PWMƵ��,pscΪԤ��Ƶֵ
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;      
    TIM_OCInitTypeDef TIM_OCInitStructure;          //���ͨ������
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;      //������ɲ������

    TimerPeriod = (SystemCoreClock / pfreq) - 1;            //�Զ���װ������ֵ

    /* ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100 */
    Channel1Pulse = (u16)((u32)(50  * (TimerPeriod - 1)) / 100 );   //ռ�ձ�50%
    Channel2Pulse = (u16)((u32)(50  * (TimerPeriod - 1)) / 100 );   //ռ�ձ�50%

    /* ʹ��TIM1,GPIOA,GPIOB*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 

    /*channel1 ,channel2  --> PA.9, PA.10*
     *channel1N,channel2N --> PB.14, PB.15*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;   
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                                 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*  ��ʼ��TIM1  */
    TIM_TimeBaseStructure.TIM_Period            = TimerPeriod;          //������װ������ֵ
    TIM_TimeBaseStructure.TIM_Prescaler         = psc;                  //����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision     = 0;                    //ʱ�ӷ�Ƶ����,�������벶���й�(��ʱ�����˲�����Ƶ�ʱ�)
    TIM_TimeBaseStructure.TIM_CounterMode       = TIM_CounterMode_Up;   //TIM���ϼ���ģʽ
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;                    //�ظ�����ж�
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);                     //��ʼ����ʱ������

    /* Channel_1   TIM_OCMode_PWM1ģʽ */  
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;             //���ϼ���ʱ,һ��TIMx_CNT<TIMx_CCR1ʱͨ��1Ϊ��Ч��ƽ
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;      //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;     //PWM�������ʹ��
    TIM_OCInitStructure.TIM_Pulse        = Channel1Pulse;               //ռ�ձ� = TIM_Pulse/TIM_Period;
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;         //��Ч��ƽΪ�ߵ�ƽ
    TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCNPolarity_High;        //����ͨ����ƽ����ͨͨ����ƽ�෴
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;         //�������״̬
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;      //PWM�����������״̬
    TIM_OC2Init(TIM1, &TIM_OCInitStructure);                            //����ָ��������ʼ������TIM1 OC1

    /* Channel_2      TIM_OCMode_PWM1ģʽ*/  
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;             ///���ϼ���ʱ,һ��TIMx_CNT>TIMx_CCR1ʱͨ��1Ϊ��Ч��ƽ
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;      ////�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;     //PWM�������ʹ��
    TIM_OCInitStructure.TIM_Pulse        = Channel2Pulse;               //ռ�ձ� = TIM_Pulse/TIM_Period;
    TIM_OCInitStructure.TIM_OCPolarity   = TIM_OCPolarity_High;         //��Ч��ƽΪ�ߵ�ƽ
    TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCNPolarity_High;        //����ͨ����ƽ����ͨͨ����ƽ�෴
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Set;         //�������״̬
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;      //PWM�����������״̬
    TIM_OC3Init(TIM1, &TIM_OCInitStructure);                            //����ָ��������ʼ������TIM1 OC2

    TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);                   //ʹ��TIM3��CCR1��Ԥװ�ؼĴ���
    TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);                   //ʹ��TIM3��CCR2��Ԥװ�ؼĴ���

    /*������ɲ����������*/
    TIM_BDTRInitStructure.TIM_OSSRState       = TIM_OSSRState_Enable;
    TIM_BDTRInitStructure.TIM_OSSIState       = TIM_OSSIState_Enable;
    TIM_BDTRInitStructure.TIM_LOCKLevel       = TIM_LOCKLevel_1;
    TIM_BDTRInitStructure.TIM_DeadTime        = 0x2F;                   //����TIM1_BDTR��DTG[7:0]
    TIM_BDTRInitStructure.TIM_Break           = TIM_Break_Disable;
    TIM_BDTRInitStructure.TIM_BreakPolarity   = TIM_BreakPolarity_High;
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

    TIM_Cmd(TIM1, ENABLE);                                              //ʹ��TIM1
    TIM_CtrlPWMOutputs(TIM1,ENABLE);                                    //PWM���ʹ��(���PWM�����һ��Ҫ�ӣ���ʱ����������������Ϊ������Ӻ�������ܲ�����Ч��)
}

/**************************************************
*   ����:���ö�ʱ��2�����жϸı�SPWM����ռ�ձ�  
***************************************************/
void TIM2_Int_Init(u16 ifreq,u16 psc)                       //ifreqΪ ����Ƶʱ�ĸ����ж�Ƶ��,pscΪԤ��Ƶֵ
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);         //ʱ��ʹ��

    TimerPeriod = (SystemCoreClock / ifreq) - 1;                             

    TIM_TimeBaseStructure.TIM_Period        = TimerPeriod;              //�����Զ���װ�ؼĴ�������ֵ
    TIM_TimeBaseStructure.TIM_Prescaler     = psc;                      //����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                        //����ʱ�ӷ�Ƶϵ��
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;       //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);           

    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE );                           //ʹ��ָ����TIM2�ж�,��������ж�(����������������ʼ��ʱ)

    NVIC_InitStructure.NVIC_IRQChannel                   = TIM2_IRQn;   //TIM2�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;           //�����ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 4;           //�����ȼ�Ϊ4��
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;      //IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);                                     //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    TIM_Cmd(TIM2, ENABLE);                                              //ʹ��TIM2                 
}

//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)  
{
    extern float Period_percent;

    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)                  //���TIM2�����жϷ������
    {    
        TIM_SetCompare2(TIM1,(u16)(Period_percent*spwm[i])) ;       //�޸�TIM1ͨ��1��PWMռ�ձ�,����Ϊ����/�ȽϼĴ���1��ֵ
        TIM_SetCompare3(TIM1,(u16)(Period_percent*spwm[i++]));      //�޸�TIM1ͨ��2��PWMռ�ձ�,����Ϊ����/�ȽϼĴ���2��ֵ
        if(i == 101)  //һ�����ڲ���100����
        {
            i = 0;
        }
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //���TIM2�����жϱ�־
    }
}
