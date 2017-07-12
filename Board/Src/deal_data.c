#include "common.h"
#include "include.h"
#include "init.h"
#include "Deal_Data.h"
#include "Function.h"
#include "PID.h"
#include "SEEKFREE_OLED.h"
#define TRIG_left    PTC11//PTC8��  PTC0��    PTC11��
#define ECHO_left    PTC12//PTC9  PTC1    PTC12
#define TRIG_mid     PTC0
#define ECHO_mid    PTC1
#define TRIG_right   PTC8
#define ECHO_right    PTC9

#define FLAGWAIT    0x0FFFF
int Mid=38 ;//�ű�����
int Error_Now=0;
int Max=0,Mid_Flag=0,Left=0,Right=0;
extern int Encoder;
volatile uint32 sudu = 0;
uint8 img[CAMERA_H][CAMERA_W];
int Last_Max=0;
extern uint8 imgbuff[CAMERA_SIZE]; //����洢����ͼ�������
int Beacon_Num=0;

int Turn_Flag=0;//Ư�Ʒ���   0���� 1����
int aid=41;//31   41
int size=18;
int ultrasonic_value=600;

int speed=0;
int speed1=0;
int firstPic=0;
int Beacon_flag=0;    //�ű������־λ
uint32 discount_left=10000;
uint32 discount_mid=10000;
uint32 discount_right=10000;

uint32 ABDistance_last =0; 
uint32 ABDistance_A =0;
uint32 ABDistance_B =0;
uint32 ABDistance_C =0;
uint32 filter[3] = {0};
uint32 ABDistance_last1 =0; 
uint32 ABDistance_A1 =0;
uint32 ABDistance_B1 =0;
uint32 ABDistance_C1 =0;
uint32 filter1[3] = {0};
uint32 ABDistance_last2 =0; 
uint32 ABDistance_A2 =0;
uint32 ABDistance_B2 =0;
uint32 ABDistance_C2 =0;
uint32 filter2[3] = {0};
uint32 filter_ChaoShenBo(uint32 *Array,uint8 FilterLen);

