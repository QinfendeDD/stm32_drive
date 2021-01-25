#include "usart.h"
#include "rda5820.h"
#include "delay.h"
#include "key.h"

int32_t FM = 0x0000;
int32_t FM1 = 0x0000;
//初始化

//0,初始化成功;
//其他,初始化失败.
u8 RDA5820_Init(void)
{
	u16 id;
	IIC_Init1();						//初始化IIC口	
	id=RDA5820_RD_Reg(RDA5820_R00);	//读取ID =0X5805
	printf("ID= %x \r\n",id);//串口发送
	if(id==0X5805)					//读取ID正确
	{ 
		printf("ID= %x \r\n",id+1);//串口发送
	 	RDA5820_WR_Reg(RDA5820_R02,0x0002);	//软复位
		delay_ms(50);
	 	RDA5820_WR_Reg(RDA5820_R02,0xD281);	//立体声,上电
		delay_ms(600);						//等待时钟稳定 
		printf("ID= %x \r\n",id+2);//串口发送
	 	RDA5820_WR_Reg(RDA5820_R05,0X88AF);	//搜索强度8,LNAN,2.1mA,VOL最大
		//启动接收模式
		RDA5820_WR_Reg(0x40,0x0000);
		RDA5820_Band_Set(2);////设置RDA5820的工作频段//band:0,87~108Mhz;1,76~91Mhz;2,76~108Mhz;3,用户自定义(53H~54H)
		RDA5820_Space_Set(0);//设置RDA5820的步进频率//band:0,100Khz;1,200Khz;3,50Khz;3,保留
//		RdaSeek();
		RDA5820_Freq_Set(7600);
		printf("ID= %x \r\n",id+11);//串口发送
 	}else return 1;//初始化失败
	return 0;
}					 				    
void RDA5820_WR_Reg(u8 addr,u16 val)
{
  IIC_Start1();  			   
	IIC_Send_Byte1(RDA5820_WRITE);	//发送写命令   	 
	IIC_Wait_Ack1();	   
  IIC_Send_Byte1(addr);   			//发送地址
	IIC_Wait_Ack1(); 	 										  		   
	IIC_Send_Byte1(val>>8);     		//发送高字节							   
	IIC_Wait_Ack1();  		    	   
 	IIC_Send_Byte1(val&0XFF);     	//发送低字节							   
 	IIC_Wait_Ack1();  		    	   
  IIC_Stop1();						//产生一个停止条件 	 
}
//读RDA5820寄存器	
u16 RDA5820_RD_Reg(u8 addr)
{
	u16 res;
    IIC_Start1();  			   
	IIC_Send_Byte1(RDA5820_WRITE);	//发送写命令   	 
	IIC_Wait_Ack1();	   
    IIC_Send_Byte1(addr);   			//发送地址
	IIC_Wait_Ack1(); 	 										  		   
    IIC_Start1();  			   
	IIC_Send_Byte1(RDA5820_READ);    //发送读命令							   
	IIC_Wait_Ack1();  		    	   
 	res=IIC_Read_Byte1(1);     		//读高字节,发送ACK	  
  	res<<=8;
  	res|=IIC_Read_Byte1(0);     		//读低字节,发送NACK							   
  	IIC_Stop1();						//产生一个停止条件 
	return res;						//返回读到的数据
}
//设置RDA5820为RX模式//接收模式
void RDA5820_RX_Mode(void)
{
	u16 temp;
	temp=RDA5820_RD_Reg(0X40);		//读取0X40的内容
	temp&=0xfff0;					//RX 模式   
	RDA5820_WR_Reg(0X40,temp) ;		//FM RX模式 
}			
//设置RDA5820为TX模式、、、发送模式
void RDA5820_TX_Mode(void)
{
	u16 temp;
	temp=RDA5820_RD_Reg(0X40);		//读取0X40的内容
	temp&=0xfff0;
	temp|=0x0001;				    //TX 模式
	RDA5820_WR_Reg(0X40,temp) ;		//FM TM 模式 
}
//得到信号强度
//返回值范围:0~127
u8 RDA5820_Rssi_Get(void)
{
	u16 temp;
	temp=RDA5820_RD_Reg(0X0B);		//读取0X0B的内容
	return temp>>9;                 //返回信号强度
}
//设置音量ok
//vol:0~15;
void RDA5820_Vol_Set(u8 vol)
{
	u16 temp;
	temp=RDA5820_RD_Reg(0X05);		//读取0X05的内容
	temp&=0XFFF0;
	temp|=vol&0X0F;				   		 
	RDA5820_WR_Reg(0X05,temp) ;		//设置音量	
}
//静音设置
//mute:0,不静音;1,静音
void RDA5820_Mute_Set(u8 mute)
{
	u16 temp;
	temp=RDA5820_RD_Reg(0X02);		//读取0X02的内容
	if(mute)temp|=1<<14;
	else temp&=~(1<<14);	    		   		 
	RDA5820_WR_Reg(0X02,temp) ;		//设置MUTE	
}
//设置灵敏度
//rssi:0~127;
void RDA5820_Rssi_Set(u8 rssi)
{
	u16 temp;
	temp=RDA5820_RD_Reg(0X05);		//读取0X05的内容
	temp&=0X80FF;
	temp|=(u16)rssi<<8;				   		 
	RDA5820_WR_Reg(0X05,temp) ;		//设置RSSI	
}
//设置TX发送功率
//gain:0~63
void RDA5820_TxPAG_Set(u8 gain)
{
	u16 temp;
	temp=RDA5820_RD_Reg(0X42);		//读取0X42的内容
	temp&=0XFFC0;
	temp|=gain;				   		//GAIN
	RDA5820_WR_Reg(0X42,temp) ;		//设置PA的功率
}
//设置TX 输入信号增益
//gain:0~7
void RDA5820_TxPGA_Set(u8 gain)
{
	u16 temp;
	temp=RDA5820_RD_Reg(0X42);		//读取0X42的内容
	temp&=0XF8FF;
	temp|=gain<<8;			    	//GAIN
	RDA5820_WR_Reg(0X42,temp) ;		//设置PGA
}
//设置RDA5820的工作频段
//band:0,87~108Mhz;1,76~91Mhz;2,76~108Mhz;3,用户自定义(53H~54H)
void RDA5820_Band_Set(u8 band)
{
	u16 temp;
	temp=RDA5820_RD_Reg(0X03);		//读取0X03的内容
	temp&=0XFFF3;
	temp|=band<<2;			     
	RDA5820_WR_Reg(0X03,temp) ;		//设置BAND
}
//设置RDA5820的步进频率
//band:0,100Khz;1,200Khz;3,50Khz;3,保留
void RDA5820_Space_Set(u8 spc)
{
	u16 temp;
	temp=RDA5820_RD_Reg(0X03);		//读取0X03的内容
	temp&=0XFFFC;
	temp|=spc;			     
	RDA5820_WR_Reg(0X03,temp) ;		//设置BAND
}
//设置RDA5820的频率
//freq:频率值(单位为10Khz),比如10805,表示108.05Mhz
void RDA5820_Freq_Set(u16 freq)
{
	u16 temp;
	u8 spc=0,band=0;
	u16 fbtm,chan;
	temp=RDA5820_RD_Reg(0X03);	//读取0X03的内容
	temp&=0X001F;
	band=(temp>>2)&0x03;		//得到频带
	spc=temp&0x03; 				//得到分辨率
	if(spc==0)spc=10;
	else if(spc==1)spc=20;
	else spc=5;
	if(band==0)fbtm=8700;
	else if(band==1||band==2)fbtm=7600;
	else 
	{
		fbtm=RDA5820_RD_Reg(0X53);//得到bottom频率
		fbtm*=10;
	}
	if(freq<fbtm)return;
	chan=(freq-fbtm)/spc;		//得到CHAN应该写入的值
	chan&=0X3FF;				//取低10位	  
	temp|=chan<<6;
	temp|=1<<4;					//TONE ENABLE			     
	RDA5820_WR_Reg(0X03,temp) ;	//设置频率
	delay_ms(20);				//等待20ms
	while((RDA5820_RD_Reg(0X0B)&(1<<7))==0);//等待FM_READY
	RDA5820_Freq_Get();
}
//得到当前频率
//返回值:频率值(单位10Khz)
u16 RDA5820_Freq_Get(void)
{
	u16 temp;
	u8 spc=0,band=0;
	u16 fbtm,chan;
	temp=RDA5820_RD_Reg(0X03);		//读取0X03的内容
	chan=temp>>6;   
	band=(temp>>2)&0x03;		//得到频带
	spc=temp&0x03; 				//得到分辨率
	if(spc==0)spc=10;
	else if(spc==1)spc=20;
	else spc=5;
	if(band==0)fbtm=8700;
	else if(band==1||band==2)fbtm=7600;
	else 
	{
		fbtm=RDA5820_RD_Reg(0X53);//得到bottom频率
		fbtm*=10;
	}
 	temp=fbtm+chan*spc;				 
	return temp;//返回频率值
}
void RdaSeek(void)
{
	int32_t temp=0X0000;
	int32_t count=0x0000;
	temp=RDA5820_RD_Reg(RDA5820_R02); //读取0X03的内容
	temp|=0X0100;
	RDA5820_WR_Reg(RDA5820_R02,temp) ; //开始搜台
	do
	{
		delay_ms(20);
		temp = RDA5820_RD_Reg(RDA5820_R02);//读取搜台结果
		temp&= 0x0100;
		count++;
    if(count > 5000)return;
  }while(temp == 0x0100);
}

