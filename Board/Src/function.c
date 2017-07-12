#include "common.h"
#include "include.h"
#include "init.h"
#include "PID.h"
#include "init.h"
#include "function.h"
#include "deal_data.h"
#include "SEEKFREE_OLED.h"

//LED��ʾ�����е���
#define CMD_IMG     1
extern uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������

//int OLED_EdgePositon[64]={7,6,5,4,3,2,1,0,15,14,13,12,11,10,9,8,23,22,21,20,19,18,17,16,31,30,29,28,27,26,25,24,39,38,37,36,35,34,33,32,47,46,45,44,43,42,41,40,55,54,53,52,51,50,49,48,59,58,57,56};
extern uint8 img[CAMERA_H][CAMERA_W]; 
  //����ӥ������ͷ��һ�ֽ�8�����أ������Ҫ��ѹΪ 1�ֽ�1�����أ����㴦��
void vcan_sendimg(uint8 *imgaddr, uint32 imgsize)
{
	uint8 cmdf[2] = {CMD_IMG, ~CMD_IMG};    //ɽ����λ�� ʹ�õ�����
	uint8 cmdr[2] = {~CMD_IMG, CMD_IMG};    //ɽ����λ�� ʹ�õ�����
	
	uart_putbuff(UART0, cmdf, sizeof(cmdf));    //�ȷ�������
	
	uart_putbuff(UART0, imgaddr, imgsize); //�ٷ���ͼ��
	
	uart_putbuff(UART0, cmdr, sizeof(cmdr));    //�ȷ�������
}
//�����ʼ��3225 4325 5425
//������ʼ��
//void Get_Distance()
//{
//	uint32 Time;
//	uint32 flag;
//	
//	flag = 0;
//	gpio_set(TRIG,1);               //������������
//	pit_delay_us(PIT1,15);
//	gpio_set(TRIG,0);
//	
//	while(!gpio_get(ECHG));             //�ȴ���ƽ��ߣ��͵�ƽһֱ�ȴ�
//	
//	pit_time_start  (PIT0);                 //��ʼ��ʱ
//	
//	while(gpio_get(ECHG))              //�ȴ���ƽ��ͣ��ߵ�ƽһֱ�ȴ�
//	{
//		flag++;
//		if(flag >WAIT_TIME)
//		{
//			break;
//		}
//	}    
//	
//	Time = pit_time_get_us(PIT0);//ֹͣ��ʱ����ȡ��ʱʱ��
//	
//	if(flag <WAIT_TIME )
//	{
//		Distance = Time * 34/2/100;			//��λ������
//	} 
//}

/*!
*  @brief      ��ֵ��ͼ���ѹ���ռ� �� ʱ�� ��ѹ��
*  @param      dst          0   ͼ���ѹĿ�ĵ�ַ
*  @param      src             ͼ���ѹԴ��ַ
*  @param      srclen          ��ֵ��ͼ���ռ�ÿռ��С
*  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
*  Sample usage:   sendimg(imgbuff, CAMERA_W * CAMERA_H);                    //���͵���λ��
*/
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
    uint8 colour[2] = {255,0}; //0 �� 1 �ֱ��Ӧ����ɫ
    //ע��ɽ�������ͷ 0 ��ʾ ��ɫ��1��ʾ ��ɫ
    uint8 tmpsrc;
    while(srclen --)
    {
        tmpsrc = *src++;
        *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
        *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
    }
}
uint8 Get_Switch()
{
	uint8 num;
	num=gpio_get(PTB9)+2*gpio_get(PTB10)+4*gpio_get(PTB11)+8*gpio_get(PTB16)+16*gpio_get(PTB17)+32*gpio_get(PTB18);
	return num;
}


//���Զ��
void Test_Servo()
{
	int i=0,flag=1;
	while(1)
	{
		for(;i<Servo_Left-Servo_Mid;i++)
		{
			tpm_pwm_duty(TPM2,TPM_CH1,Servo_Mid+i);
			DELAY_MS(1);
		}
		
		for(;i>Servo_Right-Servo_Mid;i--)
		{
			tpm_pwm_duty(TPM2, TPM_CH1,Servo_Mid+i);
			DELAY_MS(1);
		}
	}
}
void Send_One_Img()
{
	int i,j;
	for(i=0;i<159;i++)
	{
		for(j=0;j<119;j++)
		{
			uart_putchar(UART0,img[i][j]);
		}
	}
	//while(1);
}


void LCD_DrawPicture()
{
	int row;
	int col;
	LCD_clearGRAM();	
	for(row=0;row<CAMERA_H;row++)
	{
		for(col=0;col<CAMERA_W;col++)
		{
			//LCD_DRAWPoint(col,OLED_EdgePositon[row],!img[row][col]);
		}
	}	
	LCD_Refresh_Gram();
}

void Display_Num(int x,int y,int num)		//����ʾ0
{
	if(!num)
	{
		OLED_Print_Str(x,y,"0");
	}
	else
	{
		OLED_Print_Num(x,y,num);
	}
	
	if(num<10)
	{
		OLED_Print_Num(x+8,y,0);
	}
}

void Emptying_Array(int *array,int Max_Size)
{
	int i;
	for(i=0;i<Max_Size;i++)
	{
		*array=0;
		*array++;
	}
}
