#include "common.h"
#include "include.h"
#include "init.h"
#include "Deal_Data.h"
#include "Function.h"
#include "PID.h"
extern int Error_Now;  
extern int Turn_Flag;
extern int Turn_Flag1;
extern int Mid;//�ű�����
extern int Stop_Flag;
extern int Encoder;
extern int Max;
extern int deng;
extern uint32 sudu;

float u=0;//��������ֵ
float u1=0;//�ϴ������ֵ

float Kp=3.5;//����ϵ��  //3.5
float Ki=0.5;//����ϵ��
float Kd=0;//΢��ϵ��

float ek=0;//��ǰ���
float ek1=0 ;//��һ�����
float ek2=0;//���������

void motorpid(float sv,float fv)//sv�����ٶ� fv��ʵ�ٶ�
{
    float pError=0,iError=0,dError=0; 
    ek=sv-fv;
    pError=ek-ek1;
    iError=ek;
    dError=ek-ek1*2+ek2;
    ek2=ek1;
    ek1=ek;
    u=Kp*pError+Ki*iError+Kd*dError+u1;
    u1=u;
}

void advance() //ȫ������  ��pid
{   
  motorpid(5000,(float)sudu);
    if(u<0.0)
      u=0.0;
    if(u>5000.0)
    {
      u=5000.0;
    }
    tpm_pwm_duty(TPM1, TPM_CH0,0);  
    tpm_pwm_duty(TPM1, TPM_CH1,(int32)u);
}

void advance1() //��ǰ����   ��pid
{
    motorpid(4000,(float)sudu);
     if(u<0.0)
     {
       u=13.0;  //18
       tpm_pwm_duty(TPM1, TPM_CH0,0);  
       tpm_pwm_duty(TPM1,TPM_CH1,(int32)u);
      
     }
    else if(u>65)
    {
      u=65;
      tpm_pwm_duty(TPM1,TPM_CH1,0);
      tpm_pwm_duty(TPM1,TPM_CH0,(int32)u);  
      
    }
    else
    {
      tpm_pwm_duty(TPM1, TPM_CH1,0);
      tpm_pwm_duty(TPM1, TPM_CH0,(int32)u);  
      
    }
     //OLED_Print_Num(83,6,(uint32)u);
}

void Servo_PID()
{
	int Servo_P ;
	Servo_P =4;//Get_Switch();//5
	int Turn_Value;
	Turn_Value=Servo_Mid+(-1)*Error_Now*Servo_P;
	
	
	/******************���ƶ����Ƿ���******************************/
	Turn_Value=(Turn_Value >Servo_Left)?Servo_Left:Turn_Value;
	Turn_Value=(Turn_Value <Servo_Right)?Servo_Right:Turn_Value;
	/****************************************************************/
	tpm_pwm_duty(TPM2, TPM_CH1,Turn_Value);
}
void Turn_Max(int flag)			//flag=1 ���Ҵ�����  flag=0 ���������
{
	if(flag)   					//���Ҵ�����
	{
		tpm_pwm_duty(TPM2, TPM_CH1,Servo_Right);
	}
	else       					//��z�������
	{
		tpm_pwm_duty(TPM2, TPM_CH1,Servo_Left);
	}
}
void Stop()						//ͣ��
{
	DisableInterrupts;
	Back();
	DELAY_MS(200);
	tpm_pwm_duty(TPM1, TPM_CH1,0);
	tpm_pwm_duty(TPM1, TPM_CH0,0);
	gpio_set (BEEP, 0);
	Stop_Flag=1;
	while(1);
}
void Back()						//���� 
{
	tpm_pwm_duty(TPM1, TPM_CH1,0);
	tpm_pwm_duty(TPM1, TPM_CH0,5000);
}

void Advance()					//ǰ��
{
        tpm_pwm_duty(TPM1, TPM_CH0,0);
	tpm_pwm_duty(TPM1, TPM_CH1,7000);
}
void drifting(int Turnflag)					//Ư��
{
  	Turn_Max(Turnflag);
	Back();		//Ư�� ����
        //DELAY_MS(4);
}
void drifting1(int Turnflag)					//Ư��
{
	Turn_Max(Turnflag);
        Advance(); 
}
void Spin()						//��ת Ѱ���ű�
{
	Turn_Max(Turn_Flag);
        tpm_pwm_duty(TPM1, TPM_CH0,0);
	tpm_pwm_duty(TPM1, TPM_CH1,5000);
}

void Rush_Beacon()				//ײ���ű�
{
	//Back();						//ȫ�ٵ���
	tpm_pwm_duty(TPM1, TPM_CH1,0);
	tpm_pwm_duty(TPM1, TPM_CH0,10000);
	if(Mid<CAMERA_W/2)			//Ŀ���ű������
	{
		Turn_Max(1);			//���Ҵ����� �ܿ��ű�
	}
	else 						//Ŀ���ű����ұ�
	{
		Turn_Max(0);			//����������� �ܿ��ű�
	}
	DELAY_MS(Back_Delay_Time);
	if(Mid<CAMERA_W/2)			//Ŀ���ű������
	{
	 	Turn_Max(0);			//���Ҵ����� �ܿ��ű�
	}
	else 						//Ŀ���ű����ұ�
	{
		Turn_Max(1);			//����������� �ܿ��ű�
	}
	Advance();
	DELAY_MS(Back_Delay_Time);
	pit_time_start  (PIT1);
	//ǰ�� 
}

