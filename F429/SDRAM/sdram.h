#ifndef  __SDRAM_H
#define	__SDRAM_H

#include "stm32f4xx.h"


#define SDRAM_Size 0x1000000  //16M字节

#define FMC_BANK_SDRAM            FMC_Bank2_SDRAM  //SDRAM 的bank选择
#define FMC_COMMAND_TARGET_BANK   FMC_Command_Target_bank2
#define SDRAM_BANK_ADDR     ((uint32_t)0xD0000000) // FMC SDRAM 数据基地址

// 寄存器定义

#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000) 
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)      




/*---------------------- IO口配置 ------------------------*/
														
//地址线
#define FMC_A0_PORT        GPIOF
#define FMC_A0_PIN         GPIO_Pin_0
#define FMC_A0_PINSOURCE   GPIO_PinSource0
#define FMC_A0_AF          GPIO_AF_FMC

#define FMC_A1_PORT        GPIOF
#define FMC_A1_PIN         GPIO_Pin_1
#define FMC_A1_PINSOURCE   GPIO_PinSource1
#define FMC_A1_AF          GPIO_AF_FMC

#define FMC_A2_PORT        GPIOF
#define FMC_A2_PIN         GPIO_Pin_2
#define FMC_A2_PINSOURCE   GPIO_PinSource2
#define FMC_A2_AF          GPIO_AF_FMC

#define FMC_A3_PORT        GPIOF
#define FMC_A3_PIN         GPIO_Pin_3
#define FMC_A3_PINSOURCE   GPIO_PinSource3
#define FMC_A3_AF          GPIO_AF_FMC

#define FMC_A4_PORT        GPIOF
#define FMC_A4_PIN         GPIO_Pin_4
#define FMC_A4_PINSOURCE   GPIO_PinSource4
#define FMC_A4_AF          GPIO_AF_FMC

#define FMC_A5_PORT        GPIOF
#define FMC_A5_PIN         GPIO_Pin_5
#define FMC_A5_PINSOURCE   GPIO_PinSource5
#define FMC_A5_AF          GPIO_AF_FMC

#define FMC_A6_PORT        GPIOF
#define FMC_A6_PIN         GPIO_Pin_12
#define FMC_A6_PINSOURCE   GPIO_PinSource12
#define FMC_A6_AF          GPIO_AF_FMC

#define FMC_A7_PORT        GPIOF
#define FMC_A7_PIN         GPIO_Pin_13
#define FMC_A7_PINSOURCE   GPIO_PinSource13
#define FMC_A7_AF          GPIO_AF_FMC

#define FMC_A8_PORT        GPIOF
#define FMC_A8_PIN         GPIO_Pin_14
#define FMC_A8_PINSOURCE   GPIO_PinSource14
#define FMC_A8_AF          GPIO_AF_FMC

#define FMC_A9_PORT        GPIOF
#define FMC_A9_PIN         GPIO_Pin_15
#define FMC_A9_PINSOURCE   GPIO_PinSource15
#define FMC_A9_AF          GPIO_AF_FMC


#define FMC_A10_PORT        GPIOG
#define FMC_A10_PIN         GPIO_Pin_0
#define FMC_A10_PINSOURCE   GPIO_PinSource0
#define FMC_A10_AF          GPIO_AF_FMC


#define FMC_A11_PORT        GPIOG
#define FMC_A11_PIN         GPIO_Pin_1
#define FMC_A11_PINSOURCE   GPIO_PinSource1
#define FMC_A11_AF          GPIO_AF_FMC



/*数据信号线*/
#define FMC_D0_PORT        GPIOD
#define FMC_D0_PIN         GPIO_Pin_14
#define FMC_D0_PINSOURCE   GPIO_PinSource14
#define FMC_D0_AF          GPIO_AF_FMC

#define FMC_D1_PORT        GPIOD
#define FMC_D1_PIN         GPIO_Pin_15
#define FMC_D1_PINSOURCE   GPIO_PinSource15
#define FMC_D1_AF          GPIO_AF_FMC

#define FMC_D2_PORT        GPIOD
#define FMC_D2_PIN         GPIO_Pin_0
#define FMC_D2_PINSOURCE   GPIO_PinSource0
#define FMC_D2_AF          GPIO_AF_FMC

#define FMC_D3_PORT        GPIOD
#define FMC_D3_PIN         GPIO_Pin_1
#define FMC_D3_PINSOURCE   GPIO_PinSource1
#define FMC_D3_AF          GPIO_AF_FMC

#define FMC_D4_PORT        GPIOE
#define FMC_D4_PIN         GPIO_Pin_7
#define FMC_D4_PINSOURCE   GPIO_PinSource7
#define FMC_D4_AF          GPIO_AF_FMC