void Search()
{
        int Getsize(int);
        void drifting1(int);
        int ceju_left();
        int ceju_mid();
        int ceju_right();
	int i,j,first_light=0;
	int Mid_W[CAMERA_W]={0};//��������е���������
	int Beacon[CAMERA_H]={0};//��¼�ű��С
	Max=0;//�ű�����ȴ�
	Mid_Flag=0;
	Left=0;
	Right=0;
	GetImage();
        //size=Getsize(Encoder);
        //deng=Light_Value(Encoder);//��̬���ĵƴ�С
        //OLED_Print_Num(80, 2, Encoder);
        //sudu=500*(Encoder-3);
	for(i=0;i<60;i++) 
	{  
		for(j=0;j<80;j++)
		{
			if(img[i][j])
			{ 
				Beacon[i]++;	//���������1
			}
		}
		
	}
	for(i=0;i<60;i++)
	{
		if(Max<Beacon[i])
		{
			Max=Beacon[i];
			Mid_Flag=i;
		}
	}
	/********************�����˲�����ͼ��Ĵ���*********************/
	for(i=0;i<CAMERA_W;i++)
	{
		if(img[Mid_Flag][i])//�ҵ�����
		{
			first_light=i;
			break;
		}
	}
	for(;i<CAMERA_W;i++)//�ӵ�һ�����㴦��ʼ��������
	{
		
		for(j=i;j<CAMERA_W;j++)
		{
			if(img[Mid_Flag][j])
			{
				Mid_W[i]++;
			}
			else
			{
				break;//��ͷ
			}
		}
	}
	Max=0;
	for(i=first_light;i<CAMERA_W;i++)  //CAMERA_W=80
	{
		if(Max<Mid_W[i])
		{
			Max=Mid_W[i];
			Mid_Flag=i;
		}
	}
	/********************************
                  ���������
        ********************************/	
        discount_left=ceju_left();
        ABDistance_A = ABDistance_B;
        ABDistance_B = ABDistance_C;
        ABDistance_C = ABDistance_last;
        ABDistance_last = discount_left; 
        filter[0] = ABDistance_A;
        filter[1] = ABDistance_B; 
        filter[2] = ABDistance_C;
        discount_left = filter_ChaoShenBo(filter,3);
        
        discount_mid=ceju_mid();
        ABDistance_A1 = ABDistance_B1;
        ABDistance_B1 = ABDistance_C1;
        ABDistance_C1 = ABDistance_last1;
        ABDistance_last1 = discount_mid; 
        filter1[0] = ABDistance_A1;
        filter1[1] = ABDistance_B1; 
        filter1[2] = ABDistance_C1;
        discount_mid = filter_ChaoShenBo(filter1,3);
        
        discount_right=ceju_right();
        ABDistance_A2 = ABDistance_B2;
        ABDistance_B2 = ABDistance_C2;
        ABDistance_C2 = ABDistance_last2;
        ABDistance_last2 = discount_right; 
        filter2[0] = ABDistance_A2;
        filter2[1] = ABDistance_B2; 
        filter2[2] = ABDistance_C2;
        discount_right = filter_ChaoShenBo(filter2,3);
        
        if(discount_left>10&&discount_left<ultrasonic_value)
        {
//          tpm_pwm_duty(TPM2, TPM_CH1,1600);//1310
//          DELAY_MS(10);
          drifting(Turn_Flag);
        }
        if(discount_mid>10&&discount_mid<ultrasonic_value)
        {
//          tpm_pwm_duty(TPM2, TPM_CH1,1600);//1310
//          DELAY_MS(10);
          drifting(Turn_Flag);
        }
        if(discount_right>10&&discount_right<ultrasonic_value)
        {
//          tpm_pwm_duty(TPM2, TPM_CH1,1600);//1310
//          DELAY_MS(10);
          drifting(Turn_Flag);
        }
        /************************************************************************/
        
        
	if(Last_Max-Max>Beacon_Max)	//��Ϩ�� 
	{
                firstPic=0;//���ű������־λΪ0
		//��ֹ�¸��ű�����Ұ�ﲻ��Ư��)
		drifting1(Turn_Flag);//Ư��           
		Beacon_Num++; 
		Last_Max=0;
		if(Beacon_Num==10)  //ʮ���ű�ȫ���ҵ� ͣ��
		{
			Stop();
			//drifting1(Turn_Flag);//Ư�� 
		}
	}
        else if(Beacon_Num==0&&Max<Beacon_Value)   //�Զ����ܣ���û��ʲô������
        {
               //�ȴ�����
        }
        else if(Max>Beacon_Value&&Max<size)//�Ѿ���ʼ,�ҵ��ű� 
	{
		Mid=Mid_Flag+Max/2;  			//��������������п�ʼ�����ű�����
		Advance();
		Last_Max=Max;					//��Ч�ű� ��¼��С  
		Error_Now=Mid-aid;
		Servo_PID();
	}
	/*else if(Max>=size)	//����ǰƯ�� 
	{       
                firstPic++;
                if(firstPic==1&&Beacon_Num==0)
                {
                    speed=Encoder;
                }
                if(firstPic==1&&Beacon_Num==1)
                {
                    speed1=Encoder;
                }
                Last_Max=Max;
		drifting(Turn_Flag);//Ư�� 
	}*/
	else if(Max==0)					//�ϸ��ű���Ч
	{
		Spin(); //��ת Ѱ���ű�	
	}
        else
        {
                Spin(); //Ư��
        }
	Display();//��ʾ��ز���	
}


int ceju_left()
{
        uint32 timevar;
        uint32 flag;
        gpio_init(TRIG_left,GPO,0);
        gpio_init(ECHO_left,GPI,0);
        flag = 0;
        gpio_set(TRIG_left,1);               //������������
        pit_delay_us(PIT1,15);
        gpio_set(TRIG_left,0);
        
        while(gpio_get(ECHO_left) == 0);             //�ȴ���ƽ��ߣ��͵�ƽһֱ�ȴ�
        pit_time_start  (PIT0);                 //��ʼ��ʱ
        while(gpio_get(ECHO_left) == 1)              //�ȴ���ƽ��ͣ��ߵ�ƽһֱ�ȴ�
        {
            flag++;
            if(flag>FLAGWAIT)
            {
                break;
            }
        };             
        
        timevar = pit_time_get_us    (PIT0);    //ֹͣ��ʱ����ȡ��ʱʱ��
        if(flag <FLAGWAIT )
        {
            timevar = timevar * 340 /2/1000;
                
            if(timevar > 5)
            {
              if (timevar>ultrasonic_value) 
              {
                timevar=ultrasonic_value+20;
              }
              return  timevar;
            }
        }  
        DELAY_MS(10); 
  }
