#include "common.h"
#include "include.h"
#include "init.h"
#include "Deal_Data.h"
#include "Function.h"
#include "PID.h"
extern int Error_Now;  
extern int Turn_Flag;
extern int Turn_Flag1;
extern int Mid;//信标中心
extern int Stop_Flag;
extern int Encoder;
extern int Max;
extern int deng;
extern uint32 sudu;

float u=0;//这次输出的值
float u1=0;//上次输出的值

float Kp=3.5;//比例系数  //3.5
float Ki=0.5;//积分系数
float Kd=0;//微分系数

float ek=0;//当前误差
float ek1=0 ;//上一次误差
float ek2=0;//上两次误差

void motorpid(float sv,float fv)//sv理想速度 fv现实速度
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

void advance() //全力加速  用pid
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

void advance1() //灯前减速   用pid
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
	
	
	/******************限制舵机打角幅度******************************/
	Turn_Value=(Turn_Value >Servo_Left)?Servo_Left:Turn_Value;
	Turn_Value=(Turn_Value <Servo_Right)?Servo_Right:Turn_Value;
	/****************************************************************/
	tpm_pwm_duty(TPM2, TPM_CH1,Turn_Value);
}
void Turn_Max(int flag)			//flag=1 向右打死角  flag=0 向左打死角
{
	if(flag)   					//向右打死角
	{
		tpm_pwm_duty(TPM2, TPM_CH1,Servo_Right);
	}
	else       					//向z左打死角
	{
		tpm_pwm_duty(TPM2, TPM_CH1,Servo_Left);
	}
}
void Stop()						//停车
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
void Back()						//倒车 
{
	tpm_pwm_duty(TPM1, TPM_CH1,0);
	tpm_pwm_duty(TPM1, TPM_CH0,5000);
}

void Advance()					//前进
{
        tpm_pwm_duty(TPM1, TPM_CH0,0);
	tpm_pwm_duty(TPM1, TPM_CH1,7000);
}
void drifting(int Turnflag)					//漂移
{
  	Turn_Max(Turnflag);
	Back();		//漂移 倒车
        //DELAY_MS(4);
}
void drifting1(int Turnflag)					//漂移
{
	Turn_Max(Turnflag);
        Advance(); 
}
void Spin()						//旋转 寻找信标
{
	Turn_Max(Turn_Flag);
        tpm_pwm_duty(TPM1, TPM_CH0,0);
	tpm_pwm_duty(TPM1, TPM_CH1,5000);
}

void Rush_Beacon()				//撞到信标
{
	//Back();						//全速倒车
	tpm_pwm_duty(TPM1, TPM_CH1,0);
	tpm_pwm_duty(TPM1, TPM_CH0,10000);
	if(Mid<CAMERA_W/2)			//目标信标在左边
	{
		Turn_Max(1);			//往右打死角 避开信标
	}
	else 						//目标信标在右边
	{
		Turn_Max(0);			//往右左打死角 避开信标
	}
	DELAY_MS(Back_Delay_Time);
	if(Mid<CAMERA_W/2)			//目标信标在左边
	{
	 	Turn_Max(0);			//往右打死角 避开信标
	}
	else 						//目标信标在右边
	{
		Turn_Max(1);			//往右左打死角 避开信标
	}
	Advance();
	DELAY_MS(Back_Delay_Time);
	pit_time_start  (PIT1);
	//前进 
}

