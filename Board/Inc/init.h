#ifndef __INIT_H__
#define __INIT_H__
#define Start_Speed 5500 

#define Servo_Mid  1467     	//舵机中心位置 //1467
#define Servo_Left     1610     //舵机左极限，很关键，限幅防止舵机打死  //1597   1600
#define Servo_Right    1310     //舵机右极限，很关键，限幅防止舵机打死
#define Motor_P 1
#define Encoder_Min 2//编码器最小值 用于判断是否撞到信标   
#define Encoder_Rush_Max 4 //连续采到编码器最小值的最大次数
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