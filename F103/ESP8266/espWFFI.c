#include "espWFFI.h"


//���Ӷ˿ں�
const u8* portnum = "80";		//7000 
const u8* serverip = "122.152.212.18"; //148.70.122.220
	
//WIFI STAģʽ,����Ҫȥ���ӵ�·�������߲���,��������Լ���·��������,�����޸�.
const u8* wifista_ssid="sxbctvnet-651520";			            //·����SSID��(WIFI����)  ALIENTEK
const u8* wifista_encryption="wpawpa2_aes";	  //wpa/wpa2 aes���ܷ�ʽ  wpawpa2_aes
const u8* wifista_password="13772500718"; 	//�������� 15902020353


/***********************************************************
** �� �� ��: espWIFI_Init 
** ��������: ��ʼ�����ô���WiFi  UART2+TIM4+DMA1_ch7
** ��    ��: ��
** ��    ��: ��
** �� �� ֵ: ��
** ��    ��: 2020/5/3
************************************************************/
void espWIFI_Init(void)
{
	 USART2_Init(wifiBOTRATE);  /* ����2���ã�����WIFIת�����շ����ݣ�115200-1-8-N������TIM4 */
}


/***********************************************************
** �� �� ��: atk_8266_at_response
** ��������: ���յ���ATָ��Ӧ�����ݷ��ظ����Դ���
** ��    ��: mode:0,������USART2_RX_STA; 1,����USART2_RX_STA;
** ��    ��: ��
** �� �� ֵ: ��
** ��    ��: 2019/3/23
************************************************************/
void atk_8266_at_response(u8 mode)
{
	if(USART2_RX_STA&0X8000)		            //���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//���ӽ�����
		printf("%s",USART2_RX_BUF);	          //���͵�����
		if(mode)USART2_RX_STA=0;
	} 
}


/***********************************************************
** �� �� ��: atk_8266_check_cmd
** ��������: ATK-ESP8266���������,�����յ���Ӧ��
** ��    ��: str:�ڴ���Ӧ����
** ��    ��: ��
** �� �� ֵ: 0,û�еõ��ڴ���Ӧ���� ����,�ڴ�Ӧ������λ��(str��λ��)
** ��    ��: 2019/3/23
************************************************************/
u8* atk_8266_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//���յ�һ��������
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//���ӽ�����
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}


/***********************************************************
** �� �� ��: atk_8266_send_cmd
** ��������: ��ATK-ESP8266��������
** ��    ��: cmd:���͵������ַ���  ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ�� waittime:�ȴ�ʱ��(��λ:10ms)
** ��    ��: ��
** �� �� ֵ: 0,���ͳɹ�(�õ����ڴ���Ӧ����) 1,����ʧ��
** ��    ��: 2019/3/23
************************************************************/
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	u2_printf("%s\r\n",cmd);	//��������  (��WiFiģ�鷢��)
	
	if(ack&&waittime)		//��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	//�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//�õ���Ч���� 
				}
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 


/***********************************************************
** �� �� ��: atk_8266_send_data
** ��������: ��ATK-ESP8266����ָ������
** ��    ��: data:���͵�����(����Ҫ���ӻس���) ack:�ڴ���Ӧ����,���Ϊ��,���ʾ����Ҫ�ȴ�Ӧ�� waittime:�ȴ�ʱ��(��λ:10ms) 
** ��    ��: ��
** �� �� ֵ: 0,���ͳɹ�(�õ����ڴ���Ӧ����)
** ��    ��: 2019/3/23
************************************************************/
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	u2_printf("%s",data);	                 //��������
	
	if(ack&&waittime)		                   //��Ҫ�ȴ�Ӧ��
	{
		while(--waittime)	                   //�ȴ�����ʱ
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)           //���յ��ڴ���Ӧ����
			{
				if(atk_8266_check_cmd(ack))break;//�õ���Ч���� 
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}


