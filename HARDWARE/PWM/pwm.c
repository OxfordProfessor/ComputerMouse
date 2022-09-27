#include "pwm.h"
#include "timer.h"

extern int Motor_Left,Motor_Right;
extern int Target_Left,Target_Righ;

void Motor12_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure);
         
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//初始化定时器2
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC2Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC3Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); 
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM2在CCR上的预装载寄存器
 
  TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM4, ENABLE);  //使能TIM2	
}


void Motor1(int x)	
{
	if(x<0)
	{
		x=(-x);
		TIM_SetCompare1(TIM4,x);
		TIM_SetCompare2(TIM4,0);
	}
	else
	{
		TIM_SetCompare1(TIM4,0);
		TIM_SetCompare2(TIM4,x);
	}
}
void Motor2(int x)	
{
	if(x<0)
	{
		x=(-x);
		TIM_SetCompare3(TIM4,x);
		TIM_SetCompare4(TIM4,0);
	}
	else
	{
		TIM_SetCompare3(TIM4,0);
		TIM_SetCompare4(TIM4,x);
	}
}

void Speed_F(u16 speed1,u16 speed2)	//前
{
	Target_Left = speed1;
	Target_Righ = speed2;
}

void Speed_L(u16 speed1,u16 speed2)	//左
{
	Target_Left = -speed1;
	Target_Righ = speed2;
}

void Speed_R(u16 speed1,u16 speed2)	//右
{
	Target_Left = speed1;
	Target_Righ = -speed2;
}

void Speed_B(u16 speed1,u16 speed2)	//后
{
	Target_Left = -speed1;
	Target_Righ = -speed2;
}

void Speed_S()				//停止
{
	Target_Left = 0;
	Target_Righ = 0;
}

void Go_Onestep()   //前进一个迷宫步长
{
	start_time();   //开始计间
	while(1)
	{
		Speed_F(100,100);                   //需要调试
		if(mseconds()> 900) break;          //需要调试
	}
}

void turn_left(int angle)   //原地左转向
{
	start_time();
	while(1)
	{
		Speed_L(-100,100);               //需要调试
		if(mseconds()> 900) break;          //需要调试
	}
}

void turn_right(int angle)   //原地右转向
{
	start_time();
	while(1)
	{
		Speed_R(-100,100);               //需要调试
		if(mseconds()> 900) break;          //需要调试
	}
}