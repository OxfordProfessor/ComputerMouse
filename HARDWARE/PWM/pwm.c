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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOD,&GPIO_InitStructure);
         
	
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);//��ʼ����ʱ��2
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //�������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC2Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC3Init(TIM4, &TIM_OCInitStructure); 
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable); 
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR�ϵ�Ԥװ�ؼĴ���
 
  TIM_ARRPreloadConfig(TIM4,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM2	
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

void Speed_F(u16 speed1,u16 speed2)	//ǰ
{
	Target_Left = speed1;
	Target_Righ = speed2;
}

void Speed_L(u16 speed1,u16 speed2)	//��
{
	Target_Left = -speed1;
	Target_Righ = speed2;
}

void Speed_R(u16 speed1,u16 speed2)	//��
{
	Target_Left = speed1;
	Target_Righ = -speed2;
}

void Speed_B(u16 speed1,u16 speed2)	//��
{
	Target_Left = -speed1;
	Target_Righ = -speed2;
}

void Speed_S()				//ֹͣ
{
	Target_Left = 0;
	Target_Righ = 0;
}

void Go_Onestep()   //ǰ��һ���Թ�����
{
	start_time();   //��ʼ�Ƽ�
	while(1)
	{
		Speed_F(100,100);                   //��Ҫ����
		if(mseconds()> 900) break;          //��Ҫ����
	}
}

void turn_left(int angle)   //ԭ����ת��
{
	start_time();
	while(1)
	{
		Speed_L(-100,100);               //��Ҫ����
		if(mseconds()> 900) break;          //��Ҫ����
	}
}

void turn_right(int angle)   //ԭ����ת��
{
	start_time();
	while(1)
	{
		Speed_R(-100,100);               //��Ҫ����
		if(mseconds()> 900) break;          //��Ҫ����
	}
}