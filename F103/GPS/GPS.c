#include "GPS.h"
#include <stdio.h>
#include "usart3.h"
#include "string.h"
#include "stdlib.h"

extern u8 USART3_RX_BUF[USART3_REC_LEN];
_SaveData Save_Data;
double deg_lat,deg_lon;

void clrStruct(void)
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	memset(Save_Data.GPS_Buffer, 0, GPS_Buffer_Length);      //清空
	memset(Save_Data.UTCTime, 0, UTCTime_Length);
	memset(Save_Data.latitude, 0, latitude_Length);
	//ADD
	memcpy(Save_Data.latitude,"30.782241",sizeof(Save_Data.latitude));
	memset(Save_Data.N_S, 0, N_S_Length);
	memset(Save_Data.longitude, 0, longitude_Length);
	//add
	memcpy(Save_Data.longitude,"103.866675",sizeof(Save_Data.longitude));
	memset(Save_Data.E_W, 0, E_W_Length);
	
}


void parseGpsBuffer(void)
{
	char *subString;
	char *subStringNext;
	char i = 0;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		printf("**************\r\n");
		printf("%s\r\n",Save_Data.GPS_Buffer);

		for (i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取经度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//解析错误
				}
			}


		}
	}
}

double nmea_ndeg2degree(double val)
{
    double deg = ((int)(val / 100));
    val = deg + (val - deg * 100) / 60;
    return val;
}

void printGpsBuffer(void)
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		
		printf("Save_Data.UTCTime = ");
		printf("%s\r\n",Save_Data.UTCTime);
		printf("\r\n");

		if(Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
			deg_lat = atof(Save_Data.latitude);
			deg_lat = nmea_ndeg2degree(deg_lat);
			deg_lon = atof(Save_Data.longitude);
		  deg_lon = nmea_ndeg2degree(deg_lon);
			
			printf("deg_lat: %f , deg_lon: %f\r\n",deg_lat,deg_lon);
//			printf("Save_Data.latitude = ");
//			printf("%s\r\n",Save_Data.latitude);
//			printf("\r\n");


//			printf("Save_Data.N_S = ");
//			printf("%s\r\n",Save_Data.N_S);
//			printf("\r\n");

//			printf("Save_Data.longitude = ");
//			printf("%s\r\n",Save_Data.longitude);
//			printf("\r\n");

//			printf("Save_Data.E_W = ");
//			printf("%s\r\n",Save_Data.E_W);
//			printf("\r\n");
		}
		else
		{
			printf("GPS DATA is not usefull!\r\n");
		}
		
	}
}


void errorLog(int num)
{
	while (1)
	{
	  	printf("ERROR%d\r\n",num);
	}
}


void GPS_Init(void)
{
	USART3_Init(GPS_USART_BAUDRATE);
}

