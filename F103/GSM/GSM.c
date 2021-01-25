#include "GSM.h"
#include "uart_4.h"
#include "delay.h"
#include "GA6_module_errors.h"
#include "stdio.h"

extern u8 UART4_RX_BUF[UART4_REC_LEN]; 

char *phone_num = "AT+CMGS=\"18392457797\""; //�޸���������޸ĵ绰����


void GSM_Init(void)
{
	UART4_Init(GSM_UART_BOTLV);
}


/**********************************************************************************
** �� �� ��: send_text_message
** ��������: ����Ӣ�Ķ���
** ��    ��: content ���͵��ַ���
** ��    ��: ��
** �� �� ֵ: ��
** ��    ��: 2020/5/4
***********************************************************************************/ 
int send_text_message(char *content)
{
	u8 ret;
	char end_char[2];
	
	end_char[0] = 0x1A;//���� ctrl+z
	end_char[1] = '\0';
	
//	//���ô洢λ��
//	ret = UART4_Send_AT_Command("AT+CPMS=\"SM\",\"ME\",\"SM\"","OK",3,100);
//	if(ret == 0)
//	{
//		return AT_CPMS_ERROR;
//	}
	
	ret = UART4_Send_AT_Command("AT+CMGF=1","OK",3,50);//����ΪTEXTģʽ
	if(ret == 0)
	{
		return AT_CMGF_ERROR;
	}
	
	ret = UART4_Send_AT_Command("AT+CSCS=\"GSM\"","OK",3,50);//�����ַ���ʽ
	if(ret == 0)
	{
		return AT_CSCS_ERROR;
	}
	
	ret = UART4_Send_AT_Command(phone_num,">",3,50);//���������˵ĵ绰����
	if(ret == 0)
	{
		return AT_CMGS_ERROR;
	}
	
	UART4_SendString(content);  //��������

	ret = UART4_Send_AT_Command_End(end_char,"OK",1,250);//���ͽ��������ȴ�����ok,�ȴ�5S��һ�Σ���Ϊ���ųɹ����͵�״̬ʱ��Ƚϳ�
	if(ret == 0)
	{
		return END_CHAR_ERROR;
	}
	
	return 1;
}


/**********************************************************************************
** �� �� ��: check_status
** ��������: ���ģ��״̬
** ��    ��: ��
** ��    ��: ��
** �� �� ֵ: 1 �ɹ�  ����ʧ��
** ��    ��: 2020/5/4
***********************************************************************************/ 
int check_status(void)
{
	int ret;
	
	ret = UART4_Send_AT_Command("AT","OK",3,50);//����ͨ���Ƿ�ɹ�
	if(ret == 0)
	{
		return COMMUNITE_ERROR;
	}
	
	ret = UART4_Send_AT_Command("AT+CPIN?","READY",2,50);//��ѯ���Ƿ����
	if(ret == 0)
	{
		return NO_SIM_CARD_ERROR;
	}
	ret = Wait_CREG(3);//��ѯ���Ƿ�ע�ᵽ����  
	if(ret == 0)
	{
		return SIM_CARD_NO_REG_ERROR;
	}
	return 1;
}


/*******************************************************************************
* ������ : Wait_CREG
* ����   : �ȴ�ģ��ע��ɹ�
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
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


