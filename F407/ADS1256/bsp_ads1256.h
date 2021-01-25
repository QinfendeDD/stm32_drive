/*********************************************************************************************************
*
* ��    �� �����
*	ģ������ : ADS1256 ����ģ��(8/4ͨ����PGA��24λADC)
*	�ļ����� : bsp_ads1256.h
*
*********************************************************************************************************/
/********************************************************************************************************
      ADS1256ģ����߱�
      +5V   <------  5.0V      5V����
      GND   -------  GND       ��

      DRDY  ------>  PC3       ׼������
      CS    <------  PC12      SPI_CS
      DIN   <------  PC10      SPI_MOSI
      DOUT  ------>  PC9       SPI_MISO
      SCLK  <------  PC11      SPIʱ��
      GND   -------  GND       ��
      PDWN  <------  PC8       ������� ����
      RST   <------  PC13      ��λ�ź� ����
			SYNC  ------>  PC8
      NC   �ս�
      NC   �ս�
			�ս�ȫ���ӵ�
			
			ADS1256��������:
	    1��ģ�ⲿ�ֹ���5V;
			2��SPI���ֽӿڵ�ƽ��3.3V
			3��PGA���÷�Χ�� 1��2��4��8��16��32��64��
			4���ο���ѹ2.5V (�Ƽ�ȱʡ�ģ����õģ�
			5�������ѹ��Χ��PGA = 1 ʱ, ����������5V
			6. �Զ�У׼ ����������PGA,BUFʹ�ܡ����ݲ�����ʱ����������У׼)
			7. ����Ļ��������������ú͹رգ�һ��ѡ���ã�

			�ⲿ����Ƶ�� = 7.68MHz,
			  ʱ��Ƶ�� tCLK = 1/7.68M = 0.13uS
				����������� tDATA =  1 / 30K = 0.033mS  (��30Ksps����)

			��SPI��ʱ���ٶ�Ҫ��: (ads1256.pdf page 6)
				��� 4��tCLK = 0.52uS
				���� 10��tDATA = 0.3mS (�� 30Ksps ����)

				SCL�ߵ�ƽ�͵͵�ƽ����ʱ����С 200ns

			RREG, WREG, RDATA ����֮����Ҫ�ӳ� 4 * tCLK = 0.52uS;
			RDATAC, RESET, SYNC ����֮����Ҫ�ӳ� 24 * tCLK = 3.12uS;

			ʵ�ʲ��ԣ���3.3V�ϵ��, ��ʱ�����κ����ã�ADS125��DRDY ���߼���ʼ��������źţ�2.6us��,33.4�ͣ�Ƶ��30KHz��
			
			���Լ�¼
	    (1) ���üĴ���ʱ��SCK���쵼��оƬ����ÿ�ζ��յ����ݡ�ԭ��: ���͵����ڵ��ֽ�֮����Ҫ�ӳ�һС��ʱ��.
	    (2) ������λCPUʱ��ż������оƬ����������쳣��
			(3) f103��f407֮�������ϴ��ڳ�����ֲ��ʱ��ע���ں˵����ü�
			
**********************************************************************************************************/
#ifndef _BSP_ADS1256_H
#define _BSP_ADS1256_H
#include "sys.h"
#include "stm32f4xx.h"
/*���Ŷ���*/
#define RCC_SCK 	RCC_APB1Periph_GPIOC
#define PORT_SCK	GPIOC
#define PIN_SCK		GPIO_Pin_11

#define RCC_DIN 	RCC_APB2Periph_GPIOC
#define PORT_DIN	GPIOC
#define PIN_DIN		GPIO_Pin_10

#define RCC_CS 		RCC_APB2Periph_GPIOC
#define PORT_CS		GPIOC
#define PIN_CS		GPIO_Pin_12

#define RCC_DRDY 	RCC_APB2Periph_GPIOC
#define PORT_DRDY	GPIOC
#define PIN_DRDY	GPIO_Pin_3

#define RCC_DOUT 	RCC_APB2Periph_GPIOC
#define PORT_DOUT	GPIOC
#define PIN_DOUT	GPIO_Pin_9

#define RCC_SYNC 	RCC_APB2Periph_GPIOC
#define PORT_SYNC	GPIOC
#define PIN_SYNC	GPIO_Pin_8