#define FMC_D5_PORT        GPIOE
#define FMC_D5_PIN         GPIO_Pin_8
#define FMC_D5_PINSOURCE   GPIO_PinSource8
#define FMC_D5_AF          GPIO_AF_FMC

#define FMC_D6_PORT        GPIOE
#define FMC_D6_PIN         GPIO_Pin_9
#define FMC_D6_PINSOURCE   GPIO_PinSource9
#define FMC_D6_AF          GPIO_AF_FMC

#define FMC_D7_PORT        GPIOE
#define FMC_D7_PIN         GPIO_Pin_10
#define FMC_D7_PINSOURCE   GPIO_PinSource10
#define FMC_D7_AF          GPIO_AF_FMC

#define FMC_D8_PORT        GPIOE
#define FMC_D8_PIN         GPIO_Pin_11
#define FMC_D8_PINSOURCE   GPIO_PinSource11
#define FMC_D8_AF          GPIO_AF_FMC

#define FMC_D9_PORT        GPIOE
#define FMC_D9_PIN         GPIO_Pin_12
#define FMC_D9_PINSOURCE   GPIO_PinSource12
#define FMC_D9_AF          GPIO_AF_FMC

#define FMC_D10_PORT        GPIOE
#define FMC_D10_PIN         GPIO_Pin_13
#define FMC_D10_PINSOURCE   GPIO_PinSource13
#define FMC_D10_AF          GPIO_AF_FMC

#define FMC_D11_PORT        GPIOE
#define FMC_D11_PIN         GPIO_Pin_14
#define FMC_D11_PINSOURCE   GPIO_PinSource14
#define FMC_D11_AF          GPIO_AF_FMC

#define FMC_D12_PORT        GPIOE
#define FMC_D12_PIN         GPIO_Pin_15
#define FMC_D12_PINSOURCE   GPIO_PinSource15
#define FMC_D12_AF          GPIO_AF_FMC

#define FMC_D13_PORT        GPIOD
#define FMC_D13_PIN         GPIO_Pin_8
#define FMC_D13_PINSOURCE   GPIO_PinSource8
#define FMC_D13_AF          GPIO_AF_FMC

#define FMC_D14_PORT        GPIOD
#define FMC_D14_PIN         GPIO_Pin_9
#define FMC_D14_PINSOURCE   GPIO_PinSource9
#define FMC_D14_AF          GPIO_AF_FMC

#define FMC_D15_PORT        GPIOD
#define FMC_D15_PIN         GPIO_Pin_10
#define FMC_D15_PINSOURCE   GPIO_PinSource10
#define FMC_D15_AF          GPIO_AF_FMC

#define FMC_D16_PORT        GPIOH
#define FMC_D16_PIN         GPIO_Pin_8
#define FMC_D16_PINSOURCE   GPIO_PinSource8
#define FMC_D16_AF          GPIO_AF_FMC


#define FMC_D17_PORT        GPIOH
#define FMC_D17_PIN         GPIO_Pin_9
#define FMC_D17_PINSOURCE   GPIO_PinSource9
#define FMC_D17_AF          GPIO_AF_FMC

#define FMC_D18_PORT        GPIOH
#define FMC_D18_PIN         GPIO_Pin_10
#define FMC_D18_PINSOURCE   GPIO_PinSource10
#define FMC_D18_AF          GPIO_AF_FMC

#define FMC_D19_PORT        GPIOH
#define FMC_D19_PIN         GPIO_Pin_11
#define FMC_D19_PINSOURCE   GPIO_PinSource11
#define FMC_D19_AF          GPIO_AF_FMC

#define FMC_D20_PORT        GPIOH
#define FMC_D20_PIN         GPIO_Pin_12
#define FMC_D20_PINSOURCE   GPIO_PinSource12
#define FMC_D20_AF          GPIO_AF_FMC

#define FMC_D21_PORT        GPIOH
#define FMC_D21_PIN         GPIO_Pin_13
#define FMC_D21_PINSOURCE   GPIO_PinSource13
#define FMC_D21_AF          GPIO_AF_FMC     

#define FMC_D22_PORT        GPIOH
#define FMC_D22_PIN         GPIO_Pin_14
#define FMC_D22_PINSOURCE   GPIO_PinSource14
#define FMC_D22_AF          GPIO_AF_FMC

#define FMC_D23_PORT        GPIOH
#define FMC_D23_PIN         GPIO_Pin_15
#define FMC_D23_PINSOURCE   GPIO_PinSource15
#define FMC_D23_AF          GPIO_AF_FMC

#define FMC_D24_PORT        GPIOI
#define FMC_D24_PIN         GPIO_Pin_0
#define FMC_D24_PINSOURCE   GPIO_PinSource0
#define FMC_D24_AF          GPIO_AF_FMC

