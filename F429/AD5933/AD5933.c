#include "AD5933.h"
#include "DELAY.H"
#include "math.h"

uint8_t AD5933PGA=INPUT_MUL_1;
uint8_t AD5933XTAL=USE_INTERNAL_XTAL;
uint8_t AD5933Vpp=OUTPUT_200_mVPP;

uint8_t AD5933IICWrite(uint8_t Reg,uint8_t *Data,uint8_t ByteNumber)
{
	if(I2CWriteBytes(AD5933_ADDR,Reg,Data,ByteNumber))
		return 1;
	else
		return 0;
}
uint8_t AD5933IICRead(uint8_t Reg,uint8_t *Data,uint8_t Number)
{
	if(I2CReadBytes(AD5933_ADDR,Reg,Data,Number))
		return 1;
	else
		return 0;
}

uint8_t AD5933SetStartFreq(uint32_t Freq)		//AD5933设置起始频率
{
	uint8_t DataTemp[3];
	Freq=START_FREQ(Freq);
	DataTemp[0]=(uint8_t)(Freq>>16);
	DataTemp[1]=(uint8_t)(Freq>>8);
	DataTemp[2]=(uint8_t)Freq;
	
	if(AD5933IICWrite(START_FREQ_REG_H8,DataTemp,1))
		return 1;
	if(AD5933IICWrite(START_FREQ_REG_M8,DataTemp+1,1))
		return 1;
	if(AD5933IICWrite(START_FREQ_REG_L8,DataTemp+2,1))
		return 1;
	return 0;
}
uint8_t AD5933SetStepFreq(uint32_t Freq)		//AD5933设置步进频率
{
	uint8_t DataTemp[3];
	Freq=START_FREQ(Freq);
	DataTemp[0]=(uint8_t)(Freq>>16);
	DataTemp[1]=(uint8_t)(Freq>>8);
	DataTemp[2]=(uint8_t)Freq;
	
	if(AD5933IICWrite(FREQ_STEP_REG_H8,DataTemp,1))
		return 1;
	if(AD5933IICWrite(FREQ_STEP_REG_M8,DataTemp+1,1))
		return 1;
	if(AD5933IICWrite(FREQ_STEP_REG_L8,DataTemp+2,1))
		return 1;
	return 0;
}
uint8_t AD5933SetStepNumber(uint16_t Number)		//AD5933设置步进次数
{
	uint8_t DataTemp[2];
	DataTemp[0]=(uint8_t)(Number>>8);
	DataTemp[1]=(uint8_t)Number;
	if(AD5933IICWrite(FREQ_STEP_NUMBER_REG_H8,DataTemp,1))
		return 1;
	if(AD5933IICWrite(FREQ_STEP_NUMBER_REG_L8,DataTemp+1,1))
		return 1;
	return 0;
}
uint8_t AD5933SetStableCycle(uint16_t Cycle)		//设置建立时间周期
{
	uint8_t DataTemp[2];
	uint16_t Temp;
	Temp=STABLE_TIMES_CYCLE_MUL_1|STABLE_TIMES_CYCLE(Cycle);
	DataTemp[0]=(uint8_t)(Temp>>8);
	DataTemp[1]=(uint8_t)Temp;
	if(AD5933IICWrite(STABLE_TIMES_CYCLE_REG_H8,DataTemp,1))
		return 1;
	if(AD5933IICWrite(STABLE_TIMES_CYCLE_REG_L8,DataTemp+1,1))
		return 1;
	return 0;
}
uint8_t ResetAD5933()		//复位
{
	uint8_t DataTemp;
	DataTemp=RESET|AD5933XTAL;
	if(AD5933IICWrite(CONTROL_REG_L8,&DataTemp,1))
		return 1;
	return 0;
}
uint8_t AD5933StartInitCommand()		//开始以起始频率初始化
{
	uint8_t DataTemp;
	DataTemp=INIT_WITH_START_FREQ|AD5933Vpp|AD5933PGA;
	if(AD5933IICWrite(CONTROL_REG_H8,&DataTemp,1))
		return 1;
	return 0;
}
uint8_t AD5933StartMeasure()		//开始测量
{
	uint8_t DataTemp;
	DataTemp=START_SCAN|AD5933PGA|AD5933Vpp;
	if(AD5933IICWrite(CONTROL_REG_H8,&DataTemp,1))
		return 1;
	return 0;
}
uint8_t CheckAD5933DFTFinish()		//检查是否已经完成计算，返回2为完成计算
{
	uint8_t DataTemp;
	if(AD5933IICRead(STATUS_REG,&DataTemp,1))
		return 1;
	else{
		if(DataTemp&FINISH_DFT)
			return 2;
		return 0;
	}
}
uint8_t EnterStanbyMode(uint8_t Vpp,uint8_t PGA,uint8_t Clock)
{
	uint8_t DataTemp;
	AD5933XTAL=Clock;
	AD5933Vpp=Vpp;
	AD5933PGA=PGA;
	DataTemp=WATING_MODE|AD5933Vpp|AD5933PGA;
	if(AD5933IICWrite(CONTROL_REG_H8,&DataTemp,1))
		return 1;
	DataTemp=AD5933XTAL;
	if(AD5933IICWrite(CONTROL_REG_L8,&DataTemp,1))
		return 1;
	return 0;
}
uint8_t EnterLowPowerMode()
{
	uint8_t DataTemp;
	DataTemp=LOW_POWER_MODE|AD5933Vpp|AD5933PGA;
	if(AD5933IICWrite(CONTROL_REG_H8,&DataTemp,1))
		return 1;
	return 0;
}
uint8_t AD5933ReadData(uint16_t *RealPart,uint16_t *ImagPart)
{
	uint8_t DataTemp[2];
	if(AD5933IICRead(REAL_DATA_REG_H8,DataTemp,1))
		return 1;
	if(AD5933IICRead(REAL_DATA_REG_L8,DataTemp+1,1))
		return 1;
	(*RealPart)=DataTemp[0];
	(*RealPart)<<=8;
	(*RealPart)+=DataTemp[1];
	if(AD5933IICRead(IMAGINARY_DATA_REG_H8,DataTemp,1))
		return 1;
	if(AD5933IICRead(IMAGINARY_DATA_REG_L8,DataTemp+1,1))
		return 1;
	(*ImagPart)=DataTemp[0];
	(*ImagPart)<<=8;
	(*ImagPart)+=DataTemp[1];
	return 0;
}
double FullMeasure()		//一次完整的赋值测量
{
	uint16_t RealPart,ImagPart;
	double Amp,Temp;
	AD5933SetStartFreq(1000);
	AD5933SetStepFreq(0);
	AD5933SetStepNumber(0);
	AD5933SetStableCycle(5);
	EnterStanbyMode(OUTPUT_1_VPP,INPUT_MUL_1,USE_EXTERNAL_XTAL);
	AD5933StartInitCommand();
	AD5933StartMeasure();
	while(CheckAD5933DFTFinish()!=2);
	AD5933ReadData(&RealPart,&ImagPart);
	Temp=RealPart*1.0*RealPart+ImagPart*1.0*ImagPart;
	Amp=sqrt(Temp);
	//R=1.0/AD5933_GAIN/Amp;
	EnterLowPowerMode();
	//return R;
	return Amp;
}
double ContinueMeasure()		//连续的测量，不确定可用性
{
	static uint8_t First=1;
	uint16_t RealPart,ImagPart;
	double Amp,Temp,R;
	if(First==1){
		First=0;
		AD5933SetStartFreq(1000);
		AD5933SetStepFreq(0);
		AD5933SetStepNumber(0);
		AD5933SetStableCycle(10);
		//AD5933StartInitCommand(OUTPUT_200_mVPP,INPUT_MUL_1);
	}
	AD5933StartMeasure();
	while(CheckAD5933DFTFinish()!=2);
	AD5933ReadData(&RealPart,&ImagPart);
	Temp=(int16_t)RealPart*1.0*(int16_t)RealPart+(int16_t)ImagPart*1.0*(int16_t)ImagPart;
	Amp=sqrt(Temp);
	R=1.0/AD5933_GAIN/Amp;
	return R;
}
double GetAD5933Gain(double KnownR,double Amp)
{
	return 1.0/KnownR/Amp;
}
