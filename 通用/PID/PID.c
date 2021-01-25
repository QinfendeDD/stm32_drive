 /********************STMicroelectronics ********************
* File Name          : main.c
* Author             : 孙士恩
* Date First Issued  : 19-12-17
* Description        : PID
*************************************************************/

#include "pid.h"

PID sPID;               

//参数初始化
void PIDinit(void)
{
sPID.SetPoint=1375;       //设定目标 Desired value
sPID.P=20;             //比例常数 Proportional Const
sPID.I=1;              //积分常数 Integral Const
sPID.D=0.5;             //微分常数 Derivative Const
sPID.LastError=0;       //Error[K-1]
sPID.PrevError=0;       //Error[K-2]
sPID.SumError=0;
}

// pid计算
/*int PIDCalc (PID *pp,int NextPoint)
{
int dError,Error; 
Error = pp->SetPoint - NextPoint; //偏差 
pp->SumError += Error; //积分 
dError =pp->SumError-2*pp->LastError + pp->PrevError; 
//当前微分 
pp->PrevError = pp->LastError; 
pp->LastError = Error; 
return (pp->P*Error+pp->I*pp->SumError+pp->D*dError); // 微分项 
}*/
/*
int PIDCalc (PID *pp,int NextPoint)
{
int Error,dError,u;
Error=pp->SetPoint - NextPoint; //偏差
pp->SumError+=Error; //积分
dError=pp->LastError-pp->PrevError; //当前微分
u=pp->P*Error       // 比例项
      +pp->I*pp->SumError // 积分项
      +pp->D*dError;   // 微分项
pp->PrevError=pp->LastError;//Error[K-2]
pp->LastError=Error; //Error[K-1]
return (u);
}
*/

int PIDCalc (PID *pp,int NowPoint)
{
int ThisError,pError,iError,dError;
int temp;
ThisError=pp->SetPoint-NowPoint; //
pError=ThisError-pp->LastError;//Error[K]-Error[K-1]
iError=ThisError;    //Error[K]
dError=ThisError-2*(pp->LastError)+pp->PrevError; //Error[K]-2*Error[K-1]+Error[K-2]
temp=pp->P*pError+pp->I*iError+pp->D*dError;
pp->PrevError=pp->LastError;//Error[K-2]
pp->LastError=ThisError; //Error[K-1]
return (temp);
}


//---------------------------位置式PID------------------------------------
/* P, I and D parameter values
 * The K_P, K_I and K_D values (P, I and D gains)
*/
/*
#define K_P     1.00
#define K_I     0.50
#define K_D     0.10

struct PID_DATA pidData;
*/
/*! \brief Initialisation of PID controller parameters.
 *
 *  Initialise the variables used by the PID algorithm.
 *
 *  \param p_factor  Proportional term.
 *  \param i_factor  Integral term.
 *  \param d_factor  Derivate term.
 *  \param pid  Struct with PID status.
 */
 /*
void pid_Init(int16_t p_factor, int16_t i_factor, int16_t d_factor, struct PID_DATA *pid)
// Set up PID controller parameters
{
  // Start values for PID controller
  pid->sumError = 0;
  pid->lastProcessValue = 0;
  // Tuning constants for PID loop
  pid->P_Factor = p_factor;
  pid->I_Factor = i_factor;
  pid->D_Factor = d_factor;
  // Limits to avoid overflow
  pid->maxError = MAX_INT / (pid->P_Factor + 1);
  pid->maxSumError = MAX_I_TERM / (pid->I_Factor + 1);
}
 */

/*  位置式 PID control algorithm.
 *
 *  Calculates output from setpoint, process value and PID status.
 *
 *  \param setPoint  Desired value.
 *  \param processValue  Measured value.
 *  \param pid_st  PID status struct.
 */
 /*
int16_t pid_Controller(int16_t setPoint, int16_t processValue, struct PID_DATA *pid_st)
{
  int16_t error, p_term, d_term;
  int32_t i_term, ret, temp;

  error = setPoint - processValue;

  // Calculate Pterm and limit error overflow
  if (error > pid_st->maxError){
    p_term = MAX_INT;
  }
  else if (error < -pid_st->maxError){
    p_term = -MAX_INT;
  }
  else{
    p_term = pid_st->P_Factor * error;
  }

  // Calculate Iterm and limit integral runaway
  temp = pid_st->sumError + error;
  if(temp > pid_st->maxSumError){
    i_term = MAX_I_TERM;
    pid_st->sumError = pid_st->maxSumError;
  }
  else if(temp < -pid_st->maxSumError){
    i_term = -MAX_I_TERM;
    pid_st->sumError = -pid_st->maxSumError;
  }
  else{
    pid_st->sumError = temp;
    i_term = pid_st->I_Factor * pid_st->sumError;
  }

  // Calculate Dterm
  d_term = pid_st->D_Factor * (pid_st->lastProcessValue - processValue);

  pid_st->lastProcessValue = processValue;

  ret = (p_term + i_term + d_term) / SCALING_FACTOR;
  if(ret > MAX_INT){
    ret = MAX_INT;
  }
  else if(ret < -MAX_INT){
    ret = -MAX_INT;
  }

  return((int16_t)ret);
}
 */
/*! \brief Resets the integrator.
 *
 *  Calling this function will reset the integrator in the PID regulator.
 */
 /*
void pid_Reset_Integrator(pidData_t *pid_st)
{
  pid_st->sumError = 0;
}

void PIDInit(void)
{
  pid_Init(K_P * SCALING_FACTOR, K_I * SCALING_FACTOR , K_D * SCALING_FACTOR , &pidData);
}
 */
