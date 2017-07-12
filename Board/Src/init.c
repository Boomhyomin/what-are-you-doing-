#include "common.h"
#include "include.h"
#include "init.h"
#include "PID.h"
#include "function.h"
#include "deal_data.h"
#include "include.h"
#include "SEEKFREE_OLED.h"
void PORTA_IRQHandler();  //PORTA中断
void DMA0_IRQHandler();   //DMA中断
void PIT_IRQHandler();		//电机PID
extern int Max;
uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
int Encoder;
int Encoder_Rush_Num;//编码器连续采到非常小的数
int Stop_Flag=0;
void Init_All()
{
	Servo_Init();
	//uart_init(UART0,9600);//初始化蓝牙模块
	
	OLED_Init();//初始化oled屏   
	Cemera_Init();
	
	//Switch_Init();	
	Motor_Init();
	Encoder_Init();
	Beep();
	Init_PIT();
	//Test_Servo();
	
	
}

void Motor_Init()
{
	
	gpio_init(PTA12,GPO,1);//初始化电机
	gpio_init(PTA13,GPO,1);
	
	tpm_pwm_init( TPM1, TPM_CH0,10000,0);        //初始化电机PWM ，使用 TPM0_CH4，频率为8000 ，占空比为 50 / TPM0_PRECISON                                                  
	tpm_pwm_init(TPM1, TPM_CH1,10000,0);
	
	//Advance();		//前进
	
}

void Encoder_Init()
{
	tpm_pulse_init(TPM0,TPM_CLKIN0,TPM_PS_1);
	pit_init_ms(PIT0,10);
	set_vector_handler(PIT_VECTORn ,PIT_IRQHandler); //设置中断复位函数到中断向量表里。设置PIT0的中断服务函数为 PIT0_IRQHandler
	enable_irq(PIT_IRQn);
}



void Switch_Init()
{
	gpio_init(PTB9,GPI,0);
	gpio_init(PTB10,GPI,0);
	gpio_init(PTB11,GPI,0);
	gpio_init(PTB16,GPI,0);
	gpio_init(PTB17,GPI,0);
	gpio_init(PTB18,GPI,0);
	
	port_init_NoALT(PTB9,PULLUP);
	port_init_NoALT(PTB10,PULLUP);
	port_init_NoALT(PTB11,PULLUP);
	port_init_NoALT(PTB16,PULLUP);
	port_init_NoALT(PTB17,PULLUP);
	port_init_NoALT(PTB18,PULLUP);
}

void Cemera_Init()
{
	camera_init(imgbuff);//初始化摄像头	
	set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //设置LPTMR的中断复位函数为 PORTA_IRQHandler
	set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //设置LPTMR的中断复位函数为 PORTA_IRQHandler
}
//设置PORTA的中断服务函数为 PORTA_IRQHandler
void PORTA_IRQHandler()
{
	uint8  n = 0;    //引脚号
	uint32 flag = PORTA_ISFR;
	PORTA_ISFR  = ~0;                                   //清中断标志位
	
	n = 6;                                              //场中断
	if(flag & (1 << n))                                 //PTA6触发中断
	{
		camera_vsync();
	}
}
/*!
*  @brief      DMA0中断服务函数
*  @since      v5.0
*/
void DMA0_IRQHandler()
{
	camera_dma();
}


void PIT_IRQHandler()		//电机PID
{
	int motor_error=0,PWM_OUT;
	if(PIT_TFLG(PIT0) == 1)
	{
		PIT_Flag_Clear(PIT0);     //清除标志位
		 Encoder=tpm_pulse_get(TPM0);    //扑捉编码器返回的值
		tpm_pulse_clean(TPM0);
		
		if(Encoder<Encoder_Min&&pit_time_get_ms(PIT1)>500)
		{
			Encoder_Rush_Num++;
			if(Encoder_Rush_Num>Encoder_Rush_Max)
			{
				Encoder_Rush_Num=0;
				//Rush_Beacon();
				
			}
		}
		else
		{
			Encoder_Rush_Num=0;
		}
	}
	
	
}
void Servo_Init()
{
	gpio_init(PTB19,GPO,1);//初始化舵机   
	
	tpm_pwm_init(TPM2,TPM_CH1,100,Servo_Mid);      //初始化舵机PWM,使用TPM2_CH1,频率为300，占空比为1340/TPM2_PRECISON
	//while(1);
}
void Beep()
{
	gpio_init(BEEP,GPO,0);
}

void Init_PIT()
{
	pit_time_start (PIT1);                 //开始计时
	lptmr_timing_ms(Beep_Time);
	
	set_vector_handler(LPTMR_VECTORn ,LPTMR_IRQHandler);
	//设置 LPTMR 的中断服务函数为 LPTMR_IRQHandler
	enable_irq (LPTMR_IRQn);
}

void LPTMR_IRQHandler(void)
{
	gpio_set (BEEP, 0);
	//Advance();
	LPTMR_Flag_Clear(); //清中断标志位
}
