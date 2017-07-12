#include "common.h"
#include "oled.h"

//行状态定义
typedef enum
{
	initStatus,        //初始化状态
	allEdgesFound,     //两个边缘都找到
	onlyFindLeftEdge,  //只找到左边缘
	onlyFindRightEdge, //只找到右边缘
	findNone,          //两边都没有找到
	cross,             //该行为十字
	allBlackRow,       //该行为全黑线
	singleLine,
}rowStatus;

typedef enum
{
   straight,
   curve,
}roadStatus;

//速度
typedef struct
{
  	int expectSpeed;  //期盼的速度
	int minSpeed;	  //最小的速度，即能够转的速度
	int maxSpeed;     //所能够达到的最大的速度
	int baseSpeed;    //基准速度------->>>>这个速度是由拨码开关决定的，它的值是minSpeed+键值*10
	float baseFloatSpeed; //这个是实际的速度
	
	//int P;  //PID中的p的系数------比例
	//int I;  //PID中的i系数--------积分
	//int D;  //PID中的d系数--------微分
	
	float increment_car;
	int speedPWM;
	int goal_car;   
	
	float Kp; //比例
	float Ki; //积分
	float Kd; //微分
	
	int err;
	int err_last;
	int err_next;
}SMART_CAR_SPEED;

typedef struct
{
  	int8 leftEdge[60];              //存放左边缘的数组                     -1
	int8 rightEdge[60];             //存放右边缘的数组                     -1
	int8 leftEdgeFlag[60];          //存放左边缘的搜索状态 		  0
	int8 rightEdgeFlag[60];         //存放右边缘的搜索状态 		  0
	int8 leftInterrupt[60];         //存放左边缘突变点的信息 		  0
	int8 rightInterrupt[60];        //存放右边缘的突变信息点 		  0
	int8 diffLeft[60];              //存放左边缘中两行间的差值 		  0
	int8 diffRight[60];             //存放右边缘中两行的差值 		  0
	int8 outCrossStatus;            //出十字时车身的状态  		  outCrossCorrect
	int8 outCrossLeftInterrupt;     //出十字时边线突变的地方  		  0
	int8 outCrossRightInterrupt;    //出十字时边线突变的地方 		  0
	int8 centerLine[60];            //存放中线的数组 		  	  -1
        rowStatus _rowStatus[60]; 	//存放路况的数组 			  initStatus
	int8 isSingleLine;              //标志该图像中是否有单线 		  0
	int8 singleLineBeginRow;        //首次出现单线的行 			  0
	int8 singleLineEndRow;          //单线结束行 				  59
	float centerLineSlope;          //中线的斜率 				  0
	int8 firstBlackInCenter;        //遍历中线时的第一个黑色的点的列     0
	int8 firstBlackInCol20;         //遍历20列的第一个黑色的点 		  0
	int8 firstBlackInCol60;        //遍历60列的第一个黑色的点 		  0
	int crossCnt;                   //存放扫描图像中十字行的个数         0
	int minCrossRow;                //存放扫描图像中十字行的最小行数值  59
	int leftInterruptCnt;           //存放扫描图像中左突变行的行数       1
  	int minLeftInterruptRow;        //存放左突变行的最小行数值		  59
  	int rightInterruptCnt;	        //存放扫描图像中右突变行的行数 	  1
  	int minRightInterruptRow;       //存放右突变行的最小行数值	 	  59
  	int maxBlackRow;		//存放全黑行的最小行数值		  0
  	roadStatus curRoadStatus;	//存储当前的赛道类型			不需要初始化也应该可以
  	roadStatus lasRoadStatus;	//存储上一场赛道类型
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