#define RCC_RST 	RCC_APB2Periph_GPIOC
#define PORT_RST	GPIOC
#define PIN_RST	GPIO_Pin_13
//RST->HIGH	PC13
//SYNC->HIGH PC8
/* ���������0����1�ĺ� */
#define CS_0()		GPIO_ResetBits(PORT_CS, PIN_CS)
#define CS_1()		GPIO_SetBits(PORT_CS, PIN_CS)

#define SCK_0()		GPIO_ResetBits(PORT_SCK, PIN_SCK)
#define SCK_1()		GPIO_SetBits(PORT_SCK, PIN_SCK)

#define DI_0()		GPIO_ResetBits(PORT_DIN, PIN_DIN)
#define DI_1()		GPIO_SetBits(PORT_DIN, PIN_DIN)

#define SYNC_0()		GPIO_ResetBits(PORT_SYNC, PIN_SYNC)
#define SYNC_1()		GPIO_SetBits(PORT_SYNC, PIN_SYNC)

#define RST_0()		GPIO_ResetBits(PORT_RST, PIN_RST)
#define RST_1()		GPIO_SetBits(PORT_RST, PIN_RST)

#define DO_IS_HIGH()	(GPIO_ReadInputDataBit(PORT_DOUT, PIN_DOUT) == Bit_SET)

#define DRDY_IS_LOW()	(GPIO_ReadInputDataBit(PORT_DRDY, PIN_DRDY) == Bit_RESET)

/* ����ѡ�� */
typedef enum
{
	ADS1256_GAIN_1			= (0),	/* ����1��ȱʡ�� */
	ADS1256_GAIN_2			= (1),	/* ����2 */
	ADS1256_GAIN_4			= (2),	/* ����4 */
	ADS1256_GAIN_8			= (3),	/* ����8 */
	ADS1256_GAIN_16			= (4),	/* ����16 */
	ADS1256_GAIN_32			= (5),	/* ����32 */
	ADS1256_GAIN_64			= (6),	/* ����64 */
}ADS1256_GAIN_E;
/* ��������ѡ�� */
/* ����ת����ѡ��
	11110000 = 30,000SPS (default  Ĭ�� )
	11100000 = 15,000SPS
	11010000 = 7,500SPS
	11000000 = 3,750SPS
	10110000 = 2,000SPS
	10100001 = 1,000SPS
	10010010 = 500SPS
	10000010 = 100SPS
	01110010 = 60SPS
	01100011 = 50SPS
	01010011 = 30SPS
	01000011 = 25SPS
	00110011 = 15SPS
	00100011 = 10SPS
	00010011 = 5SPS
	00000011 = 2.5SPS
*/
typedef enum
{
	ADS1256_30000SPS = 0,
	ADS1256_15000SPS,
	ADS1256_7500SPS,
	ADS1256_3750SPS,
	ADS1256_2000SPS,
	ADS1256_1000SPS,
	ADS1256_500SPS,
	ADS1256_100SPS,
	ADS1256_60SPS,
	ADS1256_50SPS,
	ADS1256_30SPS,
	ADS1256_25SPS,
	ADS1256_15SPS,
	ADS1256_10SPS,
	ADS1256_5SPS,
	ADS1256_2d5SPS,

	ADS1256_DRATE_MAX
}ADS1256_DRATE_E;

#define ADS1256_DRAE_COUNT = 15;

typedef struct
{
	ADS1256_GAIN_E Gain;		/* ���� */
	ADS1256_DRATE_E DataRate;	/* ����������� */
	int32_t AdcNow[8];			/* 8·ADC�ɼ������ʵʱ���з����� */
	uint8_t Channel;			/* ��ǰͨ�� */
	uint8_t ScanMode;			/* ɨ��ģʽ��0��ʾ����8·�� 1��ʾ���4· */
}ADS1256_VAR_T;

void bsp_InitADS1256(void);
void ADS1256_CfgADC(ADS1256_GAIN_E _gain, ADS1256_DRATE_E _drate);
void ads1256_id(void);//��ȡads1256��ID���
void ads1256_allocation(void); //����ads1256
uint8_t ADS1256_ReadChipID(void);
void ADS1256_StartScan(uint8_t _ucScanMode);
void ADS1256_StopScan(void);
int32_t ADS1256_GetAdc(uint8_t _ch);
extern ADS1256_VAR_T g_tADS1256;

#endif

