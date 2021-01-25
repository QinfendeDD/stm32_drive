#include "Con_Phoenix.h"



/**************************************************************************
�������ܣ�����ֵ����
**************************************************************************/
void PWM_abs(int *number)
{ 		   
	if(*number<0)	*number=	-	*number;  
	else 					*number=	 	*number;
}

/**************************************************************************
	�������ܣ�����PWM��ֵ
		������Ϊ2.5ms��������Ӧ2.5ms��Ӧ��pwm
		�����СΪ0.5ms������С��Ӧ0.5ms��Ӧ��pwm
**************************************************************************/
void Xianfu_Pwm(int *PWM_value)
{	
	int pwm_max=2200,pwm_min=500;//2.5ms��ӦPWM������2300 ������2200
	if(*PWM_value<pwm_min) 	*PWM_value=pwm_min;	
	if(*PWM_value>pwm_max)  *PWM_value=pwm_max;		
}

/**************************************************************************
�������ܣ�λ��ʽPID������
��ڲ���������λ�ã�Ŀ��λ��
����  ֵ�����PWM
����λ��ʽ��ɢPID��ʽ 
pwm=Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  
��e(k)����e(k)�Լ�֮ǰ��ƫ����ۻ���;����kΪ1,2,,k;
pwm�������


���ȥ���ǣ����� ����ƫ��õ��� Ӧ������� PWMֵ��
						���������Ϊ��һ������ֵ
						���ص��ϴ������ͣ���ڵ�����ֵ��
**************************************************************************/
int Position_PID(int Feedback_value,int User_Target)
{ 	
	static float Bias,PWM_Out,Integral_bias,Last_Bias,Differntial_bias;

	Bias=User_Target-Feedback_value;             					//����ƫ��
	Integral_bias+=Bias;	                       					//���ƫ��Ļ���
	Differntial_bias=Bias-Last_Bias;
	
	PWM_Out=
					PID.Position_KP		*	Bias
					+PID.Position_KI	*	PID.KI_magnify	*	Integral_bias			
					+PID.Position_KD	*	PID.KD_magnify	*	Differntial_bias; //λ��ʽPID��������ʽ
	Last_Bias=Bias;                             					//������һ��ƫ�� 
	return PWM_Out;                             					//λ��ʽPWM���
}


/**************************************************************************
�������ܣ�����PI������
��ڲ���������λ�ã�Ŀ��λ��
����  ֵ�����PWM
��������ʽ��ɢPID��ʽ 
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)������ƫ�� 
e(k-1)������һ�ε�ƫ��  �Դ����� 
pwm�����������
PI������
pwm+=Kp[e��k��-e(k-1)]+Ki*e(k)
**************************************************************************/
int Increment_PI(int Feedback_value,int User_Target)
{ 	
	 static float Bias,PWM_Out,Last_Bias;
	 Bias=User_Target-Feedback_value;                           				//����ƫ��
	 PWM_Out+=PID.Increment_KP*(Bias-Last_Bias)+PID.Increment_KI*Bias;  //����ʽPI������
	 Last_Bias=Bias;	                                     							//������һ��ƫ�� 
	 return PWM_Out;                                           					//�������
}













