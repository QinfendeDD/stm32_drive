#ifndef __SPWM_H
#define __SPWM_H
#include "sys.h"

	  
void TIM1_PWM_Init(u16 pfreq ,u16 psc);
void TIM2_Int_Init(u16 ifreq,u16 psc);

#endif
