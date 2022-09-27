#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
#define ENCODER_TIM_PERIOD (u16)(65535)	//16Î»¶¨Ê±Æ÷	65535

void TIM4_Int_Init(u16 arr,u16 psc);
void start_time(void);
u16 mseconds(void);
void TIM4_IRQHandler(void);
void TIM6_Int_Init(u16 arr,u16 psc);
void TIM6_DAC_IRQHandler(void);
void TIM6_DAC_IRQHandler(void);
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM5(void);
int Read_Encoder(u8 TIMX);
void TIM2_IRQHandler(void);
void TIM5_IRQHandler(void);
int Position_PID_Lift (int a,int Target);
int Position_PID_Right (int a,int Target);
void TIM7_IRQHandler(void);
extern volatile unsigned long long FreeRTOSRunTimeTicks;
void ConfigureTimeForRunTimeStats(void);
#endif
