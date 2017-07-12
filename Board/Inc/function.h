#ifndef __FUNCTION_H__
#define __FUNCTION_H__
extern void vcan_sendimg(uint8 *imgaddr, uint32 imgsize);
extern void img_extract(uint8 *dst, uint8 *src, uint32 srclen);
extern void Test_Servo();
extern void LCD_DrawPicture();
extern void Send_One_Img();
uint8 Get_Switch();
extern void Display_Num();
void Emptying_Array(int *array,int Max_Size);
#define BEEP PTE0
#endif 