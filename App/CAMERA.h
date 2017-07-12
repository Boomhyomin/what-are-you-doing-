#include "common.h"
#include "oled.h"

//��״̬����
typedef enum
{
	initStatus,        //��ʼ��״̬
	allEdgesFound,     //������Ե���ҵ�
	onlyFindLeftEdge,  //ֻ�ҵ����Ե
	onlyFindRightEdge, //ֻ�ҵ��ұ�Ե
	findNone,          //���߶�û���ҵ�
	cross,             //����Ϊʮ��
	allBlackRow,       //����Ϊȫ����
	singleLine,
}rowStatus;

typedef enum
{
   straight,
   curve,
}roadStatus;

//�ٶ�
typedef struct
{
  	int expectSpeed;  //���ε��ٶ�
	int minSpeed;	  //��С���ٶȣ����ܹ�ת���ٶ�
	int maxSpeed;     //���ܹ��ﵽ�������ٶ�
	int baseSpeed;    //��׼�ٶ�------->>>>����ٶ����ɲ��뿪�ؾ����ģ�����ֵ��minSpeed+��ֵ*10
	float baseFloatSpeed; //�����ʵ�ʵ��ٶ�
	
	//int P;  //PID�е�p��ϵ��------����
	//int I;  //PID�е�iϵ��--------����
	//int D;  //PID�е�dϵ��--------΢��
	
	float increment_car;
	int speedPWM;
	int goal_car;   
	
	float Kp; //����
	float Ki; //����
	float Kd; //΢��
	
	int err;
	int err_last;
	int err_next;
}SMART_CAR_SPEED;

typedef struct
{
  	int8 leftEdge[60];              //������Ե������                     -1
	int8 rightEdge[60];             //����ұ�Ե������                     -1
	int8 leftEdgeFlag[60];          //������Ե������״̬ 		  0
	int8 rightEdgeFlag[60];         //����ұ�Ե������״̬ 		  0
	int8 leftInterrupt[60];         //������Եͻ������Ϣ 		  0
	int8 rightInterrupt[60];        //����ұ�Ե��ͻ����Ϣ�� 		  0
	int8 diffLeft[60];              //������Ե�����м�Ĳ�ֵ 		  0
	int8 diffRight[60];             //����ұ�Ե�����еĲ�ֵ 		  0
	int8 outCrossStatus;            //��ʮ��ʱ�����״̬  		  outCrossCorrect
	int8 outCrossLeftInterrupt;     //��ʮ��ʱ����ͻ��ĵط�  		  0
	int8 outCrossRightInterrupt;    //��ʮ��ʱ����ͻ��ĵط� 		  0
	int8 centerLine[60];            //������ߵ����� 		  	  -1
        rowStatus _rowStatus[60]; 	//���·�������� 			  initStatus
	int8 isSingleLine;              //��־��ͼ�����Ƿ��е��� 		  0
	int8 singleLineBeginRow;        //�״γ��ֵ��ߵ��� 			  0
	int8 singleLineEndRow;          //���߽����� 				  59
	float centerLineSlope;          //���ߵ�б�� 				  0
	int8 firstBlackInCenter;        //��������ʱ�ĵ�һ����ɫ�ĵ����     0
	int8 firstBlackInCol20;         //����20�еĵ�һ����ɫ�ĵ� 		  0
	int8 firstBlackInCol60;        //����60�еĵ�һ����ɫ�ĵ� 		  0
	int crossCnt;                   //���ɨ��ͼ����ʮ���еĸ���         0
	int minCrossRow;                //���ɨ��ͼ����ʮ���е���С����ֵ  59
	int leftInterruptCnt;           //���ɨ��ͼ������ͻ���е�����       1
  	int minLeftInterruptRow;        //�����ͻ���е���С����ֵ		  59
  	int rightInterruptCnt;	        //���ɨ��ͼ������ͻ���е����� 	  1
  	int minRightInterruptRow;       //�����ͻ���е���С����ֵ	 	  59
  	int maxBlackRow;		//���ȫ���е���С����ֵ		  0
  	roadStatus curRoadStatus;	//�洢��ǰ����������			����Ҫ��ʼ��ҲӦ�ÿ���
  	roadStatus lasRoadStatus;	//�洢��һ����������
}OV7725_ROAD_STATUS;

typedef struct
{
  	int maxPWM;
	int minPWM;
	int middlePWM;
	int outPWM;
	int lastPWM;
	int nextPWM;
	int goalPWM;
	int error;
	int err_last;
	int srtP;
	int strD;
}steerPWM;

void run(uint8 **ppData);
void steerControl();
void setSpeed();
void searchEdge(uint8 **ppData);
void judgeRoadStatus(uint8 **ppData);
void judgeOutCross(void);
void calculateCenterLineSlope(void);
void controlSteer_s();
void controlSteer_c(void);
void init_OV7725_ROAD_STATUS(void);
void caculateDiffer(void);
int getIntAbs(int in);
void getFirstBlackInCenter(uint8 **ppData);
void setSteer_outPWM(uint8 **ppData);
void steerControl();
