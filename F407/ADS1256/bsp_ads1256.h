/*********************************************************************************************************
*
* 作    者 ：李晨洋
*	模块名称 : ADS1256 驱动模块(8/4通道带PGA的24位ADC)
*	文件名称 : bsp_ads1256.h
*
*********************************************************************************************************/
/********************************************************************************************************
      ADS1256模块接线表
      +5V   <------  5.0V      5V供电
      GND   -------  GND       地

      DRDY  ------>  PC3       准备就绪
      CS    <------  PC12      SPI_CS
      DIN   <------  PC10      SPI_MOSI
      DOUT  ------>  PC9       SPI_MISO
      SCLK  <------  PC11      SPI时钟
      GND   -------  GND       地
      PDWN  <------  PC8       掉电控制 常高
      RST   <------  PC13      复位信号 常高
			SYNC  ------>  PC8
      NC   空脚
      NC   空脚
			空脚全部接地
			
			ADS1256基本特性:
	    1、模拟部分供电5V;
			2、SPI数字接口电平：3.3V
			3、PGA设置范围： 1、2、4、8、16、32、64、
			4、参考电压2.5V (推荐缺省的，外置的）
			5、输入电压范围：PGA = 1 时, 可输入正负5V
			6. 自动校准 （当设置了PGA,BUF使能、数据采样率时，会启动自校准)
			7. 输入的缓冲器可设置启用和关闭（一般选启用）

			外部晶振频率 = 7.68MHz,
			  时钟频率 tCLK = 1/7.68M = 0.13uS
				输出数据周期 tDATA =  1 / 30K = 0.033mS  (按30Ksps计算)

			对SPI的时钟速度要求: (ads1256.pdf page 6)
				最快 4个tCLK = 0.52uS
				最慢 10个tDATA = 0.3mS (按 30Ksps 计算)

				SCL高电平和低电平持续时间最小 200ns

			RREG, WREG, RDATA 命令之后，需要延迟 4 * tCLK = 0.52uS;
			RDATAC, RESET, SYNC 命令之后，需要延迟 24 * tCLK = 3.12uS;

			实际测试，在3.3V上电后, 及时不做任何配置，ADS125的DRDY 口线即开始输出脉冲信号（2.6us高,33.4低，频率30KHz）
			
			调试记录
	    (1) 设置寄存器时，SCK过快导致芯片不能每次都收到数据。原因: 发送的相邻的字节之间需要延迟一小段时间.
	    (2) 连续复位CPU时，偶尔出现芯片输出采样率异常。
			(3) f103和f407之间的区别较大，在程序移植的时候注意内核的配置及
			
**********************************************************************************************************/
#ifndef _BSP_ADS1256_H
#define _BSP_ADS1256_H
#include "sys.h"
#include "stm32f4xx.h"
/*引脚定义*/
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
/* 定义口线置0和置1的宏 */
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

/* 增益选项 */
typedef enum
{
	ADS1256_GAIN_1			= (0),	/* 增益1（缺省） */
	ADS1256_GAIN_2			= (1),	/* 增益2 */
	ADS1256_GAIN_4			= (2),	/* 增益4 */
	ADS1256_GAIN_8			= (3),	/* 增益8 */
	ADS1256_GAIN_16			= (4),	/* 增益16 */
	ADS1256_GAIN_32			= (5),	/* 增益32 */
	ADS1256_GAIN_64			= (6),	/* 增益64 */
}ADS1256_GAIN_E;
/* 采样速率选项 */
/* 数据转换率选择
	11110000 = 30,000SPS (default  默认 )
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
	ADS1256_GAIN_E Gain;		/* 增益 */
	ADS1256_DRATE_E DataRate;	/* 数据输出速率 */
	int32_t AdcNow[8];			/* 8路ADC采集结果（实时）有符号数 */
	uint8_t Channel;			/* 当前通道 */
	uint8_t ScanMode;			/* 扫描模式，0表示单端8路， 1表示差分4路 */
}ADS1256_VAR_T;

void bsp_InitADS1256(void);
void ADS1256_CfgADC(ADS1256_GAIN_E _gain, ADS1256_DRATE_E _drate);
void ads1256_id(void);//获取ads1256的ID编号
void ads1256_allocation(void); //配置ads1256
uint8_t ADS1256_ReadChipID(void);
void ADS1256_StartScan(uint8_t _ucScanMode);
void ADS1256_StopScan(void);
int32_t ADS1256_GetAdc(uint8_t _ch);
extern ADS1256_VAR_T g_tADS1256;

#endif

