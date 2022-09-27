#define AI 							analog
#define DI							digital
#define ClearScreen 		clear_screen
#define ShowStr 				OLED_Display_GB2312_string		//字符串数显
#define ShowAI					display_GB2312_u16 //四位数显
#define ShowDI					display_GB2312_u1		//一位数显
#define ServoSetMode		UP_CDS_SetMode					
#define ServoSetAngle		UP_CDS_SetAngle
#ifndef __OLED_H
#define __OLED_H 
#include "sys.h"
#include "stdlib.h"	

//-----------------OLED端口定义---------------- 

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_4)//SCLK
#define OLED_SCLK_Set() GPIO_SetBits(GPIOE,GPIO_Pin_4)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_5)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOE,GPIO_Pin_5)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_3)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOE,GPIO_Pin_3)

#define OLED_CS_Clr() GPIO_ResetBits(GPIOE,GPIO_Pin_2)//CS1
#define OLED_CS_Set() GPIO_SetBits(GPIOE,GPIO_Pin_2)
 		     
#define OLED_ROM()  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)//FS0


#define OLED_ROM_CS_Clr()  GPIO_ResetBits(GPIOE,GPIO_Pin_0)//CS2
#define OLED_ROM_CS_Set()  GPIO_SetBits(GPIOE,GPIO_Pin_0)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define u8 unsigned char
#define u32 unsigned int
#define u8long unsigned long

void OLED_ColorTurn(u8 i);//颜色翻转
void OLED_DisplayTurn(u8 i);//屏幕旋转180度
void OLED_WR_Byte(u32 dat,u8 mode);//
void clear_screen(void);//清屏
void OLED_address(u8 page,u8 column);
void OLED_Display_128x64(u8 *dp);//显示图像
void OLED_Display_16x16(u32 column,u32 page,u8 *dp);
void OLED_Display_8x16(u8 column,u32 page,u8 *dp);
void OLED_Display_5x7(u8 column,u32 page,u8 *dp);
void Send_Command_to_ROM(u8 dat);
u8 Get_data_from_ROM(u8 way);
void OLED_get_data_from_ROM(u8 addrHigh,u8 addrMid,u8 addrLow,u8 *pbuff,u8 DataLen);
void OLED_Display_GB2312_string(u8 y,u8 x,u8 *text);
void OLED_Display_string_5x7(u8 y,u8 x,u8 *text);
void OLED_ShowNum(u8 x,u8 y,float num,u8 len);
void OLED_Init(void);
//用户常用函数
void display_GB2312_u16(unsigned char y,unsigned char x,u16 num); //显示16位变量
void display_GB2312_u1(unsigned char y,unsigned char x,u16 num);//显示1位变量
#endif
