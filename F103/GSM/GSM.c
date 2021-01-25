#include "GSM.h"
#include "uart_4.h"
#include "delay.h"
#include "GA6_module_errors.h"
#include "stdio.h"

extern u8 UART4_RX_BUF[UART4_REC_LEN]; 

char *phone_num = "AT+CMGS=\"18392457797\""; //修改这里可以修改电话号码


void GSM_Init(void)
{
	UART4_Init(GSM_UART_BOTLV);
}


/**********************************************************************************
** 函 数 名: send_text_message
** 功能描述: 发送英文短信
** 输    入: content 发送的字符串
** 输    出: 无
** 返 回 值: 无
** 日    期: 2020/5/4
***********************************************************************************/ 
int send_text_message(char *content)
{
	u8 ret;
	char end_char[2];
	
	end_char[0] = 0x1A;//发送 ctrl+z
	end_char[1] = '\0';
	
//	//设置存储位置
//	ret = UART4_Send_AT_Command("AT+CPMS=\"SM\",\"ME\",\"SM\"","OK",3,100);
//	if(ret == 0)
//	{
//		return AT_CPMS_ERROR;
//	}
	
	ret = UART4_Send_AT_Command("AT+CMGF=1","OK",3,50);//配置为TEXT模式
	if(ret == 0)
	{
		return AT_CMGF_ERROR;
	}
	
	ret = UART4_Send_AT_Command("AT+CSCS=\"GSM\"","OK",3,50);//设置字符格式
	if(ret == 0)
	{
		return AT_CSCS_ERROR;
	}
	
	ret = UART4_Send_AT_Command(phone_num,">",3,50);//输入收信人的电话号码
	if(ret == 0)
	{
		return AT_CMGS_ERROR;
	}
	
	UART4_SendString(content);  //发送内容

	ret = UART4_Send_AT_Command_End(end_char,"OK",1,250);//发送结束符，等待返回ok,等待5S发一次，因为短信成功发送的状态时间比较长
	if(ret == 0)
	{
		return END_CHAR_ERROR;
	}
	
	return 1;
}


/**********************************************************************************
** 函 数 名: check_status
** 功能描述: 检查模块状态
** 输    入: 无
** 输    出: 无
** 返 回 值: 1 成功  其他失败
** 日    期: 2020/5/4
***********************************************************************************/ 
int check_status(void)
{
	int ret;
	
	ret = UART4_Send_AT_Command("AT","OK",3,50);//测试通信是否成功
	if(ret == 0)
	{
		return COMMUNITE_ERROR;
	}
	
	ret = UART4_Send_AT_Command("AT+CPIN?","READY",2,50);//查询卡是否插上
	if(ret == 0)
	{
		return NO_SIM_CARD_ERROR;
	}
	ret = Wait_CREG(3);//查询卡是否注册到网络  
	if(ret == 0)
	{
		return SIM_CARD_NO_REG_ERROR;
	}
	return 1;
}


/*******************************************************************************
* 函数名 : Wait_CREG
* 描述   : 等待模块注册成功
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
u8 Wait_CREG(u8 query_times)
{
	u8 i;
	u8 k;
	u8 j;
	i = 0;
	URAT4_CLR_Buf();
	while(i == 0)        			
	{

		UART4_Send_Command("AT+CREG?");
		delay_ms(100); 
		
		for(k=0;k<UART4_REC_LEN-1;k++)      			
		{
			if((UART4_RX_BUF[k] == '+')&&(UART4_RX_BUF[k+1] == 'C')&&(UART4_RX_BUF[k+2] == 'R')&&(UART4_RX_BUF[k+3] == 'E')&&(UART4_RX_BUF[k+4] == 'G')&&(UART4_RX_BUF[k+5] == ':'))
			{
					 
				if((UART4_RX_BUF[k+7] == '1')&&((UART4_RX_BUF[k+9] == '1')||(UART4_RX_BUF[k+9] == '5')))
				{
					i = 1;
					return 1;
				}
				
			}
		}
		j++;
		if(j > query_times)
		{
			return 0;
		}
		
	}
	return 0;
}


