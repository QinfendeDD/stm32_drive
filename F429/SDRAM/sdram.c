/***
	***************************************************************************
	*	@file  	sdram.c
	*	@brief   sdram初始化，本代码移植于官方stm32f429discovery例程，
	*				并作出相应修改
	*				
   ******************************************************************************
   *  
	* 
	* 
	* 
	***************************************************************************
***/
  
#include "sdram.h"   

// 函数：简易延时函数
// 参数：nCount - 延时时间
//
static void SDRAM_delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (100000 * nCount); index != 0; index--)
  {
  }
}

// 函数：SDRAM IO口配置 
//
static void SDRAM_GPIO_Config(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE |       
	RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOH | RCC_AHB1Periph_GPIOI,ENABLE); 	

	//IO口配置
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;       //配置为复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽输出
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;   

	//地址线 
	GPIO_PinAFConfig(FMC_A0_PORT,  FMC_A0_PINSOURCE ,  FMC_A0_AF);	
	GPIO_PinAFConfig(FMC_A1_PORT,  FMC_A1_PINSOURCE ,  FMC_A1_AF);	
	GPIO_PinAFConfig(FMC_A2_PORT,  FMC_A2_PINSOURCE ,  FMC_A2_AF);	
	GPIO_PinAFConfig(FMC_A3_PORT,  FMC_A3_PINSOURCE ,  FMC_A3_AF);  
	GPIO_PinAFConfig(FMC_A4_PORT,  FMC_A4_PINSOURCE ,  FMC_A4_AF);  
	GPIO_PinAFConfig(FMC_A5_PORT,  FMC_A5_PINSOURCE ,  FMC_A5_AF);  
	GPIO_PinAFConfig(FMC_A6_PORT,  FMC_A6_PINSOURCE ,  FMC_A6_AF);  
	GPIO_PinAFConfig(FMC_A7_PORT,  FMC_A7_PINSOURCE ,  FMC_A7_AF);	
	GPIO_PinAFConfig(FMC_A8_PORT,  FMC_A8_PINSOURCE ,  FMC_A8_AF);
	GPIO_PinAFConfig(FMC_A9_PORT,  FMC_A9_PINSOURCE ,  FMC_A9_AF);
	GPIO_PinAFConfig(FMC_A10_PORT, FMC_A10_PINSOURCE , FMC_A10_AF);
	GPIO_PinAFConfig(FMC_A11_PORT, FMC_A11_PINSOURCE , FMC_A11_AF);	 

	GPIO_InitStructure.GPIO_Pin = FMC_A0_PIN; 
	GPIO_Init(FMC_A0_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_A1_PIN; 
	GPIO_Init(FMC_A1_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_A2_PIN; 
	GPIO_Init(FMC_A2_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_A3_PIN; 
	GPIO_Init(FMC_A3_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_A4_PIN; 
	GPIO_Init(FMC_A4_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_A5_PIN; 
	GPIO_Init(FMC_A5_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_A6_PIN; 
	GPIO_Init(FMC_A6_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_A7_PIN; 
	GPIO_Init(FMC_A7_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_A8_PIN; 
	GPIO_Init(FMC_A8_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_A9_PIN; 
	GPIO_Init(FMC_A9_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_A10_PIN; 
	GPIO_Init(FMC_A10_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_A11_PIN; 
	GPIO_Init(FMC_A11_PORT, &GPIO_InitStructure);

   //数据线
	GPIO_PinAFConfig(FMC_D0_PORT,  FMC_D0_PINSOURCE ,  FMC_D0_AF);  
	GPIO_PinAFConfig(FMC_D1_PORT,  FMC_D1_PINSOURCE ,  FMC_D1_AF);
	GPIO_PinAFConfig(FMC_D2_PORT,  FMC_D2_PINSOURCE ,  FMC_D2_AF);
	GPIO_PinAFConfig(FMC_D3_PORT,  FMC_D3_PINSOURCE ,  FMC_D3_AF);	 
	GPIO_PinAFConfig(FMC_D4_PORT,  FMC_D4_PINSOURCE ,  FMC_D4_AF);	 
	GPIO_PinAFConfig(FMC_D5_PORT,  FMC_D5_PINSOURCE ,  FMC_D5_AF);	 
	GPIO_PinAFConfig(FMC_D6_PORT,  FMC_D6_PINSOURCE ,  FMC_D6_AF);
	GPIO_PinAFConfig(FMC_D7_PORT,  FMC_D7_PINSOURCE ,  FMC_D7_AF);  
	GPIO_PinAFConfig(FMC_D8_PORT,  FMC_D8_PINSOURCE ,  FMC_D8_AF);  
	GPIO_PinAFConfig(FMC_D9_PORT,  FMC_D9_PINSOURCE ,  FMC_D9_AF);  
	GPIO_PinAFConfig(FMC_D10_PORT, FMC_D10_PINSOURCE , FMC_D10_AF);
	GPIO_PinAFConfig(FMC_D11_PORT, FMC_D11_PINSOURCE , FMC_D11_AF);
	GPIO_PinAFConfig(FMC_D12_PORT, FMC_D12_PINSOURCE , FMC_D12_AF);
	GPIO_PinAFConfig(FMC_D13_PORT, FMC_D13_PINSOURCE , FMC_D13_AF);
	GPIO_PinAFConfig(FMC_D14_PORT, FMC_D14_PINSOURCE , FMC_D14_AF);
	GPIO_PinAFConfig(FMC_D15_PORT, FMC_D15_PINSOURCE , FMC_D15_AF);
	GPIO_PinAFConfig(FMC_D16_PORT, FMC_D16_PINSOURCE , FMC_D16_AF);
	GPIO_PinAFConfig(FMC_D17_PORT, FMC_D17_PINSOURCE , FMC_D17_AF);  
	GPIO_PinAFConfig(FMC_D18_PORT, FMC_D18_PINSOURCE , FMC_D18_AF);  
	GPIO_PinAFConfig(FMC_D19_PORT, FMC_D19_PINSOURCE , FMC_D19_AF); 
	GPIO_PinAFConfig(FMC_D20_PORT, FMC_D20_PINSOURCE , FMC_D20_AF);  
	GPIO_PinAFConfig(FMC_D21_PORT, FMC_D21_PINSOURCE , FMC_D21_AF);  
	GPIO_PinAFConfig(FMC_D22_PORT, FMC_D22_PINSOURCE , FMC_D22_AF); 
	GPIO_PinAFConfig(FMC_D23_PORT, FMC_D23_PINSOURCE , FMC_D23_AF);
	GPIO_PinAFConfig(FMC_D24_PORT, FMC_D24_PINSOURCE , FMC_D24_AF);
	GPIO_PinAFConfig(FMC_D25_PORT, FMC_D25_PINSOURCE , FMC_D25_AF);
	GPIO_PinAFConfig(FMC_D26_PORT, FMC_D26_PINSOURCE , FMC_D26_AF); 
	GPIO_PinAFConfig(FMC_D27_PORT, FMC_D27_PINSOURCE , FMC_D27_AF);	
	GPIO_PinAFConfig(FMC_D28_PORT, FMC_D28_PINSOURCE , FMC_D28_AF);  
   GPIO_PinAFConfig(FMC_D29_PORT, FMC_D29_PINSOURCE , FMC_D29_AF);	
	GPIO_PinAFConfig(FMC_D30_PORT, FMC_D30_PINSOURCE , FMC_D30_AF);  
	GPIO_PinAFConfig(FMC_D31_PORT, FMC_D31_PINSOURCE , FMC_D31_AF);      
  
	GPIO_InitStructure.GPIO_Pin = FMC_D0_PIN; 
	GPIO_Init(FMC_D0_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D1_PIN; 
	GPIO_Init(FMC_D1_PORT, &GPIO_InitStructure);
	 
	GPIO_InitStructure.GPIO_Pin = FMC_D2_PIN; 
	GPIO_Init(FMC_D2_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D3_PIN; 
	GPIO_Init(FMC_D3_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D4_PIN; 
	GPIO_Init(FMC_D4_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D5_PIN; 
	GPIO_Init(FMC_D5_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D6_PIN; 
	GPIO_Init(FMC_D6_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D7_PIN; 
	GPIO_Init(FMC_D7_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D8_PIN; 
	GPIO_Init(FMC_D8_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D9_PIN; 
	GPIO_Init(FMC_D9_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D10_PIN; 
	GPIO_Init(FMC_D10_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D11_PIN; 
	GPIO_Init(FMC_D11_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D12_PIN; 
	GPIO_Init(FMC_D12_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D13_PIN; 
	GPIO_Init(FMC_D13_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D14_PIN; 
	GPIO_Init(FMC_D14_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D15_PIN; 
	GPIO_Init(FMC_D15_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D16_PIN; 
	GPIO_Init(FMC_D16_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D17_PIN; 
	GPIO_Init(FMC_D17_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D18_PIN; 
	GPIO_Init(FMC_D18_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D19_PIN; 
	GPIO_Init(FMC_D19_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D20_PIN; 
	GPIO_Init(FMC_D20_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D21_PIN; 
	GPIO_Init(FMC_D21_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D22_PIN; 
	GPIO_Init(FMC_D22_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D23_PIN; 
	GPIO_Init(FMC_D23_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D24_PIN; 
	GPIO_Init(FMC_D24_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D25_PIN; 
	GPIO_Init(FMC_D25_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D26_PIN; 
	GPIO_Init(FMC_D26_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D27_PIN; 
	GPIO_Init(FMC_D27_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D28_PIN; 
	GPIO_Init(FMC_D28_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D29_PIN; 
	GPIO_Init(FMC_D29_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D30_PIN; 
	GPIO_Init(FMC_D30_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_D31_PIN; 
	GPIO_Init(FMC_D31_PORT, &GPIO_InitStructure);
  
   //控制线
	GPIO_PinAFConfig(FMC_CS_PORT, FMC_CS_PINSOURCE , FMC_CS_AF);  
	GPIO_PinAFConfig(FMC_BA0_PORT, FMC_BA0_PINSOURCE , FMC_BA0_AF); 
	GPIO_PinAFConfig(FMC_BA1_PORT, FMC_BA1_PINSOURCE , FMC_BA1_AF);	
	GPIO_PinAFConfig(FMC_WE_PORT, FMC_WE_PINSOURCE , FMC_WE_AF);	
	GPIO_PinAFConfig(FMC_RAS_PORT, FMC_RAS_PINSOURCE , FMC_RAS_AF); 
	GPIO_PinAFConfig(FMC_CAS_PORT, FMC_CAS_PINSOURCE , FMC_CAS_AF);  
	GPIO_PinAFConfig(FMC_CLK_PORT, FMC_CLK_PINSOURCE , FMC_CLK_AF);  
	GPIO_PinAFConfig(FMC_CKE_PORT, FMC_CKE_PINSOURCE , FMC_CKE_AF);
	GPIO_PinAFConfig(FMC_UDQM_PORT, FMC_UDQM_PINSOURCE , FMC_UDQM_AF);  
	GPIO_PinAFConfig(FMC_LDQM_PORT, FMC_LDQM_PINSOURCE , FMC_LDQM_AF);  
	GPIO_PinAFConfig(FMC_NBL2_PORT, FMC_NBL2_PINSOURCE , FMC_NBL2_AF);  
	GPIO_PinAFConfig(FMC_NBL3_PORT, FMC_NBL3_PINSOURCE , FMC_NBL3_AF);     
  
  
	GPIO_InitStructure.GPIO_Pin = FMC_CS_PIN; 
	GPIO_Init(FMC_CS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_BA0_PIN; 
	GPIO_Init(FMC_BA0_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_BA1_PIN;
	GPIO_Init(FMC_BA1_PORT, &GPIO_InitStructure);
	 
	GPIO_InitStructure.GPIO_Pin = FMC_WE_PIN; 
	GPIO_Init(FMC_WE_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_RAS_PIN; 
	GPIO_Init(FMC_RAS_PORT, &GPIO_InitStructure);
	 
	GPIO_InitStructure.GPIO_Pin = FMC_CAS_PIN; 
	GPIO_Init(FMC_CAS_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_CLK_PIN; 
	GPIO_Init(FMC_CLK_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_CKE_PIN; 
	GPIO_Init(FMC_CKE_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_UDQM_PIN; 
	GPIO_Init(FMC_UDQM_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_LDQM_PIN; 
	GPIO_Init(FMC_LDQM_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_NBL2_PIN; 
	GPIO_Init(FMC_NBL2_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FMC_NBL3_PIN; 
	GPIO_Init(FMC_NBL3_PORT, &GPIO_InitStructure);

}

// 函数：SDRAM 参数配置
//
static void SDRAM_InitSequence(void)
{
	FMC_SDRAMCommandTypeDef FMC_SDRAMCommandStructure;
	uint32_t tmpr = 0;

	//开启SDRAM时钟 
	FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_CLK_Enabled;
	FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_COMMAND_TARGET_BANK;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;

	while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET);  //等待SDRAM空闲 

	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);  

	SDRAM_delay(10); //延时100ms 
	 
	// 预充电 配置
	FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_PALL;
	FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_COMMAND_TARGET_BANK;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;

	while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET)	//等待SDRAM空闲 

	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

	//自动刷新配置 
	FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_AutoRefresh;
	FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_COMMAND_TARGET_BANK;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 8;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = 0;

	while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET);	//等待SDRAM空闲 
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);
	while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET);	//等待SDRAM空闲 
	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);  // 发送第2次命令

	//寄存器配置 
	tmpr = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_2          |
						  SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |
						  SDRAM_MODEREG_CAS_LATENCY_3           |
						  SDRAM_MODEREG_OPERATING_MODE_STANDARD |
						  SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

	FMC_SDRAMCommandStructure.FMC_CommandMode = FMC_Command_Mode_LoadMode;
	FMC_SDRAMCommandStructure.FMC_CommandTarget = FMC_COMMAND_TARGET_BANK;
	FMC_SDRAMCommandStructure.FMC_AutoRefreshNumber = 1;
	FMC_SDRAMCommandStructure.FMC_ModeRegisterDefinition = tmpr;

	while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET);	//等待SDRAM空闲 

	FMC_SDRAMCmdConfig(&FMC_SDRAMCommandStructure);

	FMC_SetRefreshCount(1386);	// 设置刷新计数器 

	while(FMC_GetFlagStatus(FMC_BANK_SDRAM, FMC_FLAG_Busy) != RESET);	//等待SDRAM空闲 
}


// 函数：SDRAM 初始化

//
void SDRAM_Init(void)
{
	FMC_SDRAMInitTypeDef  FMC_SDRAMInitStructure;
	FMC_SDRAMTimingInitTypeDef  FMC_SDRAMTimingInitStructure; 

	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FMC, ENABLE);
	SDRAM_GPIO_Config();// 初始化IO口

	// 以90M的sdram时钟来算 
	FMC_SDRAMTimingInitStructure.FMC_LoadToActiveDelay    = 2;  /* TMRD: 2 Clock cycles */  
	FMC_SDRAMTimingInitStructure.FMC_ExitSelfRefreshDelay = 7;  /* TXSR: min=70ns (7x11.11ns) */  
	FMC_SDRAMTimingInitStructure.FMC_SelfRefreshTime      = 4;  /* TRAS: min=42ns (4x11.11ns) max=120k (ns) */    
	FMC_SDRAMTimingInitStructure.FMC_RowCycleDelay        = 7;  /* TRC:  min=70 (7x11.11ns) */   
	FMC_SDRAMTimingInitStructure.FMC_WriteRecoveryTime    = 2;  /* TWR:  min=1+ 7ns (1+1x11.11ns) */        
	FMC_SDRAMTimingInitStructure.FMC_RPDelay              = 2;  /* TRP:  20ns => 2x11.11ns */              
	FMC_SDRAMTimingInitStructure.FMC_RCDDelay             = 2;  /* TRCD: 20ns => 2x11.11ns */

	FMC_SDRAMInitStructure.FMC_Bank = FMC_BANK_SDRAM; //选择BANK区
	FMC_SDRAMInitStructure.FMC_ColumnBitsNumber = FMC_ColumnBits_Number_8b; // 行地址宽度
	FMC_SDRAMInitStructure.FMC_RowBitsNumber = FMC_RowBits_Number_12b;// 列地址线宽度
	FMC_SDRAMInitStructure.FMC_SDMemoryDataWidth = FMC_SDMemory_Width_32b; // 数据宽度  
	FMC_SDRAMInitStructure.FMC_InternalBankNumber = FMC_InternalBank_Number_4; // bank数量
	FMC_SDRAMInitStructure.FMC_CASLatency = FMC_CAS_Latency_3; //CAS 
	FMC_SDRAMInitStructure.FMC_WriteProtection = FMC_Write_Protection_Disable; // 禁止写保护
	FMC_SDRAMInitStructure.FMC_SDClockPeriod = FMC_SDClock_Period_2; // 分频
	FMC_SDRAMInitStructure.FMC_ReadBurst = FMC_Read_Burst_Enable; // 突发模式  
	FMC_SDRAMInitStructure.FMC_ReadPipeDelay = FMC_ReadPipe_Delay_1; // 读延迟
	FMC_SDRAMInitStructure.FMC_SDRAMTimingStruct = &FMC_SDRAMTimingInitStructure;

	FMC_SDRAMInit(&FMC_SDRAMInitStructure); 

	SDRAM_InitSequence(); //配置SDRAM
  
}
