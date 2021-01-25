#include "oled_play.h"


void xianshi()
{
	int i;
	for(i=0;i<3;i++)
  {
		OLED_ShowCHinese(0+i*16,0,i);delay_ms(1);
	}
}
