#ifndef __GSM_H_
#define __GSM_H_
#include "sys.h"

#define GSM_UART_BOTLV  115200

void GSM_Init(void);
int send_text_message(char *content);
int check_status(void);
u8 Wait_CREG(u8 query_times);
	

#endif

