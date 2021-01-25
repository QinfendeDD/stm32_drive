/************************************************************************************
* Description:128*64点阵的OLED显示屏驱动文件，仅适用于惠特自动化(heltec.taobao.com)的SD1306驱动IIC通信方式显示屏
* Function List:
*	1. void I2C_Configuration(void) -- 配置CPU的硬件I2C
* 2. void I2C_WriteByte(uint8_t addr,uint8_t data) -- 向寄存器地址写一个byte的数据
* 3. void WriteCmd(unsigned char I2C_Command) -- 写命令
* 4. void WriteDat(unsigned char I2C_Data) -- 写数据
* 5. void OLED_Init(void) -- OLED屏初始化
* 6. void OLED_SetPos(unsigned char x, unsigned char y) -- 设置起始点坐标
* 7. void OLED_Fill(unsigned char fill_Data) -- 全屏填充
* 8. void OLED_CLS(void) -- 清屏
* 9. void OLED_ON(void) -- 唤醒
* 10. void OLED_OFF(void) -- 睡眠
* 11. void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char TextSize) -- 显示字符串(字体大小有6*8和8*16两种)
* 12. void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N) -- 显示中文(中文需要先取模，然后放到codetab.h中)
* 13. void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]) -- BMP图片
*
*************************************************************************************/

