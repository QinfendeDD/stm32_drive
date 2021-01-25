#include "Con_Phoenix.h"



/**************************************************************************
函数功能：绝对值函数
**************************************************************************/
void PWM_abs(int *number)
{ 		   
	if(*number<0)	*number=	-	*number;  
	else 					*number=	 	*number;
}

/**************************************************************************
	函数功能：限制PWM赋值
		舵机最大为2.5ms，则最大对应2.5ms对应的pwm
		舵机最小为0.5ms，则最小对应0.5ms对应的pwm
**************************************************************************/
void Xianfu_Pwm(int *PWM_value)
{	
	int pwm_max=2200,pwm_min=500;//2.5ms对应PWM满幅是2300 限制在2200
	if(*PWM_value<pwm_min) 	*PWM_value=pwm_min;	
	if(*PWM_value>pwm_max)  *PWM_value=pwm_max;		
}

/**************************************************************************
函数功能：位置式PID控制器
入口参数：反馈位置，目标位置
返回  值：舵机PWM
根据位置式离散PID公式 
pwm=Kp*e(k)+Ki*∑e(k)+Kd[e（k）-e(k-1)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  
∑e(k)代表e(k)以及之前的偏差的累积和;其中k为1,2,,k;
pwm代表输出


输出去的是：本次 基于偏差得到的 应该输出的 PWM值。
						即可以理解为是一个增量值
						加载到上次输出后停留在的脉冲值上
**************************************************************************/
int Position_PID(int Feedback_value,int User_Target)
{ 	
	static float Bias,PWM_Out,Integral_bias,Last_Bias,Differntial_bias;

	Bias=User_Target-Feedback_value;             					//计算偏差
	Integral_bias+=Bias;	                       					//求出偏差的积分
	Differntial_bias=Bias-Last_Bias;
	
	PWM_Out=
					PID.Position_KP		*	Bias
					+PID.Position_KI	*	PID.KI_magnify	*	Integral_bias			
					+PID.Position_KD	*	PID.KD_magnify	*	Differntial_bias; //位置式PID控制器公式
	Last_Bias=Bias;                             					//保存上一次偏差 
	return PWM_Out;                             					//位置式PWM输出
}


/**************************************************************************
函数功能：增量PI控制器
入口参数：反馈位置，目标位置
返回  值：舵机PWM
根据增量式离散PID公式 
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)代表本次偏差 
e(k-1)代表上一次的偏差  以此类推 
pwm代表增量输出
PI控制器
pwm+=Kp[e（k）-e(k-1)]+Ki*e(k)
**************************************************************************/
int Increment_PI(int Feedback_value,int User_Target)
{ 	
	 static float Bias,PWM_Out,Last_Bias;
	 Bias=User_Target-Feedback_value;                           				//计算偏差
	 PWM_Out+=PID.Increment_KP*(Bias-Last_Bias)+PID.Increment_KI*Bias;  //增量式PI控制器
	 Last_Bias=Bias;	                                     							//保存上一次偏差 
	 return PWM_Out;                                           					//增量输出
}













