#ifndef __GPS_H_
#define __GPS_H_
#include "sys.h"
#include "stm32f10x.h"


#define false 0
#define true 1

//GPS���ڲ�����
#define GPS_USART_BAUDRATE                    9600


//�������鳤��
#define GPS_Buffer_Length 80
#define UTCTime_Length 11
#define latitude_Length 11
#define N_S_Length 2
#define longitude_Length 12
#define E_W_Length 2 


typedef struct SaveData 
{
	char GPS_Buffer[GPS_Buffer_Length];
	char isGetData;		//�Ƿ��ȡ��GPS����
	char isParseData;	//�Ƿ�������
	char UTCTime[UTCTime_Length];		//UTCʱ��
	char latitude[latitude_Length];		//γ��
	char N_S[N_S_Length];		//N/S
	char longitude[longitude_Length];		//����
	char E_W[E_W_Length];		//E/W
	char isUsefull;		//��λ��Ϣ�Ƿ���Ч
}_SaveData;



void clrStruct(void);
void printGpsBuffer(void);
void parseGpsBuffer(void);
void GPS_Init(void);
void errorLog(int num);
double nmea_ndeg2degree(double val);

#endif

