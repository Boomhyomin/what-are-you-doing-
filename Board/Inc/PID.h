#ifndef __PID_H__
#define __PID_H__


#define	Back_Delay_Time 300 //�ܿ��ű� ����ʱ��

extern void Servo_PID();
extern void Speed_PID();
extern void Stop();				//ͣ��
extern void drifting();			//Ư��
extern void Spin();				//��ת Ѱ���ű�
extern void Advance();			//ǰ��
extern void Rush_Beacon();		//ײ���ű�
extern void Back();						//����

#endif 