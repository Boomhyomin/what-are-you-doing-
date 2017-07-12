#ifndef __PID_H__
#define __PID_H__


#define	Back_Delay_Time 300 //避开信标 后退时间

extern void Servo_PID();
extern void Speed_PID();
extern void Stop();				//停车
extern void drifting();			//漂移
extern void Spin();				//旋转 寻找信标
extern void Advance();			//前进
extern void Rush_Beacon();		//撞到信标
extern void Back();						//倒车

#endif 