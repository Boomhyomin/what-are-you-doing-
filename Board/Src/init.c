#include "common.h"
#include "include.h"
#include "init.h"
#include "PID.h"
#include "function.h"
#include "deal_data.h"
#include "include.h"
#include "SEEKFREE_OLED.h"
void PORTA_IRQHandler();  //PORTA�ж�
void DMA0_IRQHandler();   //DMA�ж�
void PIT_IRQHandler();		//���PID
extern int Max;
uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������
int Encoder;
int Encoder_Rush_Num;//�����������ɵ��ǳ�С����
int Stop_Flag=0;
void Init_All()
{
	Servo_Init();
	//uart_init(UART0,9600);//��ʼ������ģ��
	
	OLED_Init();//��ʼ��oled��   
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
	
	gpio_init(PTA12,GPO,1);//��ʼ�����
	gpio_init(PTA13,GPO,1);
	
	tpm_pwm_init( TPM1, TPM_CH0,10000,0);        //��ʼ�����PWM ��ʹ�� TPM0_CH4��Ƶ��Ϊ8000 ��ռ�ձ�Ϊ 50 / TPM0_PRECISON                                                  
	tpm_pwm_init(TPM1, TPM_CH1,10000,0);
	
	//Advance();		//ǰ��
	
}

void Encoder_Init()
{
	tpm_pulse_init(TPM0,TPM_CLKIN0,TPM_PS_1);
	pit_init_ms(PIT0,10);
	set_vector_handler(PIT_VECTORn ,PIT_IRQHandler); //�����жϸ�λ�������ж������������PIT0���жϷ�����Ϊ PIT0_IRQHandler
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
	camera_init(imgbuff);//��ʼ������ͷ	
	set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //����LPTMR���жϸ�λ����Ϊ PORTA_IRQHandler
	set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //����LPTMR���жϸ�λ����Ϊ PORTA_IRQHandler
}
//����PORTA���жϷ�����Ϊ PORTA_IRQHandler
void PORTA_IRQHandler()
{
	uint8  n = 0;    //���ź�
	uint32 flag = PORTA_ISFR;
	PORTA_ISFR  = ~0;                                   //���жϱ�־λ
	
	n = 6;                                              //���ж�
	if(flag & (1 << n))                                 //PTA6�����ж�
	{
		camera_vsync();
	}
}
/*!
*  @brief      DMA0�жϷ�����
*  @since      v5.0
*/
void DMA0_IRQHandler()
{
	camera_dma();
}


void PIT_IRQHandler()		//���PID
{
	int motor_error=0,PWM_OUT;
	if(PIT_TFLG(PIT0) == 1)
	{
		PIT_Flag_Clear(PIT0);     //�����־λ
		 Encoder=tpm_pulse_get(TPM0);    //��׽���������ص�ֵ
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
	gpio_init(PTB19,GPO,1);//��ʼ�����   
	
	tpm_pwm_init(TPM2,TPM_CH1,100,Servo_Mid);      //��ʼ�����PWM,ʹ��TPM2_CH1,Ƶ��Ϊ300��ռ�ձ�Ϊ1340/TPM2_PRECISON
	//while(1);
}
void Beep()
{
	gpio_init(BEEP,GPO,0);
}

void Init_PIT()
{
	pit_time_start (PIT1);                 //��ʼ��ʱ
	lptmr_timing_ms(Beep_Time);
	
	set_vector_handler(LPTMR_VECTORn ,LPTMR_IRQHandler);
	//���� LPTMR ���жϷ�����Ϊ LPTMR_IRQHandler
	enable_irq (LPTMR_IRQn);
}

void LPTMR_IRQHandler(void)
{
	gpio_set (BEEP, 0);
	//Advance();
	LPTMR_Flag_Clear(); //���жϱ�־λ
}