#define FMC_D25_PORT        GPIOI
#define FMC_D25_PIN         GPIO_Pin_1
#define FMC_D25_PINSOURCE   GPIO_PinSource1
#define FMC_D25_AF          GPIO_AF_FMC

#define FMC_D26_PORT        GPIOI
#define FMC_D26_PIN         GPIO_Pin_2
#define FMC_D26_PINSOURCE   GPIO_PinSource2
#define FMC_D26_AF          GPIO_AF_FMC

#define FMC_D27_PORT        GPIOI
#define FMC_D27_PIN         GPIO_Pin_3
#define FMC_D27_PINSOURCE   GPIO_PinSource3
#define FMC_D27_AF          GPIO_AF_FMC

#define FMC_D28_PORT        GPIOI
#define FMC_D28_PIN         GPIO_Pin_6
#define FMC_D28_PINSOURCE   GPIO_PinSource6
#define FMC_D28_AF          GPIO_AF_FMC

#define FMC_D29_PORT        GPIOI
#define FMC_D29_PIN         GPIO_Pin_7
#define FMC_D29_PINSOURCE   GPIO_PinSource7
#define FMC_D29_AF          GPIO_AF_FMC

#define FMC_D30_PORT        GPIOI
#define FMC_D30_PIN         GPIO_Pin_9
#define FMC_D30_PINSOURCE   GPIO_PinSource9
#define FMC_D30_AF          GPIO_AF_FMC

#define FMC_D31_PORT        GPIOI
#define FMC_D31_PIN         GPIO_Pin_10
#define FMC_D31_PINSOURCE   GPIO_PinSource10
#define FMC_D31_AF          GPIO_AF_FMC

/*控制信号线*/  
#define FMC_CS_PORT        GPIOH
#define FMC_CS_PIN         GPIO_Pin_6
#define FMC_CS_PINSOURCE   GPIO_PinSource6
#define FMC_CS_AF          GPIO_AF_FMC

#define FMC_BA0_PORT        GPIOG
#define FMC_BA0_PIN         GPIO_Pin_4
#define FMC_BA0_PINSOURCE   GPIO_PinSource4
#define FMC_BA0_AF          GPIO_AF_FMC

#define FMC_BA1_PORT        GPIOG
#define FMC_BA1_PIN         GPIO_Pin_5
#define FMC_BA1_PINSOURCE   GPIO_PinSource5
#define FMC_BA1_AF          GPIO_AF_FMC

#define FMC_WE_PORT        GPIOC
#define FMC_WE_PIN         GPIO_Pin_0
#define FMC_WE_PINSOURCE   GPIO_PinSource0
#define FMC_WE_AF          GPIO_AF_FMC

#define FMC_RAS_PORT        GPIOF
#define FMC_RAS_PIN         GPIO_Pin_11
#define FMC_RAS_PINSOURCE   GPIO_PinSource11
#define FMC_RAS_AF          GPIO_AF_FMC

#define FMC_CAS_PORT        GPIOG
#define FMC_CAS_PIN         GPIO_Pin_15
#define FMC_CAS_PINSOURCE   GPIO_PinSource15
#define FMC_CAS_AF          GPIO_AF_FMC

#define FMC_CLK_PORT        GPIOG
#define FMC_CLK_PIN         GPIO_Pin_8
#define FMC_CLK_PINSOURCE   GPIO_PinSource8
#define FMC_CLK_AF          GPIO_AF_FMC

#define FMC_CKE_PORT        GPIOH
#define FMC_CKE_PIN         GPIO_Pin_7
#define FMC_CKE_PINSOURCE   GPIO_PinSource7
#define FMC_CKE_AF          GPIO_AF_FMC

/*UDQM LDQM*/
#define FMC_UDQM_PORT        GPIOE
#define FMC_UDQM_PIN         GPIO_Pin_1
#define FMC_UDQM_PINSOURCE   GPIO_PinSource1
#define FMC_UDQM_AF          GPIO_AF_FMC

#define FMC_LDQM_PORT        GPIOE
#define FMC_LDQM_PIN         GPIO_Pin_0
#define FMC_LDQM_PINSOURCE   GPIO_PinSource0
#define FMC_LDQM_AF          GPIO_AF_FMC


#define FMC_NBL2_PORT        GPIOI
#define FMC_NBL2_PIN         GPIO_Pin_4
#define FMC_NBL2_PINSOURCE   GPIO_PinSource4
#define FMC_NBL2_AF          GPIO_AF_FMC

#define FMC_NBL3_PORT        GPIOI
#define FMC_NBL3_PIN         GPIO_Pin_5
#define FMC_NBL3_PINSOURCE   GPIO_PinSource5
#define FMC_NBL3_AF          GPIO_AF_FMC

/*-------------------- 函数声明 ----------------------*/

void  SDRAM_Init(void);



#endif 