#include "oled.h"
//#include "led.h"
#include "sys.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
//IO配置
void OLED_Init_iic(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOG,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

 
//复位信号
void res_OLED(void)
{
  RES_OLED=0;
	delay_ms(50);
	RES_OLED=1;
	delay_ms(50);
	
}
//iic起始信号
void IIC_Start_OLED(void)
{
    SDA_OUT_OLED();
    IIC_SDA_OLED=1;
    IIC_SCL_OLED=1;
    delay_us(4);
    IIC_SDA_OLED=0;
    delay_us(4);
    IIC_SCL_OLED=0;
}
//iic结束信号
void IIC_Stop_OLED(void)
{
	  SDA_OUT_OLED();
    IIC_SCL_OLED=0;
    IIC_SDA_OLED=0;
    delay_us(4);
	  IIC_SCL_OLED=1;
    IIC_SDA_OLED=1;
    delay_us(4);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack_OLED(void)
{
	u8 ucErrTime=0;
	SDA_IN_OLED();      //SDA设置为输入  
	IIC_SDA_OLED=1;delay_us(1);	   
	IIC_SCL_OLED=1;delay_us(1);	 
	while(READ_SDA_OLED)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop_OLED();
			return 1;
		}
	}
	IIC_SCL_OLED=0;//时钟输出0 	   
	return 0;  
} 
			 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_WriteByte_OLED(u8 txd)
{                        
    u8 t;   
	 SDA_OUT_OLED(); 	    
    IIC_SCL_OLED=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        IIC_SDA_OLED=(txd&0x80)>>7;
        txd<<=1; 	  
				delay_us(2);   //对TEA5767这三个延时都是必须的
				IIC_SCL_OLED=1;
				delay_us(2); 
				IIC_SCL_OLED=0;	
				delay_us(2);
    }	 
} 	    
//iic写命令
void WriteCmd_OLED(u8 command)
{
    IIC_Start_OLED();
    IIC_WriteByte_OLED(0x78);
	  IIC_Wait_Ack_OLED();
    IIC_WriteByte_OLED(0x00);
	  IIC_Wait_Ack_OLED();	
    IIC_WriteByte_OLED(command);
	  IIC_Wait_Ack_OLED();	
    IIC_Stop_OLED();
}
//iic写数据
void WriteDat_OLED(u8 data)
{
    IIC_Start_OLED();
    IIC_WriteByte_OLED(0x78);
	  IIC_Wait_Ack_OLED();
    IIC_WriteByte_OLED(0x40);
	  IIC_Wait_Ack_OLED();
    IIC_WriteByte_OLED(data);
	  IIC_Wait_Ack_OLED();	
    IIC_Stop_OLED();
}
//OLED写函数，在总线上往OLED写数据或写命令  cmd:1:写数据 0：写命令
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{
   WriteDat_OLED(dat);
	}
	else 
	{
   WriteCmd_OLED(dat);	
	}
}
//坐标设置
void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f)|0x01,OLED_CMD); 
} 
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}
//填充全屏,整个屏幕是白色的
void OLED_Fill(unsigned char fill_Data)
{
    unsigned char m,n;
    for(m=0;m<8;m++)
    {
        WriteCmd_OLED(0xb0+m);       
        WriteCmd_OLED(0x00);    
        WriteCmd_OLED(0x10);
        for(n=0;n<130;n++)
            {
                WriteDat_OLED(fill_Data);
            }
    }
}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_CLS(void)//清屏
{
	OLED_Fill(0x00);
}
//显示图片
//x0,y0 -- 起始点坐标(x0:0~127, y0:0~7); x1,y1 -- 起点对角线(结束点)的坐标(x1:1~128,y1:1~8)
void OLED_DrawBMP(u8 x0,u8 y0,u8 x1,u8 y1,u8 BMP[])
{
	unsigned int j=0;
	unsigned char x,y;

  if(y1%8==0)
		y = y1/8;
  else
		y = y1/8 + 1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
		{
			WriteDat_OLED(BMP[j++]);
		}
	}
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-32;//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}//如果超出了128列就从0列开始并换到下一页
		if(Char_Size ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			OLED_WR_Byte(F8X16[c*16+i+8],OLED_DATA);
			}
		if(Char_Size ==12)
			{
				unsigned char wm=0;
				OLED_Set_Pos(x,y);
				for(wm=0;wm<8;wm++)
				OLED_WR_Byte(F81X16[chr*16+wm],OLED_DATA);
				OLED_Set_Pos(x,y+1);
				for(wm=0;wm<8;wm++)
				OLED_WR_Byte(F81X16[chr*16+wm+8],OLED_DATA);
//			OLED_Set_Pos(x,y);	
//			for(i=0;i<8;i++)
//			OLED_WR_Byte(F81X16[c*16+i],OLED_DATA);
//			OLED_Set_Pos(x,y+1);
//			for(i=0;i<8;i++)
//			OLED_WR_Byte(F81X16[c*16+i+8],OLED_DATA);
			}
			else {
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				OLED_WR_Byte(F6x8[c][i],OLED_DATA);
			}
}
void OLED_ShowSZ(unsigned char x, unsigned char y, unsigned char Nn)
{
	unsigned char wm=0;
	OLED_Set_Pos(x,y);
	for(wm=0;wm<8;wm++)
	WriteDat_OLED(F8X16[Nn*16+wm]);
	OLED_Set_Pos(x,y+1);
	for(wm=0;wm<8;wm++)
	WriteDat_OLED(F8X16[Nn*16+wm+8]);
}
//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
}
//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 chr[],u8 TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(chr[j] != '\0')
			{
				c = chr[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
					WriteDat_OLED(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(chr[j] != '\0')
			{
				c = chr[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_Set_Pos(x,y);
				for(i=0;i<8;i++)
					WriteDat_OLED(F8X16[c*16+i]);
				OLED_Set_Pos(x,y+1);
				for(i=0;i<8;i++)
				WriteDat_OLED(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}
//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 N)
{      			    
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_Set_Pos(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat_OLED(F16x16[adder]);
		adder += 1;
	}
	OLED_Set_Pos(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteDat_OLED(F16x16[adder]);
		adder += 1;
	}				
}
//OLED初始化
void OLED_Init(void)
{
	  OLED_Init_iic();
	  //res();
    delay_ms(100); //????????
    WriteCmd_OLED(0xAE); //display off
    WriteCmd_OLED(0x20); //Set Memory Addressing Mode    
    WriteCmd_OLED(0x10); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
    WriteCmd_OLED(0xb0); //Set Page Start Address for Page Addressing Mode,0-7
    WriteCmd_OLED(0xc8); //Set COM Output Scan Direction
    WriteCmd_OLED(0x00); //---set low column address
    WriteCmd_OLED(0x10); //---set high column address
    WriteCmd_OLED(0x40); //--set start line address
    WriteCmd_OLED(0x81); //--set contrast control register
    WriteCmd_OLED(0xff); //???? 0x00~0xff
    WriteCmd_OLED(0xa1); //--set segment re-map 0 to 127
    WriteCmd_OLED(0xa6); //--set normal display
    WriteCmd_OLED(0xa8); //--set multiplex ratio(1 to 64)
    WriteCmd_OLED(0x3F); //
    WriteCmd_OLED(0xa4); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
    WriteCmd_OLED(0xd3); //-set display offset
    WriteCmd_OLED(0x00); //-not offset
    WriteCmd_OLED(0xd5); //--set display clock divide ratio/oscillator frequency
    WriteCmd_OLED(0xf0); //--set divide ratio
    WriteCmd_OLED(0xd9); //--set pre-charge period
    WriteCmd_OLED(0x22); //
    WriteCmd_OLED(0xda); //--set com pins hardware configuration
    WriteCmd_OLED(0x12);
    WriteCmd_OLED(0xdb); //--set vcomh
    WriteCmd_OLED(0x20); //0x20,0.77xVcc
    WriteCmd_OLED(0x8d); //--set DC-DC enable
    WriteCmd_OLED(0x14); //
    WriteCmd_OLED(0xaf); //--turn on oled panel
}