/***********************************************************
** �� �� ��: atk_8266_quit_trans
** ��������: ATK-ESP8266�˳�͸��ģʽ
** ��    ��: �� 
** ��    ��: ��
** �� �� ֵ: 0,�˳��ɹ�; 1,�˳�ʧ��
** ��    ��: 2019/3/23
************************************************************/
u8 atk_8266_quit_trans(void)
{
	while((USART2->SR&0X40)==0);	//�ȴ����Ϳ�
	USART2->DR='+';      
	delay_ms(15);					        //���ڴ�����֡ʱ��(10ms)
	while((USART2->SR&0X40)==0);	//�ȴ����Ϳ�
	USART2->DR='+';      
	delay_ms(15);					        //���ڴ�����֡ʱ��(10ms)
	while((USART2->SR&0X40)==0);	//�ȴ����Ϳ�
	USART2->DR='+';      
	delay_ms(500);					      //�ȴ�500ms
	return atk_8266_send_cmd("AT","OK",20);//�˳�͸���ж�.
}


/***********************************************************
** �� �� ��: wifi_sta_rans_config
** ��������: ATK-ESP8266ģ��TCP͸��ģʽ����
** ��    ��: ��
** ��    ��: ��
** �� �� ֵ: ��
** ��    ��: 2019/4/23
************************************************************/
void wifi_sta_rans_config(void)
{
	  char p[50];
	
		//һ�����ù���ģʽ(��������Ч) 1��stationģʽ   2��APģʽ  3������ AP+stationģʽ
		atk_8266_send_cmd("AT+CWMODE=1","OK",50);
		
		//����Wifiģ��������Ч
		atk_8266_send_cmd("AT+RST","ready",20);
		
		delay_ms(1000);         //��ʱ3S�ȴ������ɹ�
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		
		//����ģ���������Լ���·��
	  sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//�������߲���:ssid,����
		while(atk_8266_send_cmd((u8 *)p,"WIFI GOT IP",600));
		
		//�ġ��Ƿ����ö�·����  0:��·����ģʽ 1:��·����ģʽ
		atk_8266_send_cmd("AT+CIPMUX=0","OK",20);
		
		//�塢����TCP����  ������ֱ�����ˣ�Ҫ���ӵ�ID��0~4   ��������  Զ�̷�����IP��ַ   Զ�̷������˿ں�
		sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",serverip,(u8*)portnum);    //����Ŀ��TCP������
		while(atk_8266_send_cmd((u8 *)p,"CONNECT",200));
		
		//��������͸��ģʽ  0����ʾ�ر� 1����ʾ����͸��
		atk_8266_send_cmd("AT+CIPMODE=1","OK",200);
		
		//�ߡ�͸��ģʽ�¿�ʼ��������  ���ָ��֮��Ϳ���ֱ�ӷ�������
		atk_8266_send_cmd("AT+CIPSEND","OK",50);
}


/***********************************************************
** �� �� ��: atk_8266_test
** ��������: ATK-ESP8266ģ�����������
** ��    ��: ��
** ��    ��: ��
** �� �� ֵ: ��
** ��    ��: 2019/3/23
************************************************************/
void atk_8266_test(void)
{
	while(atk_8266_send_cmd("AT","OK",20))         //���WIFIģ���Ƿ�����,1��ʾ������  ����AT ����OK
	{
		atk_8266_quit_trans();                       //�˳�͸��
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //�ر�͸��ģʽ	
		printf("δ��⵽ģ��!!!\r\n");
		delay_ms(800);
		printf("��������ģ��...\r\n");
	} 
	while(atk_8266_send_cmd("ATE0","OK",50));      //�رջ���
	
	//printf("��ʼ����WIFIģ��...\r\n");

	delay_ms(20); 
  atk_8266_at_response(1);                      //���ATK-ESP8266ģ�鷢�͹���������,��ʱ�ϴ�������
		
	wifi_sta_rans_config();												//����WIFI TCP͸��ģʽ
}


















































