/**********************************************************										 
���ܣ�stm32f103rct6���ƣ�25MHzʱ�ӣ� AD9833���Ҳ���Ƶ�������Χ0-10M(�ɱ༭0-100M)��
			������0-500K�����ǲ���0-1M��ɨƵĬ�����Ҳ� ��
			��ʾ��12864cog
�ӿڣ����ƽӿ������AD9833.h  �����ӿ������key.h
**********************************************************/

#include "Communication.h"
void AD983_GPIO_Init() 
{

    GPIO_InitTypeDef GPIO_InitStructure ; 
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PB,PE�˿�ʱ��

//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5;   // PA.3 PA.4 PA.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4| GPIO_Pin_11| GPIO_Pin_12; 

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz ; 

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ; 

    GPIO_Init(GPIOC ,&GPIO_InitStructure) ; 
} 
/***************************************************************************//**
 * @brief Initializes the SPI communication peripheral.
 *
 * @param lsbFirst - Transfer format (0 or 1).
 *                   Example: 0x0 - MSB first.
 *                            0x1 - LSB first.
 * @param clockFreq - SPI clock frequency (Hz).
 *                    Example: 1000 - SPI clock frequency is 1 kHz.
 * @param clockPol - SPI clock polarity (0 or 1).
 *                   Example: 0x0 - idle state for SPI clock is low.
 *	                          0x1 - idle state for SPI clock is high.
 * @param clockPha - SPI clock phase (0 or 1).
 *                   Example: 0x0 - data is latched on the leading edge of SPI
 *                                  clock and data changes on trailing edge.
 *                            0x1 - data is latched on the trailing edge of SPI
 *                                  clock and data changes on the leading edge.
 *
 * @return 0 - Initialization failed, 1 - Initialization succeeded.
*******************************************************************************/
unsigned char AD9833_SPI_Init(unsigned char lsbFirst,
                       unsigned long clockFreq,
                       unsigned char clockPol,
                       unsigned char clockPha)
{
						AD983_GPIO_Init();
	return SUCCESS;
}

/***************************************************************************//**
 * @brief Writes data to SPI.
 *
 * @param data - Write data buffer:
 *               - first byte is the chip select number;
 *               - from the second byte onwards are located data bytes to write.
 * @param bytesNumber - Number of bytes to write.
 *
 * @return Number of written bytes.
*******************************************************************************/
unsigned char AD9833_SPI_Write(unsigned char* data,
                        unsigned char bytesNumber)
{
    unsigned char i,j; 
	unsigned char writeData[5]	= {0,0, 0, 0, 0};
	
    AD9833_SCLK=1; 
    AD9833_FSYNC=0 ; 

	for(i = 0;i < bytesNumber;i ++)
	{
		writeData[i] = data[i + 1];
	}
	
	for(i=0 ;i<bytesNumber ;i++) 
	{
    for(j=0 ;j<8 ;j++)      
		{ 
        if(writeData[i] & 0x80) 
          AD9833_SDATA=1; 
        else 
          AD9833_SDATA=0; 

        AD9833_SCLK=0; 
        writeData[i] <<= 1; 
        AD9833_SCLK=1; 
    } 
	}
    AD9833_SDATA=1; 
    AD9833_FSYNC=1; 
		
		return i;
}

/***************************************************************************//**
 * @brief Reads data from SPI.
 *
 * @param data - As an input parameter, data represents the write buffer:
 *               - first byte is the chip select number;
 *               - from the second byte onwards are located data bytes to write.
 *               As an output parameter, data represents the read buffer:
 *               - from the first byte onwards are located the read data bytes. 
 * @param bytesNumber - Number of bytes to write.
 *
 * @return Number of written bytes.
*******************************************************************************/
unsigned char AD9833_SPI_Read(unsigned char* data,
                       unsigned char bytesNumber)
{
	return bytesNumber;
}

