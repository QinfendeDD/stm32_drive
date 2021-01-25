#include "PE4302.H"

void PE_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	 //ʹ��PE�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOE, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOE
	GPIO_SetBits(GPIOE,GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);					 //PBE �����
}
void HMC624_Set(unsigned char db)
{
  unsigned char i;
  unsigned char W_DB;
  db = db&0X3F;
	
	db = db^0X3F;	//����λȡ��
  PE_CLK_0;
	delay_us(500);
  PE_LE_0_EN;
	delay_us(500);
  for(i = 0; i < 6;i++)
  {
    W_DB = (db>>5);
    if(W_DB == 1)
    {
      PE_DAT_1;
    }
    else
    {
      PE_DAT_0;
    }
		delay_us(500);
    PE_CLK_1;
		delay_us(500);
    db = (db << 1)&0X3F;
    PE_CLK_0;
		delay_us(500);
  }
  PE_LE_0_DIS;
	delay_us(500);
	PE_DAT_0;
	PE_LE_0_EN;
}
