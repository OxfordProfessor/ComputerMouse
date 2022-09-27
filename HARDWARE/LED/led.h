#ifndef _LED_H
#define _LED_H
#include "sys.h"


//LED端口定义
#define LED1 PDout(0)
#define LED2 PDout(1)
#define LED3 PDout(2)
#define LED4 PDout(3)

void LED_Init(void);  //初始化
#endif
