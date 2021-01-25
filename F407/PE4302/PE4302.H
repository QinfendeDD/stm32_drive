#ifndef _PE4302_H_
#define _PE4302_H_

#include "sys.h"
#include "delay.h"

#define DB_0    ((u8)0X00)
#define DB_0_5  ((u8)0X01)
#define DB_1    ((u8)0X02)
#define DB_2    ((u8)0X04)
#define DB_4    ((u8)0X08)
#define DB_8    ((u8)0X10)
#define DB_16   ((u8)0X20)
#define DB_31_5 ((u8)0X3F)

#define PE_LE_0					PEout(9)
#define PE_CLK					PEout(11)
#define PE_DAT					PEout(10)

#define PE_LE_0_EN      PE_LE_0 = 0

#define PE_LE_0_DIS     PE_LE_0 = 1

#define PE_CLK_0        PE_CLK = 0
#define PE_CLK_1        PE_CLK = 1

#define PE_DAT_0        PE_DAT = 0
#define PE_DAT_1        PE_DAT = 1

void PE_GPIO_Init(void);
void HMC624_Set(unsigned char db);
#endif
