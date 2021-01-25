#include "HC-12.h"
#include "usart_2.h"

void HC12_Init(void)
{
	USART2_Init(HC12_USART_BOTLV);
}