////按键控制FM的大小
//void keycanFM()
//{
//	int key;
//	key=KEY_Scan(0);			  
//	if(key==1)
//	{		 
//		
//		FM=RDA5820_Freq_Get();
//	  delay_ms(10);
//		FM+=10;
//		if(FM>10800)FM=7600;
//		RDA5820_Freq_Set(FM);
//		delay_ms(10);
//	  //设置FM+值	
//	}
//	if(key==2)	
//	{
//		FM=RDA5820_Freq_Get();
//		delay_ms(10);
//		FM-=10;
//		if(FM<7600)FM=10800;
//		RDA5820_Freq_Set(FM);
//		delay_ms(10);
//		//设置FM-值
//	}
//	if(key==3)
//	{		 
//		
////		FM=RDA5820_1_Freq_Get();
//	  delay_ms(10);
//		FM+=10;
//		if(FM>10800)FM=7600;
////		RDA5820_1_Freq_Set(FM);
//		delay_ms(10);
//	  //设置FM+值	
//	}
//	if(key==4)	
//	{
////		FM=RDA5820_1_Freq_Get();
//		delay_ms(10);
//		FM-=10;
//		if(FM<7600)FM=10800;
////		RDA5820_1_Freq_Set(FM);
//		delay_ms(10);
//		//设置FM-值
//	}
//	if(key==5)	
//	{
////		play();
////		RdaSeek();
////		RdaSeek_1();
//	}
//}
