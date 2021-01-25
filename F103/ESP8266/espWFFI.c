#include "espWFFI.h"


//连接端口号
const u8* portnum = "80";		//7000 
const u8* serverip = "122.152.212.18"; //148.70.122.220
	
//WIFI STA模式,设置要去连接的路由器无线参数,请根据你自己的路由器设置,自行修改.
const u8* wifista_ssid="sxbctvnet-651520";			            //路由器SSID号(WIFI名称)  ALIENTEK
const u8* wifista_encryption="wpawpa2_aes";	  //wpa/wpa2 aes加密方式  wpawpa2_aes
const u8* wifista_password="13772500718"; 	//连接密码 15902020353


/***********************************************************
** 函 数 名: espWIFI_Init 
** 功能描述: 初始化配置串口WiFi  UART2+TIM4+DMA1_ch7
** 输    入: 无
** 输    出: 无
** 返 回 值: 无
** 日    期: 2020/5/3
************************************************************/
void espWIFI_Init(void)
{
	 USART2_Init(wifiBOTRATE);  /* 串口2配置，用于WIFI转串口收发数据，115200-1-8-N，包含TIM4 */
}


/***********************************************************
** 函 数 名: atk_8266_at_response
** 功能描述: 将收到的AT指令应答数据返回给电脑串口
** 输    入: mode:0,不清零USART2_RX_STA; 1,清零USART2_RX_STA;
** 输    出: 无
** 返 回 值: 无
** 日    期: 2019/3/23
************************************************************/
void atk_8266_at_response(u8 mode)
{
	if(USART2_RX_STA&0X8000)		            //接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		printf("%s",USART2_RX_BUF);	          //发送到串口
		if(mode)USART2_RX_STA=0;
	} 
}


/***********************************************************
** 函 数 名: atk_8266_check_cmd
** 功能描述: ATK-ESP8266发送命令后,检测接收到的应答
** 输    入: str:期待的应答结果
** 输    出: 无
** 返 回 值: 0,没有得到期待的应答结果 其他,期待应答结果的位置(str的位置)
** 日    期: 2019/3/23
************************************************************/
u8* atk_8266_check_cmd(u8 *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}


/***********************************************************
** 函 数 名: atk_8266_send_cmd
** 功能描述: 向ATK-ESP8266发送命令
** 输    入: cmd:发送的命令字符串  ack:期待的应答结果,如果为空,则表示不需要等待应答 waittime:等待时间(单位:10ms)
** 输    出: 无
** 返 回 值: 0,发送成功(得到了期待的应答结果) 1,发送失败
** 日    期: 2019/3/23
************************************************************/
u8 atk_8266_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	u2_printf("%s\r\n",cmd);	//发送命令  (向WiFi模块发送)
	
	if(ack&&waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(atk_8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;//得到有效数据 
				}
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
} 


/***********************************************************
** 函 数 名: atk_8266_send_data
** 功能描述: 向ATK-ESP8266发送指定数据
** 输    入: data:发送的数据(不需要添加回车了) ack:期待的应答结果,如果为空,则表示不需要等待应答 waittime:等待时间(单位:10ms) 
** 输    出: 无
** 返 回 值: 0,发送成功(得到了期待的应答结果)
** 日    期: 2019/3/23
************************************************************/
u8 atk_8266_send_data(u8 *data,u8 *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	u2_printf("%s",data);	                 //发送命令
	
	if(ack&&waittime)		                   //需要等待应答
	{
		while(--waittime)	                   //等待倒计时
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)           //接收到期待的应答结果
			{
				if(atk_8266_check_cmd(ack))break;//得到有效数据 
				USART2_RX_STA=0;
			} 
		}
		if(waittime==0)res=1; 
	}
	return res;
}


/***********************************************************
** 函 数 名: atk_8266_quit_trans
** 功能描述: ATK-ESP8266退出透传模式
** 输    入: 无 
** 输    出: 无
** 返 回 值: 0,退出成功; 1,退出失败
** 日    期: 2019/3/23
************************************************************/
u8 atk_8266_quit_trans(void)
{
	while((USART2->SR&0X40)==0);	//等待发送空
	USART2->DR='+';      
	delay_ms(15);					        //大于串口组帧时间(10ms)
	while((USART2->SR&0X40)==0);	//等待发送空
	USART2->DR='+';      
	delay_ms(15);					        //大于串口组帧时间(10ms)
	while((USART2->SR&0X40)==0);	//等待发送空
	USART2->DR='+';      
	delay_ms(500);					      //等待500ms
	return atk_8266_send_cmd("AT","OK",20);//退出透传判断.
}


/***********************************************************
** 函 数 名: wifi_sta_rans_config
** 功能描述: ATK-ESP8266模块TCP透传模式配置
** 输    入: 无
** 输    出: 无
** 返 回 值: 无
** 日    期: 2019/4/23
************************************************************/
void wifi_sta_rans_config(void)
{
	  char p[50];
	
		//一、设置工作模式(需重启生效) 1：station模式   2：AP模式  3：兼容 AP+station模式
		atk_8266_send_cmd("AT+CWMODE=1","OK",50);
		
		//二、Wifi模块重启生效
		atk_8266_send_cmd("AT+RST","ready",20);
		
		delay_ms(1000);         //延时3S等待重启成功
		delay_ms(1000);
		delay_ms(1000);
		delay_ms(1000);
		
		//三、模块连接上自己的路由
	  sprintf((char*)p,"AT+CWJAP=\"%s\",\"%s\"",wifista_ssid,wifista_password);//设置无线参数:ssid,密码
		while(atk_8266_send_cmd((u8 *)p,"WIFI GOT IP",600));
		
		//四、是否启用多路连接  0:单路连接模式 1:多路连接模式
		atk_8266_send_cmd("AT+CIPMUX=0","OK",20);
		
		//五、建立TCP连接  这四项分别代表了：要连接的ID号0~4   连接类型  远程服务器IP地址   远程服务器端口号
		sprintf((char*)p,"AT+CIPSTART=\"TCP\",\"%s\",%s",serverip,(u8*)portnum);    //配置目标TCP服务器
		while(atk_8266_send_cmd((u8 *)p,"CONNECT",200));
		
		//六、开启透传模式  0：表示关闭 1：表示开启透传
		atk_8266_send_cmd("AT+CIPMODE=1","OK",200);
		
		//七、透传模式下开始发送数据  这个指令之后就可以直接发数据了
		atk_8266_send_cmd("AT+CIPSEND","OK",50);
}


/***********************************************************
** 函 数 名: atk_8266_test
** 功能描述: ATK-ESP8266模块测试主函数
** 输    入: 无
** 输    出: 无
** 返 回 值: 无
** 日    期: 2019/3/23
************************************************************/
void atk_8266_test(void)
{
	while(atk_8266_send_cmd("AT","OK",20))         //检查WIFI模块是否在线,1表示不在线  发送AT 返回OK
	{
		atk_8266_quit_trans();                       //退出透传
		atk_8266_send_cmd("AT+CIPMODE=0","OK",200);  //关闭透传模式	
		printf("未检测到模块!!!\r\n");
		delay_ms(800);
		printf("尝试连接模块...\r\n");
	} 
	while(atk_8266_send_cmd("ATE0","OK",50));      //关闭回显
	
	//printf("开始配置WIFI模块...\r\n");

	delay_ms(20); 
  atk_8266_at_response(1);                      //检查ATK-ESP8266模块发送过来的数据,及时上传给电脑
		
	wifi_sta_rans_config();												//配置WIFI TCP透传模式
}



















