int ceju_mid()
{
        uint32 timevar;
        uint32 flag;
        gpio_init(TRIG_mid,GPO,0);
        gpio_init(ECHO_mid,GPI,0);
        flag = 0;
        gpio_set(TRIG_mid,1);               //������������
        pit_delay_us(PIT1,15);
        gpio_set(TRIG_mid,0);
        
        while(gpio_get(ECHO_mid) == 0);             //�ȴ���ƽ��ߣ��͵�ƽһֱ�ȴ�
        pit_time_start  (PIT0);                 //��ʼ��ʱ
        while(gpio_get(ECHO_mid) == 1)              //�ȴ���ƽ��ͣ��ߵ�ƽһֱ�ȴ�
        {
            flag++;
            if(flag>FLAGWAIT)
            {
                break;
            }
        };             
        
        timevar = pit_time_get_us    (PIT0);    //ֹͣ��ʱ����ȡ��ʱʱ��
        if(flag <FLAGWAIT )
        {
            timevar = timevar * 340 /2/1000;
                
            if(timevar > 5)
            {
              if (timevar>ultrasonic_value) 
              {
                timevar=ultrasonic_value+20;
              }
              return  timevar;
            }
        }  
        DELAY_MS(10);    
  }
int ceju_right()
{
        uint32 timevar;
        uint32 flag;
        gpio_init(TRIG_right,GPO,0);
        gpio_init(ECHO_right,GPI,0);
        flag = 0;
        gpio_set(TRIG_right,1);               //������������
        pit_delay_us(PIT1,15);
        gpio_set(TRIG_right,0);
        
        while(gpio_get(ECHO_right) == 0);             //�ȴ���ƽ��ߣ��͵�ƽһֱ�ȴ�
        pit_time_start  (PIT0);                 //��ʼ��ʱ
        while(gpio_get(ECHO_right) == 1)              //�ȴ���ƽ��ͣ��ߵ�ƽһֱ�ȴ�
        {
            flag++;
            if(flag>FLAGWAIT)
            {
                break;
            }
        };             
        timevar = pit_time_get_us    (PIT0);    //ֹͣ��ʱ����ȡ��ʱʱ��
        if(flag <FLAGWAIT )
        {
            timevar = timevar * 340 /2/1000;
                
            if(timevar > 5)
            {
              if (timevar>ultrasonic_value) 
              {
                timevar=ultrasonic_value+20;
              }
              return  timevar;
            }
        }  
        DELAY_MS(10);      
  }
uint32 filter_ChaoShenBo(uint32 *Array,uint8 FilterLen)//��������ֵ�˲�
{
  uint8 i,j;// ѭ������
  uint32 Temp;
  for (j=0; j<FilterLen-1; j++) // �������������
  {
    for (i =0; i<FilterLen-j-1; i++)
    {
      if (Array[i]>Array[i + 1])
      {
        Temp = Array[i];
        Array[i] = Array[i+1];
        Array[i+1] = Temp;
      }
    }
  }
  // ������ֵ
  if ((FilterLen & 1) > 0)
  {
    // ������������Ԫ�أ������м�һ��Ԫ��
    Temp = Array[(FilterLen + 1) / 2];
  }
  else
  {
    // ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
    Temp = (Array[FilterLen/2] + Array[FilterLen/2 + 1])/2;
  }
  return Temp;
}


void Display()
{
  OLED_Print_Num(80, 2, discount_left);
  OLED_Print_Num(80, 4, discount_mid);//���ֵ
  OLED_Print_Num(80, 6, discount_right);
}
void GetImage()
{
                camera_get_img();                                   //����ͷ��ȡͼ��
	        img_extract((uint8*)img,imgbuff,CAMERA_SIZE);		
	        dis_bmp(CAMERA_H, CAMERA_W, (uint8*)img,0xB0);
}
int Getsize(int Encoder)
{
    int size;
    if(Encoder<20)
    {
      size=45;//24
    }
    if(Encoder>=20&&Encoder<30)
    {
      size=25;//23
    }
    else if(Encoder>=30&&Encoder<40)
    {
      size=21;//21
    }
    else if(Encoder>=40&&Encoder<50)
    {
      size=20;//19
    }
    else if(Encoder>=50&&Encoder<60)
    {
      size=20;//19
    }
    else if(Encoder>=60&&Encoder<65)
    {
      size=20;//18
    }
    else if(Encoder>=65&&Encoder<70)
    {
      size=20;//19
    }
    else if(Encoder>=70&&Encoder<75)
    {
      size=18;//18
    }
    else if(Encoder>=75&&Encoder<80)
    {
      size=17;//17
    }
    else if(Encoder>=80&&Encoder<92)
    {
      size=16;//16
    }
    else if(Encoder>=92&&Encoder<102)
    {
      size=14;//13
    }
    else if(Encoder>=102&&Encoder<112)
    {
      size=13;//13
    }
    else if(Encoder>=112&&Encoder<132)
    {
      size=12;//13
    }
    else if(Encoder>=132)
    {
       size=11;
     }
    else
    {
      size=45;
    }
    return size;
}
