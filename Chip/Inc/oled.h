#ifndef __OLED_H__
#define __OLED_H__


#include "common.h"
//#define LCD_Port     PTE

#define LCD_SCL_Pin   PTE30
#define LCD_SDA_Pin   PTE31
#define LCD_RST_Pin   PTE24
#define LCD_DC_Pin    PTE25 

#define LCD_SCL  PTXn_T(LCD_SCL_Pin,OUT)
#define LCD_SDA  PTXn_T(LCD_SDA_Pin,OUT)
#define LCD_RST  PTXn_T(LCD_RST_Pin,OUT)
#define LCD_DC   PTXn_T(LCD_DC_Pin,OUT)

#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel		((XLevelH&0x0F)*16+XLevelL)
//#define Max_Column	128
//#define Max_Row		  64
#define Max_Column	128
#define Max_Row		  64
#define	Brightness	0xCF 


#define X_WIDTH 128
#define Y_WIDTH 64

#define byte unsigned char  
#define word unsigned int   


extern void LCD_Init(void);
extern void LCD_CLS(void);
extern void LED_P6x8Str(byte x,byte y,byte ch[]);
extern void LCD_P8x16Str(byte x,byte y,byte ch[]);
extern void LCD_P14x16Str(byte x,byte y,byte ch[]);
extern void LCD_Print(byte x, byte y, byte ch[]);
 extern void LCD_PutPixel(byte x,byte y);
 extern void LCD_Set_Pos(byte x, byte y);
extern void LCD_Rectangle(byte x1,byte y1,byte x2,byte y2,byte gif);
extern void Draw_BMP(byte x0,byte y0,byte x1,byte y1,byte bmp[]); 
extern void LCD_Fill(byte dat);
extern void LCD_PrintImage(void);
extern void LCD_ShowPic(void);
extern void LCD_ShowLine(void);
extern void LCD_WrCmd(byte cmd);
extern void LCD_WrDat(byte data);

extern void LED_P6x8Char(unsigned char x,unsigned char y,unsigned char ch);
extern void LED_PrintValueC(unsigned char x, unsigned char y, char data);
extern void LED_PrintValueI(unsigned char x, unsigned char y, int data);
extern void LED_PrintValueFP(unsigned char x, unsigned char y, unsigned int data, unsigned char num);
extern void LED_PrintValueF(unsigned char x, unsigned char y, float data, unsigned char num);
extern void LCD_Refresh_Gram(void) ;
extern void LCD_DRAWPoint(uint8 x,uint8 y,uint8 t) ;
extern void LCD_clearGRAM(void);



#endif
