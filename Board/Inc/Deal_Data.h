#ifndef __DEAL_DATA_H__
#define __DEAL_DATA_H__
extern void Search();
extern void Display();
extern void Start();
extern int ceju();
extern void GetImage();
#define Aid 30			//55//
#define Beacon_Value 3 //信标有效宽度
//#define Aid 55 //目标位置 位于信标左边  防止撞到信标
#define Left_Start 0			//左边起点				
#define Right_Start   CAMERA_W 
#define Down_Start CAMERA_H
#define Up_Start 0
#define Beacon_Max 8
#endif
