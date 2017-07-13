#ifndef __INIT_H__
#define __INIT_H__
#define Start_Speed 5500 

#define Servo_Mid  1467     	//�������λ�� //1467
#define Servo_Left     1610     //������ޣ��ܹؼ����޷���ֹ�������  //1597   1600
#define Servo_Right    1310     //����Ҽ��ޣ��ܹؼ����޷���ֹ�������
#define Motor_P 1
#define Encoder_Min 2//��������Сֵ �����ж��Ƿ�ײ���ű�   
#define Encoder_Rush_Max 4 //�����ɵ���������Сֵ��������
#define Beep_Time 100//150
void Init_All();
void Cemera_Init();
extern void Motor_Init();
void Switch_Init();
void Servo_Init();
void Encoder_Init();
void Beep();
extern void Init_PIT();
void LPTMR_IRQHandler(void);
#endif 