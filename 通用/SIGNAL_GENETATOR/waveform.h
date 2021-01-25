#ifndef __WAVEFORM_H
#define __WAVEFORM_H	
#include "sys.h"

#define tableSize 128

void No_Generation(void);

void sin_example_Generation(void);
void square_example_Generation(void);
void tri_example_Generation(void);
void saw_example_Generation(void);

void sin_Generation(u16 sin_amp);
void square_Generation(u16 square_amp);
void tri_Generation(u16 tri_amp);
void saw_Generation(u16 saw_amp);

void MYDAC_Init(void);
void DacGPIO_Configuration(void);
void DAC_DMA_Configuration(u32 cmar);
void TIM6_Configuration(u16 arr,u16 psc);
u32 Get_PSCARR(u32 Frequency);
void Get_TIM_PSCARR(u32 Frequency);
u16 Get_VOLTAGE(u16 vol);

u8 sg_get_keynum(void);


void SG_Welcome(void);

void SG_Freq_Set(void);
void SG_Amp_Set(void);




#endif  

