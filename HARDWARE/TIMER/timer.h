#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"


void TIM5_Int_Init(u16 arr,u16 psc);

extern volatile unsigned long long FreeRTOSRunTimeTicks;
void ConfigureTimeForRunTimeStats(void);
#endif
